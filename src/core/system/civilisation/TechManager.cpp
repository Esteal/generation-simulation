#include "TechManager.h"
#include "ConfigManager.h"

TechManager::TechManager() {
    ConfigManager& cfg = ConfigManager::getInstance();
    
    COST_WOOD_BRONZE = cfg.getConfig().costWoodBronze;
    REQ_POP_BRONZE = cfg.getConfig().reqPopBronze;
    REQ_COLONIES_BRONZE = cfg.getConfig().reqColoniesBronze;
    COST_BRONZE_IRON = cfg.getConfig().costBronzeIron;
    COST_COAL_IRON = cfg.getConfig().costCoalIron;
    REQ_POP_IRON = cfg.getConfig().reqPopIron;
    REQ_COLONIES_IRON = cfg.getConfig().reqColoniesIron;
}

void TechManager::processFaction(Map& map, Faction& faction, float deltaTime) {
    const int numberOfColonies = static_cast<int>(faction.colonies.size());
    if (faction.techLevel == TechLevel::STONE_AGE) {
        
        if (faction.populationTotale >= REQ_POP_BRONZE && 
            numberOfColonies >= REQ_COLONIES_BRONZE && 
            faction.stockWood >= COST_WOOD_BRONZE) 
        {
            faction.stockWood -= COST_WOOD_BRONZE;
            
            faction.techLevel = TechLevel::BRONZE_AGE;
            applyTechBonus(faction);
            
            std::cout << "=== EVOLUTION === La faction " << faction.id << " est passee a l'Age de Bronze !" << std::endl;
        }
    }
    else if (faction.techLevel == TechLevel::BRONZE_AGE) {
        
        if (faction.populationTotale >= REQ_POP_IRON && 
            numberOfColonies >= REQ_COLONIES_IRON && 
            faction.stockBronze >= COST_BRONZE_IRON &&
            faction.stockCoal >= COST_COAL_IRON) 
        {
            faction.stockBronze -= COST_BRONZE_IRON;
            faction.stockCoal -= COST_COAL_IRON;
            
            faction.techLevel = TechLevel::IRON_AGE;
            applyTechBonus(faction);
            
            std::cout << "=== EVOLUTION === La faction " << faction.id << " est passee a l'Age de Fer !" << std::endl;
        }
    }
}

void TechManager::applyTechBonus(Faction& faction) {
    
    if (faction.techLevel == TechLevel::BRONZE_AGE) {
        faction.radiusMigration += 1;
        faction.radiusExploitation += 1;
    }
    else if (faction.techLevel == TechLevel::IRON_AGE) {
        faction.radiusMigration += 2;
        faction.radiusExploitation += 1;
    }
}