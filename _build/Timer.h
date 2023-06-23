#pragma once
#include <chrono>
#include <string>
#include "MeasurementCollector.h"

/// \class Timer
/// \brief A class for measuring the execution time of a function.
///
/// The Timer class provides functionality to measure the execution time of a function. It uses
/// the high-resolution clock to record the start time point when the Timer object is constructed.
/// The duration can later be calculated by comparing the start time point with the end time point.
class Timer {

public:
	/// \brief Constructor for Timer objects.
	///
	/// This constructor initializes a Timer object with the provided function name and storage map.
	///
	/// \param functionName The name of the function being timed.
	/// \param storage A pointer to the map where the duration will be stored.
	Timer(std::string functionName, MeasurementCollector& measureCollector, int);
	/// \brief Destructor for Timer objects.
	///
	/// The destructor calculates the duration and stores it in the storage map.
	~Timer();
private:
	std::string fnName; ///< The name of the function being timed.
	MeasurementCollector& measureCollector; ///< Reference to the map where the duration will be stored.
	std::chrono::time_point<std::chrono::high_resolution_clock> start; ///< The start time point for measuring the duration.
	int nOfParticles;
};