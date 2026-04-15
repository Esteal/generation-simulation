#include "TechManager.h"

void TechManager::processFaction(Map& map, Faction& faction, float deltaTime) {
    
    if (faction.techLevel == TechLevel::STONE_AGE) {
        
        if (faction.populationTotale >= REQ_POP_BRONZE && 
            faction.colonies.size() >= REQ_COLONIES_BRONZE && 
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
            faction.colonies.size() >= REQ_COLONIES_IRON && 
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