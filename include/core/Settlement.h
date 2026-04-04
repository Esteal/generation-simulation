#ifndef SETTLEMENT_H
#define SETTLEMENT_H

#include <cstddef>

/**
 * @struct Settlement
 * @brief Représente un regroupement de population sur une case donnée.
 */
struct Settlement {
    size_t x;                 /**< Position X sur la grille */
    size_t y;                 /**< Position Y sur la grille */
    int population;           /**< Nombre d'habitants actuels */
    int factionId;            /**< Identifiant de la civilisation (pour la couleur/diplomatie) */
    
    // Potentielles futures variables :
    // float storedFood;      // Nourriture stockée 
    // int techLevel;         // Niveau technologique
};

#endif