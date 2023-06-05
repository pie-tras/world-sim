#include "Timer.h"

ScopedTimer::ScopedTimer(const std::string& name, TimeOutput output) : name(name), output(output) {
	startTimepoint = std::chrono::high_resolution_clock::now();
}

ScopedTimer::~ScopedTimer() {
	std::chrono::time_point<std::chrono::high_resolution_clock> endTimepoint = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

	auto duration = end - start;
	double print_time = -1.0;
	std::string format;

	switch (output) {
	case TimeOutput::MICROSECOND:
		print_time = (double)duration;
		format = "us";
		break;
	case TimeOutput::MILLISECOND:
		print_time = duration * 0.001;
		format = "ms";
		break;
	case TimeOutput::SECOND:
		print_time = duration * 0.000001;
		format = "s";
		break;
	default:
		break;
	}

	std::cout << "[" << name << "]: " << print_time << format << "\n";
}

Timer::Timer() {
	lastTime = std::chrono::high_resolution_clock::now();
	stopwatchStart = lastTime;
}

void Timer::setStopwatch() {
	stopwatchStart = std::chrono::high_resolution_clock::now();
}

long long Timer::getStopwatch() {
	std::chrono::time_point<std::chrono::high_resolution_clock> stopwatchEnd = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(stopwatchStart).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(stopwatchEnd).time_since_epoch().count();

	return end - start;
}

long long Timer::getDelta() {
	std::chrono::time_point<std::chrono::high_resolution_clock> currentTime = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(lastTime).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(currentTime).time_since_epoch().count();

	lastTime = currentTime;
	return end - start;
}