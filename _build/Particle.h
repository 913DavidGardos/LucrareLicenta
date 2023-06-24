#pragma once
#include "raylib.h"
#include "ParticleInterface2D.h"


/// \brief Variabila statica pentru a tine evidenta numarului de instante Particle.
static int countInstance = 0;

/**
 * \class Particle
 * \brief Reprezinta o particula intr-un spatiu bidimensional.
 */
class Particle : public ParticleInterface2D
{
public:
    /**
     * \brief Construieste o particula cu raza si pozitia specificate.
     * \param radius Raza particulei.
     * \param position Pozitia particulei.
     */
    Particle(float radius, Vector2 position);

    /**
     * \brief Constructorul de copiere este dezactivat pentru a preveni copierea instantelor de Particle.
     */
    Particle(const Particle& other) = delete;

    /**
     * \brief Distrugerea instantei Particle.
     */
    ~Particle();

    /**
     * \brief Obtine coordonata X a pozitiei particulei.
     * \return Coordonata X a pozitiei particulei.
     */
    float getX() override;

    /**
     * \brief Obtine coordonata Y a pozitiei particulei.
     * \return Coordonata Y a pozitiei particulei.
     */
    float getY() override;

    /**
     * \brief Obtine raza particulei.
     * \return Raza particulei.
     */
    float getRadius() override;

    /**
     * \brief Seteaza directia particulei.
     * \param direction Vectorul de directie al particulei.
     */
    void setDirection(const Vector2& direction) override;

    /**
     * \brief Obtine directia particulei.
     * \return Vectorul de directie al particulei.
     */
    Vector2 getDirection() override;

    /**
     * \brief Obtine pozitia particulei.
     * \return Vectorul de pozitie al particulei.
     */
    Vector2 getPosition() override;

    /**
     * \brief Seteaza pozitia particulei.
     * \param position Vectorul de pozitie al particulei.
     */
    void setPosition(const Vector2& position) override;

    /**
     * \brief Seteaza coordonata X a pozitiei particulei.
     * \param x Coordonata X a pozitiei particulei.
     */
    void setX(float x) override;

    /**
     * \brief Seteaza coordonata Y a pozitiei particulei.
     * \param y Coordonata Y a pozitiei particulei.
     */
    void setY(float y) override;

    /**
     * \brief Rezolva coliziunea intre particula si cadrul cu dimensiunile specificate.
     * \param frameWidth Latimea cadrului.
     * \param frameHeight Inaltimea cadrului.
     */
    void solveCollisionWithFrame(int frameWidth, int frameHeight);

    /**
     * \brief Rezolva coliziunea elastica intre aceasta particula si o alta particula.
     * \param particle Un pointer catre cealalta particula implicata in coliziune.
     */
    void circleElasticCollisionResolution(Particle* particle);

    /**
     * \brief Obtine ID-ul particulei.
     * \return ID-ul particulei.
     */
    int getId();

    /**
     * \brief Obtine culoarea particulei.
     * \return Culoarea particulei.
     */
    Color getColor();

    /**
     * \brief Obtine reprezentarea sub forma de dreptunghi a particulei.
     * \return Dreptunghiul care reprezinta particula.
     */
    Rectangle getRectangle();

    /**
     * \brief Obtine masa particulei.
     * \return Masa particulei.
     */
    float getMass();

    /**
     * \brief Seteaza culoarea particulei.
     * \param other Noua culoare pentru particula.
     */
    void setColor(Color other);

private:
    float radius;           ///< Raza particulei.
    Vector2 direction;      ///< Vectorul de directie al particulei.
    Vector2 position;       ///< Vectorul de pozitie al particulei.
    int id;                 ///< ID-ul particulei.
    //Color color;          ///< Culoarea particulei.
    float mass;             ///< Masa particulei.
};
