#pragma once
#include <map>
#include <string>

/**
 * \brief Colector pentru masuratori.
 */
class MeasurementCollector
{
public:
    /**
     * \brief Constructorul clasei MeasurementCollector.
     */
    MeasurementCollector() = default;

    /**
     * \brief Insereaza un timer in colector.
     * \param name Numele timerului.
     * \param time Durata timerului in secunde.
     * \param count Numarul de masuratori.
     */
    void insertTimer(std::string name, double time, int count);

    /**
     * \brief Obtine toate timerele din colector.
     * \return Un map cu toate timerele, avand numele ca cheie si o pereche de valori (numarul de masuratori, durata) ca valoare.
     */
    std::map<std::string, std::pair<int, double>>& getTimers();

    /**
     * \brief Insereaza o dimensiune in colector.
     * \param name Numele dimensiunii.
     * \param size Dimensiunea in octeti.
     * \param numberOfItems Numarul de elemente.
     */
    void insertSize(std::string name, size_t size, int numberOfItems);

    /**
     * \brief Obtine toate dimensiunile din colector.
     * \return Un map cu toate dimensiunile, avand numele ca cheie si o pereche de valori (numarul de elemente, dimensiunea) ca valoare.
     */
    std::map<std::string, std::pair<int, size_t>>& getSizes();

private:
    std::map<std::string, std::pair<int, double>> storeTimers;  ///< Map pentru stocarea timerelelor.
    std::map<std::string, std::pair<int, size_t>> storeSizes;   ///< Map pentru stocarea dimensiunilor.
};