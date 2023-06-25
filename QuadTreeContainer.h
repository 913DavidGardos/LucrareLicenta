#pragma once
#include <iostream>
#include "QuadTree.h"

/**
 * \brief Un container care utilizeaza un quadtree static pentru a stoca elemente de tip T.
 *
 * Clasa template StaticQuadTreeContainer este un container care utilizeaza o structura de date quadtree
 * static pentru a stoca si gestiona elemente de tip T. Aceasta permite insertia, eliminarea si cautarea
 * eficienta a elementelor pe baza pozitiei lor intr-un spatiu 2D.
 */
template <typename T>
class StaticQuadTreeContainer
{
    using QuadTreeContainer = std::list<std::shared_ptr<T>>;  ///< Tipul de container subiacent.

protected:
    QuadTreeContainer allItems; ///< Lista tuturor elementelor din container.
    StaticQuadTree<typename QuadTreeContainer::iterator> root; ///< Radacina quadtree-ului static.
    std::list<std::shared_ptr<StaticQuadTree<typename QuadTreeContainer::iterator>>> leaves; ///< Nodurile quadtree-ului.

public:
    /**
     * \brief Construieste un obiect StaticQuadTreeContainer cu dreptunghiul si adancimea specificate.
     * \param rectangle Dreptunghiul de delimitare al quadtree-ului.
     * \param depth Adancimea maxima a quadtree-ului.
     */
    StaticQuadTreeContainer(const Rectangle& rectangle, const size_t depth) : root(rectangle, depth)
    {

    }

    /**
     * \brief Redimensioneaza quadtree-ul pentru a se potrivi cu zona specificata.
     * \param rArea Noul dreptunghi de delimitare al quadtree-ului.
     */
    void resize(const Rectangle& rArea)
    {
        root.resize(rArea);
    }

    /**
     * \brief Returneaza numarul de elemente din container.
     * \return Numarul de elemente din container.
     */
    size_t size() const
    {
        return allItems.size();
    }

    /**
     * \brief Returneaza dimensiunea totala a structurii de date quadtree.
     * \return Dimensiunea totala a structurii de date quadtree.
     */
    size_t sizeOfDataStructure() const
    {
        return root.sizeOfDataStructure();
    }

    /**
     * \brief Verifica daca containerul este gol.
     * \return `true` daca containerul este gol, `false` in caz contrar.
     */
    bool empty() const
    {
        return allItems.empty();
    }

    /**
     * \brief Goleste containerul, eliminand toate elementele.
     */
    void clear()
    {
        root.clear();
        allItems.clear();
    }

    /**
     * \brief Insereaza un element in container la pozitia specificata.
     * \param item Un pointer partajat la elementul de inserat.
     * \param itemSize Dimensiunea dreptunghiulara a elementului.
     */
    void insert(const std::shared_ptr<T> item, const Rectangle& itemSize)
    {
        allItems.push_back(item);
        root.insert(std::prev(allItems.end()), itemSize);
    }

    /**
     * \brief Cauta elemente in interiorul zonei specificate.
     * \param rArea Zona in care se cauta.
     * \return O lista de iteratori care indica elementele gasite in zona respectiva.
     */
    std::list<typename QuadTreeContainer::iterator> search(const Rectangle& rArea) const
    {
        return root._search(rArea);
    }

    /**
     * \brief Returneaza un iterator care indica inceputul containerului.
     * \return Un iterator care indica inceputul containerului.
     */
    typename QuadTreeContainer::iterator begin()
    {
        return allItems.begin();
    }

    /**
     * \brief Returneaza un iterator care indica sfarsitul containerului.
     * \return Un iterator care indica sfarsitul containerului.
     */
    typename QuadTreeContainer::iterator end()
    {
        return allItems.end();
    }

    /**
     * \brief Actualizeaza quadtree-ul prin reinserarea tuturor elementelor.
     *
     * Aceasta functie actualizeaza quadtree-ul prin reinserarea tuturor elementelor in el. Este apelata
     * atunci cand pozitiile sau dimensiunile elementelor s-au modificat.
     */
    void update()
    {
        root.clear();

        for (auto iter = allItems.begin(); iter != allItems.end(); ++iter)
        {
            auto it = *iter;
            root.insert(iter, Rectangle{ it->getX() - it->getRadius(), it->getY() - it->getRadius(), it->getRadius() * 2.f, it->getRadius() * 2.f });
        }
    }

    /**
     * \brief Deseneaza liniile quadtree-ului in scopuri de vizualizare.
     *
     * Aceasta functie deseneaza liniile quadtree-ului in scopuri de vizualizare. Poate fi
     * utilizata pentru a vizualiza structura quadtree-ului.
     */
    void drawLines()
    {
        root.traverse();
    }

    /**
     * \brief Verifica daca primul dreptunghi contine al doilea dreptunghi.
     * \param first Primul dreptunghi.
     * \param second Al doilea dreptunghi.
     * \return `true` daca primul dreptunghi contine al doilea dreptunghi, `false` in caz contrar.
     */
    bool firstContainsSecond(const Rectangle& first, const Rectangle& second) const
    {
        if (first.x < second.x &&
            first.y < second.y &&
            first.x + first.width  > second.x + second.width &&
            first.y + first.height > second.y + second.height)
        {
            return true;
        }

        return false;
    }
};
