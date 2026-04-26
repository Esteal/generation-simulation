#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "IFactionSystem.h"

/**
 * @file ResourceManager.h
 * @brief Déclaration de la classe ResourceManager
 * 
 * Cette classe gère l'extraction et la récolte des ressources naturelles (bois, minerais)
 * par les factions dans la simulation.
 */

/**
 * @class ResourceManager
 * @brief Gestionnaire de l'extraction des ressources naturelles
 * 
 * Cette classe est responsable de :
 * - L'extraction du bois à partir des arbres
 * - L'exploitation minière des gisements de minerais (bronze, fer, charbon, or)
 * - La gestion de la quantité de ressources disponibles dans chaque gisement
 * - L'ajout des ressources extraites aux stocks de la faction
 * 
 * Elle implémente l'interface IFactionSystem pour traiter l'exploitation des ressources
 * à chaque itération de simulation.
 */
class ResourceManager : public IFactionSystem {
private:
    // --- Paramètres d'extraction ---
    float TREE_CHOP_RATE;           /**< Vitesse d'extraction du bois (0.0 à 1.0, ex: 0.2 = 20% par itération) */
    float MINING_RATE;              /**< Vitesse d'exploitation minière (0.0 à 1.0, ex: 0.2 = 20% par itération) */

    // --- Capacités de ressources ---
    float MAX_WOOD_PER_TREE;        /**< Quantité maximale de bois contenue dans un arbre complètement développé (100%) */
    float MAX_ORE_PER_VEIN;         /**< Quantité maximale de minerai contenue dans un gisement complètement exploitable (100%) */

public:
    /**
     * @brief Constructeur du gestionnaire de ressources
     * 
     * Initialise le gestionnaire avec les paramètres d'extraction par défaut.
     */
    ResourceManager();

    /**
     * @brief Traite l'extraction des ressources pour une faction à chaque itération
     * 
     * Cette méthode gère les étapes de l'exploitation des ressources :
     * - Identification des cellules contenant des ressources exploitables (arbres, minerais)
     * - Extraction progressive du bois et des minerais selon les taux configurés
     * - Ajout des ressources extraites aux stocks de la faction
     * - Réduction du stade de développement du gisement à mesure qu'il est exploité
     * 
     * @param map Référence à la carte de simulation
     * @param faction Référence à la faction dont exploiter les ressources
     * @param deltaTime Intervalle de temps écoulé depuis la dernière mise à jour (en secondes)
     * 
     * @override Implémente la méthode virtuelle de IFactionSystem
     */
    void processFaction(Map& map, Faction& faction, float deltaTime) override;
};

#endif