#pragma once
#include "ParticleInterface.h"

class ParticleInterface2D : public ParticleInterface 
{
public:
	virtual float getX() = 0;
	virtual float getY() = 0;
	virtual float getRadius() = 0;
	virtual void setDirection(const Vector2&) = 0;
	virtual Vector2 getDirection() = 0;
	virtual Vector2 getPosition() = 0;

	virtual void setPosition(const Vector2&) = 0;
	virtual void setX(float x) = 0;
	virtual void setY(float y) = 0;

};