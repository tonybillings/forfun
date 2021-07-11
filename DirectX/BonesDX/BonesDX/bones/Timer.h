#pragma once

#include <chrono>
#include <iomanip>

class Timer
{
public:
	Timer();
	
public:
	float Mark();
	float Peek() const;

private:
	std::chrono::steady_clock::time_point _last;
};