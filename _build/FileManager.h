#pragma once
#include "MeasurementCollector.h"
#include <fstream>
#include <iostream>
#include <chrono>

/**
 * @brief Clasa FileManager gestioneaza operatiile de gestionare a fisierelor.
 */
class FileManager
{
public:
    /**
     * @brief Constructorul implicit pentru clasa FileManager.
     */
    FileManager() = default;

    /**
     * @brief Stocheaza datele de masurare intr-un fisier.
     *
     * Aceasta functie primeste un obiect de tip MeasurementCollector ca parametru si stocheaza
     * masuratorile colectate intr-un fisier. Formatul specific al fisierului si implementarea de
     * stocare pot varia.
     *
     * @param measureCollector Obiectul MeasurementCollector care contine masuratorile de stocat.
     */
    void storeToFile(MeasurementCollector& measureCollector);
};