#ifndef MAP_H
#define MAP_H
#include "Grid.h"
#include "Cell.h"

class Map
{
private:
    Grid<Cell> grid;
public:
    
    Map(size_t width, size_t height) : grid(width, height) {}
    
    /**
     * @brief Retourne la grille de la carte, accès en lecture et écriture
     * @return Référence vers la grille
     */
    Grid<Cell>& getGrid() { return grid; }
    
    /**
     * @brief Retourne la grille de la carte, accès en lecture seule
     * @return Référence constante vers la grille
     */
    const Grid<Cell>& getGrid() const { return grid; }

    size_t getWidth() const { return grid.getWidth(); }
    size_t getHeight() const { return grid.getHeight(); }
};

#endif