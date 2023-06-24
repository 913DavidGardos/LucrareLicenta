#pragma once
#include<vector>
#include<map>
#include<array>
#include<iostream>


/// \struct Cell
/// \brief Reprezinta o celula ce contine o lista de identificatori de elemente.
struct Cell
{
    std::vector<int> itemIds; ///< Lista de identificatori de elemente.
};

/// \class GridContainer
/// \brief Reprezinta un container bazat pe o retea pentru impartirea spatiala a elementelor.
/// \tparam T Tipul elementelor de stocat in retea.
template <typename T>
class GridContainer
{
public:
    /// \brief Construieste un obiect GridContainer cu numarul specificat de randuri si coloane, si dimensiunile ecranului.
    /// \param rows Numarul de randuri in retea.
    /// \param cols Numarul de coloane in retea.
    /// \param screenWidth Latimea ecranului.
    /// \param screenHeight Inaltimea ecranului.
    GridContainer(int rows, int cols, int screenWidth, int screenHeight) : rows(rows), cols(cols)
    {
        cellWidth = screenWidth / cols;
        cellHeight = screenHeight / rows;
        grid.resize(rows * cols);
    }

    /// \brief Insereaza un element cu un identificator si coordonatele centrului specificate in celula corespunzatoare din retea.
    /// \param id Identificatorul elementului de inserat.
    /// \param centerX Coordonata X a centrului elementului.
    /// \param centerY Coordonata Y a centrului elementului.
    void insert(int id, float centerX, float centerY)
    {
        // calculeaza indexul
        int bigY = (int)(centerY / cellHeight);
        int bigX = (int)(centerX / cellWidth);
        long index = bigY * cols + bigX;

        if (index < 0 || index >= grid.size())
        {
            std::cout << "index in afara domeniului\n";
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

        // adauga id-ul la pozitia corecta in vector
        grid[index].itemIds.push_back(id);

        // adauga in mapare perechea id-index
        idToIndexMap[id] = index;
    }

    /// \brief Elimina elementul cu identificatorul specificat din retea.
    /// \param id Identificatorul elementului de eliminat.
    void remove(int id)
    {
        // gaseste indexul la care se afla id-ul
        int index = idToIndexMap[id];

        // sterge elementul din mapare
        idToIndexMap.erase(id);

        // indexul de la care se va elimina id-ul
        int delIndex = -1;

        // elimina din containerul in care se afla id-ul
        for (int i = 0; i < grid[index].itemIds.size(); i++)
        {
            if (grid[index].itemIds[i] == id)
            {
                // am gasit indexul
                delIndex = i;
                break;
            }
        }

        // sterge
        if (delIndex != -1)
        {
            grid[index].itemIds.erase(grid[index].itemIds.begin() + delIndex);
        }
    }

    /// \brief Actualizeaza reteaua pe baza pozitiilor elementelor din harta particulelor furnizata.
    /// \param particles O mapare ce contine particulele cu identificatorii lor ca chei.
    void update(std::map<int, std::shared_ptr<T>>& particles)
    {
        for (auto elem : particles)
        {
            // verifica daca particula apartine aceleiasi celule ca inainte
            int oldIndex = idToIndexMap[elem.second->getId()];
            int newIndex = (elem.second->getY() / cellHeight) * cols + (elem.second->getX() / cellWidth) - 2;

            // elementul a parasit celula originala
            if (oldIndex != newIndex)
            {
                // elimina din celula veche
                remove(elem.second->getId());

                // re-insereaza
                insert(elem.second->getId(), elem.second->getX(), elem.second->getY());
            }
        }
    }

    /// \brief Obtine un vector de identificatori de elemente in celulele adiacente celei care contine identificatorul specificat.
    /// \param id Identificatorul elementului de interogat.
    /// \return Un vector de identificatori de elemente in celulele adiacente.
    std::vector<int> query(int id)
    {
        std::vector<int> result;

        int index = idToIndexMap[id];

        if (grid[index].itemIds.empty())
        {
            return result;
        }

        // indicii celulelor in jurul obiectului selectat
        int topLeft = index - cols - 1;
        int topMid = index - cols;
        int topRight = index - cols + 1;

        int indexLeft = index - 1;
        int indexRight = index + 1;

        int bottomLeft = index + cols - 1;
        int bottomMid = index + cols;
        int bottomRight = index + cols + 1;

        std::array<int, 9> cellsForQuery{ topLeft, topMid, topRight,
                                         indexLeft, index, indexRight,
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

    /// \brief Obtine numarul de randuri din retea.
    /// \return Numarul de randuri din retea.
    int getRows()
    {
        return rows;
    }

    /// \brief Obtine numarul de coloane din retea.
    /// \return Numarul de coloane din retea.
    int getCols()
    {
        return cols;
    }

    /// \brief Reseteaza reteaua prin stergerea tuturor celulelor si a maparii ID-ului catre index.
    void reset()
    {
        grid.clear();
        idToIndexMap.clear();
    }

    /// \brief Calculeaza dimensiunea totala a structurii de date.
    /// \return Dimensiunea totala a structurii de date in bytes.
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
    int rows;                      ///< Numarul de randuri in retea.
    int cols;                      ///< Numarul de coloane in retea.
    float cellWidth;               ///< Latimea fiecarei celule.
    float cellHeight;              ///< Inaltimea fiecarei celule.
    std::vector<Cell> grid;        ///< Reteaua care contine celulele.
    std::map<int, int> idToIndexMap; ///< Mapare pentru a stoca identificatori de elemente si indicii celulelor corespunzatoare.
};
