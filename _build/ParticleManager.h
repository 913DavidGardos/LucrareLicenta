#pragma once

#include <vector>
#include <raylib.h>
#include "ParticleInterface.h"
#include "Particle.h"
#include "BvhContainer.h"
#include <random>
#include <memory>
#include <utility>
#include "QuadTreeContainer.h"
#include "GridContainer.h"
#include "MeasurementCollector.h"


/**
 * \enum Algo
 * \brief Defineste diferite algoritmi pentru ParticleManager.
 */
enum class Algo
{
    QuadTree, ///< Algoritmul QuadTree.
    Grid,     ///< Algoritmul Grid.
    BoundingVolume ///< Algoritmul de ierarhie a volumelor marginale.
};

/**
 * \class ParticleManager
 * \brief Gestionarea particulelor si furnizarea operatiilor pe acestea.
 *
 * Clasa ParticleManager este responsabila de gestionarea particulelor,
 * de initializarea lor, de actualizarea starii si de desenarea lor
 * folosind diferiti algoritmi.
 */
class ParticleManager
{
public:
    /**
     * \brief Constructor.
     *
     * \param screenWidth Latimea ecranului.
     * \param screenHeight Inaltimea ecranului.
     * \param measurementCollector Referinta la MeasurementCollector.
     */
    ParticleManager(int screenWidth, int screenHeight, MeasurementCollector& measurementCollector);

    /**
     * \brief Initializeaza particulele.
     *
     * \param numberOfParticles Numarul de particule de initializat.
     */
    void InitParticles(int numberOfParticles);

    /**
     * \brief Deseneaza particulele.
     */
    void drawParticles();

    /**
     * \brief Actualizeaza numarul de particule.
     *
     * \param nParticles Noul numar de particule.
     */
    void updateNumberOfParticles(int nParticles);

    /**
     * \brief Obtine particulele QuadTree.
     *
     * \return Particulele QuadTree.
     */
    const StaticQuadTreeContainer<Particle>& getQuadTreeParticles();

    /**
     * \brief Actualizeaza particulele.
     *
     * \param deltaT Pasul de timp pentru actualizare.
     */
    void updateParticles(float deltaT);

    /**
     * \brief Actualizeaza viteza particulelor.
     *
     * \param newVelocity Noua viteza pentru particule.
     */
    void updateParticleVelocity(float newVelocity);

    /**
     * \brief Comuta afisarea liniilor pentru particule.
     */
    void toggleLines();

    /**
     * \brief Porneste algoritmul QuadTree.
     */
    void startQuadTree();

    /**
     * \brief Porneste algoritmul Grid.
     */
    void startGrid();

    /**
     * \brief Porneste algoritmul de ierarhie a volumelor marginale.
     */
    void startBoundingVolume();

    /**
     * \brief Obtine latimea ecranului.
     *
     * \return Latimea ecranului.
     */
    int getScreenWidth();

    /**
     * \brief Obtine inaltimea ecranului.
     *
     * \return Inaltimea ecranului.
     */
    int getScreenHeight();

private:
    /**
     * \brief Genereaza o particula.
     */
    void generateParticle();

    /**
     * \brief Deseneaza particulele folosind algoritmul QuadTree.
     */
    void drawWithQuadTree();

    /**
     * \brief Actualizeaza particulele folosind algoritmul QuadTree.
     *
     * \param deltaT Pasul de timp pentru actualizare.
     */
    void updateWithQuadTree(float deltaT);

    /**
     * \brief Deseneaza particulele folosind algoritmul de ierarhie a volumelor marginale.
     */
    void drawWithBvh();

    /**
     * \brief Actualizeaza particulele folosind algoritmul de ierarhie a volumelor marginale.
     *
     * \param deltaT Pasul de timp pentru actualizare.
     */
    void updateWithBvh(float deltaT);

    /**
     * \brief Deseneaza particulele folosind algoritmul Grid.
     */
    void drawWithGrid();

    /**
     * \brief Actualizeaza particulele folosind algoritmul Grid.
     *
     * \param deltaT Pasul de timp pentru actualizare.
     */
    void updateWithGrid(float deltaT);

    int screenWidth; ///< Latimea ecranului.
    int screenHeight; ///< Inaltimea ecranului.
    int numberOfParticles = 0;

    std::list<std::shared_ptr<ParticleInterface>> allParticles; ///< Lista de toate particulele.
    std::map<int, std::shared_ptr<Particle>> particleMap; ///< Harta a particulelor.

    StaticQuadTreeContainer<Particle> quadTreeParticles; ///< Container QuadTree pentru particule.
    std::unique_ptr<BvhContainer<Particle>> bvhContainer; ///< Container de ierarhie a volumelor marginale pentru particule.
    std::unique_ptr<GridContainer<Particle>> gridContainer; ///< Container Grid pentru particule.
    MeasurementCollector& measurementCollector;

    std::random_device randomDevice; ///< Dispozitiv random pentru generarea particulelor.
    std::mt19937 randomGenerator; ///< Generator random pentru generarea particulelor.

    bool onOffLines; ///< Indicator pentru afisarea liniilor pentru particule.
    Algo algoState; ///< Starea algoritmului curent.
};
