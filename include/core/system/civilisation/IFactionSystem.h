#ifndef I_FACTION_SYSTEM_H
#define I_FACTION_SYSTEM_H

#include "Map.h"
#include "Faction.h"

struct Candidate {
    int x;
    int y;
    float attractivity;
};

class IFactionSystem {
private:
    // Taux de calcul d'attractivité
    static constexpr float SOIL_BONUS_MULTIPLIER = 0.8f;
    static constexpr float SOIL_BONUS_MAX = 0.40f;
    static constexpr float ALTITUDE_THRESHOLD = 1.2f;
    static constexpr float ALTITUDE_MALUS_MULTIPLIER = 0.3f;
    static constexpr float FERTILITY_TEMPERATURE_WEIGHT = 0.25f;
    static constexpr float FERTILITY_HUMIDITY_WEIGHT = 0.35f;
    static constexpr float FERTILITY_LIGHT_WEIGHT = 0.10f;
    static constexpr int SCAN_RADIUS = 2;
    // Bonus par ère technologique
    static constexpr float STONE_AGE_WATER_BONUS = 0.10f;
    static constexpr float STONE_AGE_WOOD_BONUS = 0.15f;
    static constexpr float BRONZE_AGE_WOOD_BONUS = 0.05f;
    static constexpr float BRONZE_AGE_RESOURCE_BONUS = 0.25f;
    static constexpr float IRON_AGE_BRONZE_BONUS = 0.10f;
    static constexpr float IRON_AGE_RESOURCE_BONUS = 0.30f;

public:
    const int MIN_MIGRANTS = 50;               // Nombre minimum d'habitants pour fonder une ville
    const float GROWTH_RATE = 0.1f;           // +50% de population par seconde si score 1.0
    static float calculateTechAttractivity(const Map& map, int cx, int cy, TechLevel techLevel);
    virtual void killPopulation(Map& map, Faction& faction, int totalDeaths, Faction* conqueror = nullptr);
    void resetCell(Cell& cell);
    virtual ~IFactionSystem() = default;    
    virtual void processFaction(Map& map, Faction& faction, float deltaTime) = 0;
};

#endif