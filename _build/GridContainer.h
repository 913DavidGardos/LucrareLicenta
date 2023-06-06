#pragma once
#include<vector>
#include<map>
#include<array>
#include<iostream>

struct Cell { std::vector<int> itemIds; };

template <typename T>
class GridContainer
{
public:
	GridContainer(int rows, int cols, int screenWidth, int screenHeight) : rows(rows), cols(cols)
	{
		cellWidth = screenWidth / cols;
		cellHeight = screenHeight / rows;
		grid.resize(rows* cols);
	}

	void insert(int id, float centerX, float centerY)
	{
		// calculate the index
		int bigY = (int)((int)centerY / (int)cellHeight);
		int bigX = (int)((int)centerX / (int)cellWidth);

		int index = bigY * cols + bigX;

		if (index < 0 || index >= grid.size())
		{
			std::cout << "index out of scope\n";
			std::cout << "index = " << index << "\n";
			std::cout << "X = " << centerX << " , Y = " << centerY << "\n";
			std::cout << "cols = " << cols << " , rows = " << rows << "\n";
			std::cout << "cellhight = " << cellHeight << " cellWitdh = " << cellWidth << "\n";
			std::cout << "(centerY / cellHeight) = " << bigY << "\n";
			std::cout << "(centerY / cellHeight) * cols = " << bigX * cols << "\n";
			std::cout << "(centerX / cellWidth) = " << bigX << "\n";
			std::cout << "((centerY / cellHeight) * cols + (centerX / cellWidth)) = " << ((centerY / cellHeight) * cols + (centerX / cellWidth)) << "\n";
			std::cout << "((centerY / cellHeight) * cols + (centerX / cellWidth)) - 2.f = " << ((centerY / cellHeight) * cols + (centerX / cellWidth)) - 2.f << "\n";

			return;
		}

		// push the Id at the correct spot in the array
		grid[index].itemIds.push_back(id);

		// add into the map the id to index pair
		idToIndexMap[id] = index;
	}
	void remove(int id)
	{
		// find the index where the id is located
		int index = idToIndexMap[id];

		// erase the element from the map
		idToIndexMap.erase(id);

		// index where to remove the id
		int delIndex = -1;

		// remove from the container where the id is located
		for (int i = 0; i < grid[index].itemIds.size(); i++)
			if (grid[index].itemIds[i] == id)
			{
				// found the index
				delIndex = i;
				break;
			}

		// erase 
		if (delIndex != -1)
			grid[index].itemIds.erase(grid[index].itemIds.begin() + delIndex);
	}
	void update(std::map<int, std::shared_ptr<T>>& particles)
	{
		for (auto elem : particles)
		{
			// check if the particle belongs to the same cell as before
			int oldIndex = idToIndexMap[elem.second->getId()];
			int newIndex = (elem.second->getY() / cellHeight) * cols + (elem.second->getX() / cellWidth) - 2;

			// element left the original cell
			if (oldIndex != newIndex)
			{
				// erase from old cell
				remove(elem.second->getId());

				// reinsert
				insert(elem.second->getId(), elem.second->getX(), elem.second->getY());
			}
		}
	}
	std::vector<int> query(int id)
	{
		std::vector<int> result;

		int index = idToIndexMap[id];

		if (grid[index].itemIds.empty())
		{
			return result;
		}


		// indices around the object selected
		int topLeft = index - cols - 1;
		int topMid = index - cols;
		int topRight = index - cols + 1;

		int indexLeft = index - 1;
		int indexRight = index + 1;

		int bottomLeft = index + cols - 1;
		int bottomMid = index + cols;
		int bottomRight = index + cols + 1;

		std::array<int, 9> cellsForQuery{ topLeft,    topMid,    topRight,
										 indexLeft,  index,     indexRight,
										 bottomLeft, bottomMid, bottomRight };

		for (auto elem : cellsForQuery)
		{
			if (elem >= 0 && elem < grid.size())
			{
				result.insert(result.end(), grid[elem].itemIds.begin(), grid[elem].itemIds.end());
			}
		}

		return result;
	}

	int getRows()
	{
		return rows;
	}
	int getCols()
	{
		return cols;
	}

private:
	int rows, cols;
	float cellWidth, cellHeight;
	std::vector<Cell> grid;
	std::map<int, int> idToIndexMap;
};

