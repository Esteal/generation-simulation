/**
 * @file IFactionSystem.h
 * @brief Déclaration de l'interface pour les systèmes de gestion des factions
 */

#ifndef I_FACTION_SYSTEM_H
#define I_FACTION_SYSTEM_H

#include "Map.h"
#include "Faction.h"

/**
 * @struct Candidate
 * @brief Représente un site candidate pour l'expansion d'une faction
 * 
 * Cette structure est utilisée pour évaluer et classer les emplacements potentiels
 * où une faction pourrait fonder une nouvelle colonie ou effectuer une expansion.
 */
struct Candidate {
    int x;                  /**< Coordonnée X du site candidat sur la grille */
    int y;                  /**< Coordonnée Y du site candidat sur la grille */
    float attractivity;     /**< Score d'attractivité du site (plus élevé = plus attrayant) */
};

/**
 * @class IFactionSystem
 * @brief Interface de base pour les systèmes de gestion des factions
 * 
 * Cette classe abstraite définit l'interface commune pour tous les sous-systèmes
 * chargés de traiter les aspects spécifiques des factions (démographie, alimentation,
 * diplomatie, etc.). Elle fournit également des utilitaires partagés pour l'évaluation
 * de l'attractivité des terrains en fonction du niveau technologique.
 */
class IFactionSystem {
private:
    // --- Paramètres de calcul d'attractivité ---
    float SOIL_BONUS_MULTIPLIER;                /**< Multiplicateur du bonus de sol fertile */
    float SOIL_BONUS_MAX;                       /**< Bonus maximal accordé pour la fertilité du sol */
    float ALTITUDE_THRESHOLD;                   /**< Seuil d'altitude au-delà duquel une pénalité s'applique */
    float ALTITUDE_MALUS_MULTIPLIER;            /**< Multiplicateur de la pénalité d'altitude */
    float FERTILITY_TEMPERATURE_WEIGHT;         /**< Poids de la température dans le calcul de fertilité */
    float FERTILITY_HUMIDITY_WEIGHT;            /**< Poids de l'humidité dans le calcul de fertilité */
    float FERTILITY_LIGHT_WEIGHT;               /**< Poids de la lumière dans le calcul de fertilité */
    int SCAN_RADIUS;                            /**< Rayon de balayage pour la recherche de sites candidats */
    
    // --- Bonus par ère technologique ---
    float STONE_AGE_WATER_BONUS;                /**< Bonus d'attractivité pour l'accès à l'eau (Âge de pierre) */
    float STONE_AGE_WOOD_BONUS;                 /**< Bonus d'attractivité pour l'accès au bois (Âge de pierre) */
    float BRONZE_AGE_WOOD_BONUS;                /**< Bonus d'attractivité pour l'accès au bois (Âge du bronze) */
    float BRONZE_AGE_RESOURCE_BONUS;            /**< Bonus d'attractivité pour l'accès aux minerais (Âge du bronze) */
    float IRON_AGE_BRONZE_BONUS;                /**< Bonus d'attractivité pour l'accès au bronze (Âge du fer) */
    float IRON_AGE_RESOURCE_BONUS;              /**< Bonus d'attractivité pour l'accès aux minerais avancés (Âge du fer) */

public:
    int MIN_MIGRANTS;                           /**< Nombre minimum d'habitants requis pour fonder une nouvelle colonie */
    float GROWTH_RATE;                          /**< Taux de croissance démographique (+50% de population par seconde si score 1.0) */
    
    /**
     * @brief Calcule l'attractivité d'un site basée sur le niveau technologique
     * 
     * Évalue l'attractivité d'une cellule en fonction de ses caractéristiques 
     * environnementales et des bonus spécifiques au niveau technologique de la faction.
     * 
     * @param map Référence constante à la carte de simulation
     * @param cx Coordonnée X de la cellule à évaluer
     * @param cy Coordonnée Y de la cellule à évaluer
     * @param techLevel Niveau technologique de la faction pour appliquer les bonus appropriés
     * @return Score d'attractivité (plus élevé = plus attrayant)
     */
    float calculateTechAttractivity(const Map& map, int cx, int cy, TechLevel techLevel);
    
    /**
     * @brief Tue une portion de la population d'une faction
     * 
     * Réduit la population d'une faction en raison de famine, maladie, guerre ou autres
    * causes. Peut également transférer une partie de la population vaincue à un conquérant.
     * 
     * @param map Référence à la carte de simulation
     * @param faction Référence à la faction dont réduire la population
     * @param totalDeaths Nombre d'habitants décédés
     * @param conqueror Pointeur optionnel vers la faction conquérante qui peut augmenter sa population
     */
    virtual void killPopulation(Map& map, Faction& faction, int totalDeaths, Faction* conqueror = nullptr);
    
    /**
     * @brief Réinitialise une cellule à son état initial
     * 
     * Réinialise les propriétés d'une cellule après sa capture, destruction ou libération,
     * en supprimant les marques de propriété et en restaurant ses caractéristiques naturelles.
     * 
     * @param cell Référence à la cellule à réinitialiser
     */
    void resetCell(Cell& cell);
    
    /**
     * @brief Destructeur virtuel
     * 
     * Assure la destruction correcte des classes dérivées.
     */
    virtual ~IFactionSystem() = default;
    
    /**
     * @brief Constructeur de l'interface des systèmes de factions
     * 
     * Initialise les paramètres par défaut pour la gestion des factions.
     */
    IFactionSystem();
    
    /**
     * @brief Traite l'évolution d'une faction à chaque itération de simulation
     * 
     * Méthode abstraite que chaque sous-système doit implémenter pour traiter
     * son aspect spécifique de la gestion des factions.
     * 
     * @param map Référence à la carte de simulation
     * @param faction Référence à la faction à traiter
     * @param deltaTime Intervalle de temps écoulé depuis la dernière mise à jour (en secondes)
     */
    virtual void processFaction(Map& map, Faction& faction, float deltaTime) = 0;
    
    // TODO : implémenter ce système à la toute fin quand tous les .h chargeront la config 
    // virtual void loadConfig() = 0;
};

#endif