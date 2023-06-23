#pragma once
/// \file
/// This file contains the definition of the ParticleInterface class and its pure virtual member functions.

/// \class ParticleInterface
/// \brief An interface for particle objects in a two-dimensional space.
class ParticleInterface
{
public:
    /// \brief Retrieves the X coordinate of the particle's position.
    /// \return The X coordinate of the particle's position.
    virtual float getX() = 0;

    /// \brief Retrieves the Y coordinate of the particle's position.
    /// \return The Y coordinate of the particle's position.
    virtual float getY() = 0;

    /// \brief Retrieves the radius of the particle.
    /// \return The radius of the particle.
    virtual float getRadius() = 0;
};