/**
 * @file ISimulationSystem.h
 * @brief Définition de l'interface de base pour les systèmes de simulation du monde.
 */

#ifndef IS_SIMULATION_SYSTEM_H
#define IS_SIMULATION_SYSTEM_H

#include "Map.h"

/**
 * @class ISimulationSystem
 * @brief Interface abstraite représentant un système physique ou géologique.
 * * Tous les systèmes modifiant la carte (Érosion, Hydrologie, Tectonique, etc.) 
 * doivent hériter de cette classe et implémenter la méthode process(). Elle fournit 
 * également des outils mathématiques et géographiques communs à toutes les simulations.
 */
class ISimulationSystem {
public:
    /**
     * @brief Destructeur virtuel.
     * Assure la destruction correcte des classes dérivées lors de l'utilisation du polymorphisme.
     */
    virtual ~ISimulationSystem() = default;

    /**
     * @brief Exécute la logique de simulation sur la carte.
     * @param map La carte du monde à mettre à jour.
     * @param deltaTime Le temps à écouler ou le nombre d'itérations à simuler durant ce cycle.
     */
    virtual void process(Map& map, float deltaTime) = 0;

    /**
     * @brief Fonction utilitaire partagée : Cherche le voisin le plus bas autour d'une cellule.
     * * Analyse les cellules adjacentes (celle ci que N/S/E/O) pour 
     * trouver la plus grande différence d'altitude vers le bas (Steepest Descent).
     * * @param map La carte topographique en lecture seule.
     * @param x La coordonnée X de la cellule d'origine.
     * @param y La coordonnée Y de la cellule d'origine.
     * @param[out] outMaxDiff Référence vers la variable qui stockera la plus grande différence d'altitude trouvée (Pente).
     * @param[out] outLowestX Référence vers la variable qui stockera la coordonnée X du voisin le plus bas.
     * @param[out] outLowestY Référence vers la variable qui stockera la coordonnée Y du voisin le plus bas.
     */
    void findSteepestNeighbor(const Map& map, int x, int y, float& outMaxDiff, int& outLowestX, int& outLowestY) const;
    
    /**
     * @brief Trouve l'indice 1D du voisin le plus bas autour d'une coordonnée (diagonale + cardinale).
     * @param map La carte du monde.
     * @param cx Position X de la cellule.
     * @param cy Position Y de la cellule.
     * @return L'index 1D (y * width + x) du voisin le plus bas, ou -1 si c'est une cuvette.
     */
    int getLowestNeighborIndex(const Map& map, int cx, int cy) const;


    void calculateAttractivity(Map& map);
    void calculateAttractivityForCell(Map& map, int x, int y);  

    /**
     * @brief Effectue un test de régression pour valider le comportement de la carte.
     * @return true si tous les tests passent, false sinon.
     */
    static bool testRegression();
};

#endif // IS_SIMULATION_SYSTEM_H