/**
 * @file Cell.h
 * @brief Définition de la structure de base d'une cellule de la carte.
 */

#ifndef CELL_H
#define CELL_H

#include "BiomeIndex.h"

/**
 * @enum Material
 * @brief Énumération des types de ressources possibles.
 */
enum class Material : uint8_t
{
    // Végétation naturelle
    NONE,           /**< Terre battue, roche, sable */
    GRASS,          /**< Herbes simples */
    MOSS,           /**< Mousse et fougères (Fonds sombres et humides) */
    CACTUS,         /**< Désert chaud et sec */
    PINE_TREE,      /**< Conifères (Froid) */
    OAK_TREE,       /**< Chênes / Forêts feuillues (Tempéré) */
    JUNGLE_TREE,     /**< Arbres tropicaux (Chaud et très humide) */

    // Ressources minérales 
    BRONZE_ORE,      /**< Minerai de bronze (Ressource de base pour les civilisations primitives) */
    IRON_ORE,        /**< Minerai de fer (Ressource avancée pour les civilisations plus développées) */
    COAL_ORE,        /**< Minerai de charbon (Ressource pour la production d'énergie et la métallurgie) */
    GOLD_ORE,        /**< Minerai d'or (Ressource précieuse pour le commerce et les civilisations avancées) */

    // Agriculture
    WHEAT           /**< Blé (Céréale de base pour l'alimentation */

    // Civilisation
    
};

enum class Stage : uint8_t
{
    UNDEFINED = 0,
    STAGE_1 = 1,
    STAGE_2 = 2,
    STAGE_3 = 3,
    STAGE_4 = 4
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
    float bedrock;                      /**< Altitude de la roche mère, initialement générée par le bruit. Creusable par l'eau. */
    float granular;                     /**< Épaisseur des sédiments (terre meuble). Déplacée par le courant et la gravité. */
    float temperature;                  /**< Température locale (influence le climat et la détermination du biome). */
    float humidity;                     /**< Humidité locale (favorise la pousse de la végétation et détermine le biome). */
    float light;                        /**< Quantité de lumière solaire atteignant la cellule (influence la croissance de la végétation). */
    float attractivityLevel;            /**< Niveau d'attractivité pour les civilisations, basé sur la combinaison de facteurs environnementaux. */
    float pourcentageEvolution = 0.0f;  /**< Pourcentage d'évolution de la cellule, utilisé pour les transitions progressives (ex: désertification, reforestation). */
    Stage stage = Stage::UNDEFINED;     /**< Stade de développement de la cellule */   
    bool isOccupied = false;
    BiomeIndex biome;                   /**< Type d'écosystème final assigné à la cellule pour son rendu visuel. */
    Material material = Material::NONE; /**< Type de végétation présente sur la cellule, déterminé par les conditions climatiques. */
};

#endif // CELL_H