#include <algorithm>
#include <cmath>    // std::acos, std::sin
#include <iomanip>  // std::setprecision
#include <iostream>
#include <numeric>  // std::accumulate
#include <unordered_set>

#include "parse.h"
#include "transport_guide.h"

using namespace std;

void TransportGuide::AddStops(std::istream& in) {
    size_t create_queries_count;
    in >> create_queries_count >> std::ws;
    _buffer.reserve(create_queries_count);

    // полностью обрабатываем ТОЛЬКО ту часть входного потока, которая касается остановок
    string line{};
    for (size_t i = 0; i < create_queries_count; ++i) {
        getline(in, line);
        ProcessCreateQuery(line);
    }
}

void TransportGuide::AddBusRoutes() {
    if (_buffer.empty()) { return; }

    for (string_view bus_route_entry: _buffer) {
        const auto& [bus_route_name, stop_names] = ParseBusRouteView(bus_route_entry);
        _bus_routes.emplace_back(stop_names.size());
        BusRoute& current_bus_route = _bus_routes.back();
        _bus_name_to_bus_route[bus_route_name] = &current_bus_route;

        unordered_set<string_view> unique_stops;

        for (size_t i = 0; i < stop_names.size(); ++i) {
            current_bus_route[i] = _stop_name_to_stop_ptr[stop_names[i]];
            unique_stops.insert(stop_names[i]);
            _stop_name_to_bus_routes[stop_names[i]].insert(bus_route_name);
        }

        vector<double> geo_distances;
        geo_distances.reserve(stop_names.size() - 1);

        vector<unsigned long> road_distances;
        road_distances.reserve(stop_names.size() - 1);

        for (size_t i = 0, j = 1; j < stop_names.size(); ++i, ++j) {
            Stop* lhs = current_bus_route[i];
            Stop* rhs = current_bus_route[j];

            geo_distances.push_back(acos(sin(lhs->latitude_rad) * sin(rhs->latitude_rad) +
                                         cos(lhs->latitude_rad) * cos(rhs->latitude_rad) *
                                         cos(abs(lhs->longitude_rad - rhs->longitude_rad)))
                                    * 6'371'000);

            if (!_distances_table.empty()) {
                auto it = _distances_table.find({lhs, rhs});
                if (it != _distances_table.end()) {
                    road_distances.push_back(it->second);
                } else {
                    ostringstream oss;
                    oss << "Distance for pair of stops: {"
                        << lhs->stop_name << ", " << rhs->stop_name << "} not found."
                        << endl;
                    throw std::logic_error(oss.str());
                }
            }
        }

        double length_by_coordinates = accumulate(geo_distances.begin(),
                                                  geo_distances.end(),
                                                  0.0);
        unsigned long length_by_roads = accumulate(road_distances.begin(),
                                                   road_distances.end(),
                                                   0ul);

        _bus_name_to_bus_route_stats[bus_route_name] = BusRouteStats{
                stop_names.size(),
                unique_stops.size(),
               {length_by_coordinates, length_by_roads}
        };
    }
}

void TransportGuide::PrintBusRouteInfo(std::string_view bus_route_name, std::ostream& out) const {
    if (_bus_name_to_bus_route.count(bus_route_name)) {
        const BusRouteStats& bus_stats = GetStatsForBusRoute(bus_route_name);
        out << "Bus " << bus_route_name << ": "
            << bus_stats.stops_count << " stops on route, "
            << bus_stats.unique_stops_count << " unique stops, "
            << std::setprecision(6) << bus_stats.lengths.roads_length << " route length, "
            << std::setprecision(7) << (double(bus_stats.lengths.roads_length) / bus_stats.lengths.geo_length) << " curvature"
            << endl;
    } else {
        out << "Bus " << bus_route_name
            << ": not found"
            << endl;
    }
}

void TransportGuide::PrintStopInfo(std::string_view stop_name, std::ostream& out) const {
    if (0 == _stop_name_to_stop_ptr.count(stop_name)) {
        out << "Stop " << stop_name
            << ": not found"
            << endl;
    } else {
        if (0 == _stop_name_to_bus_routes.count(stop_name)) {
            out << "Stop " << stop_name
                << ": no buses"
                << endl;
        } else {
            out << "Stop " << stop_name
                << ": buses";
            for (const auto bus_name: _stop_name_to_bus_routes.at(stop_name)) {
                out << " " << bus_name;
            }
            out << endl;
        }
    }
}

void TransportGuide::ProcessRetrievalQueries(std::istream& in, std::ostream& out) {
    size_t retrieve_queries_count;
    in >> retrieve_queries_count >> std::ws;

    string line{};
    for (size_t i = 0; i < retrieve_queries_count; ++i) {
        getline(in, line);
        const auto& [command, arg] = SplitIntoTwoPartsView(line, " ");

        switch (str_to_retrieve_command[command]) {
            case RetrieveCommand::Bus:
                PrintBusRouteInfo(arg, out);
                break;
            case RetrieveCommand::Stop: {
                PrintStopInfo(arg, out);
                break;
            }
        }
    }
}

const deque<Stop>& TransportGuide::GetStops() const {
    return _stops;
}

const vector<string>& TransportGuide::GetBuffer() const {
    return _buffer;
}

const TransportGuide::StopNameToStopPtr& TransportGuide::GetStopNameToStopPtr() const {
    return _stop_name_to_stop_ptr;
}

const deque<TransportGuide::BusRoute>& TransportGuide::GetBusRoutes() const {
    return _bus_routes;
}

const TransportGuide::BusNameToBusRoute& TransportGuide::GetBusNameToBusRouteMapping() const {
    return _bus_name_to_bus_route;
}

size_t TransportGuide::GetUniqueStopsCountForBusRoute(string_view bus_route_name) const {
    return _bus_name_to_bus_route_stats.at(bus_route_name).unique_stops_count;
}

const BusRouteStats& TransportGuide::GetStatsForBusRoute(string_view bus_route_name) const {
    return _bus_name_to_bus_route_stats.at(bus_route_name);
}

const TransportGuide::DistancesTable& TransportGuide::GetDistancesTable() const {
    return _distances_table;
}

// parse string in the following of 2 formats:
// 1) "stop_name: latitude_deg, longitude_deg"
//     Tolstopaltsevo: 55.611087, 37.20829
// 2) "stop_name: latitude_deg, longitude_deg DISTANCEm to other_stop_name"
pair<Stop, optional<TransportGuide::EndStopsToDistances>> TransportGuide::ParseAddStopQuery(string_view s) {
    auto [stop_name_from, rest_part] = SplitIntoTwoPartsView(s, ":");
    vector<string_view> separated_by_comma = SplitBy(rest_part, ",");
    string_view latitude = separated_by_comma[0];
    string_view longitude = separated_by_comma[1];

    if (separated_by_comma.size() == 2u) {  // stop contains just "latitude" and "longitude"
        return {Stop{string(stop_name_from),
                     stod(string{latitude}),
                     stod(string{longitude})},
                nullopt};
    } else if (separated_by_comma.size() > 2u) {
        EndStopsToDistances end_stops_to_distances;

        for (size_t i = 2; i < separated_by_comma.size(); ++i) {
            auto [suffixed_distance, stop_name_to] = SplitIntoTwoPartsView(separated_by_comma[i], string("to"));
            if (auto it1 = _stop_name_to_stop_ptr.find(stop_name_to);
                    it1 != _stop_name_to_stop_ptr.end()) {
                stop_name_to = it1->first;
            } else {
                auto [it2, is_inserted] = _stop_names.insert(string(stop_name_to));
                stop_name_to = *it2;
            }

            suffixed_distance.remove_suffix(1);  // remove letter 'm' at the end of distance
            auto distance = static_cast<size_t>(std::stoi(string(suffixed_distance)));

            end_stops_to_distances.insert({stop_name_to, distance});
        }

        return {Stop{string(stop_name_from),
                     stod(string{latitude}),
                     stod(string{longitude})},
                std::move(end_stops_to_distances)};
    } else {
        throw std::logic_error("Wrong format for adding stop: " + string(stop_name_from));
    }
}

void TransportGuide::ProcessCreateQuery(std::string_view s) {
    s = trim_view(s);
    size_t first_space_pos = s.find_first_of(' ');
    string_view command = s.substr(0, first_space_pos);
    s.remove_prefix(first_space_pos);
    s = trim_view(s);

    switch (str_to_create_command[command]) {
        case CreateCommand::Bus:
            // складываем во временный буфер предобработанные запросы на создание маршрута (без слова "Bus")
            _buffer.emplace_back(s);
            break;
        case CreateCommand::Stop: {
            const auto& [stop_from, end_stop_names_to_distances] = ParseAddStopQuery(s);
            _stops.push_back(stop_from);
            _stop_name_to_stop_ptr.insert({_stops.back().stop_name, &(_stops.back())});

            // Ниже происходит ПРЕДВАРИТЕЛЬНОЕ заполнение таблицы расстояний между остановками, поскольку
            // в данный момент информация о второй остановке остановочного интервала (stop_to) скорее всего недоступна
            // т.е. еще не было запроса на добавление остановки "stop_to"
            if (end_stop_names_to_distances) {
                for (auto [stop_name_to, distance]: *end_stop_names_to_distances) {
                    _pair_stop_names_view_to_distance.insert({{_stops.back().stop_name, stop_name_to}, distance});
                }
            }
            break;
        }
    }
}

void TransportGuide::CreateDistancesTable() {
    for (auto [stop_names_pair, distance]: _pair_stop_names_view_to_distance) {
        string_view stop_name_from = stop_names_pair.first;
        string_view stop_name_to = stop_names_pair.second;

        auto it_from = _stop_name_to_stop_ptr.find(stop_name_from);
        auto it_to = _stop_name_to_stop_ptr.find(stop_name_to);

        if (it_from != _stop_name_to_stop_ptr.end() &&
            it_to   != _stop_name_to_stop_ptr.end()) {
            _distances_table.insert({{it_from->second, it_to->second}, distance});

            if (0 == _pair_stop_names_view_to_distance.count({it_to->first, it_from->first})) {
                _distances_table.insert({{it_to->second, it_from->second}, distance});
            }
        } else {
            throw std::logic_error("Distance between "
                                   + string(stop_name_from) + " and " + string(stop_name_to) +
                                   " can not be calculated\n");
        }
    }
}

pair<string_view, vector<string_view>> TransportGuide::ParseBusRouteView(std::string_view bus_route_entry) {
    auto [bus_route_name, delimited_stops] = SplitIntoTwoPartsView(bus_route_entry, ":");
    delimited_stops = trim_view(delimited_stops);

    size_t first_delim_pos = delimited_stops.find_first_of(">-");
    if (first_delim_pos == string_view::npos) { return {}; }

    const string separator = string(1, delimited_stops[first_delim_pos]);
    vector<string_view> stops = SplitBy(delimited_stops, separator);

    // in case of circular bus route: A - B - C <=> A > B > |C| > B > A
    if (separator == "-") {
        stops.resize(stops.size() * 2 - 1);
        std::copy_backward(next(stops.rbegin()), stops.rend(),
                           stops.end());
    }

    return {bus_route_name, stops};
}

void TransportGuide::ProcessCreationQueries(std::istream& in) {
    AddStops(in);
    CreateDistancesTable();
    AddBusRoutes();
}
