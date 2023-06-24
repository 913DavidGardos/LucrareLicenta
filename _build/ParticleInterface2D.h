#pragma once
#include "ParticleInterface.h"

/**
 * \class ParticleInterface2D
 * \brief O interfata pentru obiecte de particule bidimensionale.
 * \details ParticleInterface2D extinde clasa ParticleInterface si adauga functii membru suplimentare specifice particulelor bidimensionale.
 */
class ParticleInterface2D : public ParticleInterface
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

    /**
     * \brief Seteaza directia particulei.
     * \param direction Vectorul de directie nou pentru particula.
     */
    virtual void setDirection(const Vector2& direction) = 0;

    /**
     * \brief Obtine directia particulei.
     * \return Vectorul de directie al particulei.
     */
    virtual Vector2 getDirection() = 0;

    /**
     * \brief Obtine pozitia particulei.
     * \return Vectorul de pozitie al particulei.
     */
    virtual Vector2 getPosition() = 0;

    /**
     * \brief Seteaza pozitia particulei.
     * \param position Vectorul de pozitie nou pentru particula.
     */
    virtual void setPosition(const Vector2& position) = 0;

    /**
     * \brief Seteaza coordonata X a pozitiei particulei.
     * \param x Noua coordonata X pentru pozitia particulei.
     */
    virtual void setX(float x) = 0;

    /**
     * \brief Seteaza coordonata Y a pozitiei particulei.
     * \param y Noua coordonata Y pentru pozitia particulei.
     */
    virtual void setY(float y) = 0;
};
