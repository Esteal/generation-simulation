#ifndef MINERAL_SYSTEM_H
#define MINERAL_SYSTEM_H

#include "ISimulationSystem.h"
#include "FastNoiseLite.h" // Puisque vous l'avez dans votre projet

class MineralSystem : public ISimulationSystem {
private:
    FastNoiseLite noiseGen;

    float NOISE_FREQUENCY = 0.05f;      /**< Fréquence du bruit (taille des grappes globales) */
    float BASE_SPAWN_CHANCE = 0.05f;    /**< 5% : Probabilité qu'une case lance l'algo de filon */
    
    // --- Paramètres de Propagation des Filons ---
    float VEIN_INITIAL_CHANCE = 0.60f;  /**< 60% : Probabilité que le premier bloc du filon s'étende */
    float VEIN_DECAY_RATE = 0.20f;      /**< -20% : Perte de probabilité à chaque case franchie */

    // --- Offsets de désynchronisation pour le bruit ---
    float OFFSET_COAL  ;
    float OFFSET_IRON  ;
    float OFFSET_GOLD  ;
    float OFFSET_BRONZE;

    // --- Paramètres : CHARBON ---
    float COAL_MIN_ALTITUDE;
    float COAL_MAX_ALTITUDE;
    float COAL_RARITY;           /**< Plus la valeur est haute, plus c'est commun (max 1.0) */
    float COAL_BIOME_BONUS;      /**< Multiplicateur si Forêt/Jungle */
    float COAL_BIOME_PENALTY;    /**< Multiplicateur pour les autres biomes */

    // --- Paramètres : FER ---
    float IRON_MIN_ALTITUDE;
    float IRON_MAX_ALTITUDE;
    float IRON_RARITY;
    float IRON_BIOME_BONUS;
    float IRON_BIOME_PENALTY;

    // --- Paramètres : OR ---
    float GOLD_MIN_ALTITUDE;
    float GOLD_MAX_ALTITUDE;
    float GOLD_RARITY;
    float GOLD_BIOME_BONUS;
    float GOLD_BIOME_PENALTY;

    // --- Paramètres : BRONZE ---
    float BRONZE_MIN_ALTITUDE;
    float BRONZE_MAX_ALTITUDE;
    float BRONZE_RARITY;
    float BRONZE_BIOME_BONUS;
    float BRONZE_BIOME_PENALTY;

    /**
     * @brief Détermine le minerai le plus probable pour une cellule donnée.
     * @return Le matériau choisi, ou Material::NONE si les conditions ne sont pas réunies.
     */
    Material determineBestMineral(const Cell& cell, float x, float y) const;

    /**
     * @brief Propage un filon autour d'une cellule de départ avec une probabilité décroissante.
     * @param map La carte.
     * @param x, y Les coordonnées actuelles.
     * @param mineral Le type de minerai à placer.
     * @param spawnChance La probabilité actuelle que le bloc se place et continue de se propager.
     */
    void spreadVein(Map& map, int x, int y, Material mineral, float spawnChance);

    /**
     * @brief Calcule le score d'un minerai basé sur l'altitude, le biome et le bruit.
     * @param altitude L'altitude de la cellule.
     * @param minAltitude Altitude minimale pour ce minerai.
     * @param maxAltitude Altitude maximale pour ce minerai.
     * @param rarity Rareté du minerai (0.0 à 1.0).
     * @param biomeFactor Facteur de biome (bonus ou pénalité).
     * @param x, y Les coordonnées pour le calcul du bruit.
     * @param noiseOffset Offset pour désynchroniser le bruit par minerai.
     * @return Le score calculé, ou 0.0 si les conditions ne sont pas réunies.
     */
    float calculateScore(float altitude, float minAltitude, float maxAltitude, 
                         float rarity, float biomeFactor, float x, float y, float noiseOffset) const;

public:
    MineralSystem();
    void process(Map& map, float deltaTime) override;
    
    /**
     * @brief Affiche en console le nombre de chaque type de minerai présent sur la carte.
     * @param map La carte sur laquelle compter les minerais.
     */
    void displayMineralCount(const Map& map) const;
};

#endif