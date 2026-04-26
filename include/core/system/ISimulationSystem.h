/**
 * @file ISimulationSystem.h
 * @brief Définition de l'interface de base pour les systèmes de simulation du monde.
 *
 * Cette interface doit être héritée par tous les systèmes qui appliquent des
 * modifications à la carte (érosion, hydrologie, agriculture, civilisations, ...).
 * Elle expose la méthode process() obligatoire et plusieurs utilitaires géographiques
 * réutilisables par les systèmes concrets.
 */

#ifndef IS_SIMULATION_SYSTEM_H
#define IS_SIMULATION_SYSTEM_H

#include "Map.h"

/**
 * @class ISimulationSystem
 * @brief Interface abstraite représentant un système de simulation.
 *
 * Tous les systèmes modifiant la carte (Érosion, Hydrologie, Tectonique, etc.)
 * doivent hériter de cette classe et implémenter la méthode process().
 * La classe fournit des utilitaires pour la recherche de voisins et le calcul
 * d'attractivité qui sont communs à plusieurs systèmes.
 */
class ISimulationSystem {
public:
    /**
     * @brief Destructeur virtuel.
     *
     * Assure la destruction correcte des classes dérivées lors de l'utilisation
     * du polymorphisme.
     */
    virtual ~ISimulationSystem() = default;

    /**
     * @brief Exécute la logique de simulation sur la carte.
     *
     * Méthode abstraite que doivent implémenter tous les systèmes concrets.
     *
     * @param map Référence à la carte du monde à mettre à jour.
     * @param deltaTime Durée écoulée (en secondes ou unité de simulation) depuis la dernière mise à jour.
     */
    virtual void process(Map& map, float deltaTime) = 0;

    /**
     * @brief Cherche le voisin le plus bas autour d'une cellule (Steepest Descent).
     *
     * Analyse les voisins cardinaux (Nord, Sud, Est, Ouest) de la cellule source
     * pour trouver la plus forte pente descendante. Les paramètres outLowestX/outLowestY
     * reçoivent les coordonnées du voisin le plus bas et outMaxDiff la différence d'altitude.
     *
     * @param map Référence constante à la carte topographique utilisée pour la recherche.
     * @param x Coordonnée X de la cellule de départ.
     * @param y Coordonnée Y de la cellule de départ.
     * @param[out] outMaxDiff Référence qui recevra la plus grande différence d'altitude trouvée (>= 0.0f).
     * @param[out] outLowestX Référence qui recevra la coordonnée X du voisin le plus bas.
     * @param[out] outLowestY Référence qui recevra la coordonnée Y du voisin le plus bas.
     */
    void findSteepestNeighbor(const Map& map, int x, int y, float& outMaxDiff, int& outLowestX, int& outLowestY) const;
    
    /**
     * @brief Trouve l'indice 1D du voisin le plus bas autour d'une cellule (diagonales incluses).
     *
     * Parcourt les huit voisins (cardinaux + diagonaux) de la cellule donnée et retourne
     * l'index 1D (row-major : y * width + x) du voisin de plus faible altitude.
     * Si aucun voisin n'est plus bas, la fonction retourne -1 (cuve locale).
     *
     * @param map Référence constante à la carte du monde.
     * @param cx Coordonnée X de la cellule source.
     * @param cy Coordonnée Y de la cellule source.
     * @return Index 1D du voisin le plus bas, ou -1 si la cellule est la plus basse de son voisinage.
     */
    int getLowestNeighborIndex(const Map& map, int cx, int cy) const;

    /**
     * @brief Calcule et met à jour l'attractivité pour toutes les cellules de la carte.
     *
     * Parcourt la carte et calcule un score d'attractivité pour chaque cellule en
     * combinant facteurs environnementaux (fertilité, altitude, humidité, lumière, ressources, ...).
     * Le résultat est destiné à être utilisé par les systèmes de factions pour l'expansion et
     * la sélection de sites.
     *
     * @param map Référence à la carte à analyser et mettre à jour.
     */
    void calculateAttractivity(Map& map);

    /**
     * @brief Calcule l'attractivité d'une cellule spécifique et met à jour ses données.
     *
     * Évalue la cellule aux coordonnées (x,y) en utilisant ses propriétés locales
     * et les règles de calcul d'attractivité du projet.
     *
     * @param map Référence à la carte contenant la cellule.
     * @param x Coordonnée X de la cellule à évaluer.
     * @param y Coordonnée Y de la cellule à évaluer.
     */
    void calculateAttractivityForCell(Map& map, int x, int y);  

    /**
     * @brief Exécute une suite de tests de régression pour valider les utilitaires de la classe.
     *
     * Tests simples pour vérifier le comportement des fonctions utilitaires (recherche de voisins,
     * calculs d'attractivité basiques, etc.). Utilisé en développement pour détecter les régressions.
     *
     * @return true si tous les tests passent, false sinon.
     */
    static bool testRegression();
};

#endif // IS_SIMULATION_SYSTEM_H