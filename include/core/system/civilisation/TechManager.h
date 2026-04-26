#ifndef TECH_MANAGER_H
#define TECH_MANAGER_H

#include "IFactionSystem.h"

/**
 * @file TechManager.h
 * @brief Déclaration de la classe TechManager
 *
 * Gestion de la progression technologique des factions : coûts, prérequis et application
 * des bonus associés aux transitions d'ère (p.ex. Stone -> Bronze -> Iron).
 */

/**
 * @class TechManager
 * @brief Gestionnaire des recherches et des améliorations technologiques
 *
 * Cette classe hérite de IFactionSystem et gère :
 * - le suivi des coûts et prérequis pour débloquer chaque palier technologique,
 * - l'application des bonus liés à une montée en niveau technologique,
 * - la consommation de ressources nécessaires à la recherche.
 */
class TechManager : public IFactionSystem {
private:
    // --- Coûts et prérequis pour le passage à l'Âge du Bronze ---
    int COST_WOOD_BRONZE;        /**< Quantité de bois requise pour passer au Bronze */
    int REQ_POP_BRONZE;          /**< Population minimale requise pour débloquer le Bronze */
    int REQ_COLONIES_BRONZE;     /**< Nombre minimal de colonies requis pour le Bronze */

    // --- Coûts et prérequis pour le passage à l'Âge du Fer ---
    int COST_BRONZE_IRON;        /**< Quantité de bronze requise pour passer au Fer */
    int COST_COAL_IRON;          /**< Quantité de charbon requise pour passer au Fer */
    int REQ_POP_IRON;            /**< Population minimale requise pour débloquer le Fer */
    int REQ_COLONIES_IRON;       /**< Nombre minimal de colonies requis pour le Fer */

    /**
     * @brief Applique les bonus associés au niveau technologique actuel de la faction
     *
     * Modifie les attributs de la faction (productivité, capacité d'exploitation, etc.)
     * en fonction du palier technologique atteint.
     *
     * @param faction Référence à la faction qui reçoit les bonus
     */
    void applyTechBonus(Faction& faction);

public:
    /**
     * @brief Constructeur
     *
     * Initialise les coûts et prérequis par défaut pour les transitions technologiques.
     */
    TechManager();

    /**
     * @brief Traite la progression technologique d'une faction
     *
     * Cette méthode vérifie si la faction satisfait les prérequis et dispose des ressources
     * nécessaires pour effectuer une transition technologique. Si les conditions sont
     * remplies, les ressources sont consommées et applyTechBonus() est appelé pour
     * appliquer les effets de la nouvelle technologie.
     *
     * @param map Référence à la carte de simulation
     * @param faction Référence à la faction à traiter
     * @param deltaTime Intervalle de temps écoulé depuis la dernière mise à jour (en secondes)
     *
     * @override Implémente la méthode virtuelle de IFactionSystem
     */
    void processFaction(Map& map, Faction& faction, float deltaTime) override;
};

#endif