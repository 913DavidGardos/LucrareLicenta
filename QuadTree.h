#pragma once
#include <raylib.h>
#include <vector>
#include <array>
#include <memory>
#include <list>

/**
 * \class StaticQuadTree
 * \brief O implementare statica a unei structuri de date quadtree.
 * \details StaticQuadTree este o clasa templata care reprezinta o structura de date quadtree. Este folosit pentru a partitiona un spatiu bidimensional in regiuni mai mici, permitand interogari spatiale eficiente.
 */
template <typename T>
class StaticQuadTree
{
public:
    /**
     * \brief Construieste un obiect StaticQuadTree cu un dreptunghi initial optional si o adancime.
     * \param rectangle Dreptunghiul initial care reprezinta limitele quadtree-ului.
     * \param depth Adancimea initiala a quadtree-ului.
     */
    StaticQuadTree(const Rectangle& rectangle = { 0.f, 0.f, 100.f, 100.f }, const size_t depth = 0) :
        parent(this), rectangle(rectangle), depth(depth)
    {
        resize(rectangle);
    }

    /**
     * \brief Redimensioneaza quadtree-ul cu un nou dreptunghi.
     * \param rArea Noul dreptunghi care reprezinta limitele quadtree-ului.
     */
    void resize(const Rectangle& rArea)
    {
        clear();
        rectangle = rArea;
        float childWidth = rectangle.width / 2.0f;
        float childHeight = rectangle.height / 2.0f;

        childRec =
        {
            Rectangle {rectangle.x, rectangle.y, childWidth, childHeight},                                // top left
            Rectangle {rectangle.x + childWidth, rectangle.y, childWidth, childHeight},                    // top right
            Rectangle {rectangle.x, rectangle.y + childHeight, childWidth, childHeight},                // bottom left
            Rectangle {rectangle.x + childWidth, rectangle.y + childHeight, childWidth, childHeight}    // bottom right
        };
    }

    /**
     * \brief Curata quadtree-ul, eliminand toate elementele si nodurile copil.
     */
    void clear()
    {
        items.clear();
        for (int i = 0; i < 4; i++)
        {
            if (childPtr[i])
                childPtr[i]->clear();
            childPtr[i].reset();
        }
    }

    /**
     * \brief Returneaza numarul de elemente stocate in quadtree.
     * \return Numarul de elemente stocate in quadtree.
     */
    size_t size() const
    {
        size_t count = items.size();
        for (int i = 0; i < 4; i++)
            if (childPtr[i])
                count += childPtr[i]->size();

        return count;
    }

    /**
     * \brief Returneaza dimensiunea totala a structurii de date quadtree.
     * \return Dimensiunea totala a structurii de date quadtree.
     */
    size_t sizeOfDataStructure() const
    {
        size_t count = items.size();
        count += sizeof(depth);
        count += sizeof(rectangle);
        count += sizeof(parent);

        for (int i = 0; i < 4; i++)
            count += sizeof(childRec[i]);

        for (int i = 0; i < 4; i++)
            if (childPtr[i])
                count += childPtr[i]->sizeOfDataStructure();

        return count;
    }

    /**
     * \brief Insereaza un element impreuna cu dimensiunea sa corespunzatoare in quadtree.
     * \param item Elementul de inserat.
     * \param itemSize Dimensiunea (dreptunghiul) elementului.
     */
    void insert(const T& item, const Rectangle& itemSize)
    {
        for (int i = 0; i < 4; i++)
        {
            if (firstContainsSecond(childRec[i], itemSize))
            {
                // Am atins limita de adancime?
                if (depth + 1 < maxDepth)
                {
                    // Exista copilul?
                    if (!childPtr[i])
                    {
                        // Nu, creeaza unul
                        childPtr[i] = std::make_shared<StaticQuadTree<T>>(childRec[i], depth + 1);
                        childPtr[i]->parent = this;
                    }

                    childPtr[i]->insert(item, itemSize);
                    return;
                }
            }
        }

        items.push_back({ itemSize, item });
    }

    /**
     * \brief Cauta elemente intr-o zona specificata.
     * \param rArea Zona in care se cauta elemente.
     * \return O lista de elemente in interiorul zonei specificate.
     */
    std::list<T> _search(const Rectangle& rArea) const
    {
        std::list<T> listItems;
        search(rArea, listItems);
        return listItems;
    }

    /**
     * \brief Verifica daca primul dreptunghi contine al doilea dreptunghi.
     * \param first Primul dreptunghi.
     * \param second Al doilea dreptunghi.
     * \return True daca primul dreptunghi contine al doilea dreptunghi, false in caz contrar.
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

    /**
     * \brief Parcurge quadtree-ul si deseneaza limitele sale.
     */
    void traverse()
    {
        DrawRectangleLines(rectangle.x, rectangle.y, rectangle.width, rectangle.height, GRAY);

        for (int i = 0; i < 4; i++)
        {
            if (childPtr[i])
                childPtr[i]->traverse();
        }
    }

    /**
     * \brief Returneaza dreptunghiul quadtree-ului parinte.
     * \return Dreptunghiul quadtree-ului parinte.
     */
    Rectangle getParentRec()
    {
        return parent->rectangle;
    }

    /**
     * \brief Returneaza dreptunghiul quadtree-ului curent.
     * \return Dreptunghiul quadtree-ului curent.
     */
    Rectangle getRec()
    {
        return rectangle;
    }

    /**
     * \brief Returneaza quadtree-ul parinte.
     * \return Un pointer la quadtree-ul parinte.
     */
    StaticQuadTree<T>* getParent()
    {
        return parent;
    }

    /**
     * \brief Returneaza quadtree-ul copil de la indexul specificat.
     * \param i Indexul quadtree-ului copil.
     * \return Un pointer partajat la quadtree-ul copil.
     */
    std::shared_ptr<StaticQuadTree<T>> getChildPtr(int i)
    {
        return childPtr[i];
    }

    /**
     * \brief Returneaza un vector de elemente stocate in quadtree.
     * \return Un vector de elemente stocate in quadtree.
     */
    std::vector<std::pair<Rectangle, T>> getItems()
    {
        return items;
    }

protected:
    /**
     * \brief Recupereaza elementele din zona specificata si le adauga in lista furnizata.
     * \param rArea Zona in care se cauta elemente.
     * \param listItems Lista in care se vor stoca elementele gasite.
     */
    void search(const Rectangle& rArea, std::list<T>& listItems) const
    {
        for (const auto& p : items)
        {
            if (CheckCollisionRecs(rArea, p.first))
                listItems.push_back(p.second);
        }

        for (int i = 0; i < 4; i++)
        {
            if (childPtr[i])
            {
                if (firstContainsSecond(rArea, childRec[i]))
                {
                    childPtr[i]->retriveItems(listItems);
                }
                else if (CheckCollisionRecs(childRec[i], rArea))
                {
                    childPtr[i]->search(rArea, listItems);
                }
            }
        }
    }

    /**
     * \brief Recupereaza elementele stocate in quadtree si le adauga in lista furnizata.
     * \param listItems Lista in care se vor stoca elementele recuperate.
     */
    void retriveItems(std::list<T>& listItems) const
    {
        for (const auto& p : items)
            listItems.push_back(p.second);

        for (int i = 0; i < 4; i++)
            if (childPtr[i])
                childPtr[i]->retriveItems(listItems);
    }

    StaticQuadTree<T>* parent = nullptr;                             ///< Pointer la quadtree-ul parinte.
    size_t depth = 0;                                                ///< Adancimea quadtree-ului.
    Rectangle rectangle;                                             ///< Dreptunghiul care reprezinta limitele quadtree-ului.
    std::array<Rectangle, 4> childRec{};                              ///< Dreptunghiurile copiilor quadtree-ului.
    std::array<std::shared_ptr<StaticQuadTree<T>>, 4> childPtr{};     ///< Pointeri la quadtree-urile copii.
    std::vector<std::pair<Rectangle, T>> items;                       ///< Container pentru elementele stocate la acest nivel.
    static const int maxDepth = 7;                                    ///< Adancimea maxima a quadtree-ului.
};
