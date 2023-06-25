#pragma once

/**
 * \class ParticleInterface
 * \brief O interfata pentru obiecte de particule intr-un spatiu bidimensional.
 */
class ParticleInterface
{
public:
    /**
     * \brief Obtine coordonata X a pozitiei particulei.
     * \return Coordonata X a pozitiei particulei.
     */
    virtual float getX() = 0;

    /**
     * \brief Obtine coordonata Y a pozitiei particulei.
     * \return Coordonata Y a pozitiei particulei.
     */
    virtual float getY() = 0;

    /**
     * \brief Obtine raza particulei.
     * \return Raza particulei.
     */
    virtual float getRadius() = 0;
};
