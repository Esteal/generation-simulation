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
    float SOIL_BONUS_MULTIPLIER;
    float SOIL_BONUS_MAX;
    float ALTITUDE_THRESHOLD;
    float ALTITUDE_MALUS_MULTIPLIER;
    float FERTILITY_TEMPERATURE_WEIGHT;
    float FERTILITY_HUMIDITY_WEIGHT;
    float FERTILITY_LIGHT_WEIGHT;
    int SCAN_RADIUS;
    // Bonus par ère technologique
    float STONE_AGE_WATER_BONUS;
    float STONE_AGE_WOOD_BONUS;
    float BRONZE_AGE_WOOD_BONUS;
    float BRONZE_AGE_RESOURCE_BONUS;
    float IRON_AGE_BRONZE_BONUS;
    float IRON_AGE_RESOURCE_BONUS;

public:
    int MIN_MIGRANTS;               // Nombre minimum d'habitants pour fonder une ville
    float GROWTH_RATE;           // +50% de population par seconde si score 1.0
    float calculateTechAttractivity(const Map& map, int cx, int cy, TechLevel techLevel);
    virtual void killPopulation(Map& map, Faction& faction, int totalDeaths, Faction* conqueror = nullptr);
    void resetCell(Cell& cell);
    virtual ~IFactionSystem() = default;
    IFactionSystem();;
    virtual void processFaction(Map& map, Faction& faction, float deltaTime) = 0;
    // TODO : implémenter ce système à la toute fin quand tous les .h chargeront la config 
    //virtual void loadConfig() = 0;
};

#endif