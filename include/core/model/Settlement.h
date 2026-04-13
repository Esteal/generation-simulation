#ifndef FACTION_H
#define FACTION_H

#include <cstddef>
#include <vector>

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


struct Faction {
    int id;
    int populationTotale = 0;
    
    // --- Inventaire Global ---
    int stockBronze = 0;
    int stockIron = 0;
    int stockCoal = 0;
    int stockGold = 0;

    // --- Technologies ---
    int techLevel = 1;

    //
    std::vector<Settlement> colonies; 
};

#endif