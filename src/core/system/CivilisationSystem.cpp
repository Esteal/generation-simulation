#include "system/CivilisationSystem.h"
#include "system/WorldSimulator.h"

#include <algorithm>

struct Candidate {
    int x;
    int y;
    float attractivity;
};

void CivilisationSystem::process(Map& map, float deltaTime) 
{
    if(deltaTime > 0)
    {
        // évolution au cours du temps des civilisations
        evolveCivilizations(map, deltaTime);
    }
    else 
    {
        initializeCivilizations(map);
    }
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
            float attractivity = map.getGrid().get(x, y).attractivityLevel;
            if(attractivity > 0.0f) {
                candidates.push_back({x, y, attractivity});
            }
        }
    }

    std::sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
        return a.attractivity > b.attractivity;
    });

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
            newFaction.populationTotale = 100;

            Settlement capital;
            capital.x = candidate.x;
            capital.y = candidate.y;
            capital.population = 100; 
            capital.factionId = newFaction.id;

            newFaction.colonies.push_back(capital);
            factions.push_back(newFaction);
            
            map.getGrid().get(candidate.x, candidate.y).isOccupied = true;
            std::cout << "Faction en : " << capital.x << " / " << capital.y << std::endl;
        }
    }
    
    map.setFactions(factions);
}

void CivilisationSystem::evolveCivilizations(Map& map, float deltaTime) 
{
    std::vector<Faction>& factions = map.getFactions();

    for (Faction& faction : factions) {
        
        std::vector<Settlement> newColonies; 
        faction.populationTotale = 0;        // On remet à zéro pour recompter cette frame

        for (Settlement& settlement : faction.colonies) {
            
            const Cell& currentCell = map.getGrid().get(settlement.x, settlement.y);
            int maxPopulation = static_cast<int>(CARRYING_CAPACITY_BASE * currentCell.attractivityLevel);

            settlement.population += static_cast<int>(settlement.population * GROWTH_RATE * deltaTime);

            // Gérer la surpopulation (Expansion)
            if (settlement.population > maxPopulation) {
                int migrants = settlement.population - maxPopulation;
                settlement.population = maxPopulation; 

                if (migrants >= MIN_MIGRANTS) {
                    int bestX = -1;
                    int bestY = -1;
                    float bestScore = 0.0f;

                    for (int dy = -1; dy <= 1; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            if (dx == 0 && dy == 0) continue; 

                            int nx = settlement.x + dx;
                            int ny = settlement.y + dy;

                            if (nx >= 0 && nx < map.getWidth() && ny >= 0 && ny < map.getHeight()) {
                                const Cell& neighborCell = map.getGrid().get(nx, ny);
                                
                                if (!neighborCell.isOccupied && neighborCell.attractivityLevel > bestScore) {
                                    bestScore = neighborCell.attractivityLevel;
                                    bestX = nx;
                                    bestY = ny;
                                }
                            }
                        }
                    }

                    if (bestX != -1 && bestY != -1) {
                        Settlement newCity; 
                        newCity.x = bestX;
                        newCity.y = bestY;
                        newCity.population = migrants;
                        newCity.factionId = faction.id; //
                        
                        newColonies.push_back(newCity);
                        map.getGrid().get(bestX, bestY).isOccupied = true;
                    }
                }
            }

            faction.populationTotale += settlement.population;
        }

        if (!newColonies.empty()) {
            for(const auto& nc : newColonies) {
                faction.populationTotale += nc.population; // N'oublions pas les migrants !
            }
            faction.colonies.insert(faction.colonies.end(), newColonies.begin(), newColonies.end());
        }
    }
}