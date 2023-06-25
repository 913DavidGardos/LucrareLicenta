#pragma once
#include <chrono>
#include <string>
#include "MeasurementCollector.h"

/// \class Timer
/// \brief O clasa pentru masurarea timpului de executie al unei functii.
///
/// Clasa Timer furnizeaza functionalitate pentru masurarea timpului de executie al unei functii. 
/// Ea utilizeaza ceasul cu rezolutie inalta pentru a inregistra momentul de inceput cand obiectul Timer este construit.
/// Durata poate fi ulterior calculata prin compararea momentului de inceput cu momentul de sfarsit.
class Timer {

public:
    /// \brief Constructor pentru obiectele Timer.
    ///
    /// Acest constructor initializeaza un obiect Timer cu numele functiei si harta de stocare furnizate.
    ///
    /// \param functionName Numele functiei care este cronometrata.
    /// \param measureCollector Referinta la harta unde va fi stocata durata.
    Timer(std::string functionName, MeasurementCollector& measureCollector, int);

    /// \brief Destructor pentru obiectele Timer.
    ///
    /// Destructorul calculeaza durata si o stocheaza in harta de stocare.
    ~Timer();

private:
    std::string fnName; ///< Numele functiei care este cronometrata.
    MeasurementCollector& measureCollector; ///< Referinta la harta unde va fi stocata durata.
    std::chrono::time_point<std::chrono::high_resolution_clock> start; ///< Momentul de inceput pentru masurarea duratei.
    int nOfParticles;
};
