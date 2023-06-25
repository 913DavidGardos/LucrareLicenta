#include "ParticleManager.h"
#include <chrono>
#include <typeinfo>
#include <iostream>
#include "Timer.h"
#define GRID_ROWS 50
#define GRID_COLS 96

ParticleManager::ParticleManager(int screenWidth, int screenHeight, MeasurementCollector& measurementCollector) :
	screenWidth(screenWidth),
	screenHeight(screenHeight),
	measurementCollector(measurementCollector),
	quadTreeParticles(Rectangle{ 0.f, 0.f, static_cast<float>(screenWidth), static_cast<float>(screenHeight) }, 0),
	algoState(Algo::QuadTree)
{
	randomGenerator = std::mt19937(randomDevice());
	onOffLines = true;
}

void ParticleManager::InitParticles(int numberOfParticles)
{
	this->numberOfParticles = numberOfParticles;

	Timer h("InitParticles", measurementCollector, numberOfParticles);

	particleMap.clear();

	allParticles.clear();

	quadTreeParticles.clear();

	bvhContainer.reset();

	gridContainer.reset();


	for (int i = 0; i < numberOfParticles; i++)
		generateParticle();

	for (auto elem : particleMap)
	{
		int randomX = elem.second->getX();
		int randomRadius = elem.second->getRadius();
		int randomY = elem.second->getY();

		Rectangle rect{ randomX - randomRadius, randomY - randomRadius, randomRadius * 2.f, randomRadius * 2.f };
		quadTreeParticles.insert(elem.second, rect);
	}

	bvhContainer = std::make_unique<BvhContainer<Particle>>(particleMap);
	bvhContainer->buildBVH();

	gridContainer = std::make_unique<GridContainer<Particle>>(GRID_ROWS, GRID_COLS, screenWidth, screenHeight);
	for (auto elem : particleMap)
	{
		gridContainer->insert(elem.second->getId(), elem.second->getX(), elem.second->getY());
	}
}

void ParticleManager::drawParticles()
{
	if (algoState == Algo::QuadTree)
		drawWithQuadTree();
	else if (algoState == Algo::Grid)
		drawWithGrid();
	else if (algoState == Algo::BoundingVolume)
		drawWithBvh();
}

void ParticleManager::updateNumberOfParticles(int nParticles)
{
	InitParticles(nParticles);
}

const StaticQuadTreeContainer<Particle>& ParticleManager::getQuadTreeParticles()
{
	return quadTreeParticles;
}

void ParticleManager::updateParticles(float deltaT)
{
	if (algoState == Algo::QuadTree)
		updateWithQuadTree(deltaT);
	else if (algoState == Algo::Grid)
	{
		updateWithGrid(deltaT);
	}
	else if (algoState == Algo::BoundingVolume)
	{
		updateWithBvh(deltaT);
	}
}

void ParticleManager::updateParticleVelocity(float newVelocity)
{
	for (auto iter = quadTreeParticles.begin(); iter != quadTreeParticles.end(); ++iter)
	{
		auto it = *iter;
		Vector2 newDirection{ it->getDirection().x * newVelocity, it->getDirection().y * newVelocity };
		it->setDirection(newDirection);
		particleMap[it->getId()]->setDirection(newDirection);
	}
}

void ParticleManager::toggleLines()
{
	onOffLines = !onOffLines;
}

void ParticleManager::startQuadTree()
{
	algoState = Algo::QuadTree;
}

void ParticleManager::startGrid()
{
	algoState = Algo::Grid;
}

void ParticleManager::startBoundingVolume()
{
	algoState = Algo::BoundingVolume;
}

int ParticleManager::getScreenWidth()
{
	return screenWidth;
}

int ParticleManager::getScreenHeight()
{
	return screenHeight;
}

void ParticleManager::generateParticle()
{
	std::uniform_real_distribution<float> xDistrib(0, screenWidth);
	std::uniform_real_distribution<float> yDistrib(0, screenHeight);
	std::uniform_real_distribution<float> radiusDistrib(4.1f, 8.9f);

	float randomX = xDistrib(randomGenerator);
	float randomY = yDistrib(randomGenerator);
	float randomRadius = radiusDistrib(randomGenerator);

	std::shared_ptr<Particle> particlePtr = std::make_shared<Particle>(randomRadius, Vector2{ randomX, randomY });

	particlePtr->setDirection(Vector2{ 3.0f, 3.0f });

	std::pair<int, std::shared_ptr<Particle>> particlePair(particlePtr->getId(), particlePtr);
	particleMap.insert(particlePair);
}

void ParticleManager::drawWithQuadTree()
{
	for (auto iter = quadTreeParticles.begin(); iter != quadTreeParticles.end(); ++iter)
	{
		auto it = *iter;
		DrawCircle(it->getX(), it->getY(), it->getRadius(), it->getColor());
	}

	if (onOffLines)
		quadTreeParticles.drawLines();
}

void ParticleManager::updateWithQuadTree(float deltaT)
{
	measurementCollector.insertSize("updateWithQuadTree", quadTreeParticles.sizeOfDataStructure(), numberOfParticles);
	Timer b("updateWithQuadTree", measurementCollector, numberOfParticles);

	for (auto iter = quadTreeParticles.begin(); iter != quadTreeParticles.end(); ++iter)
	{
		auto it = *iter;
		Particle* ptr = &*it;

		ptr->setX(ptr->getX() + 1 * ptr->getDirection().x * deltaT);
		ptr->setY(ptr->getY() + 1 * ptr->getDirection().y * deltaT);

		ptr->solveCollisionWithFrame(screenWidth, screenHeight);
	}

	quadTreeParticles.update();

	for (auto iter = quadTreeParticles.begin(); iter != quadTreeParticles.end(); ++iter)
	{
		auto it = *iter;
		auto listOfPossibleParticleCollisions = quadTreeParticles.search(it->getRectangle());

		for (const auto& particleIt : listOfPossibleParticleCollisions)
		{
			auto particle = *particleIt;
			if (CheckCollisionCircles(it->getPosition(), it->getRadius(), particle->getPosition(), particle->getRadius()))
			{
				// elastic collision resolution
				Particle* first = &*it;
				Particle* second = &*particle;
				first->circleElasticCollisionResolution(second);
			}
		}
	}
}

void ParticleManager::drawWithBvh()
{
	for (auto elem : particleMap)
	{
		DrawCircle(elem.second->getX(), elem.second->getY(), elem.second->getRadius(), elem.second->getColor());
	}

	if (onOffLines)
		for (auto elem : bvhContainer->getBvhNodes())
		{
			int x0 = elem.aabbMin.x;
			int y0 = elem.aabbMin.y;
			int x1 = elem.aabbMax.x;
			int y1 = elem.aabbMax.y;
			DrawLine(x0, y0, x1, y0, GRAY);
			DrawLine(x0, y0, x0, y1, GRAY);
			DrawLine(x1, y0, x1, y1, GRAY);
			DrawLine(x0, y1, x1, y1, GRAY);
		}
}

void ParticleManager::updateWithBvh(float deltaT)
{
	measurementCollector.insertSize("updateWithBvh", bvhContainer->sizeOfDataStructure(), numberOfParticles);
	Timer d("updateWithBvh", measurementCollector, numberOfParticles);

	for (auto it = particleMap.begin(); it != particleMap.end(); ++it)
	{
		Particle* ptr = &*(it->second);

		ptr->setX(ptr->getX() + 1 * ptr->getDirection().x * deltaT);
		ptr->setY(ptr->getY() + 1 * ptr->getDirection().y * deltaT);

		ptr->solveCollisionWithFrame(screenWidth, screenHeight);
	}

	bvhContainer->update(deltaT, particleMap);

	if (!bvhContainer->detectCollisions().empty())
	{
		auto colisions = bvhContainer->detectCollisions();

		for (auto colision : colisions)
		{
			Particle* first = &(*particleMap[colision.first]);
			Particle* second = &(*particleMap[colision.second]);

			first->circleElasticCollisionResolution(second);
		}
	}
}

void ParticleManager::drawWithGrid()
{
	for (auto elem : particleMap)
	{
		DrawCircle(elem.second->getX(), elem.second->getY(), elem.second->getRadius(), elem.second->getColor());
	}

	if (onOffLines)
	{
		int columnCoef = screenWidth / gridContainer->getCols();
		int rowCoef = screenHeight / gridContainer->getRows();
		for (int i = 0; i < GRID_COLS; i++)
		{
			int x0 = i * columnCoef;
			int y0 = 0;
			int x1 = i * columnCoef;
			int y1 = screenHeight;

			DrawLine(x0, y0, x1, y1, GRAY);
		}

		for (int i = 0; i < GRID_ROWS; i++)
		{
			int x0 = 0;
			int y0 = i * rowCoef;
			int x1 = screenWidth;
			int y1 = i * rowCoef;

			DrawLine(x0, y0, x1, y1, GRAY);
		}
	}
}

void ParticleManager::updateWithGrid(float deltaT)
{
	measurementCollector.insertSize("updateWithGrid", gridContainer->sizeOfDataStructure(), numberOfParticles);
	Timer f("updateWithGrid", measurementCollector, numberOfParticles);

	for (auto it = particleMap.begin(); it != particleMap.end(); ++it)
	{
		Particle* ptr = &*(it->second);

		ptr->setX(ptr->getX() + 1 * ptr->getDirection().x * deltaT);
		ptr->setY(ptr->getY() + 1 * ptr->getDirection().y * deltaT);

		ptr->solveCollisionWithFrame(screenWidth, screenHeight);
	}

	gridContainer->update(particleMap);

	for (auto elem : particleMap)
	{
		auto cellQuery = gridContainer->query(elem.second->getId());
		if (!cellQuery.empty())
		{
			for (auto id : cellQuery)
			{
				if (id != elem.second->getId())
				{
					Particle* first = &*particleMap[id];
					Particle* second = &*particleMap[elem.second->getId()];

					Vector2 center1 = Vector2{ first->getX(), first->getY() };
					float radius1 = particleMap[id]->getRadius();
					Vector2 center2 = Vector2{ second->getX(), second->getY() };
					float radius2 = particleMap[second->getId()]->getRadius();

					if (CheckCollisionCircles(center1, radius1, center2, radius2))
					{
						first->circleElasticCollisionResolution(second);
					}
				}
			}
		}
	}
}