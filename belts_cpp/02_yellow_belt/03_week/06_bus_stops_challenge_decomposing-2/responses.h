#pragma once

#include <map>
#include <ostream>  // std::endl, std::ostream
#include <string>
#include <utility>  // std::pair
#include <vector>
#include <sstream>  // std::ostringstream

using namespace std::literals;

struct BusesForStopResponse {
    std::vector<std::string> buses_for_stop;
};

struct StopsForBusResponse {
    std::vector<std::pair<std::string, std::vector<std::string>>> stops_for_bus;
};

struct AllBusesResponse {
    std::map<std::string, std::vector<std::string>> response;
};

std::ostream& operator<<(std::ostream& os, const BusesForStopResponse& r);

std::ostream& operator<<(std::ostream& os, const StopsForBusResponse& r);

std::ostream& operator<<(std::ostream& os, const AllBusesResponse& r);
