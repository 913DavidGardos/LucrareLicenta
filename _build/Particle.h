#pragma once
#include "raylib.h"
#include "ParticleInterface2D.h"

/// \file
/// This file contains the definition of the Particle class and its member functions.

/// \brief A static variable to keep track of the number of Particle instances.
static int countInstance = 0;

/// \class Particle
/// \brief Represents a particle in a two-dimensional space.
class Particle : public ParticleInterface2D
{
public:
    /// \brief Constructs a Particle with the specified radius and position.
    /// \param radius The radius of the particle.
    /// \param position The position of the particle.
    Particle(float radius, Vector2 position);

    /// \brief Deleted copy constructor to prevent copying Particle instances.
    Particle(const Particle& other) = delete;

    /// \brief Destroys the Particle instance.
    ~Particle();

    /// \brief Retrieves the X coordinate of the particle's position.
    /// \return The X coordinate of the particle's position.
    float getX() override;

    /// \brief Retrieves the Y coordinate of the particle's position.
    /// \return The Y coordinate of the particle's position.
    float getY() override;

    /// \brief Retrieves the radius of the particle.
    /// \return The radius of the particle.
    float getRadius() override;

    /// \brief Sets the direction of the particle.
    /// \param direction The direction vector of the particle.
    void setDirection(const Vector2& direction) override;

    /// \brief Retrieves the direction of the particle.
    /// \return The direction vector of the particle.
    Vector2 getDirection() override;

    /// \brief Retrieves the position of the particle.
    /// \return The position vector of the particle.
    Vector2 getPosition() override;

    /// \brief Sets the position of the particle.
    /// \param position The position vector of the particle.
    void setPosition(const Vector2& position) override;

    /// \brief Sets the X coordinate of the particle's position.
    /// \param x The X coordinate of the particle's position.
    void setX(float x) override;

    /// \brief Sets the Y coordinate of the particle's position.
    /// \param y The Y coordinate of the particle's position.
    void setY(float y) override;

    //float getX();
    //float getY();
    //float getRadius();
    //void setDirection(const Vector2&);
    //Vector2 getDirection();
    //Vector2 getPosition();
    //void setPosition(const Vector2&);
    //void setX(float x);
    //void setY(float y);

    /// \brief Solves collision between the particle and the frame of the specified dimensions.
    /// \param frameWidth The width of the frame.
    /// \param frameHeight The height of the frame.
    void solveCollisionWithFrame(int frameWidth, int frameHeight);

    /// \brief Resolves elastic collision between this particle and another particle.
    /// \param particle A pointer to the other particle involved in the collision.
    void circleElasticCollisionResolution(Particle* particle);

    /// \brief Retrieves the ID of the particle.
    /// \return The ID of the particle.
    int getId();

    /// \brief Retrieves the color of the particle.
    /// \return The color of the particle.
    Color getColor();

    /// \brief Retrieves the rectangle representation of the particle.
    /// \return The rectangle representing the particle.
    Rectangle getRectangle();

    /// \brief Retrieves the mass of the particle.
    /// \return The mass of the particle.
    float getMass();

    /// \brief Sets the color of the particle.
    /// \param other The new color for the particle.
    void setColor(Color other);

private:
    float radius;           ///< The radius of the particle.
    Vector2 direction;      ///< The direction vector of the particle.
    Vector2 position;       ///< The position vector of the particle.
    int id;                 ///< The ID of the particle.
    //Color color;          ///< The color of the particle.
    float mass;             ///< The mass of the particle.
};