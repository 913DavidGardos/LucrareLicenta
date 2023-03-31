#pragma once
#include <vector>
#include <raylib.h>
#include "ParticleInterface.h"
#include "Particle.h"
#include "KDTreeContainer.h"
#include <random>
#include <memory>
#include "QuadTreeContainer.h"

class ParticleManager
{
public:
	ParticleManager(int screenWidth, int screenHeight);
	void InitParticles(int numberOfParticles);
	void removeParticles(int number);
	void drawParticles();
	const StaticQuadTreeContainer<Particle>& getQuadTreeParticles();
	void updateParticles(float);

private:
	void generateParticle();

	void solveCollisionWithFrame(std::list<Particle>::iterator it);

	void circleElasticCollisionResolution(std::list<Particle>::iterator it, std::list<Particle>::iterator particle);

	void drawWithQuadTree();

	void updateWithQuadTree(float);

	void drawWithKDTree();

	void updateWithKDTree(float deltaT);

	void resolveCollisionPulse(std::list<Particle>::iterator circle1, std::list<Particle>::iterator cirlce2);

	int screenWidth, screenHeight;
	std::list<std::shared_ptr<ParticleInterface>> particleList;
	StaticQuadTreeContainer<Particle> quadTreeParticles;
	KDTreeContainer<Particle> kdTreeContainer;

	std::random_device randomDevice;
	std::mt19937 randomGenerator;
};

