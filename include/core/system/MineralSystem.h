#ifndef MINERAL_SYSTEM_H
#define MINERAL_SYSTEM_H

#include "ISimulationSystem.h"
#include "FastNoiseLite.h" // Puisque vous l'avez dans votre projet

class MineralSystem : public ISimulationSystem {
private:
    FastNoiseLite noiseGen;

    const float NOISE_FREQUENCY = 0.05f;      /**< Fréquence du bruit (taille des grappes globales) */
    const float BASE_SPAWN_CHANCE = 0.05f;    /**< 5% : Probabilité qu'une case lance l'algo de filon */
    
    // --- Paramètres de Propagation des Filons ---
    const float VEIN_INITIAL_CHANCE = 0.60f;  /**< 60% : Probabilité que le premier bloc du filon s'étende */
    const float VEIN_DECAY_RATE = 0.20f;      /**< -20% : Perte de probabilité à chaque case franchie */

    // --- Offsets de désynchronisation pour le bruit ---
    const float OFFSET_COAL   = 0.0f;
    const float OFFSET_IRON   = 10000.0f;
    const float OFFSET_GOLD   = 20000.0f;
    const float OFFSET_BRONZE = 30000.0f;

    // --- Paramètres : CHARBON ---
    const float COAL_MIN_ALTITUDE = 0.3f;
    const float COAL_MAX_ALTITUDE = 0.7f;
    const float COAL_RARITY = 0.4f;           /**< Plus la valeur est haute, plus c'est commun (max 1.0) */
    const float COAL_BIOME_BONUS = 1.5f;      /**< Multiplicateur si Forêt/Jungle */
    const float COAL_BIOME_PENALTY = 0.5f;    /**< Multiplicateur pour les autres biomes */

    // --- Paramètres : FER ---
    const float IRON_MIN_ALTITUDE = 0.5f;
    const float IRON_MAX_ALTITUDE = 0.9f;
    const float IRON_RARITY = 0.2f;
    const float IRON_BIOME_BONUS = 2.0f;      /**< Multiplicateur si Montagne */
    const float IRON_BIOME_PENALTY = 0.8f;

    // --- Paramètres : OR ---
    const float GOLD_MIN_ALTITUDE = 0.2f;
    const float GOLD_MAX_ALTITUDE = 0.5f;
    const float GOLD_RARITY = 0.15f;
    const float GOLD_BIOME_BONUS = 1.2f;      /**< Multiplicateur si Désert ou rivière*/
    const float GOLD_BIOME_PENALTY = 0.5f;

    // --- Paramètres : BRONZE ---
    const float BRONZE_MIN_ALTITUDE = 0.25f;
    const float BRONZE_MAX_ALTITUDE = 0.65f;
    const float BRONZE_RARITY = 0.45f;        /**< Ressource primitive, modérément rare */
    const float BRONZE_BIOME_BONUS = 1.8f;    /**< Multiplicateur si Plaines/Collines */
    const float BRONZE_BIOME_PENALTY = 0.6f;  /**< Multiplicateur pour les autres biomes */

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