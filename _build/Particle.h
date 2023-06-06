#pragma once
#include "raylib.h"
#include "ParticleInterface2D.h"

static int countInstance = 0;

class Particle : public ParticleInterface2D
{
public:
	Particle(float radius, Vector2 position);
	float getX() override;
	float getY() override;
	float getRadius() override;
	void setDirection(const Vector2&) override;
	Vector2 getDirection() override;
	Vector2 getPosition() override;
	void setPosition(const Vector2&) override;
	void setX(float x) override;
	void setY(float y) override;

	void solveCollisionWithFrame(int, int);
	void circleElasticCollisionResolution(Particle* particle);

	int getId();
	Color getColor();
	Rectangle getRectangle();
	float getMass();
	void setColor(Color other);

private:
	float radius;
	Vector2 direction;
	Vector2 position;
	int id;
	Color color;
	float mass;
};

