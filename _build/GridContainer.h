#pragma once
#include<vector>
#include<map>
#include<array>
#include<iostream>

/// \file
/// This file contains the definition of the GridContainer class used for spatial partitioning in a grid-based system.

/// \struct Cell
/// \brief Represents a cell in the grid, storing a vector of item IDs contained within it.
struct Cell { std::vector<int> itemIds; };

/// \class GridContainer
/// \brief Represents a grid-based container for spatial partitioning of items.
/// \tparam T The type of the items to be stored in the grid.
template <typename T>
class GridContainer
{
public:
	/// \brief Constructs a GridContainer with the specified number of rows and columns, and the screen dimensions.
	/// \param rows The number of rows in the grid.
	/// \param cols The number of columns in the grid.
	/// \param screenWidth The width of the screen.
	/// \param screenHeight The height of the screen.
	GridContainer(int rows, int cols, int screenWidth, int screenHeight) : rows(rows), cols(cols)
	{
		cellWidth = screenWidth / cols;
		cellHeight = screenHeight / rows;
		grid.resize(rows* cols);
	} 

	/// \brief Inserts an item with the specified ID and center coordinates into the appropriate cell in the grid.
	/// \param id The ID of the item to be inserted.
	/// \param centerX The X coordinate of the item's center.
	/// \param centerY The Y coordinate of the item's center.
	void insert(int id, float centerX, float centerY)
	{
		// calculate the index
		int bigY = (int)((int)centerY / (int)cellHeight);
		int bigX = (int)((int)centerX / (int)cellWidth);

		long index = bigY * cols + bigX;

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

	/// \brief Removes the item with the specified ID from the grid.
	/// \param id The ID of the item to be removed.
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

	/// \brief Updates the grid based on the positions of the items in the provided particle map.
	/// \param particles A map containing the particles with their respective IDs as keys.
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

	/// \brief Retrieves a vector of item IDs within the cells adjacent to the cell containing the specified ID.
	/// \param id The ID of the item to query.
	/// \return A vector of item IDs within the adjacent cells.
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

	/// \brief Retrieves the number of rows in the grid.
	/// \return The number of rows in the grid.
	int getRows()
	{
		return rows;
	}

	/// \brief Retrieves the number of columns in the grid.
	/// \return The number of columns in the grid.
	int getCols()
	{
		return cols;
	}

	/// \brief Resets the grid by clearing all cells and the ID-to-index map.
	void reset()
	{
		grid.clear();
		idToIndexMap.clear();
	}

	size_t sizeOfDataStructure()
	{
		size_t count = 0;
		for (auto elem : idToIndexMap)
		{
			count += sizeof(elem.first);
			count += sizeof(elem.second);
		}

		for (auto elem : grid)
		{
			count += elem.itemIds.size();
		}

		count += sizeof(rows);
		count += sizeof(cols);
		count += sizeof(cellWidth);
		count += sizeof(cellHeight);

		return count;
	}

private:
	int rows;                      ///< The number of rows in the grid.
	int cols;                      ///< The number of columns in the grid.
	float cellWidth;               ///< The width of each cell.
	float cellHeight;              ///< The height of each cell.
	std::vector<Cell> grid;        ///< The grid containing cells.
	std::map<int, int> idToIndexMap; ///< A map to store item IDs and their corresponding cell indices.
};