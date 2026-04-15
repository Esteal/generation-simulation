#include "civilisation/CivilisationSystem.h"
#include "WorldSimulator.h"
#include "FoodManager.h"
#include "CityManager.h"
#include "ResourceManager.h"
#include "TechManager.h"
#include "WarfareManager.h"
#include <random>
#include <algorithm>


CivilisationSystem::CivilisationSystem(Map& map) {
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());
    subSystems.push_back(std::make_unique<FoodManager>());
    subSystems.push_back(std::make_unique<ResourceManager>());
    subSystems.push_back(std::make_unique<CityManager>());
    subSystems.push_back(std::make_unique<WarfareManager>());
    subSystems.push_back(std::make_unique<TechManager>());
    
    initializeCivilizations(map);
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

void CivilisationSystem::initializeCivilizations(Map& map) 
{
    calculateAttractivity(map);
    int currentFactionId = 1;
    std::vector<Candidate> candidates;
    std::vector<Faction> factions; 
    
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());

    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            float attractivity = IFactionSystem::calculateTechAttractivity(map, x, y, TECH_BASE_LEVEL);
            if(attractivity > 0.1f) {
                candidates.push_back({x, y, attractivity});
            }
        }
    }

    std::sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
        return a.attractivity > b.attractivity;
    });

    // On mélange seulement le top des candidates pour avoir un peu d'aléatoire logique
    size_t topN = std::min(candidates.size(), static_cast<size_t>(numberOfFactions * 5));
    if (topN > 0) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(candidates.begin(), candidates.begin() + topN, g);
    }

    for (const Candidate& candidate : candidates) {
        if (factions.size() >= static_cast<size_t>(numberOfFactions)) {
            break; 
        }

        bool isTooClose = false;

        for (const Faction& existingFaction : factions) {
            for (const Settlement& existingSettlement : existingFaction.colonies) {
                int distance = std::abs(static_cast<int>(candidate.x) - static_cast<int>(existingSettlement.x)) +
                               std::abs(static_cast<int>(candidate.y) - static_cast<int>(existingSettlement.y));
                
                if (distance < MIN_DISTANCE) {
                    isTooClose = true;
                    break; 
                }
            }
            if (isTooClose) break;
        }
        
        if (!isTooClose) {
            Faction newFaction;
            newFaction.id = currentFactionId++;
            newFaction.populationTotale = POPULATION_TOTALE;
            newFaction.stockFood = STOCK_FOOD;
            newFaction.techLevel = TECH_BASE_LEVEL;
            newFaction.capitalX = candidate.x;
            newFaction.capitalY = candidate.y;
            newFaction.radiusMigration = DEFAULT_EXPAND_MIGRATION_RADIUS;            
            newFaction.radiusExploitation = DEFAULT_EXPAND_PLANT_RADIUS;
            Settlement capital;
            capital.x = candidate.x;
            capital.y = candidate.y;
            capital.population = POPULATION_TOTALE;
            capital.factionId = newFaction.id;

            newFaction.colonies.push_back(capital);
            factions.push_back(newFaction);
            
            map.getGrid().get(candidate.x, candidate.y).isOccupied = true;
            std::cout << "Faction " << capital.factionId << " en : " << capital.x << " / " << capital.y << std::endl;
        }
    }
//    for(Faction& faction : factions)
  //      sowSeeds(map, faction, 5);
    map.setFactions(factions);
    
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

