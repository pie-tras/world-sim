#pragma once

#include "Global.h"
#include <chrono>

enum class TimeOutput {
    MICROSECOND, MILLISECOND, SECOND
};

class ScopedTimer {
private:
    std::string name;
    TimeOutput output;

    std::chrono::time_point<std::chrono::high_resolution_clock> startTimepoint;
public:
    ScopedTimer(const std::string& name, TimeOutput output);
    ~ScopedTimer();
};

class Timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime, stopwatchStart;
public:
    Timer();
    void setStopwatch();
    long long getStopwatch();
    long long getDelta();
};