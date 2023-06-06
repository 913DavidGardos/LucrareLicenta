#include "Particle.h"
#include<iostream>

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

void Particle::solveCollisionWithFrame(int screenWidth, int screenHeight)
{
	// if it hits the bottom
	if (this->getY() + this->getRadius() > screenHeight)
		if (this->getDirection().x > 0 && this->getDirection().y > 0)
			this->setDirection(Vector2{ 3.0f, -3.0f });
		else if (this->getDirection().x < 0 && this->getDirection().y > 0)
			this->setDirection(Vector2{ -3.0f, -3.0f });

	// if this hits the left side
	if (this->getX() - this->getRadius() < 0)
		if (this->getDirection().x < 0 && this->getDirection().y < 0)
			this->setDirection(Vector2{ 3.0f, -3.0f });
		else if (this->getDirection().x < 0 && this->getDirection().y > 0)
			this->setDirection(Vector2{ 3.0f, 3.0f });

	// if this hits  the right side
	if (this->getX() + this->getRadius() > screenWidth)
		if (this->getDirection().x > 0 && this->getDirection().y > 0)
			this->setDirection(Vector2{ -3.0f, 3.0f });
		else if (this->getDirection().x > 0 && this->getDirection().y < 0)
			this->setDirection(Vector2{ -3.0f, -3.0f });

	// if this hits  the top
	if (this->getY() - this->getRadius() < 0)
		if (this->getDirection().x > 0 && this->getDirection().y < 0)
			this->setDirection(Vector2{ 3.0f, 3.0f });
		else if (this->getDirection().x < 0 && this->getDirection().y < 0)
			this->setDirection(Vector2{ -3.0f, 3.0f });
}

void Particle::circleElasticCollisionResolution(Particle* particle)
{
	if (!this || !particle)
	{
		std::cout << "Pointer failure in Particle::circleElasticCollisionResolution\n";
		return;
	}

	float particleX = particle->getX() + particle->getRadius();
	float particleY = particle->getY() + particle->getRadius();

	if (this->getId() == particle->getId())
	{
		return;
	}
	this->setColor(BLUE);
	particle->setColor(BLUE);

	float distance = sqrt((this->getX() - particleX) * (this->getX() - particleX) + (this->getY() - particleY) * (this->getY() - particleY));

	float normalX = (particleX - this->getX()) / distance;
	float normalY = (particleY - this->getY()) / distance;

	float tangentX = -normalY;
	float tangentY = normalX;

	float dpTangent1 = this->getDirection().x * tangentX + this->getDirection().y * tangentY;
	float dpTangent2 = particle->getDirection().x * tangentX + particle->getDirection().y * tangentY;

	float dpNormal1 = this->getDirection().x * normalX + this->getDirection().y * normalY;
	float dpNormal2 = particle->getDirection().x * normalX + particle->getDirection().y * normalY;

	// conservation of momentum
	float m1 = (dpNormal1 * (this->getMass() - particle->getMass()) + 2.0f * particle->getMass() * dpNormal2) / (this->getMass() + particle->getMass());
	float m2 = (dpNormal2 * (particle->getMass() - this->getMass()) + 2.0f * this->getMass() * dpNormal1) / (this->getMass() + particle->getMass());

	this->setDirection(Vector2{ tangentX * dpTangent1 + normalX * m1, tangentY * dpTangent1 + normalY * m1 });
	particle->setDirection(Vector2{ tangentX * dpTangent2 + normalX * m2, tangentY * dpTangent2 + normalY * m2 });
}
