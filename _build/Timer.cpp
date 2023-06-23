#include "Timer.h"
#include <iostream>

/// \brief Constructor for Timer objects.
///
/// This constructor initializes a Timer object with the provided function name and storage map.
/// It records the start time point using the high-resolution clock.
///
/// \param functionName The name of the function being timed.
/// \param storage A reference to the map where the duration will be stored.
Timer::Timer(std::string functionName, MeasurementCollector& measureCollector, int nOfParticles) : fnName(functionName), measureCollector(measureCollector), nOfParticles(nOfParticles)
{
	start = std::chrono::high_resolution_clock::now();
}

/// \brief Destructor for Timer objects.
///
/// The destructor calculates the duration by comparing the start time point with the current time point.
/// It stores the duration in the storage map using the function name as the key.
Timer::~Timer()
{
	auto end = std::chrono::high_resolution_clock::now();
	// Calculate the duration in milliseconds
	std::chrono::duration<double, std::milli> duration = end - start;
	measureCollector.insertTimer(fnName, static_cast<double>(duration.count()), nOfParticles);

}