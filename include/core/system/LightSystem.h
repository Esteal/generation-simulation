#ifndef LIGHT_SYSTEM_H
#define LIGHT_SYSTEM_H

#include "ISimulationSystem.h"

/**
 * @file LightSystem.h
 * @brief Calcul et propagation de la lumière/ombre sur la carte.
 *
 * LightSystem estime la quantité de lumière atteignant chaque cellule en tenant
 * compte de l'ombrage local (topographie). Le calcul s'effectue
 * en échantillonnant plusieurs directions autour de chaque cellule et en analysant
 * l'occlusion sur un rayon donné.
 *
 * Les résultats sont écrits dans les propriétés de la carte (par ex. Cell::light)
 * et peuvent être utilisés par d'autres systèmes (agriculture, attractivité, rendu).
 */

/**
 * @class LightSystem
 * @brief Système de propagation lumineuse et d'occlusion.
 *
 * Composants :
 * - radius : rayon d'échantillonnage en cellules (distance maximale d'occultation).
 * - numDirections : nombre de directions d'échantillonnage autour de la cellule
 *   (ex. 8 -> échantillonnage octogonal). Plus le nombre est élevé, plus la
 *   précision de l'ombrage augmente au prix du coût de calcul.
 *
 * Utilisation :
 * - Instancier puis appeler process(map, deltaTime) depuis la boucle de simulation.
 * - Les valeurs de lumière sont recalculées pour chaque cellule et mises à jour
 *   directement dans la Map.
 */
class LightSystem : public ISimulationSystem {
private:
    int radius;         /**< Rayon d'analyse de l'occlusion en nombre de cellules. */
    int numDirections;  /**< Nombre de directions d'échantillonnage pour approximer l'occlusion. */

public:
    /**
     * @brief Constructeur par défaut.
     *
     * Initialise le système avec des paramètres par défaut (ex. radius raisonnable et numDirections).
     */
    LightSystem();

    /**
     * @brief Met à jour la distribution de lumière sur la carte.
     *
     * Parcourt la carte et calcule pour chaque cellule la quantité de lumière reçue.
     * L'algorithme échantillonne numDirections rayons sur une distance radius et
     * combine les résultats pour estimer l'occlusion locale.
     *
     * @param map Référence à la carte de simulation à mettre à jour.
     * @param deltaTime Temps écoulé depuis la dernière mise à jour (en secondes).
     *
     * @override Implémente la méthode virtuelle de ISimulationSystem.
     */
    void process(Map& map, float deltaTime) override;
};

#endif