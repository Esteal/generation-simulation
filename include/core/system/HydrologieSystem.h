/**
 * @file HydrologieSystem.h
 * @brief Définition du système d'hydrologie (génération des rivières et lacs).
 */

#ifndef HYDROLOGIESYSTEM_H
#define HYDROLOGIESYSTEM_H

#include "ISimulationSystem.h"

using namespace std;

/**
 * @struct TerrainCell
 * @brief Structure utilitaire représentant une cellule du tableau.
 */
struct TerrainCell
{
    size_t x;   /**< Coordonnée X de la cellule sur la grille. */
    size_t y;   /**< Coordonnée Y de la cellule sur la grille. */
    float alt;  /**< Altitude de la cellule. */
};

/**
 * @class HydrologieSystem
 * @brief Système simulant l'écoulement des eaux de surface et la formation des rivières.
 * * Ce système calcule l'accumulation de l'eau de pluie en fonction de la topologie 
 * du terrain. Lorsque l'eau accumulée dépasse un certain seuil, elle creuse le sol 
 * pour former le lit d'une rivière de manière permanente.
 */
class HydrologieSystem : public ISimulationSystem 
{
private:
    /** * @brief Seuil critique d'eau. 
     * Quantité d'eau accumulée nécessaire sur une cellule pour qu'elle soit 
     * considérée comme une rivière et qu'elle creuse le terrain. 
     */
    float riverThreshold;
    const float dropoff = 0.1f; /**< Perte d'humidité par case franchie (0.3f équivaut à un rayon max de ~3 cases - 0.1 -> ~10 cases). Plus la valeur est petite, plus l'humidité voyage loin. */
    const float diagDropoff = dropoff * 1.414f; // Perte en diagonale (racine de 2)

    // --- Buffers persistants ---
    std::vector<float> waterMap;
    std::vector<TerrainCell> landCells;
    std::vector<bool> riverMask;

    /**
     * @brief Initialise la carte des eaux et extrait la liste des cellules terrestres.
     * @param map La carte du monde.
     * @param[out] waterMap Vecteur stockant la quantité d'eau sur chaque cellule (initialisé à 1.0 par défaut).
     * @param[out] landCells Vecteur qui sera rempli avec uniquement les cellules situées au-dessus du niveau de la mer.
     */
    void initializeWaterMapAndLandCells(const Map& map);

    /**
     * @brief Effectue un tri topologique des cellules terrestres.
     * @param[in,out] landCells Vecteur de cellules terrestres à trier de la plus haute à la plus basse.
     */
    void sortLandCellsByAltitude();

    /**
     * @brief Simule l'écoulement de l'eau de cellule en cellule vers les points les plus bas.
     * @param map La carte du monde (utilisée pour vérifier les altitudes et les bords).
     * @param[in,out] waterMap Vecteur accumulant les volumes d'eau transitant par chaque cellule.
     * @param landCells Les cellules terrestres triées par altitude décroissante.
     */
    void routeWater(const Map& map);

    /**
     * @brief Creuse le lit des rivières là où l'eau s'est accumulée massivement.
     * Modifie l'altitude ou les sédiments de la carte pour marquer visuellement le passage de la rivière.
     * @param map La carte du monde à modifier.
     * @param waterMap Vecteur contenant les volumes d'eau finaux après écoulement.
     */
    void carveRivers(Map& map);

public:
    /**
     * @brief Constructeur du système d'hydrologie.
     * @param riverThreshold Le seuil d'eau requis pour former une rivière (par défaut 40.0f).
     */
    HydrologieSystem(float riverThreshold = 40.0f);

    /**
     * @brief Exécute l'algorithme complet de génération des rivières.
     * Appelle séquentiellement l'initialisation, le tri, le routage de l'eau et le creusement.
     * @param map La carte du monde à traiter.
     * @param deltaTime non utilisé dans cette classe.
     */
    void process(Map& map, float deltaTime) override;

    /**
     * @brief Diffuse l'humidité depuis les points d'eau vers les terres environnantes.
     * @param map La carte du monde à modifier.
     */
    void diffuseHumidity(Map& map) const;

    /**
     * @brief Effectue un test de régression pour valider le comportement de la carte.
     * @return true si tous les tests passent, false sinon.
     */
    static bool testRegression();
};

#endif // HYDROLOGIESYSTEM_H