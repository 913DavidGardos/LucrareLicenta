#pragma once
#include "MeasurementCollector.h"
#include <fstream>
#include <iostream>
#include <chrono>



class FileManager
{
public:
	FileManager() = default;
	void storeToFile(MeasurementCollector& measureCollector);
};

