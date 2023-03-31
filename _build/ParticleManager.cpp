#include "ParticleManager.h"
#include <chrono>
#include <iostream>

ParticleManager::ParticleManager(int screenWidth, int screenHeight) : 
	screenWidth(screenWidth), screenHeight(screenHeight),
	quadTreeParticles(Rectangle{ 0.f, 0.f, static_cast<float>(screenWidth), static_cast<float>(screenHeight)}, 0),
	kdTreeContainer(2)
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
	//drawWithQuadTree();
	drawWithKDTree();
}

const StaticQuadTreeContainer<Particle>& ParticleManager::getQuadTreeParticles()
{
	return quadTreeParticles;
}

void ParticleManager::updateParticles(float deltaT)
{
	//updateWithQuadTree(deltaT);
	//updateWithKDTree(deltaT);
}

void ParticleManager::generateParticle()
{
	std::uniform_real_distribution<float> xDistrib(0, screenWidth);
	std::uniform_real_distribution<float> yDistrib(0, screenHeight);
	std::uniform_real_distribution<float> radiusDistrib(4.f, 5.2f);

	float randomX = xDistrib(randomGenerator);
	float randomY = yDistrib(randomGenerator);
	float randomRadius = radiusDistrib(randomGenerator);

	std::shared_ptr<Particle> particlePtr = std::make_shared<Particle>(randomRadius, Vector2{ randomX, randomY });

	particlePtr->setDirection(Vector2{3.0f, 3.0f});
	
	//quadTreeParticles.insert(*particlePtr.get(), Rectangle{randomX - randomRadius, randomY - randomRadius, randomRadius * 2.f, randomRadius * 2.f });
	kdTreeContainer.insert(*particlePtr.get(), std::vector<float>{randomX, randomY});
	particleList.push_back(particlePtr);
}

void ParticleManager::solveCollisionWithFrame(std::list<Particle>::iterator it)
{
	// if it hits the bottom
	if (it->getY() + it->getRadius() > screenHeight)
		if (it->getDirection().x > 0 && it->getDirection().y > 0)
			it->setDirection(Vector2{ 3.0f, -3.0f });
		else if (it->getDirection().x < 0 && it->getDirection().y > 0)
			it->setDirection(Vector2{ -3.0f, -3.0f });

	// if it hits the left side
	if (it->getX() - it->getRadius() < 0)
		if (it->getDirection().x < 0 && it->getDirection().y < 0)	
			it->setDirection(Vector2{ 3.0f, -3.0f });
		else if (it->getDirection().x < 0 && it->getDirection().y > 0)
			it->setDirection(Vector2{ 3.0f, 3.0f });

	// if it hits the right side
	if (it->getX() + it->getRadius() > screenWidth)
		if (it->getDirection().x > 0 && it->getDirection().y > 0)
			it->setDirection(Vector2{ -3.0f, 3.0f });
		else if (it->getDirection().x > 0 && it->getDirection().y < 0)
			it->setDirection(Vector2{ -3.0f, -3.0f });

	// if it hits the top
	if(it->getY() - it->getRadius() < 0)
		if (it->getDirection().x > 0 && it->getDirection().y < 0)
			it->setDirection(Vector2{ 3.0f, 3.0f });
		else if (it->getDirection().x < 0 && it->getDirection().y < 0)
			it->setDirection(Vector2{ -3.0f, 3.0f });

}

void ParticleManager::circleElasticCollisionResolution(std::list<Particle>::iterator it, std::list<Particle>::iterator particle)
{
	float particleX = particle->getX() + particle->getRadius();
	float particleY = particle->getY() + particle->getRadius();

	if (it->getId() == particle->getId())
	{
		return;
	}
	it->setColor(BLUE);
	particle->setColor(BLUE);

	float distance = sqrt((it->getX() - particleX) * (it->getX() - particleX) + (it->getY() - particleY) * (it->getY() - particleY));

	float normalX = (particleX - it->getX()) / distance;
	float normalY = (particleY - it->getY()) / distance;

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

void ParticleManager::drawWithQuadTree()
{
	for (auto& it = quadTreeParticles.begin(); it != quadTreeParticles.end(); ++it)
	{
		//DrawRectangle(it->getX() - it->getRadius(), it->getY() - it->getRadius(), it->getRadius() * 2, it->getRadius() * 2, GREEN);

		DrawCircle(it->getX(), it->getY(), it->getRadius(), it->getColor());
		//DrawPixel(it->getX(), it->getY(), it->getColor());

	}

	quadTreeParticles.drawLines();
}

void ParticleManager::updateWithQuadTree(float deltaT)
{
	// update position for all elements 
	for (auto& it = quadTreeParticles.begin(); it != quadTreeParticles.end(); ++it)
	{
		it->setX(it->getX() + 1 * it->getDirection().x * deltaT);
		it->setY(it->getY() + 1 * it->getDirection().y * deltaT);

		solveCollisionWithFrame(it);
	}


	auto start_time = std::chrono::high_resolution_clock::now();

	quadTreeParticles.update();

	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

	std::cout << "Execution time update 10000: " << duration.count() << " milliseconds" << std::endl;

	for (auto& it = quadTreeParticles.begin(); it != quadTreeParticles.end(); ++it)
	{
		//Rectangle particle1Rec{it->getX()  - 0, it->getY() - 0, it->getRadius() + 0, it->getRadius() + 0 };
		auto listOfPossibleParticleCollisions = quadTreeParticles.search(it->getRectangle());

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

void ParticleManager::drawWithKDTree()
{
	for (auto& it = kdTreeContainer.begin(); it != kdTreeContainer.end(); ++it)
	{
		DrawCircle(it->getX(), it->getY(), it->getRadius(), it->getColor());
	}
}

void ParticleManager::updateWithKDTree(float deltaT)
{
	// update position for all elements 
	for (auto& it = kdTreeContainer.begin(); it != kdTreeContainer.end(); ++it)
	{
		it->setX(it->getX() + 1 * it->getDirection().x * deltaT);
		it->setY(it->getY() + 1 * it->getDirection().y * deltaT);

		solveCollisionWithFrame(it);
	}

	auto start_time = std::chrono::high_resolution_clock::now();

	kdTreeContainer.update();

	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

	std::cout << "Execution time update 10000: " << duration.count() << " milliseconds" << std::endl;

	for (auto& it = kdTreeContainer.begin(); it != kdTreeContainer.end(); ++it)
	{
		//Rectangle particle1Rec{it->getX()  - 0, it->getY() - 0, it->getRadius() + 0, it->getRadius() + 0 };
		auto listOfPossibleParticleCollisions = kdTreeContainer.search(it->getRectangle());

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