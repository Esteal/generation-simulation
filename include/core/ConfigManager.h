#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include "dependencies/json.hpp"
#include "model/Faction.h"
// Structure contenant toutes les variables pré-chargées
struct GameConfig {
    
    // --- General ---
    float seaLevel = 0.0f;

    // --- Agriculture ---
    float wheatGrowthSpeed = 0.3f;

    // --- Hydrolic Erosion ---
    float K = 0.1f; 
    float M = 0.5f; 
    float n = 1.0f;

    // --- Thermal Erosion ---
    float talusAngle = 0.005f;
    float friction = 0.5f;
    int iterationsPerTick = 3;

    // --- Hydrologie ---
    float riverThreshold = 50.0f;  /**< Seuil de débit pour qu'une cellule devienne une rivière */
    float dropOff = 0.1f;          /**< Pourcentage d'eau qui s'écoule vers les cellules plus basses à chaque itération*/

    // --- Minerals ---

    float noiseFrequency = 0.05;
    float baseSpawnChance = 0.05;

    float veinInitialChance = 0.60;
    float veinDecayRate = 0.20;

    float offsetCoal = 0.0;
    float offsetIron = 10000.0;
    float offsetGold = 20000.0;
    float offsetBronze = 30000.0;

    float coalMinAltitude = 0.3;
    float coalMaxAltitude = 0.7;
    float coalRarity = 0.4;
    float coalBiomeBonus = 1.5;
    float coalBiomePenalty = 0.5;

    float ironMinAltitude = 0.5;
    float ironMaxAltitude = 0.9;
    float ironRarity = 0.2;
    float ironBiomeBonus = 2.0;
    float ironBiomePenalty = 0.8;

    float goldMinAltitude = 0.2;
    float goldMaxAltitude = 0.5;
    float goldRarity = 0.15;
    float goldBiomeBonus = 1.2;
    float goldBiomePenalty = 0.5;

    float bronzeMinAltitude = 0.25;
    float bronzeMaxAltitude = 0.65;
    float bronzeRarity = 0.45;
    float bronzeBiomeBonus = 1.8;
    float bronzeBiomePenalty = 0.6;

    // --- Light ---
    int radius = 10;
    int numDirections = 8;

    // --- vegetation ---
    float pollinizationChance = 0.1f;
    float growthFactor = 0.5f;
    float deathFactor = 0.9f;
    int radiusCactus = 2;
    int radiusPineTree = 3;
    int radiusOakTree = 6;
    int radiusJungleTree = 5;

    // --- Faction Interface ---
    float soilBonusMultiplier = 0.8f;
    float soilBonusMax = 0.40f;
    float altitudeThreshold = 1.2f;
    float altitudeMalusMultiplier = 0.3f;
    float fertilityTemperatureWeight = 0.25f;
    float fertilityHumidityWeight = 0.35f;
    float fertilityLightWeight = 0.10f;
    int scanRadius = 2;
    float stoneAgeWaterBonus = 0.10f;
    float stoneAgeWoodBonus = 0.15f;
    float bronzeAgeWoodBonus = 0.05f;
    float bronzeAgeResourceBonus = 0.25f;
    float ironAgeBronzeBonus = 0.10f;
    float ironAgeResourceBonus = 0.30f;
    int minMigrants = 50;               // Nombre minimum d'habitants pour fonder une ville
    float growthRate = 0.1f;           // +50% de population par seconde si score 1.0

    // --- City Manager---
    int carryingCapacityBase = 800;
    float cohesionBonusPerCity = 1.0f;
    float penaltyDistanceFromMotherCityFactor = 0.15f;

    // --- Warfare Manager ---
    float baseCasualtyRate = 0.2f;
    int combatRangeBonus = 1;

    // --- Food Manager ---
    float consumptionRate = 0.8f;          // Nourriture consommée par habitant par seconde
    float replantChance = 0.8f;             // Chance de replanter une graine après récolte
    int foodHarvested = 100;
    
    // --- Ressource Manager ---
    // La vitesse à laquelle la ressource est prélevée (ex: 0.2 = 20% par itération)
    float treeChopRate = 0.5f; 
    float miningRate = 0.2f;    
    // La quantité de ressource brute que contient une case pleine à 100% (1.0f)
    float maxWoodPerTree = 50.0f;
    float maxOrePerVein = 300.0f;

    // --- Tech Manager ---
    int costWoodBronze = 500;
    int reqPopBronze = 1000;
    int reqColoniesBronze = 3;
    int costBronzeIron = 800;
    int costCoalIron = 1000;
    int reqPopIron = 3000;
    int reqColoniesIron = 6;

    // --- Tech Manager ---
    int tensionFactor = 5;             // Facteur d'augmentation de la tension

    // --- Civilisation System ---
    int minDistance = 20;               // Écart minimum en nombre de cases entre deux "capitales"
    int numberOfFactions = 5;                      // Nombre de civilisations à créer
    int stockFood = 300;                     //penser que le blé ne pousse pas instantanément (par exemple donné de quoi faire 3 ans)
    int populationTotale = 100; 
    int defaultExpandMigrationRadius = 3;
    int defaultExpandPlantRadius = 2;
    int defaultExpandInfluenceRadius = 4;
    TechLevel techBaseLevel = TechLevel::STONE_AGE;

    // --- Map Generator ---
    float altitudeFrequency = 0.008f;
    int altitudeOctaves = 6;
    float altitudeLacunarity = 2.0f;
    float altitudeGain = 0.25f;
    float temperatureFrequency = 0.005f;
    float humidityFrequency = 0.005f;
    float granularFrequency = 0.05f;
};

class ConfigManager {
private:
    GameConfig currentConfig;
    
    // Constructeur privé (Singleton)
    ConfigManager() = default;

public:
    // Accès à l'instance unique
    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }


    // Fonction de chargement à appeler au démarrage
    bool loadConfig(const std::string& filename, const std::string& directory = "config/");

    // Accès rapide aux données (inline pour la performance)
    const GameConfig& getConfig() const { return currentConfig; }
    GameConfig& getMutableConfig() { return currentConfig; }
    bool saveConfig(const std::string& filename, const std::string& directory = "config/");
};

#endif