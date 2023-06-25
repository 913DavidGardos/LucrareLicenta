#include "MeasurementCollector.h"

void MeasurementCollector::insertTimer(std::string fnName, double time, int noItems)
{
	fnName += std::to_string(noItems);
	storeTimers.insert(std::make_pair(fnName, std::make_pair(noItems, time)));
}

std::map<std::string, std::pair<int, double>>& MeasurementCollector::getTimers()
{
	return storeTimers;
}

void MeasurementCollector::insertSize(std::string fnName, size_t size, int numberOfItems)
{
	fnName += std::to_string(numberOfItems);
	storeSizes.insert(std::make_pair(fnName, std::make_pair(numberOfItems, size)));
}

std::map<std::string, std::pair<int, size_t>>& MeasurementCollector::getSizes()
{
	return storeSizes;
}