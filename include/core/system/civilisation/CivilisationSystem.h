#ifndef CIVILISATIONSYSTEM_H
#define CIVILISATIONSYSTEM_H

#include "ISimulationSystem.h"
#include "IFactionSystem.h"
#include "Faction.h"
#include <vector>
#include <memory>

/**
 * @file CivilisationSystem.h
 * @brief Déclaration de la classe CivilisationSystem
 * 
 * Cette classe gère l'ensemble des systèmes relatifs aux civilisations et factions
 * dans la simulation. Elle coordonne les sous-systèmes de gestion des factions.
 */

/**
 * @class CivilisationSystem
 * @brief Système de gestion global des civilisations
 * 
 * Cette classe est responsable de :
 * - La coordination des sous-systèmes de factions (CityManager, WarfareManager, etc.)
 * - L'exécution périodique des traitements de simulation pour chaque faction
 * - L'agrégation et l'affichage des statistiques démographiques
 * 
 * Elle implémente l'interface ISimulationSystem pour s'intégrer dans la boucle principale
 * de simulation et utilise le pattern Strategy avec les sous-systèmes IFactionSystem.
 */
class CivilisationSystem : public ISimulationSystem {
private:
    std::vector<std::unique_ptr<IFactionSystem>> subSystems;  /**< Liste des sous-systèmes de gestion des factions */

public:
    /**
     * @brief Constructeur du système de civilisations
     * 
     * Initialise le système et crée les sous-systèmes de gestion des factions.
     * 
     * @param map Référence à la carte de simulation
     */
    CivilisationSystem(Map& map);

    /**
     * @brief Traite l'évolution de toutes les civilisations à chaque itération
     * 
     * Cette méthode exécute les traitements périodiques pour chaque faction présente
     * sur la carte en délégant le traitement aux sous-systèmes enregistrés.
     * 
     * @param map Référence à la carte de simulation
     * @param deltaTime Intervalle de temps écoulé depuis la dernière mise à jour
     * 
     * @override Implémente la méthode virtuelle de ISimulationSystem
     */
    void process(Map& map, float deltaTime) override;

    /**
     * @brief Affiche la population moyenne par ville
     * 
     * Cette méthode calcule et affiche des statistiques démographiques sur la distribution
     * de la population entre les différentes colonies et civilisations.
     * 
     * @param map Référence constante à la carte de simulation
     */
    void displayAveragePopulationPerCity(const Map& map);
};

#endif