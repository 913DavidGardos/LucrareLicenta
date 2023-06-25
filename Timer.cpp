#include "Timer.h"
#include <iostream>

Timer::Timer(std::string functionName, MeasurementCollector& measureCollector, int nOfParticles) : fnName(functionName), measureCollector(measureCollector), nOfParticles(nOfParticles)
{
	start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
	auto end = std::chrono::high_resolution_clock::now();
	// Calculate the duration in milliseconds
	std::chrono::duration<double, std::milli> duration = end - start;
	measureCollector.insertTimer(fnName, static_cast<double>(duration.count()), nOfParticles);

}