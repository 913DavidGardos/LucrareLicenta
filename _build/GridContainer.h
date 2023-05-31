#pragma once
#include<vector>
#include<map>
#include<array>
#include"Particle.h"
#include<iostream>

struct Cell { std::vector<int> itemIds; };

class GridContainer
{
public:
	GridContainer(int rows, int cols, int screenWidth, int screanHeight);
	void insert(int id, float centerX, float centerY);
	void remove(int id);
	void update(std::map<int, std::shared_ptr<Particle>>& particles);
	std::vector<int> query(int id);

	int getRows();
	int getCols();

private:
	int rows, cols;
	float cellWidth, cellHeight;
	std::vector<Cell> grid;
	std::map<int, int> idToIndexMap;
};

