#ifndef GRID_H
#define GRID_H

#include <vector>
/**
 * @file Grid.h
 * @brief Déclaration de la classe Grid
 * Cette classe représente une grille de cellules générique, permettant de stocker et manipuler des données de type T.
 * Elle fournit des méthodes pour accéder et modifier les cellules, ainsi que pour obtenir les dimensions de la grille.
*/

 template<typename T>
/**
 * @class Grid
 * @brief Représente une grille de cellules
 * Cette classe générique permet de créer une grille de n'importe quel type de cellule.
 * Elle gère les dimensions de la grille et fournit des méthodes pour accéder et modifier les cellules
 */
class Grid
{
private:
    size_t width; 
    size_t height;
    std::vector<T> tab;

public:
    // Constructeurs
    Grid() : width(0), height(0) {}
    Grid(size_t w, size_t h) : width(w), height(h), tab(w*h) {}

    // Accès aux cellules
    T& get(size_t x, size_t y)
    {
        return tab[y * width + x];
    }
    // Accesseur
    const T& get(size_t x, size_t y) const
    {
        return tab[y * width + x];
    }
    // Mutateur
    void set(size_t x, size_t y, const T& value)
    {
        tab[y * width + x] = value;
    }

    // Opérateur pratique
    T& operator()(size_t x, size_t y)
    { 
        return tab[y * width + x]; 
    }

    const T& operator()(size_t x, size_t y) const 
    { 
        return tab[y * width + x]; 
    }

    // Dimensions
    size_t getWidth() const { return width; }
    size_t getHeight() const { return height; }

};

#endif