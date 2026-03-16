#ifndef CELL_H
#define CELL_H

#include "BiomeIndex.h"

/**
 * @struct Cell
 * @brief Représente une cellule unique
 */
struct Cell
{
    
    float altitude;
    float temperature;
    float humidity;
    BiomeIndex biome;
};

#endif