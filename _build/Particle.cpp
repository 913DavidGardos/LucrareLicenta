#include "Particle.h"

Particle::Particle(float radius, Vector2 position) : 
	id(countInstance), radius(radius), direction(Vector2{ 0, 0 }), position(position), color(BLACK), mass(2 * radius)
{
	countInstance++;
}

float Particle::getX() 
{
	return position.x;
}

float Particle::getY()
{
	return position.y;
}

float Particle::getRadius()
{
	return radius;
}

void Particle::setDirection(const Vector2& _direction)
{
	direction = _direction;
}

Vector2 Particle::getDirection()
{
	return direction;
}

Vector2 Particle::getPosition()
{
	return position;
}

void Particle::setPosition(const Vector2& _position)
{
	position = _position;
}

void Particle::setX(float x)
{
	position.x = x;
}

void Particle::setY(float y)
{
	position.y = y;
}

int Particle::getId()
{
	return id;
}

Color Particle::getColor()
{
	return color;
}

Rectangle Particle::getRectangle()
{
	return Rectangle{position.x - radius, position.y - radius, 2 * radius, 2 * radius };
}

float Particle::getMass()
{
	return mass;
}

void Particle::setColor(Color other)
{
	color = other;
}
