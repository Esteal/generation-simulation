#ifndef TERRITORY_SYSTEM_H
#define TERRITORY_SYSTEM_H

#include "ISimulationSystem.h"
#include <vector>
#include <utility>

/**
 * @file TerritorySystem.h
 * @brief Gestion des revendications territoriales et résolution des conflits entre factions.
 *
 * Le TerritorySystem gère les cellules revendiquées par les factions, calcule les tensions
 * lorsque plusieurs factions s'opposent sur une même zone et applique les règles de
 * résolution (conquête, abandon, stabilisation [non implémentée]) à chaque itération de la simulation.
 */

/**
 * @class TerritorySystem
 * @brief Système de gestion des territoires et des tensions inter-factions.
 *
 * Hérite de ISimulationSystem pour être exécuté périodiquement depuis la boucle de simulation.
 * Il maintient une liste de cellules revendiquées et applique un facteur de tension lors
 * des chevauchements d'influence entre factions.
 */
class TerritorySystem : public ISimulationSystem {
private:
    std::vector<std::pair<int, int>> claimedCells; /**< Liste des cellules revendiquées (coordonnées x,y). */
    int TENSION_FACTOR;                             /**< Coefficient utilisé pour augmenter la tension lors d'un conflit. */

public:
    /**
     * @brief Constructeur par défaut.
     *
     * Initialise le système de territoire et ses paramètres par défaut.
     */
    TerritorySystem();

    /**
     * @brief Met à jour l'état des revendications territoriales.
     *
     * À chaque itération, cette méthode :
     * - parcourt les cellules revendiquées,
     * - calcule et applique l'augmentation de tension en cas de conflit,
     * - résout ou propage les changements de propriété selon la logique de la simulation.
     *
     * @param map Référence à la carte de simulation sur laquelle opérer.
     * @param deltaTime Temps écoulé depuis la dernière mise à jour
     *
     * @override Implémente la méthode virtuelle de ISimulationSystem.
     */
    void process(Map& map, float deltaTime) override;
};

#endif