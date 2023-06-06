#pragma once
#include <vector>
#include <raylib.h>
#include "ParticleInterface.h"
#include "Particle.h"
#include "BvhContainer.h"
#include <random>
#include <memory>
#include <utility>
#include "QuadTreeContainer.h"
#include "GridContainer.h"

enum class Algo
{
	QuadTree,
	Grid,
	BoundingVolume
};


class ParticleManager
{
public:
	ParticleManager(int screenWidth, int screenHeight);
	void InitParticles(int numberOfParticles);
	void drawParticles();
	void updateNumberOfParticles(int);
	const StaticQuadTreeContainer<Particle>& getQuadTreeParticles();
	void updateParticles(float);
	void updateParticleVelocity(float newVelocity);
	void updateQuadTreeContainerWithMap(int deltaT);
	void toggleLines();
	void startQuadTree();
	void startGrid();
	void startBoundingVolume();
	int getScreenWidth();
	int getScreenHeight();

private:
	void generateParticle();
	void drawWithQuadTree();
	void updateWithQuadTree(float);
	void drawWithBvh();
	void updateWithBvh(float deltaT);

	void drawWithGrid();

	void updateWithGrid(float deltaT);

	int screenWidth, screenHeight;

	std::list<std::shared_ptr<ParticleInterface>> allParticles;
	std::map<int, std::shared_ptr<Particle>> particleMap;

	StaticQuadTreeContainer<Particle> quadTreeParticles;
	std::unique_ptr<BvhContainer<Particle>> bvhContainer;
	std::unique_ptr<GridContainer<Particle>> gridContainer;

	std::random_device randomDevice;
	std::mt19937 randomGenerator;
	bool onOffLines;
	Algo algoState;
};

