#pragma once
#include "ParticleManager.h"
#include "Gui.h"
#include <sstream>
#include <memory>

class Ui
{
public:
    /// \brief Constructor pentru obiectele Ui.
    ///
    /// Acest constructor initializeaza un obiect Ui cu referinta catre ParticleManager.
    ///
    /// \param pm Referinta catre ParticleManager.
    Ui(ParticleManager& pm);

    /// \brief Divizeaza un sir de caractere in functie de un delimitator.
    ///
    /// Aceasta functie primeste un sir de caractere si un delimitator si il imparte intr-un vector de subsiruri.
    ///
    /// \param input Sirul de caractere de divizat.
    /// \param delimiter Delimitatorul folosit pentru impartire.
    /// \return Vectorul rezultat in urma impartirii.
    std::vector<std::string> tokenizeString(const std::string& input, char delimiter);

    /// \brief Executa comenzile specifice quadtree.
    ///
    /// Aceasta functie primeste un vector de subsiruri reprezentand comenzile specifice quadtree
    /// si le executa in contextul ParticleManager-ului.
    ///
    /// \param tokens Vectorul de subsiruri reprezentand comenzile.
    void quadTreeCommands(std::vector<std::string>& tokens);

    /// \brief Executa comenzile specifice BVH.
    ///
    /// Aceasta functie primeste un vector de subsiruri reprezentand comenzile specifice BVH
    /// si le executa in contextul ParticleManager-ului.
    ///
    /// \param tokens Vectorul de subsiruri reprezentand comenzile.
    void bvhCommands(std::vector<std::string>& tokens);

    /// \brief Executa comenzile specifice grid.
    ///
    /// Aceasta functie primeste un vector de subsiruri reprezentand comenzile specifice grid
    /// si le executa in contextul ParticleManager-ului.
    ///
    /// \param tokens Vectorul de subsiruri reprezentand comenzile.
    void gridCommands(std::vector<std::string>& tokens);

    /// \brief Executa comenzile specifice help.
    ///
    /// Aceasta functie primeste un vector de subsiruri reprezentand comenzile specifice help
    /// si le executa in contextul ParticleManager-ului.
    ///
    /// \param tokens Vectorul de subsiruri reprezentand comenzile.
    void helpCommands(std::vector<std::string>& tokens);

    /// \brief Executa comenzile specifice start.
    ///
    /// Aceasta functie primeste un vector de subsiruri reprezentand comenzile specifice start
    /// si le executa in contextul ParticleManager-ului.
    ///
    /// \param tokens Vectorul de subsiruri reprezentand comenzile.
    void startCommands(std::vector<std::string>& tokens);

    /// \brief Executa comenzile specifice gui.
    ///
    /// Aceasta functie primeste un vector de subsiruri reprezentand comenzile specifice gui
    /// si le executa in contextul ParticleManager-ului.
    ///
    /// \param tokens Vectorul de subsiruri reprezentand comenzile.
    void guiCommands(std::vector<std::string>& tokens);

    /// \brief Executa comenzile specifice test.
    ///
    /// Aceasta functie primeste un vector de subsiruri reprezentand comenzile specifice test
    /// si le executa in contextul ParticleManager-ului.
    ///
    /// \param tokens Vectorul de subsiruri reprezentand comenzile.
    void testCommands(std::vector<std::string>& tokens);

    /// \brief Ruleaza interfata utilizatorului.
    ///
    /// Aceasta functie ruleaza interfata utilizatorului si primeste comenzile de la utilizator.
    void run();

private:
    ParticleManager& pm; ///< Referinta catre ParticleManager.
    int screenWidth, screenHeight; ///< Dimensiunile ecranului.
    std::unique_ptr<Gui> gui; ///< Pointer unic catre obiectul Gui.
    float previousTime, currentTime; ///< Timpul anterior si timpul curent.
};

