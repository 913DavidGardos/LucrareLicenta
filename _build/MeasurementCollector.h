#pragma once
#include <map>
#include <string>

class MeasurementCollector
{
public:
	MeasurementCollector() = default;
	void insertTimer(std::string, double, int);
	std::map<std::string, std::pair<int, double>>& getTimers();
	void insertSize(std::string, size_t, int numberOfItems);
	std::map<std::string, std::pair<int, size_t>>& getSizes();

private:
	std::map<std::string, std::pair<int, double>> storeTimers;
	std::map<std::string, std::pair<int, size_t>> storeSizes;
};

