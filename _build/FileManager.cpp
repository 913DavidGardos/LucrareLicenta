#include "FileManager.h"



void FileManager::storeToFile(MeasurementCollector& measureCollector)
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Convert the time to a string
    long long int timeNumber = static_cast<long long int>(currentTime);

    // Open the file for writing
    std::string path = "Measurements/measurement_" + std::to_string(timeNumber) + ".csv";
    std::cout<<path<<"\n";
    std::ofstream file(path, std::ios::out);

    // Check if the file was successfully opened
    if (!file) 
    {
        std::cerr << "Failed to open the file." << std::endl;
    }

    // Iterate over the map and write each key-value pair to the file
    auto sizes = measureCollector.getSizes();
    file << "Function Name, Number of Items, Execution time (miliseconds), Memory space (kilobytes)" << std::endl;
    for (const auto& pair : measureCollector.getTimers())
    {
        double space = sizes[pair.first].second/1000.f;
        file << pair.first << ", " << pair.second.first<<", " << pair.second.second <<", "<< space << std::endl;
    }

  /*  for (const auto& pair : measureCollector.getSizes()) 
    {
        file << pair.first << ", " << pair.second.first << ", " << pair.second.second << std::endl;
    }*/

    // Close the file
    file.close();

}
