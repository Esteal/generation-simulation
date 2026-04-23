#ifndef CITY_MANAGER_H
#define CITY_MANAGER_H

#include "IFactionSystem.h"

class CityManager : public IFactionSystem 
{
private:
    int CARRYING_CAPACITY_BASE;
    float COHESION_BONUS_PER_CITY;
    float PENALITY_DISTANCE_FROM_MOTHER_CITY_FACTOR;

    // --- Paramètres d'équilibrage de la simulation ---
    int MIN_DISTANCE ;               // Écart minimum en nombre de cases entre deux "capitales"
    int numberOfFactions;                      // Nombre de civilisations à créer

    // --- Paramètres d'équilibrage à l'initialisation des factions ---
    int STOCK_FOOD;                     //penser que le blé ne pousse pas instantanément (par exemple donné de quoi faire 3 ans)
    int POPULATION_TOTALE; 
    int DEFAULT_EXPAND_MIGRATION_RADIUS;
    int DEFAULT_EXPAND_PLANT_RADIUS;
    int DEFAULT_EXPAND_INFLUENCE_RADIUS;
    TechLevel TECH_BASE_LEVEL;
    
public:
    CityManager(Map& map);
    void initializeCivilizations(Map& map);
    void processFaction(Map& map, Faction& faction, float deltaTime) override;
};

#endif