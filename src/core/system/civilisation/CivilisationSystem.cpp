#include "civilisation/CivilisationSystem.h"
#include "WorldSimulator.h"
#include "FoodManager.h"
#include "CityManager.h"
#include "ResourceManager.h"
#include "TechManager.h"
#include "WarfareManager.h"
#include <algorithm>
#include "ConfigManager.h"

CivilisationSystem::CivilisationSystem(Map& map) {
    //int width = static_cast<int>(map.getWidth());
    //int height = static_cast<int>(map.getHeight());
    calculateAttractivity(map);

    subSystems.push_back(std::make_unique<CityManager>(map));
    subSystems.push_back(std::make_unique<ResourceManager>());
    subSystems.push_back(std::make_unique<FoodManager>());
    subSystems.push_back(std::make_unique<WarfareManager>());
    subSystems.push_back(std::make_unique<TechManager>());
    
}

void CivilisationSystem::process(Map& map, float deltaTime) 
{   
    if(deltaTime <= 0) return;

    std::vector<Faction>& factions = map.getFactions();

    for (auto it = factions.begin(); it != factions.end(); ) {
        
        for (auto& system : subSystems) {
            system->processFaction(map, *it, deltaTime);
        }
        
        // Afficher le niveau d'animositer + l'état des relations
        /*
        for(const auto& relation : it->relations) {
            std::string stateStr;
            switch (relation.second.state) {
                case DiplomaticState::PEACE: stateStr = "Peace"; break;
                case DiplomaticState::WAR: stateStr = "War"; break;
                default: stateStr = "Unknown"; break;
            }
            
            std::cout << "Faction " << it->id << " - Relation with Faction " << relation.first 
                      << ": Tension = " << relation.second.tension 
                      << ", State = " << stateStr << std::endl;
        }
        */

        if (it->colonies.empty() || it->populationTotale <= 0) {
            std::cout << "La faction " << it->id << " a disparu." << std::endl;
            it = factions.erase(it);
        } else {
            ++it;
        }
    }

    //displayAveragePopulationPerCity(map);
}

void CivilisationSystem::displayAveragePopulationPerCity(const Map& map)
{
    const std::vector<Faction>& factions = map.getFactions();
    
    int totalPopulation = 0;
    int totalCities = 0;
    int woodResources = 0;
    int bronzeResources = 0;
    int ironResources = 0;
    int coalResources = 0;
    int goldResources = 0;
    for (const Faction& faction : factions) {
        for (const Settlement& settlement : faction.colonies) {
            totalPopulation += settlement.population;
            totalCities++;
        }
        woodResources += faction.stockWood;
        bronzeResources += faction.stockBronze;
        ironResources += faction.stockIron;
        coalResources += faction.stockCoal;
        goldResources += faction.stockGold;
    }
    
    if (totalCities > 0) {
        float averagePopulation = static_cast<float>(totalPopulation) / static_cast<float>(totalCities);
        std::cout << "Population moyenne par ville : " << averagePopulation << " (Total : " 
                  << totalPopulation << " habitants / " << totalCities << " villes / "<<factions.size()<< "factions)" 
                  << " Ressources moyennes : " << woodResources / static_cast<float>(factions.size()) << " bois, " 
                  << bronzeResources / static_cast<float>(factions.size()) << " bronze, " 
                  << ironResources / static_cast<float>(factions.size()) << " fer, " 
                  << coalResources / static_cast<float>(factions.size()) << " charbon, " 
                  << goldResources / static_cast<float>(factions.size()) << " or" 
                  << std::endl;
    } else {
        std::cout << "Aucune ville n'existe pour le moment." << std::endl;
    }
}

