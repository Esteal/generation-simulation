/**
 * @file Faction.h
 * @brief Déclaration de la structure Faction et des types associés
 * 
 * Cette structure représente une faction/civilisation dans la simulation.
 */


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

/**
 * @enum DiplomaticState
 * @brief États diplomatiques possibles entre deux factions.
 */
enum class DiplomaticState : uint8_t {
    PEACE,  /**< État de paix entre les deux factions */
    WAR     /**< État de guerre entre les deux factions */
};

/**
 * @struct FactionRelation
 * @brief Représente la relation diplomatique entre deux factions.
 */
struct FactionRelation {
    float tension = 0.0f;                       /**< Niveau de tension entre 0.0 (Neutre) et 100.0 (Guerre imminente) */
    DiplomaticState state = DiplomaticState::PEACE;  /**< État diplomatique actuel */
};

/**
 * @enum TechLevel
 * @brief Énumération des niveaux technologiques possibles pour une faction.
 */
enum class TechLevel : uint8_t
{
    UNDEFINED = 0,      /**< Niveau technologique non défini */
    STONE_AGE = 1,      /**< Âge de pierre (outils primitifs, agriculture naissante) */
    BRONZE_AGE = 2,     /**< Âge du bronze (métallurgie du bronze, urbanisation) */
    IRON_AGE = 3        /**< Âge du fer (métallurgie avancée, civilisations complexes) */
};

/**
 * @struct Faction
 * @brief Représente une faction/civilisation dans la simulation.
 *
 * Cette structure contient toutes les informations essentielles d'une faction :
 * démographie, ressources, territoire, technologie et relations diplomatiques.
 */
struct Faction {
    int id;                             /**< Identifiant unique de la faction */
    int populationTotale = 0;           /**< Population totale de la faction */
    
    // --- Inventaire Global ---
    int stockBronze = 0;                /**< Stock de minerai de bronze */
    int stockIron = 0;                  /**< Stock de minerai de fer */
    int stockCoal = 0;                  /**< Stock de minerai de charbon */
    int stockGold = 0;                  /**< Stock de minerai d'or */
    int stockWood = 0;                  /**< Stock de bois (ressource agricole/construction) */
    int stockFood = 0;                  /**< Stock de nourriture (alimentation de la population) */

    // --- Expansion et exploitation ---
    int radiusMigration;                /**< Rayon d'influence pour la migration de la population */
    int radiusExploitation;             /**< Rayon d'exploitation des ressources naturelles */
    int radiusInfluence;                /**< Rayon d'influence diplomatique et culturelle */

    int capitalX;                       /**< Coordonnée X de la capitale */
    int capitalY;                       /**< Coordonnée Y de la capitale */
    
    // --- Technologies ---
    TechLevel techLevel = TechLevel::UNDEFINED;  /**< Niveau technologique actuel de la faction */

    // --- Relations et colonies ---
    std::map<int, FactionRelation> relations;   /**< Relations diplomatiques avec les autres factions (clé: id faction, valeur: relation) */
    std::vector<Settlement> colonies;           /**< Liste des colonies/établissements de la faction */
};

#endif