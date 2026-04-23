#include "ResourceManager.h"
#include <algorithm>
#include "ConfigManager.h"

ResourceManager::ResourceManager() {
    // Charger les paramètres depuis la configuration
    ConfigManager& cfg = ConfigManager::getInstance();
    
    TREE_CHOP_RATE = cfg.getConfig().treeChopRate; 
    MINING_RATE = cfg.getConfig().miningRate;    
    MAX_WOOD_PER_TREE = cfg.getConfig().maxWoodPerTree; 
    MAX_ORE_PER_VEIN = cfg.getConfig().maxOrePerVein; 
}

void ResourceManager::processFaction(Map& map, Faction& faction, float deltaTime) {
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());
    
    int radius = faction.radiusExploitation; 

    for (const Settlement& settlement : faction.colonies) {
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                
                int nx = settlement.x + dx;
                int ny = settlement.y + dy;

                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    Cell& cell = map.getGrid().get(nx, ny);

                    if(faction.techLevel >= TechLevel::STONE_AGE) {
                        if (cell.material == Material::OAK_TREE || 
                            cell.material == Material::PINE_TREE || 
                            cell.material == Material::JUNGLE_TREE) 
                        {
                            if (cell.stage == Stage::STAGE_3) {
                                
                                float extracted = std::min(TREE_CHOP_RATE * deltaTime, cell.pourcentageEvolution);
                                cell.pourcentageEvolution -= extracted;
                                
                                faction.stockWood += static_cast<int>(extracted * MAX_WOOD_PER_TREE);
                                //std::cout << "Faction " << faction.id << " a récolté du bois. Quantité : "<< faction.stockWood << std::endl;
                                if (cell.pourcentageEvolution <= 0.0f) {
                                    resetCell(cell);
                                }
                            }
                            continue;
                        }
                    }

                    if (faction.techLevel >= TechLevel::BRONZE_AGE) {
                        if (cell.material == Material::BRONZE_ORE || cell.material == Material::COAL_ORE) {
                            
                            float extracted = std::min(MINING_RATE * deltaTime, cell.pourcentageEvolution);
                            cell.pourcentageEvolution -= extracted;

                            int yield = static_cast<int>(extracted * MAX_ORE_PER_VEIN);
                            
                            if (cell.material == Material::BRONZE_ORE) faction.stockBronze += yield;
                            else faction.stockCoal += yield;

                            if (cell.pourcentageEvolution <= 0.0f) {
                                resetCell(cell);
                            }
                            continue;
                        }
                    }

                    if (faction.techLevel >= TechLevel::IRON_AGE) {
                        if (cell.material == Material::IRON_ORE || cell.material == Material::GOLD_ORE) {
                            
                            float extracted = std::min(MINING_RATE * deltaTime, cell.pourcentageEvolution);
                            cell.pourcentageEvolution -= extracted;

                            int yield = static_cast<int>(extracted * MAX_ORE_PER_VEIN);

                            if (cell.material == Material::IRON_ORE) faction.stockIron += yield;
                            else faction.stockGold += yield;

                            if (cell.pourcentageEvolution <= 0.0f) {
                                resetCell(cell);
                            }
                            continue;
                        }
                    }
                }
            }
        }
    }
}