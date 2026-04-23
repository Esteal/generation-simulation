/**
 * @file HydrolicErosionSystem.h
 * @brief Définition du système d'érosion hydraulique.
 */

#ifndef EROSIONSYSTEM_H
#define EROSIONSYSTEM_H

#include "ISimulationSystem.h"

/**
 * @struct CellData
 * @brief Structure utilitaire représentant une cellule de la carte.
 * * Utilisée principalement pour optimiser le tri topologique des cellules
 * avant le calcul de l'écoulement de l'eau.
 */
struct CellData {
    int x;          /**< Coordonnée X de la cellule sur la grille. */
    int y;          /**< Coordonnée Y de la cellule sur la grille. */
    float height;   /**< Altitude totale de la cellule (généralement bedrock + granular). */
};

/**
 * @class HydrolicErosionSystem
 * @brief Système de simulation de l'érosion hydraulique par puissance de flux (Stream Power Erosion).
 * * Cette classe simule l'action de l'eau de pluie qui s'accumule pour former des cours d'eau,
 * creusant des vallées et des lits de rivières en fonction de la pente et du volume d'eau.
 */
class HydrolicErosionSystem : public ISimulationSystem {
private:
    // Paramètres mathématiques de l'érosion hydraulique
    
    /** * @brief Puissance de base de l'érosion (Constante K). 
     * Détermine la quantité globale de matière arrachée à chaque itération. 
     */
    float K;         

    /** * @brief Influence de l'accumulation d'eau (Exposant m). 
     * Gère l'avantage géologique des grands fleuves par rapport aux petits ruisseaux. 
     */
    float m;         

    /** * @brief Influence de la pente (Exposant n). 
     * Définit à quel point la verticalité du terrain accélère le processus d'érosion. 
     */
    float n;

    float seaLevel;    // Niveau de la mer, en dessous duquel aucune érosion ne se produit

    std::vector<CellData> sortedCells;
    std::vector<float> water;
    std::vector<int> receivers;

    /**
     * @brief Met à jour le tableau interne sortedCells sans réallouer de mémoire.
     */
    void updateSortedCells(const Map& map);

    /**
     * @brief Simule l'écoulement de l'eau pour calculer l'aire de drainage de chaque cellule.
     * * L'eau coule de cellule en cellule vers le voisin le plus bas.
     * @param map La carte topographique.
     * @param sortedCells Les cellules triées par ordre d'altitude décroissante.
     * @param[in,out] water Un vecteur représentant le volume d'eau présent sur chaque cellule.
     * @param[out] receivers Un vecteur mémorisant l'index de la cellule de destination (la plus basse) pour chaque cellule.
     */
    void calculateDrainage(const Map& map, const std::vector<CellData>& sortedCells, std::vector<float>& water, std::vector<int>& receivers) const;

    /**
     * @brief Applique l'équation de Stream Power Erosion pour creuser le terrain.
     * * Retire la matière (sédiments en priorité, puis bedrock) en fonction de la 
     * force du courant calculée via l'aire de drainage et la pente.
     * @param map La carte à modifier.
     * @param sortedCells Les cellules triées par ordre d'altitude décroissante.
     * @param water Les volumes d'eau accumulés après le drainage.
     * @param receivers Les destinations d'écoulement permettant de calculer la pente locale.
     */
    void applyIncision(Map& map, const std::vector<CellData>& sortedCells, const std::vector<float>& water, const std::vector<int>& receivers) const;

public:
    /**
     * @brief Exécute une ou plusieurs itérations de l'érosion hydraulique sur la carte.
     * @param map La carte du monde à éroder.
     * @param deltaTime Le multiplicateur de temps ou nombre d'itérations à effectuer durant ce cycle.
     */
    void process(Map& map, float deltaTime) override;

    HydrolicErosionSystem();

    /**
     * @brief Effectue un test de régression pour valider le comportement de la carte.
     * @return true si tous les tests passent, false sinon.
     */
    static bool testRegression();
};

#endif // EROSIONSYSTEM_H