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

    for (string_view bus_route_entry : _buffer) {
        const auto& [bus_route_name, stop_names] = ParseBusRouteView(bus_route_entry);
        _bus_routes.emplace_back(stop_names.size());
        BusRoute& current_bus_route = _bus_routes.back();
        _bus_name_to_bus_route[bus_route_name] = &current_bus_route;

        unordered_set<string_view> unique_stops;
        vector<double> distances;
        for (size_t i = 0; i < stop_names.size(); ++i) {
            current_bus_route[i] = _stop_name_to_stop_ptr[stop_names[i]];
            unique_stops.insert(stop_names[i]);
            _stop_name_to_bus_routes[stop_names[i]].insert(bus_route_name);
        }

        for (size_t i = 0, j = 1; j < stop_names.size(); ++i, ++j) {
            Stop* lhs{current_bus_route[i]};
            Stop* rhs{current_bus_route[j]};
            distances.push_back(acos(sin(lhs->latitude_rad) * sin(rhs->latitude_rad) +
                                     cos(lhs->latitude_rad) * cos(rhs->latitude_rad) *
                                     cos(abs(lhs->longitude_rad - rhs->longitude_rad)))
                                * 6'371'000);
        }

        double bus_route_length = accumulate(distances.begin(),
                                             distances.end(),
                                             0.0);

        _bus_name_to_bus_route_stats[bus_route_name] = BusRouteStats{
                stop_names.size(),
                unique_stops.size(),
                bus_route_length
        };
    }
}

void TransportGuide::PrintBusRouteInfo(std::string_view bus_route_name, std::ostream& out) const {
    if (_bus_name_to_bus_route.count(bus_route_name)) {
        const BusRouteStats& bus_stats = GetStatsForBusRoute(bus_route_name);
        out << "Bus " << bus_route_name << ": "
             << bus_stats.stops_count << " stops on route, "
             << bus_stats.unique_stops_count << " unique stops, "
             << std::setprecision(6) << bus_stats.bus_route_length << " route length"
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
        const auto& [command, arg] = SplitIntoTwoPartsView(line, ' ');

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
            auto [stop, optional_pair] = ParseAddStopQuery(s);
            _stops.push_back(stop);
            _stop_name_to_stop_ptr.insert({_stops.back().stop_name, &(_stops.back())});

            // TODO: реализовать заполнение таблицы расстояний между остановками
            if (optional_pair) {

//                _stops_pair_to_distance
            }
            break;
        }
    }
}

pair<string_view, vector<string_view>> TransportGuide::ParseBusRouteView(std::string_view bus_route_entry) {
    auto [bus_route_name, delimited_stops] = SplitIntoTwoPartsView(bus_route_entry, ':');
    delimited_stops = trim_view(delimited_stops);

    size_t first_delim_pos = delimited_stops.find_first_of(">-");
    if (first_delim_pos == string_view::npos) { return {}; }

    const char separator = delimited_stops[first_delim_pos];
    vector<string_view> stops = SplitBy(delimited_stops, separator);

    // in case of circular bus route: A - B - C <=> A > B > |C| > B > A
    if (separator == '-') {
        stops.resize(stops.size() * 2 - 1);
        std::copy_backward(next(stops.rbegin()), stops.rend(),
                           stops.end());
    }

    return {bus_route_name, stops};
}

void TransportGuide::CreateDB(std::istream& in) {
    AddStops(in);
    AddBusRoutes();
}

