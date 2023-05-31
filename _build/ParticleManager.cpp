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


	bvhContainer = std::make_unique<BvhContainer>(particleMap);
	bvhContainer->buildBVH();

	gridContainer = std::make_unique<GridContainer>(108, 192, screenWidth, screenHeight);
	for (auto elem : particleMap)
	{
		gridContainer->insert(elem.second->getId(), elem.second->getX(), elem.second->getY());
	}
}

void ParticleManager::drawParticles()
{
	drawWithQuadTree();
	//drawWithBvh();
	//drawWithGrid();
}

const StaticQuadTreeContainer<Particle>& ParticleManager::getQuadTreeParticles()
{
	return quadTreeParticles;
}

void ParticleManager::updateParticles(float deltaT)
{
	updateWithQuadTree(deltaT);
	//updateWithBvh(deltaT);
	//updateWithGrid(deltaT);
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

void ParticleManager::solveCollisionWithFrame(Particle* it)
{
	if (!it)
	{
		std::cout << "Pointer Failure in solveCollisionWithFrame !\n";
		return;
	}

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

void ParticleManager::circleElasticCollisionResolution(Particle* it, Particle* particle)
{
	if (!it || !particle)
	{
		std::cout << "Pointer failure in circleElasticCollisionResolution!\n";
		return;
	}

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
	for (auto it = quadTreeParticles.begin(); it != quadTreeParticles.end(); ++it)
	{
		Particle* ptr = &*it;

		ptr->setX(ptr->getX() + 1 * ptr->getDirection().x * deltaT);
		ptr->setY(ptr->getY() + 1 * ptr->getDirection().y * deltaT);

		solveCollisionWithFrame(ptr);
	}

	//auto start_time = std::chrono::high_resolution_clock::now();
	{
		auto start_time = std::chrono::high_resolution_clock::now();

		quadTreeParticles.update();

		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

		printf("update(erase all and insert all) Particle %d \n", duration.count());
	}

	{
		auto start_time = std::chrono::high_resolution_clock::now();

		for (auto& it = quadTreeParticles.begin(); it != quadTreeParticles.end(); ++it)
		{
			//Rectangle particle1Rec{it->getX()  - 0, it->getY() - 0, it->getRadius() + 0, it->getRadius() + 0 };
			auto listOfPossibleParticleCollisions = quadTreeParticles.search(it->getRectangle());

			for (const auto& particle : listOfPossibleParticleCollisions)
			{
				if (CheckCollisionCircles(it->getPosition(), it->getRadius(), particle->getPosition(), particle->getRadius()))
				{
					// elastic collision resolution
					Particle* first = &*it;
					Particle* second = &*particle;
					circleElasticCollisionResolution(first, second);
				}
			}
		}
		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

		printf("collision with other circles Particle %d \n\n\n", duration.count());
	}


	// update element position in the QuadTree
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

		solveCollisionWithFrame(ptr);
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
					
					circleElasticCollisionResolution(first, second);
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

		solveCollisionWithFrame(ptr);
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
						circleElasticCollisionResolution(first, second);
					}
				}
			}
		}
	}
}