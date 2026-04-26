#ifndef WARFARE_MANAGER_H
#define WARFARE_MANAGER_H

#include "IFactionSystem.h"

/**
 * @file WarfareManager.h
 * @brief Déclaration du gestionnaire des opérations militaires et des combats entre factions.
 *
 * Ce module gère la détection des escarmouches, le calcul des pertes, l'application des
 * effets de la supériorité technologique et la propagation des conséquences d'un conflit
 * (pertes de population, capture/repli de colonies).
 */

/**
 * @class WarfareManager
 * @brief Gestionnaire des conflits militaires entre factions
 *
 * Hérite de IFactionSystem et implémente la logique de combat pour une faction donnée.
 * Ses responsabilités incluent :
 * - Détection des affrontements entre factions rivales à portée
 * - Calcul des pertes humaines et matérielles (villes)
 * - Application d'avantages/désavantages liés au niveau technologique (tech multiplier)
 * - Mise à jour des états de la carte et des factions après combat
 */
class WarfareManager : public IFactionSystem {
private:
    // --- Paramètres d'équilibrage de la guerre ---
    float BASE_CASUALTY_RATE;        /**< Taux de pertes de base appliqué lors d'un affrontement (ex : 0.1 = 10% par tick). */
    int COMBAT_RANGE_BONUS;          /**< Bonus de portée ou d'avantage tactique (ex : portée supplémentaire pour l'attaque). */

    /**
     * @brief Calcule un multiplicateur de puissance en fonction du niveau technologique.
     *
     * Applique un facteur de pondération pour refléter l'avantage militaire conféré par
     * un meilleur niveau technologique (armes, organisation, tactiques). Valeurs supérieures
     * à 1.0 favorisent la faction possédant la technologie supérieure.
     *
     * @param techLevel Niveau technologique de la faction
     * @return Multiplicateur de puissance (> 0.0)
     */
    float getTechMultiplier(TechLevel techLevel) const;

public:
    /**
     * @brief Constructeur par défaut.
     *
     * Initialise les paramètres d'équilibrage militaires avec des valeurs par défaut.
     */
    WarfareManager();

    /**
     * @brief Traite les opérations militaires d'une faction lors d'une itération de simulation.
     *
     * Cette méthode recherche les actions militaires pertinentes
     * impliquant la faction fournie, calcule les pertes en utilisant BASE_CASUALTY_RATE et le
     * multiplicateur technologique, et applique les conséquences (réduction de population,
     * modifications d'occupation de cellules, éventuelle prise de contrôle).
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