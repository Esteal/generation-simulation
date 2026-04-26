#ifndef MINERAL_SYSTEM_H
#define MINERAL_SYSTEM_H

#include "ISimulationSystem.h"
#include "FastNoiseLite.h"

/**
 * @file MineralSystem.h
 * @brief Génération et gestion des filons minéraux sur la carte.
 *
 * MineralSystem utilise du bruit procédural (FastNoiseLite) combiné à des règles
 * d'altitude et de biome pour décider quels minerais apparaissent et comment
 * les filons se propagent. Les minerais sont placés dans les Cell de la Map et
 * peuvent ensuite être exploités par ResourceManager.
 */
class MineralSystem : public ISimulationSystem {
private:
    FastNoiseLite noiseGen;              /**< Générateur de bruit pour la distribution procédurale. */

    float NOISE_FREQUENCY = 0.05f;      /**< Fréquence du bruit */
    float BASE_SPAWN_CHANCE = 0.05f;    /**< Probabilité initiale qu'une cellule démarre l'algorithme de filon. */

    // --- Paramètres de Propagation des Filons ---
    float VEIN_INITIAL_CHANCE = 0.60f;  /**< Probabilité que le filon s'étende au premier voisin. */
    float VEIN_DECAY_RATE = 0.20f;      /**< Décroissance de la probabilité à chaque nouvelle case parcourue. */

    // --- Offsets de désynchronisation pour le bruit (valeurs initialisées dans le constructeur) ---
    float OFFSET_COAL;                  /**< Offset de bruit pour le charbon. */
    float OFFSET_IRON;                  /**< Offset de bruit pour le fer. */
    float OFFSET_GOLD;                  /**< Offset de bruit pour l'or. */
    float OFFSET_BRONZE;                /**< Offset de bruit pour le bronze. */

    // --- Paramètres : CHARBON ---
    float COAL_MIN_ALTITUDE;            /**< Altitude minimale favorable au charbon. */
    float COAL_MAX_ALTITUDE;            /**< Altitude maximale favorable au charbon. */
    float COAL_RARITY;                  /**< Rareté du charbon (0.0 rare -> 1.0 commun). */
    float COAL_BIOME_BONUS;             /**< Multiplicateur d'attractivité dans les biomes favorables (forêt/jungle). */
    float COAL_BIOME_PENALTY;           /**< Multiplicateur d'attractivité pour biomes défavorables. */

    // --- Paramètres : FER ---
    float IRON_MIN_ALTITUDE;            /**< Altitude minimale favorable au fer. */
    float IRON_MAX_ALTITUDE;            /**< Altitude maximale favorable au fer. */
    float IRON_RARITY;                  /**< Rareté du fer (0.0..1.0). */
    float IRON_BIOME_BONUS;             /**< Bonus d'attractivité pour biomes favorables (ex: steppe/plateau). */
    float IRON_BIOME_PENALTY;           /**< Pénalité d'attractivité pour biomes défavorables. */

    // --- Paramètres : OR ---
    float GOLD_MIN_ALTITUDE;            /**< Altitude minimale favorable à l'or. */
    float GOLD_MAX_ALTITUDE;            /**< Altitude maximale favorable à l'or. */
    float GOLD_RARITY;                  /**< Rareté de l'or (0.0..1.0). */   
    float GOLD_BIOME_BONUS;             /**< Bonus d'attractivité pour biomes favorables (ex: montagnes). */
    float GOLD_BIOME_PENALTY;           /**< Pénalité d'attractivité pour biomes défavorables. */

    // --- Paramètres : BRONZE ---
    float BRONZE_MIN_ALTITUDE;          /**< Altitude minimale favorable au bronze. */
    float BRONZE_MAX_ALTITUDE;          /**< Altitude maximale favorable au bronze. */
    float BRONZE_RARITY;                /**< Rareté du bronze (0.0..1.0). */
    float BRONZE_BIOME_BONUS;           /**< Bonus d'attractivité pour biomes favorables (ex: forêt). */
    float BRONZE_BIOME_PENALTY;         /**< Pénalité d'attractivité pour biomes défavorables. */

    /**
     * @brief Détermine le minerai le plus probable pour une cellule donnée.
     *
     * Examine les scores calculés pour chaque minerai en fonction de l'altitude,
     * du biome et du bruit procédural, puis retourne le matériau le plus pertinent.
     *
     * @param cell Référence constante à la cellule à analyser.
     * @param x Coordonnée X (utilisée pour l'échantillonnage de bruit).
     * @param y Coordonnée Y (utilisée pour l'échantillonnage de bruit).
     * @return Material Le minerai choisi, ou Material::NONE si aucun minerai approprié.
     */
    Material determineBestMineral(const Cell& cell, float x, float y) const;

    /**
     * @brief Propage un filon autour d'une cellule source selon une probabilité décroissante.
     *
     * L'algorithme place un bloc du minerai fourni à la position (x,y) si la probabilité
     * spawnChance est satisfaite puis tente de propager le filon vers les voisins en
     * appliquant VEIN_DECAY_RATE à la probabilité.
     *
     * @param map Référence à la carte sur laquelle placer le filon.
     * @param x Coordonnée X de la cellule de départ.
     * @param y Coordonnée Y de la cellule de départ.
     * @param mineral Type de minerai à propager.
     * @param spawnChance Probabilité actuelle de placement/propagation (0.0..1.0).
     */
    void spreadVein(Map& map, int x, int y, Material mineral, float spawnChance);

    /**
     * @brief Calcule un score d'aptitude pour un minerai donné sur une cellule.
     *
     * Le score combine la conformité d'altitude, la rareté, les facteurs de biome
     * et une composante de bruit procédural pour diversifier la distribution.
     *
     * @param altitude Altitude de la cellule.
     * @param minAltitude Altitude minimale favorable pour ce minerai.
     * @param maxAltitude Altitude maximale favorable pour ce minerai.
     * @param rarity Rareté relative du minerai (0.0..1.0).
     * @param biomeFactor Facteur multiplicatif issu du biome (bonus ou pénalité).
     * @param x Coordonnée X pour l'échantillonnage de bruit.
     * @param y Coordonnée Y pour l'échantillonnage de bruit.
     * @param noiseOffset Offset appliqué au générateur de bruit pour ce minerai.
     * @return float Score calculé (>= 0.0). Un score nul signifie conditions non réunies.
     */
    float calculateScore(float altitude, float minAltitude, float maxAltitude,
                         float rarity, float biomeFactor, float x, float y, float noiseOffset) const;

public:
    /**
     * @brief Constructeur.
     *
     * Initialise le générateur de bruit, les offsets et les valeurs par défaut des paramètres.
     */
    MineralSystem();

    /**
     * @brief Exécute la génération des minerais sur la carte.
     *
     * Parcourt la carte et, pour chaque cellule, évalue la probabilité de départ d'un filon
     * (BASE_SPAWN_CHANCE modulée par le bruit). Si un filon doit être créé, determineBestMineral()
     * choisit le type de minerai puis spreadVein() propage le filon.
     *
     * @param map Référence à la carte de simulation à modifier.
     * @param deltaTime Temps écoulé depuis la dernière mise à jour (non utilisé pour la génération initiale mais conservé pour compatibilité).
     *
     * @override Implémente la méthode virtuelle de ISimulationSystem.
     */
    void process(Map& map, float deltaTime) override;
    
    /**
     * @brief Affiche le décompte de chaque type de minerai présent sur la carte.
     *
     * Parcourt toutes les cellules et incrémente un compteur par type de Material puis
     * affiche les résultats sur la sortie standard pour diagnostic.
     *
     * @param map Référence constante à la carte à analyser.
     */
    void displayMineralCount(const Map& map) const;
};

#endif