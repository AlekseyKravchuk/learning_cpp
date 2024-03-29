#pragma once

#include <algorithm>  // std::string::find_last_not_of
#include <map>
#include <string>  // std::getline
#include <string_view>

#include "http_request.h"
#include "stats.h"
#include "test_runner.h"

Stats ServeRequests(std::istream& input);
