#include "CityManager.h"
#include "ConfigManager.h"
#include "Map.h"
#include <random>


CityManager::CityManager(Map& map){
    //loadConfig();
    const GameConfig& cfg = ConfigManager::getInstance().getConfig();
    CARRYING_CAPACITY_BASE = cfg.carryingCapacityBase;
    COHESION_BONUS_PER_CITY = cfg.cohesionBonusPerCity;
    PENALITY_DISTANCE_FROM_MOTHER_CITY_FACTOR = cfg.penaltyDistanceFromMotherCityFactor;
    MIN_DISTANCE = cfg.minDistance;
    numberOfFactions = cfg.numberOfFactions;
    STOCK_FOOD = cfg.stockFood;
    POPULATION_TOTALE = cfg.populationTotale;
    DEFAULT_EXPAND_MIGRATION_RADIUS = cfg.defaultExpandMigrationRadius;
    DEFAULT_EXPAND_PLANT_RADIUS = cfg.defaultExpandPlantRadius;
    DEFAULT_EXPAND_INFLUENCE_RADIUS = cfg.defaultExpandInfluenceRadius;
    TECH_BASE_LEVEL = cfg.techBaseLevel;

    // affichher les variables :
    std::cout << "Carrying capa : " << CARRYING_CAPACITY_BASE << "\nCOHESION_BONUS_PER_CITY" 
    << COHESION_BONUS_PER_CITY << "\nPENALITY_DISTANCE_FROM_MOTHER_CITY_FACTOR" 
    << PENALITY_DISTANCE_FROM_MOTHER_CITY_FACTOR << std::endl;

    initializeCivilizations(map);
};

/*
void CityManager::loadConfig()
{
    const GameConfig& cfg = ConfigManager::getInstance().getConfig();
    CARRYING_CAPACITY_BASE = cfg.carryingCapacityBase;
    COHESION_BONUS_PER_CITY = cfg.cohesionBonusPerCity;
    PENALITY_DISTANCE_FROM_MOTHER_CITY_FACTOR = cfg.penaltyDistanceFromMotherCityFactor;
}
*/

void CityManager::processFaction(Map& map, Faction &faction, float deltaTime)
{
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());

    const GameConfig& cfg = ConfigManager::getInstance().getConfig();
    const int CARRYING_CAPACITY_BASE = cfg.carryingCapacityBase;
    const float COHESION_BONUS_PER_CITY = cfg.cohesionBonusPerCity;
    const float PENALITY_DISTANCE_FROM_MOTHER_CITY_FACTOR = cfg.penaltyDistanceFromMotherCityFactor;

    std::vector<Settlement> newColonies; 
    faction.populationTotale = 0; 
    int maxRadiusToEmigrate = faction.radiusMigration;
    
    for (Settlement& settlement : faction.colonies) {
    
        const Cell& currentCell = map.getGrid().get(settlement.x, settlement.y);
        int maxPopulation = static_cast<int>(CARRYING_CAPACITY_BASE * currentCell.attractivityLevel);

            settlement.population += static_cast<int>(settlement.population * GROWTH_RATE * deltaTime);
            
            if (settlement.population > maxPopulation) {
                int migrants = settlement.population - maxPopulation;
                settlement.population = maxPopulation; 

                if (migrants >= MIN_MIGRANTS) {
                    int bestX = -1;
                    int bestY = -1;
                    float bestScore = 0.0f;
                    for (int dy = -maxRadiusToEmigrate; dy <= maxRadiusToEmigrate; ++dy) {
                        for (int dx = -maxRadiusToEmigrate; dx <= maxRadiusToEmigrate; ++dx) {
                            if (dx == 0 && dy == 0) continue; 

                            int nx = settlement.x + dx;
                            int ny = settlement.y + dy;

                            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                                const Cell& neighborCell = map.getGrid().get(nx, ny);
                                if (!neighborCell.isOccupied && 
                                    neighborCell.material == Material::NONE &&
                                    neighborCell.biome != BiomeIndex::OCEAN && 
                                    neighborCell.biome != BiomeIndex::RIVER && 
                                    neighborCell.biome != BiomeIndex::GLACE) {
                                    
                                    if (neighborCell.faction == 0 || neighborCell.faction == faction.id) {
                                                                            
                                        float dynamicScore = calculateTechAttractivity(map, nx, ny, faction.techLevel);
                                        
                                        int adjacentFriendlyCities = 0;
                                        for (int adjY = -1; adjY <= 1; ++adjY) {
                                            for (int adjX = -1; adjX <= 1; ++adjX) {
                                                if (adjX == 0 && adjY == 0) continue;
                                                
                                                int checkX = nx + adjX;
                                                int checkY = ny + adjY;
                                                
                                                if (checkX >= 0 && checkX < width && checkY >= 0 && checkY < height) {
                                                    const Cell& checkCell = map.getGrid().get(checkX, checkY);
                                                    if (checkCell.isOccupied && checkCell.faction == faction.id) {
                                                        adjacentFriendlyCities++;
                                                    }
                                                }
                                            }
                                        }

                                        float cohesionBonus = adjacentFriendlyCities * COHESION_BONUS_PER_CITY; 

                                        int distanceToMotherCity = std::abs(dx) + std::abs(dy);
                                        float distancePenalty = distanceToMotherCity * PENALITY_DISTANCE_FROM_MOTHER_CITY_FACTOR;

                                        float finalScore = dynamicScore + cohesionBonus - distancePenalty;

                                        if (finalScore > bestScore) {
                                            bestScore = finalScore;
                                            bestX = nx;
                                            bestY = ny;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (bestX != -1 && bestY != -1) {
                        Settlement newCity; 
                        newCity.x = bestX;
                        newCity.y = bestY;
                        newCity.population = migrants;
                        newCity.factionId = faction.id;
                        
                        newColonies.push_back(newCity);
                        map.getGrid().get(bestX, bestY).isOccupied = true;
                    }
                }
            }

            faction.populationTotale += settlement.population;
        }

        // Ajout des nouvelles villes
        if (!newColonies.empty()) {
            for(const auto& nc : newColonies) {
                faction.populationTotale += nc.population; 
            }
            //std::cout << "La faction " << faction.id << " a fondé une nouvelle ville en : " << newColonies.back().x << " / " << newColonies.back().y << " avec une population de " << newColonies.back().population << std::endl;
            faction.colonies.insert(faction.colonies.end(), newColonies.begin(), newColonies.end());
        }
        //std::cout << "Faction " << faction.id << " - Population totale : " << faction.populationTotale << " - Stock de nourriture : " << faction.stockFood << std::endl;

        //expandRadius(faction);
}


void CityManager::initializeCivilizations(Map& map) 
{
    int currentFactionId = 1;
    std::vector<Candidate> candidates;
    std::vector<Faction> factions; 
    
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());

    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            float attractivity = calculateTechAttractivity(map, x, y, TECH_BASE_LEVEL);
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
            std::cout << "Nombre de factions souhaité  (" << numberOfFactions << ")." << std::endl;
            std::cout << "Factions créées : " << factions.size() << std::endl;
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
            newFaction.radiusInfluence = DEFAULT_EXPAND_INFLUENCE_RADIUS;
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
