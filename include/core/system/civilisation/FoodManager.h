#ifndef FOODMANAGER_H
#define FOODMANAGER_H

#include "IFactionSystem.h"

/**
 * @file FoodManager.h
 * @brief Déclaration de la classe FoodManager
 * 
 * Cette classe gère la production agricole, la consommation alimentaire et la gestion
 * des stocks de nourriture des factions dans la simulation.
 */

/**
 * @class FoodManager
 * @brief Gestionnaire de l'agriculture et des ressources alimentaires
 * 
 * Cette classe est responsable de :
 * - La gestion des stocks alimentaires de chaque faction
 * - La consommation de nourriture par la population
 * - L'ensemencement de nouvelles terres agricoles
 * - La régulation de la population en fonction des ressources disponibles
 * 
 * Elle implémente l'interface IFactionSystem pour traiter le cycle agricole à chaque
 * itération de simulation.
 */
class FoodManager : public IFactionSystem {
private:
    // --- Paramètres d'Agriculture ---
    float CONSOMMATION_RATE;          /**< Nourriture consommée par habitant à chaque itération */
    float REPLANT_CHANCE;             /**< Probabilité (entre 0.0 et 1.0) de replanter une graine après récolte */
    int FOOD_HARVESTED;               /**< Quantité de nourriture obtenue par récolte de blé */

    /**
     * @brief Ensemence une cellule avec des graines
     * 
     * Transforme une cellule vide ou cultivable en champ de blé pour la faction spécifiée.
     * 
     * @param targetCell Référence à la cellule à ensemencer
     * @param factionID Identifiant de la faction propriétaire de la culture [outdated, les cultures ne sont plus liés à une faction spécifique]
     */
    void sowSeed(Cell& targetCell, int factionID);

    /**
     * @brief Ensemence plusieurs cellules autour de la faction
     * 
     * Recherche et ensemence les cellules disponibles dans le rayon d'exploitation
     * de la faction pour cultiver du blé.
     * 
     * @param map Référence à la carte de simulation
     * @param faction Référence à la faction pour laquelle ensemencer
     * @param seedsToPlant Nombre de graines à planter
     */
    void sowSeeds(Map& map, Faction& faction, int seedsToPlant); 

    /**
     * @brief Récolte les cultures matures de la faction
     * 
     * Parcourt les cellules occupées par la faction et récolte le blé mature,
     * augmentant ainsi le stock alimentaire de la faction.
     * 
     * @param map Référence à la carte de simulation
     * @param faction Référence à la faction dont récolter les cultures
     * @param deltaTime Intervalle de temps écoulé depuis la dernière mise à jour (en secondes)
     */
    void harvestCrops(Map& map, Faction& faction, float deltaTime);

public:
    /**
     * @brief Constructeur du gestionnaire alimentaire
     * 
     * Initialise le gestionnaire avec les paramètres agricoles par défaut.
     */
    FoodManager();

    /**
     * @brief Traite le cycle agricole d'une faction à chaque itération
     * 
     * Cette méthode gère les étapes du cycle alimentaire :
     * - Consommation de nourriture par la population
     * - Récolte des cultures matures
     * - Ensemencement de nouvelles terres agricoles
     * - Ajustement de la population en fonction des ressources disponibles
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