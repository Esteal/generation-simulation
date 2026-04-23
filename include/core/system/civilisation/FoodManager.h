#include "IFactionSystem.h"

#ifndef FOODMANAGER_H
#define FOODMANAGER_H

class FoodManager : public IFactionSystem {
private:


    // --- Paramètres d'Agriculture ---
    float CONSOMMATION_RATE;          // Nourriture consommée par habitant par seconde
    float REPLANT_CHANCE;             // Chance de replanter une graine après récolte
    int FOOD_HARVESTED;                // +100 si un blé est trouvé
    void sowSeed(Cell& targetCell, int factionID);
    void sowSeeds(Map& map, Faction& faction, int seedsToPlant); 
    void harvestCrops(Map &map, Faction &faction, float deltaTime);
public:
    FoodManager();
    void processFaction(Map& map, Faction& faction, float deltaTime) override;
};

#endif