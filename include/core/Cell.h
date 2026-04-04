/**
 * @file Cell.h
 * @brief Définition de la structure de base d'une cellule de la carte.
 */

#ifndef CELL_H
#define CELL_H

#include "BiomeIndex.h"

/**
 * @enum FloraType
 * @brief Énumération des types de végétation possibles.
 */
enum class FloraType : uint8_t
{
    NONE,           /**< Terre battue, roche, sable */
    GRASS,          /**< Herbes simples */
    MOSS,           /**< Mousse et fougères (Fonds sombres et humides) */
    CACTUS,         /**< Désert chaud et sec */
    PINE_TREE,      /**< Conifères (Froid) */
    OAK_TREE,       /**< Chênes / Forêts feuillues (Tempéré) */
    JUNGLE_TREE     /**< Arbres tropicaux (Chaud et très humide) */
};


/**
 * @struct Cell
 * @brief Représente une cellule unique (ou tuile) sur la grille de la carte.
 * * Cette structure fondamentale agit comme un conteneur de données pour chaque point 
 * du monde. Elle stocke l'état géologique, climatique et biologique local, qui sera 
 * lu et modifié par les différents systèmes de simulation (ErosionSystem, HydrologieSystem, etc.).
 */
struct Cell
{
    float bedrock;          /**< Altitude de la roche mère, initialement générée par le bruit. Creusable par l'eau. */
    float granular;         /**< Épaisseur des sédiments (terre meuble). Déplacée par le courant et la gravité. */
    float temperature;      /**< Température locale (influence le climat et la détermination du biome). */
    float humidity;         /**< Humidité locale (favorise la pousse de la végétation et détermine le biome). */
    float light;            /**< Quantité de lumière solaire atteignant la cellule (influence la croissance de la végétation). */
    float attractivityLevel; /**< Niveau d'attractivité pour les civilisations, basé sur la combinaison de facteurs environnementaux. */
    bool isOccupied = false;
    BiomeIndex biome;       /**< Type d'écosystème final assigné à la cellule pour son rendu visuel. */
    FloraType flora = FloraType::NONE;  /**< Type de végétation présente sur la cellule, déterminé par les conditions climatiques. */
};

#endif // CELL_H