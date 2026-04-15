#ifndef CIVILISATIONSYSTEM_H
#define CIVILISATIONSYSTEM_H
#include "ISimulationSystem.h"
#include "IFactionSystem.h"
#include "Faction.h"
#include <vector>
#include <memory>

class CivilisationSystem : public ISimulationSystem {
private:
    std::vector<std::unique_ptr<IFactionSystem>> subSystems;

    // --- Paramètres d'équilibrage de la simulation ---
    const int MIN_DISTANCE = 20;               // Écart minimum en nombre de cases entre deux "capitales"
    int numberOfFactions = 5;                      // Nombre de civilisations à créer

    // --- Paramètres d'équilibrage à l'initialisation des factions ---
    const int STOCK_FOOD = 300;                     //penser que le blé ne pousse pas instantanément (par exemple donné de quoi faire 3 ans)
    const int POPULATION_TOTALE = 100; 
    const int DEFAULT_EXPAND_MIGRATION_RADIUS = 3;
    const int DEFAULT_EXPAND_PLANT_RADIUS = 2;
    const TechLevel TECH_BASE_LEVEL = TechLevel::STONE_AGE;
    
    void initializeCivilizations(Map& map);
public:
    CivilisationSystem(Map& map);
    void process(Map& map, float deltaTime) override;
    void displayAveragePopulationPerCity(const Map& map);

};

#endif