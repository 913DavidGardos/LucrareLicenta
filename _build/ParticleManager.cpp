#include "ParticleManager.h"
#include <iostream>

ParticleManager::ParticleManager(int screenWidth, int screenHeight) : 
	screenWidth(screenWidth), screenHeight(screenHeight),
	quadTreeParticles(Rectangle{ 0.f, 0.f, static_cast<float>(screenWidth), static_cast<float>(screenHeight)}, 0)
{
	randomGenerator = std::mt19937(randomDevice());
}

void ParticleManager::InitParticles(int numberOfParticles)
{
	for (int i = 0; i < numberOfParticles; i++)
		generateParticle();
}

void ParticleManager::drawParticles()
{
	for (auto& it = quadTreeParticles.begin(); it != quadTreeParticles.end(); ++it)
	{
		//DrawRectangle(it->getX() - it->getRadius(), it->getY() - it->getRadius(), it->getRadius() * 2, it->getRadius() * 2, GREEN);

		DrawCircle(it->getX(), it->getY(), it->getRadius(), it->getColor());
	}

	quadTreeParticles.drawLines();
}

const StaticQuadTreeContainer<Particle>& ParticleManager::getQuadTreeParticles()
{
	return quadTreeParticles;
}

void ParticleManager::updateParticles(float deltaT)
{
	// update position for all elements 
	for (auto& it = quadTreeParticles.begin(); it != quadTreeParticles.end(); ++it)
	{
		it->setX(it->getX() + 1 * it->getDirection().x * deltaT);
		it->setY(it->getY() + 1 * it->getDirection().y * deltaT);

		solveCollisionWithFrame(it);
	}

	quadTreeParticles.update();

	for (auto& it = quadTreeParticles.begin(); it != quadTreeParticles.end(); ++it)
	{
		Rectangle particle1Rec{it->getX()  - 10, it->getY() - 10, it->getRadius() + 10, it->getRadius() + 10 };
		auto listOfPossibleParticleCollisions = quadTreeParticles.search(particle1Rec);

		for (const auto& particle : listOfPossibleParticleCollisions)
		{
			if (CheckCollisionCircles(it->getPosition(), it->getRadius(), particle->getPosition(), particle->getRadius()))
			{
				// elastic collision resolution
				circleElasticCollisionResolution(it, particle);
			}
		}
	}

	// update element position in the QuadTree
}

void ParticleManager::generateParticle()
{
	std::uniform_real_distribution<float> xDistrib(0, screenWidth);
	std::uniform_real_distribution<float> yDistrib(0, screenHeight);
	std::uniform_real_distribution<float> radiusDistrib(3.f, 5.f);

	float randomX = xDistrib(randomGenerator);
	float randomY = yDistrib(randomGenerator);
	float randomRadius = radiusDistrib(randomGenerator);


	std::shared_ptr<Particle> particlePtr = std::make_shared<Particle>(randomRadius, Vector2{ randomX, randomY });

	particlePtr->setDirection(Vector2{1.2f, 1.2f});
	
	quadTreeParticles.insert(*particlePtr.get(), Rectangle{randomX - randomRadius, randomY - randomRadius, randomRadius * 2.f, randomRadius * 2.f });
	particleList.push_back(particlePtr);
}

void ParticleManager::solveCollisionWithFrame(std::list<Particle>::iterator it)
{
	// if it hits the bottom
	if (it->getY() + it->getRadius() > screenHeight)
		if (it->getDirection().x > 0 && it->getDirection().y > 0)
			it->setDirection(Vector2{ 1.2f, -1.2f });
		else if (it->getDirection().x < 0 && it->getDirection().y > 0)
			it->setDirection(Vector2{ -1.2f, -1.2f });

	// if it hits the left side
	if (it->getX() - it->getRadius() < 0)
		if (it->getDirection().x < 0 && it->getDirection().y < 0)
			it->setDirection(Vector2{ 1.2f, -1.2f });
		else if (it->getDirection().x < 0 && it->getDirection().y > 0)
			it->setDirection(Vector2{ 1.2f, 1.2f });

	// if it hits the right side
	if (it->getX() + it->getRadius() > screenWidth)
		if (it->getDirection().x > 0 && it->getDirection().y > 0)
			it->setDirection(Vector2{ -1.2f, 1.2f });
		else if (it->getDirection().x > 0 && it->getDirection().y < 0)
			it->setDirection(Vector2{ -1.2f, -1.2f });

	// if it hits the top
	if(it->getY() - it->getRadius() < 0)
		if (it->getDirection().x > 0 && it->getDirection().y < 0)
			it->setDirection(Vector2{ 1.2f, 1.2f });
		else if (it->getDirection().x < 0 && it->getDirection().y < 0)
			it->setDirection(Vector2{ -1.2f, 1.2f });

}

void ParticleManager::circleElasticCollisionResolution(std::list<Particle>::iterator it, std::list<Particle>::iterator particle)
{
	if (it->getId() == particle->getId())
	{
		return;
	}
	it->setColor(RED);
	particle->setColor(RED);

	float distance = sqrt((it->getX() - particle->getX()) * (it->getX() - particle->getX()) + (it->getY() - particle->getY()) * (it->getY() - particle->getY()));

	float normalX = (particle->getX() - it->getX()) / distance;
	float normalY = (particle->getY() - it->getY()) / distance;

	float tangentX = -normalY;
	float tangentY = normalX;

	float dpTangent1 = it->getDirection().x * tangentX + it->getDirection().y * tangentY;
	float dpTangent2 = particle->getDirection().x * tangentX + particle->getDirection().y * tangentY;

	float dpNormal1 = it->getDirection().x * normalX + it->getDirection().y * normalY;
	float dpNormal2 = particle->getDirection().x * normalX + particle->getDirection().y * normalY;
	
	// conservation of momentum
	float m1 = (dpNormal1 * (it->getMass() - particle->getMass()) + 2.0f * particle->getMass() * dpNormal2) / (it->getMass() + particle->getMass());
	float m2 = (dpNormal2 * (particle->getMass() - it->getMass()) + 2.0f * it->getMass() * dpNormal1) / (it->getMass() + particle->getMass());

	it->setDirection(Vector2{ tangentX * dpTangent1 + normalX * m1, tangentY * dpTangent1 + normalY * m1});
	particle->setDirection(Vector2{ tangentX * dpTangent2 + normalX * m2, tangentY * dpTangent2 + normalY * m2});
}

void ParticleManager::resolveCollisionPulse(std::list<Particle>::iterator circle1, std::list<Particle>::iterator circle2)
{
	// Calculate relative velocity
	Vector2 relativeVelocity{ circle2->getDirection().x - circle1->getDirection().x, circle2->getDirection().y - circle1->getDirection().y };

	// Calculate relative velocity in terms of the normal direction (unit vector)
	Vector2 normal = normalize({ circle2->getPosition().x - circle1->getPosition().x, circle2->getPosition().y - circle1->getPosition().y });
	float normalVelocity = dotProduct(relativeVelocity, normal);

	// Check if circles are moving away from each other
	if (normalVelocity > 0)
	{
		return;
	}

	// Calculate restitution coefficient
	float e = std::min(0.000002f, 0.7f);

	// Calculate impulse scalar
	float j = -(1 + e) * normalVelocity;
	j /= (1 / circle1->getMass()) + (1 / circle2->getMass());

	// Apply impulse
	Vector2 impulse{ j * normal.x, j * normal.y };

	Vector2 a1{ impulse.x / circle1->getMass(), impulse.y / circle1->getMass() };
	Vector2 fin1{ circle1->getDirection().x - a1.x, circle1->getDirection().y - a1.y };

	Vector2 a2{ impulse.x / circle2->getMass(), impulse.y / circle2->getMass() };
	Vector2 fin2{ circle2->getDirection().x + a2.x, circle2->getDirection().y + a2.y };

	circle1->setDirection(fin1);
	circle2->setDirection(fin2);

	// Correct interpenetration
	float overlap = circle1->getRadius() + circle2->getRadius() - distance(circle1->getPosition(), circle2->getPosition());
	if (overlap > 0)
	{
		Vector2 normalized = normalize(Vector2{ circle2->getPosition().x - circle1->getPosition().x, circle2->getPosition().y - circle1->getPosition().y });
		Vector2 correction{ normalized.x * overlap, normalized.y * overlap };

		Vector2 divide1{ correction.x / 2.f , correction.y / 2.f };
		Vector2 sub1{ circle1->getPosition().x - divide1.x, circle1->getPosition().y - divide1.y };

		Vector2 divide2{ correction.x / 2.f , correction.y / 2.f };
		Vector2 add2{ circle2->getPosition().x + divide2.x, circle2->getPosition().y + divide2.y };

		circle1->setPosition(sub1);
		circle2->setPosition(add2);
	}
}

Vector2 ParticleManager::normalize(Vector2 v)
{
	float length1 = sqrt(v.x * v.x + v.y * v.y);
	Vector2 normalized1 = { v.x / length1, v.y / length1 };
	return normalized1;
}

float ParticleManager::dotProduct(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float ParticleManager::distance(const Vector2& v1, const Vector2& v2) {
	float dx = v2.x - v1.x;
	float dy = v2.y - v1.y;
	return std::sqrt(dx * dx + dy * dy);
}