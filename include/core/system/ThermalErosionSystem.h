/**
 * @file ThermalErosionSystem.h
 * @brief Définition du système d'érosion thermique (gravité et éboulements).
 */

#ifndef THERMALEROSIONSYSTEM_H
#define THERMALEROSIONSYSTEM_H

#include "ISimulationSystem.h"
#include <vector>

using namespace std;

/**
 * @class ThermalErosionSystem
 * @brief Système simulant l'érosion thermique.
 * * Ce système intervient après l'érosion hydraulique. Il scanne le terrain
 * et fait s'effondrer les pentes qui sont trop abruptes pour supporter leur propre poids,
 * élargissant ainsi les vallées et adoucissant les falaises.
 */
class ThermalErosionSystem : public ISimulationSystem {
private:
    /** * @brief Angle de repos (Talus Angle). 
     * Tolérance maximale de la roche à la verticalité. Si la différence d'altitude 
     * entre deux cellules dépasse ce seuil, un éboulement se déclenche.
     */
    float talusAngle = 0.005f; 

    /** * @brief Viscosité ou vitesse de glissement de la matière. 
     * Détermine la proportion de matière instable qui s'effondre en une seule itération.
     * Une valeur de 0.5 offre un bon compromis pour un glissement fluide.
     */
    float friction = 0.5f;     

    /** * @brief Fréquence d'exécution locale. 
     * Nombre d'itérations de gravité effectuées pour chaque appel au système. 
     * Assure que les éboulements ont le temps de cascader jusqu'au bas des montagnes.
     */
    int iterationsPerTick = 3; 

    /**
     * @brief Analyse une cellule spécifique et déclenche un éboulement si nécessaire.
     * * Recherche le voisin le plus bas. Si la pente vers ce voisin dépasse \p talusAngle,
     * la cellule est considérée comme instable et la matière glisse.
     * * @param map La carte topographique en cours de modification.
     * @param x Coordonnée X de la cellule à analyser.
     * @param y Coordonnée Y de la cellule à analyser.
     */
    void processCell(Map& map, int x, int y) const;

    /**
     * @brief Transfère physiquement la matière lors d'un éboulement.
     * * Fait glisser en priorité la terre meuble (granular), puis arrache et brise 
     * la roche mère (bedrock) si la falaise est toujours trop abrupte. La roche 
     * qui tombe se transforme en sédiments (granular) sur la cellule du bas.
     * * @param[in,out] currentCell La cellule haute (instable) qui perd de la matière.
     * @param[in,out] lowestCell La cellule basse qui réceptionne les gravats.
     * @param maxDiff La différence d'altitude maximale constatée avec la cellule basse.
     */
    void transferMaterial(Cell& currentCell, Cell& lowestCell, float maxDiff) const;

public:
    /**
     * @brief Exécute l'érosion thermique sur l'ensemble de la carte.
     * * Parcourt toutes les cellules de la carte pour appliquer les règles de gravité.
     * La boucle interne s'exécutera (deltaTime * \p iterationsPerTick) fois.
     * * @param map La carte du monde à éroder.
     * @param deltaTime Le multiplicateur de temps de l'itération actuelle.
     */
    void process(Map& map, float deltaTime) override;

    /**
     * @brief Effectue un test de régression pour valider le comportement de la carte.
     * @return true si tous les tests passent, false sinon.
     */
    static bool testRegression();
};

#endif // THERMALEROSIONSYSTEM_H