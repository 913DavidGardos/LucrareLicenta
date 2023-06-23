#pragma once
#include "ParticleInterface.h"

/// \file
/// This file contains the definition of the ParticleInterface2D class and its pure virtual member functions.

/// \class ParticleInterface2D
/// \brief An interface for two-dimensional particle objects.
/// \details ParticleInterface2D extends the ParticleInterface class and adds additional member functions specific to two-dimensional particles.
class ParticleInterface2D : public ParticleInterface
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

    /// \brief Sets the direction of the particle.
    /// \param direction The new direction vector for the particle.
    virtual void setDirection(const Vector2& direction) = 0;

    /// \brief Retrieves the direction of the particle.
    /// \return The direction vector of the particle.
    virtual Vector2 getDirection() = 0;

    /// \brief Retrieves the position of the particle.
    /// \return The position vector of the particle.
    virtual Vector2 getPosition() = 0;

    /// \brief Sets the position of the particle.
    /// \param position The new position vector for the particle.
    virtual void setPosition(const Vector2& position) = 0;

    /// \brief Sets the X coordinate of the particle's position.
    /// \param x The new X coordinate for the particle's position.
    virtual void setX(float x) = 0;

    /// \brief Sets the Y coordinate of the particle's position.
    /// \param y The new Y coordinate for the particle's position.
    virtual void setY(float y) = 0;
};