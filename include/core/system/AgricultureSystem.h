#ifndef AGRICULTURESYSTEM_H
#define AGRICULTURESYSTEM_H

#include "ISimulationSystem.h"

/**
 * @file AgricultureSystem.h
 * @brief Gestion de la croissance agricole (blé) dans la simulation.
 *
 * Ce système met à jour la progression des cultures (blé) en fonction des
 * conditions locales et d'un paramètre global de vitesse de croissance.
 */

/**
 * @class AgricultureSystem
 * @brief Système chargé de la croissance des cultures (blé).
 *
 * Parcourt la carte et met à jour l'état des cellules cultivées : progression
 * du stade de croissance, maturation et interaction avec d'autres systèmes.
 */
class AgricultureSystem : public ISimulationSystem {
private:
    /**
     * @brief Vitesse de croissance du blé.
     *
     * Valeur en unité relative (0.0f..1.0f) représentant la fraction de
     * progression par seconde dans des conditions optimales. Exemple :
     * 0.9f correspond à une croissance quasi-complète en ~1.1 secondes.
     */
    float WHEAT_GROWTH_SPEED;

public:
    /**
     * @brief Constructeur.
     *
     * Initialise les paramètres par défaut du système agricole.
     */
    AgricultureSystem();

    /**
     * @brief Met à jour la croissance du blé sur la carte.
     *
     * Parcourt les cellules cultivées et avance leur stade de croissance en
     * fonction de WHEAT_GROWTH_SPEED et du temps écoulé.
     *
     * @param map Référence à la carte de simulation.
     * @param deltaTime Temps écoulé depuis la dernière mise à jour (en secondes).
     *
     * @override Implémente la méthode virtuelle de ISimulationSystem.
     */
    void process(Map& map, float deltaTime) override;

};

#endif