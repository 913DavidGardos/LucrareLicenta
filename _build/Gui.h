#pragma once
#include "raylib.h"
#include "ParticleManager.h"
#include <chrono>
#include <string>
#include <iostream>
#define MAX_OPTIONS 5
#define ALGO_OPTIONS 3
#define MAX_INPUT_LENGTH 5

/**
 * \brief Starea programului.
 */
enum class ProgramState
{
    MainMenu,                ///< Meniul principal.
    Simulation,              ///< Simularea in desfasurare.
    Paused,                  ///< Simularea este in pauza.
    AlgoMenu,                ///< Meniul algoritmilor.
    Lines,                   ///< Optiunea pentru afisarea liniilor.
    VelocityOfParticles,     ///< Optiunea pentru viteza particulelor.
    NumberOfParticles,       ///< Optiunea pentru numarul de particule.
    ExitToDesktop            ///< Iesirea la desktop.
};

/**
 * \brief Interfata grafica a aplicatiei.
 */
class Gui
{
public:
    /**
     * \brief Constructorul clasei Gui.
     * \param pm Referinta la ParticleManager.
     */
    Gui(ParticleManager& pm);

    /**
     * \brief Destructorul clasei Gui.
     */
    ~Gui();

    /**
     * \brief Ruleaza interfata grafica.
     */
    void Run();

    /**
     * \brief Preia valoarea pentru numarul de particule introdusa de utilizator.
     */
    void takeInputNumberOfParticles();

    /**
     * \brief Deseneaza caseta de text pentru introducerea numarului de particule.
     */
    void drawTextBoxNr();

    /**
     * \brief Deseneaza caseta de text pentru introducerea valorii.
     */
    void drawTextBox();

    /**
     * \brief Preia valoarea pentru viteza introdusa de utilizator.
     */
    void takeInputVelocity();

    /**
     * \brief Actualizeaza optiunile algoritmului.
     */
    void updateAlgoOptions();

    /**
     * \brief Deseneaza optiunile algoritmului.
     */
    void drawAlgoOptions();

    /**
     * \brief Verifica intrarile utilizatorului.
     */
    void checkInput();

    /**
     * \brief Actualizeaza optiunile meniului.
     */
    void updateMenuOptions();

    /**
     * \brief Deseneaza optiunile meniului.
     */
    void drawMenuOptions();

    /**
     * \brief Deseneaza butonul meniului.
     */
    void drawMenuButton();

private:
    /**
     * \brief Actualizeaza starea interfetei grafice.
     */
    void update();

    /**
     * \brief Deseneaza interfata grafica.
     */
    void draw();

    int screenWidth;                            ///< Latimea ecranului.
    int screenHeight;                           ///< Inaltimea ecranului.
    bool isPaused;                              ///< Indicator pentru pauza.
    ProgramState programState;                   ///< Starea programului.
    ParticleManager& pm;                        ///< Referinta la ParticleManager.
    double previousTime;                        ///< Timpul anterior.
    double currentTime;                         ///< Timpul curent.

    const char* options[MAX_OPTIONS] = {
        "Algoritmi",
        "Linii Pornite / Oprite",
        "Viteza Particulelor",
        "Numarul de Particule",
        "Iesire la Desktop"
    };                                         ///< Optiunile meniului principal.

    const char* algoOptions[ALGO_OPTIONS] = {
        "Quad Tree",
        "Grid",
        "Volum Delimitator"
    };                                          ///< Optiunile meniului algoritmilor.

    int selectedOption = -1;                     ///< Optiunea selectata.
    int keyOption = -1;                          ///< Optiunea selectata prin tastatura.
    int mouseOption = -1;                        ///< Optiunea selectata prin mouse.
    Rectangle optionRectangles[MAX_OPTIONS];     ///< Rectangular pentru optiunile meniului principal.
    Rectangle algoRectangles[ALGO_OPTIONS];       ///< Rectangular pentru optiunile meniului algoritmilor.
    Rectangle buttonRectangle = { 20, screenHeight - 70.f, 120, 50 };  ///< Rectangular pentru butonul meniului.

    std::string inputVelocityBox;               ///< Caseta de text pentru introducerea valorii pentru viteza.
    std::string inputNumberBox;                 ///< Caseta de text pentru introducerea valorii pentru numarul de particule.
    bool inputVelocity = false;                 ///< Indicator pentru introducerea valorii pentru viteza.
    bool inputN = false;                         ///< Indicator pentru introducerea valorii pentru numarul de particule.
};
