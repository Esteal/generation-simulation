#ifndef CITY_MANAGER_H
#define CITY_MANAGER_H

#include "IFactionSystem.h"

/**
 * @file CityManager.h
 * @brief Déclaration de la classe CityManager
 * 
 * Cette classe gère la création, l'initialisation et l'évolution des civilisations dans la simulation.
 * Elle contrôle les paramètres démographiques, technologiques et géographiques des factions.
 */

/**
 * @class CityManager
 * @brief Gestionnaire des villes et civilisations
 * 
 * Cette classe est responsable de :
 * - L'initialisation des factions au démarrage de la simulation
 * - La gestion de la capacité de charge des colonies
 * - Le contrôle des paramètres d'expansion territoriale
 * 
 * Elle implémente l'interface IFactionSystem pour intégrer le système de simulation.
 */
class CityManager : public IFactionSystem 
{
private:
    int CARRYING_CAPACITY_BASE;                             /**< Capacité de charge de base pour une colonie */
    float COHESION_BONUS_PER_CITY;                          /**< Bonus de cohésion accordé par chaque colonie supplémentaire */
    float PENALITY_DISTANCE_FROM_MOTHER_CITY_FACTOR;        /**< Facteur de pénalité appliqué à la distance de la ville mère */

    // --- Paramètres d'équilibrage de la simulation ---
    int MIN_DISTANCE;                                       /**< Distance minimale (en cases) entre deux capitales de factions différentes */
    int numberOfFactions;                                   /**< Nombre total de civilisations à créer lors de l'initialisation */

    // --- Paramètres d'équilibrage à l'initialisation des factions ---
    int STOCK_FOOD;                                         /**< Stock de nourriture initial pour chaque faction (couvre plusieurs années de sustentation) */
    int POPULATION_TOTALE;                                  /**< Population totale initiale pour chaque faction */
    int DEFAULT_EXPAND_MIGRATION_RADIUS;                    /**< Rayon par défaut pour la migration de la population */
    int DEFAULT_EXPAND_PLANT_RADIUS;                        /**< Rayon par défaut pour l'exploitation des ressources naturelles */
    int DEFAULT_EXPAND_INFLUENCE_RADIUS;                    /**< Rayon par défaut pour l'influence diplomatique et culturelle */
    TechLevel TECH_BASE_LEVEL;                              /**< Niveau technologique initial des factions */
    
public:
    /**
     * @brief Constructeur du gestionnaire de villes
     * 
     * Initialise le gestionnaire avec les paramètres de la carte.
     * 
     * @param map Référence à la carte de simulation
     */
    CityManager(Map& map);

    /**
     * @brief Initialise toutes les civilisations au démarrage
     * 
     * Cette méthode crée et positionne les factions initiales sur la carte,
     * en respectant les contraintes de distance minimale entre capitales.
     * Elle configure également les ressources initiales et les paramètres de chaque faction.
     * 
     * @param map Référence à la carte où placer les civilisations
     */
    void initializeCivilizations(Map& map);

    /**
     * @brief Traite l'évolution d'une faction à chaque itération de simulation
     * 
     * Cette méthode gère les mises à jour périodiques d'une faction :
     * - Croissance/décroissance démographique
     * - Consommation de ressources
     * - Migration et expansion territoriale
     * - Progression technologique
     * - Interactions avec les autres factions
     * 
     * @param map Référence à la carte de simulation
     * @param faction Référence à la faction à traiter
     * @param deltaTime Intervalle de temps écoulé depuis la dernière mise à jour
     * 
     * @override Implémente la méthode virtuelle de IFactionSystem
     */
    void processFaction(Map& map, Faction& faction, float deltaTime) override;
};

#endif