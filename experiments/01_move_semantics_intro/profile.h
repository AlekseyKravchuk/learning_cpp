#pragma once

#include <chrono>
#include <iostream>
#include <utility>
#include <sstream>

class LogDuration {
    std::chrono::steady_clock::time_point start;
    std::string _message;
    bool is_blocked_output = false;

public:
    LogDuration(const std::string& message = "",
                bool is_blocked = false) :
        start(std::chrono::steady_clock::now()), _message(message + ": "), is_blocked_output(is_blocked) {}

    auto GetCurrentTime() const {
        namespace chr = std::chrono;
        auto end = chr::steady_clock::now();
        auto duration = end - start;
        return chr::duration_cast<chr::milliseconds>(duration).count();
    }

    ~LogDuration() { 
        if (!is_blocked_output) {
            std::cerr << _message
                      << GetCurrentTime()  << " ms" << std::endl;
        }
    }
};


class LogDurationCallback : public LogDuration {
    double& callback_time;
public:
    LogDurationCallback(double& callable)
        : LogDuration("", true), callback_time(callable) {}

    ~LogDurationCallback() {
        callback_time = GetCurrentTime();
    }
};

class TotalDuration {
public:
    std::string message;
    std::chrono::steady_clock::duration value;

    explicit TotalDuration(std::string msg = " ")
        : message(std::move(msg)), value (0) {}

    ~TotalDuration() {
        std::ostringstream os;
        os << message
           << std::chrono::duration_cast<std::chrono::milliseconds>(value).count()
           << " ms" << std::endl;
        std::cerr << os.str();
    }
};

class AddDuration {
    std::chrono::steady_clock::duration& add_to;
    std::chrono::steady_clock::time_point start;
public:
    AddDuration(std::chrono::steady_clock::duration& dest)
        : add_to(dest), start(std::chrono::steady_clock::now()) {}

    AddDuration(TotalDuration& dest)
        : AddDuration(dest.value) {}

    ~AddDuration() {
        add_to += std::chrono::steady_clock::now() - start;
    }
};

#define GENERATE(x) a_local_var_ ## x
#define PROXY(x) GENERATE(x)
#define UNIQ_ID PROXY(__LINE__)

#define LOG_DURATION(message) LogDuration UNIQ_ID{message};
#define ADD_DURATION(dest)    AddDuration UNIQ_ID{dest};
