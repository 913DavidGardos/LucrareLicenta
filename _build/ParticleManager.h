#pragma once

/// \file ParticleManager.h
/// \brief Contains the ParticleManager class and Algo enum.
///
/// This file provides the definition of the ParticleManager class
/// and the Algo enum, which are used to manage particles and define
/// different algorithms.

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


/// \enum Algo
/// \brief Defines different algorithms for the ParticleManager.
enum class Algo
{
    QuadTree, ///< QuadTree algorithm.
    Grid,     ///< Grid algorithm.
    BoundingVolume ///< Bounding volume hierarchy algorithm.
};

/// \class ParticleManager
/// \brief Manages particles and provides operations on them.
///
/// The ParticleManager class is responsible for managing particles,
/// initializing particles, updating their state, and drawing them
/// using different algorithms.
class ParticleManager
{
public:
    /// \brief Constructor.
    ///
    /// \param screenWidth The width of the screen.
    /// \param screenHeight The height of the screen.
    ParticleManager(int screenWidth, int screenHeight, MeasurementCollector& measurementCollector);

    /// \brief Initializes particles.
    ///
    /// \param numberOfParticles The number of particles to initialize.
    void InitParticles(int numberOfParticles);

    /// \brief Draws the particles.
    void drawParticles();

    /// \brief Updates the number of particles.
    ///
    /// \param nParticles The new number of particles.
    void updateNumberOfParticles(int nParticles);

    /// \brief Gets the quadtree particles.
    ///
    /// \return The quadtree particles.
    const StaticQuadTreeContainer<Particle>& getQuadTreeParticles();

    /// \brief Updates the particles.
    ///
    /// \param deltaT The time step for the update.
    void updateParticles(float deltaT);

    /// \brief Updates the particle velocity.
    ///
    /// \param newVelocity The new velocity for the particles.
    void updateParticleVelocity(float newVelocity);


    /// \brief Toggles the lines for particles.
    void toggleLines();

    /// \brief Starts the quadtree algorithm.
    void startQuadTree();

    /// \brief Starts the grid algorithm.
    void startGrid();

    /// \brief Starts the bounding volume hierarchy algorithm.
    void startBoundingVolume();

    /// \brief Gets the screen width.
    ///
    /// \return The screen width.
    int getScreenWidth();

    /// \brief Gets the screen height.
    ///
    /// \return The screen height.
    int getScreenHeight();

private:
    /// \brief Generates a particle.
    void generateParticle();

    /// \brief Draws the particles using the quadtree algorithm.
    void drawWithQuadTree();

    /// \brief Updates the particles using the quadtree algorithm.
    ///
    /// \param deltaT The time step for the update.
    void updateWithQuadTree(float deltaT);

    /// \brief Draws the particles using the bounding volume hierarchy algorithm.
    void drawWithBvh();

    /// \brief Updates the particles using the bounding volume hierarchy algorithm.
    ///
    /// \param deltaT The time step for the update.
    void updateWithBvh(float deltaT);

    /// \brief Draws the particles using the grid algorithm.
    void drawWithGrid();

    /// \brief Updates the particles using the grid algorithm.
    ///
    /// \param deltaT The time step for the update.
    void updateWithGrid(float deltaT);

    int screenWidth; ///< The width of the screen.
    int screenHeight; ///< The height of the screen.
    int numberOfParticles = 0;

    std::list<std::shared_ptr<ParticleInterface>> allParticles; ///< List of all particles.
    std::map<int, std::shared_ptr<Particle>> particleMap; ///< Map of particles.

    StaticQuadTreeContainer<Particle> quadTreeParticles; ///< QuadTree container for particles.
    std::unique_ptr<BvhContainer<Particle>> bvhContainer; ///< Bounding volume hierarchy container for particles.
    std::unique_ptr<GridContainer<Particle>> gridContainer; ///< Grid container for particles.
    MeasurementCollector& measurementCollector;

    std::random_device randomDevice; ///< Random device for particle generation.
    std::mt19937 randomGenerator; ///< Random generator for particle generation.

    bool onOffLines; ///< Flag to toggle lines for particles.
    Algo algoState; ///< Current algorithm state.
};