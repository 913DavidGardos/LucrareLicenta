#include "ParticleManager.h"
#include <chrono>
#include <typeinfo>
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

	bvhContainer = std::make_unique<BvhContainer<Particle>>(particleMap);
	bvhContainer->buildBVH();

	gridContainer = std::make_unique<GridContainer<Particle>>(108, 192, screenWidth, screenHeight);
	for (auto elem : particleMap)
	{
		gridContainer->insert(elem.second->getId(), elem.second->getX(), elem.second->getY());
	}
}

void ParticleManager::drawParticles()
{
	//drawWithQuadTree();
	//drawWithBvh();
	drawWithGrid();
}

const StaticQuadTreeContainer<Particle>& ParticleManager::getQuadTreeParticles()
{
	return quadTreeParticles;
}

void ParticleManager::updateParticles(float deltaT)
{
	//updateWithQuadTree(deltaT);
	//updateWithBvh(deltaT);
	updateWithGrid(deltaT);
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
	
	allParticles.push_back(particlePtr);
	std::pair<int, std::shared_ptr<Particle>> particlePair(particlePtr->getId(), particlePtr);
	particleMap.insert(particlePair);

	Rectangle rect{ randomX - randomRadius, randomY - randomRadius, randomRadius * 2.f, randomRadius * 2.f };
	quadTreeParticles.insert(*particlePtr, rect);
}

void ParticleManager::drawWithQuadTree()
{
	for (auto& it = quadTreeParticles.begin(); it != quadTreeParticles.end(); ++it)
		DrawCircle(it->getX(), it->getY(), it->getRadius(), it->getColor());

	quadTreeParticles.drawLines();
}

void ParticleManager::updateWithQuadTree(float deltaT)
{
	for (auto it = quadTreeParticles.begin(); it != quadTreeParticles.end(); ++it)
	{
		Particle* ptr = &*it;

		ptr->setX(ptr->getX() + 1 * ptr->getDirection().x * deltaT);
		ptr->setY(ptr->getY() + 1 * ptr->getDirection().y * deltaT);

		ptr->solveCollisionWithFrame(screenWidth, screenHeight);
	}

	quadTreeParticles.update();

	for (auto& it = quadTreeParticles.begin(); it != quadTreeParticles.end(); ++it)
	{
		auto listOfPossibleParticleCollisions = quadTreeParticles.search(it->getRectangle());

		for (const auto& particle : listOfPossibleParticleCollisions)
		{
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
	for (auto it = particleMap.begin(); it != particleMap.end(); ++it)
	{
		Particle* ptr = &*(it->second);
		
		ptr->setX(ptr->getX() + 1 * ptr->getDirection().x * deltaT);
		ptr->setY(ptr->getY() + 1 * ptr->getDirection().y * deltaT);

		ptr->solveCollisionWithFrame(screenWidth, screenHeight);
	}

	bvhContainer->update(deltaT, particleMap);
	auto boxes = bvhContainer->getBoxes();
	auto nodes = bvhContainer->getBvhNodes();

	for (auto node : nodes)
	{
		if (node.isLeaf())
		{
			//check collision in the leaf
			if (node.boxCount == 1)
			{
				// no collision i guess
				continue;
			}
			if (node.boxCount > 1)
			{
				// might be collision between the 2 objects in the leaf
				Vector2 center1 = Vector2{ boxes[node.firstBox].vertex0.x, boxes[node.firstBox].vertex0.y };
				float radius1 = particleMap[boxes[node.firstBox].id]->getRadius();
				Vector2 center2 = Vector2{ boxes[node.firstBox + 1].vertex0.x, boxes[node.firstBox + 1].vertex0.y };
				float radius2 = particleMap[boxes[node.firstBox + 1].id]->getRadius();

				// if collision
				if (CheckCollisionCircles(center1, radius1, center2, radius2))
				{
					Particle* first = &(*particleMap[boxes[node.firstBox].id]);
					Particle* second = &(*particleMap[boxes[node.firstBox + 1].id]);
					
					first->circleElasticCollisionResolution(second);
				}
			}
		}
	}
}

void ParticleManager::drawWithGrid()
{
	for (auto elem : particleMap)
	{
		DrawCircle(elem.second->getX(), elem.second->getY(), elem.second->getRadius(), elem.second->getColor());
	}

	// print columns 108, 192
	int columnCoef = screenWidth/gridContainer->getCols();
	int rowCoef = screenHeight/ gridContainer->getRows();
	for (int i = 0; i < 192; i++)
	{
		int x0 = i * columnCoef;
		int y0 = 0;
		int x1 = i * columnCoef;
		int y1 = screenHeight;
	
		DrawLine(x0, y0, x1, y1, GRAY);
	}

	for (int i = 0; i < 108; i++)
	{
		int x0 = 0;
		int y0 = i * rowCoef;
		int x1 = screenWidth;
		int y1 = i * rowCoef;

		DrawLine(x0, y0, x1, y1, GRAY);
	}
}

void ParticleManager::updateWithGrid(float deltaT)
{
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
					
					Vector2 center1 = Vector2{first->getX(), first->getY()};
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