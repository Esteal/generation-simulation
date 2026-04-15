#include "IFactionSystem.h"

#ifndef FOODMANAGER_H
#define FOODMANAGER_H

class FoodManager : public IFactionSystem {
private:


    // --- Paramètres d'Agriculture ---
    const float CONSOMMATION_RATE = 0.8f;          // Nourriture consommée par habitant par seconde
    const float SHOULD_PLANT_THRESHOLD = 0.7f;     // Seuil pour déclancher une chance de planté une plantation
    const float CHANCE_TO_PLANT = 0.5f;            // 50% de chance de planter qqchose.
    const float FAMINE_THRESHOLD = 0.3f;           // Plante assurément qqchose en cas de famine
    const float CRITICAL_FAMINE_THRESHOLD = 0.05f; // décès d'une partie de la population (même s'il y a assez de nourriture, pas de distrib équitable)
    const float REPLANT_CHANCE = 0.8f;             // Chance de replanter une graine après récolte
    const float STARVATION_DEATH_RATE = 0.15f;     // 15% de morts par deltatime en famine critique 
    const int FOOD_HARVESTED = 100;                // +100 si un blé est trouvé
    void sowSeed(Cell& targetCell, int factionID);
    void sowSeeds(Map& map, Faction& faction, int seedsToPlant); 
    void harvestCrops(Map &map, Faction &faction);
public:
    void processFaction(Map& map, Faction& faction, float deltaTime) override;
};

#endif