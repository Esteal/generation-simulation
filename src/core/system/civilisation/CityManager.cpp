#include "CityManager.h"

void CityManager::processFaction(Map& map, Faction &faction, float deltaTime)
{
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

                            if (nx >= 0 && nx < map.getWidth() && ny >= 0 && ny < map.getHeight()) {
                                const Cell& neighborCell = map.getGrid().get(nx, ny);
                                
                                if (neighborCell.biome != BiomeIndex::OCEAN && 
                                    neighborCell.biome != BiomeIndex::RIVER && 
                                    neighborCell.biome != BiomeIndex::GLACE) {
                                    
                                    float dynamicScore = calculateTechAttractivity(map, nx, ny, faction.techLevel);

                                    if (neighborCell.isOccupied && neighborCell.faction != faction.id && neighborCell.faction != 0) {
                                        if (dynamicScore > 0.8f) {
                                            
                                            // On augmente la tension (facteur à équilibrer)
                                            float tensionIncrease = dynamicScore * TENSION_FACTOR * deltaTime;
                                            faction.relations[neighborCell.faction].tension += tensionIncrease;

                                            // Déclenchement de la guerre
                                            if (faction.relations[neighborCell.faction].tension >= 100.0f && 
                                                faction.relations[neighborCell.faction].state == DiplomaticState::PEACE) 
                                            {
                                                faction.relations[neighborCell.faction].state = DiplomaticState::WAR;
                                                std::cout << "=== DECLARATION DE GUERRE === La faction " << faction.id 
                                                        << " declare la guerre a la faction " << neighborCell.faction 
                                                        << " pour le controle des ressources !" << std::endl;
                                            }
                                        }
                                    } 
                                    else if (!neighborCell.isOccupied && neighborCell.material == Material::NONE) {
                                        
                                        if (dynamicScore > bestScore) {
                                            bestScore = dynamicScore;
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

void CityManager::expandRadius(Faction &faction)
{
    if (faction.colonies.size() >= 12 && faction.radiusMigration < 5)
    {
        std::cout << "Évolution territoriale (Tiers 2) pour la faction " << faction.id 
                  << " ! Rayon de migration : " << faction.radiusMigration << " -> 5" << std::endl;
        
        faction.radiusMigration = 5;
        faction.radiusExploitation = 4;
    }
    else if (faction.colonies.size() >= 9 && faction.radiusMigration < 4)
    {
        std::cout << "Évolution territoriale (Tiers 1) pour la faction " << faction.id 
                  << " ! Rayon de migration : " << faction.radiusMigration << " -> 4" << std::endl;
        
        faction.radiusMigration = 4;
        faction.radiusExploitation = 3;
    }
}