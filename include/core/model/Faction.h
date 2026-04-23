#ifndef FACTION_H
#define FACTION_H

#include <cstddef>
#include <vector>
#include <map>

/**
 * @struct Settlement
 * @brief Représente un regroupement de population sur une case donnée.
 */
struct Settlement {
    size_t x;                 /**< Position X sur la grille */
    size_t y;                 /**< Position Y sur la grille */
    int population;           /**< Nombre d'habitants actuels */
    int factionId;            /**< Identifiant de la civilisation (pour la couleur/diplomatie) */
    
};

enum class DiplomaticState : uint8_t {
    PEACE,
    WAR
};

struct FactionRelation {
    float tension = 0.0f;                       // De 0.0 (Neutre) à 100.0 (Guerre imminente)
    DiplomaticState state = DiplomaticState::PEACE;
};

enum class TechLevel : uint8_t
{
    UNDEFINED = 0,
    STONE_AGE = 1,
    BRONZE_AGE = 2,
    IRON_AGE = 3
};

struct Faction {
    int id;
    int populationTotale = 0;
    
    // --- Inventaire Global ---
    int stockBronze = 0;
    int stockIron = 0;
    int stockCoal = 0;
    int stockGold = 0;
    int stockWood = 0;
    int stockFood = 0;

    // --- Expansion et exploitation ---
    int radiusMigration;
    int radiusExploitation;
    int radiusInfluence;

    int capitalX;
    int capitalY;
    // --- Technologies ---
    TechLevel techLevel = TechLevel::UNDEFINED;

    //
    std::map<int, FactionRelation> relations;
    std::vector<Settlement> colonies;
};

#endif