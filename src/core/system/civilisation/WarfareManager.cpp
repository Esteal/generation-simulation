#include "WarfareManager.h"
#include "ConfigManager.h"
#include <cmath>
#include <iostream>
#include <map>

WarfareManager::WarfareManager()
{

    const GameConfig& cfg = ConfigManager::getInstance().getConfig();
    BASE_CASUALTY_RATE = cfg.baseCasualtyRate;
    COMBAT_RANGE_BONUS = cfg.combatRangeBonus;

    std::cout << "BASE_CASUALTY_RATE" << BASE_CASUALTY_RATE << "\nCOMBAT_RANGE_BONUS : " << COMBAT_RANGE_BONUS << std::endl;
}

float WarfareManager::getTechMultiplier(TechLevel techLevel) const {
    switch (techLevel) {
        case TechLevel::UNDEFINED:
        case TechLevel::STONE_AGE:  return 1.0f; // Force de base
        case TechLevel::BRONZE_AGE: return 1.5f; // +50% de puissance
        case TechLevel::IRON_AGE:   return 2.5f; // +150% de puissance
        default:                    return 1.0f;
    }
}

void WarfareManager::processFaction(Map& map, Faction& faction, float deltaTime) {
    std::vector<Faction>& allFactions = map.getFactions();
    int combatRange = faction.radiusMigration + COMBAT_RANGE_BONUS;

    // Variables pour accumuler les pertes avant de les appliquer
    int totalMyCasualties = 0;
    std::map<int, int> enemyCasualtiesMap; // Clé = ID Faction Ennemie, Valeur = Pertes

    // --- ANALYSE ET CALCUL DES PERTES (Aucune modification) ---
    for (const Settlement& myCity : faction.colonies) {
        
        for (auto& [enemyId, relation] : faction.relations) {
            if (relation.state == DiplomaticState::WAR) {
                
                // Recherche de la faction ennemie
                Faction* enemyFaction = nullptr;
                for (auto& f : allFactions) {
                    if (f.id == enemyId) { 
                        enemyFaction = &f; 
                        break; 
                    }
                }
                if (!enemyFaction) continue; // La faction a déjà été détruite

                // Comparaison avec les villes ennemies
                for (const Settlement& enemyCity : enemyFaction->colonies) {
                    
                    int dist = std::abs(static_cast<int>(myCity.x) - static_cast<int>(enemyCity.x)) + 
                               std::abs(static_cast<int>(myCity.y) - static_cast<int>(enemyCity.y));

                    if (dist <= combatRange) {
                        
                        // Calcul de la puissance militaire (Population * Bonus Tech), à voir plus tard avec différents type d'évolution pour le TechBonus
                        float myPower = myCity.population * getTechMultiplier(faction.techLevel);
                        float enemyPower = enemyCity.population * getTechMultiplier(enemyFaction->techLevel);

                        if (myPower <= 0) myPower = 1.0f;
                        if (enemyPower <= 0) enemyPower = 1.0f;

                        // Calcul croisé des morts
                        int myDead = static_cast<int>((enemyPower / myPower) * BASE_CASUALTY_RATE * deltaTime * myCity.population);
                        int enemyDead = static_cast<int>((myPower / enemyPower) * BASE_CASUALTY_RATE * deltaTime * enemyCity.population);

                        // Accumulation
                        totalMyCasualties += myDead;
                        enemyCasualtiesMap[enemyId] += enemyDead;
                    }
                }
            }
        }
    }

    // --- APPLIQUATION DES PERTEZ ---
    
    // Nos pertes
    if (totalMyCasualties > 0) {
        // TODO : Pour l'instant on ne voit pas la faction qui nous as tué
        killPopulation(map, faction, totalMyCasualties);
    }

    // Appliquer les pertes aux factions ennemies
    for (auto& [enemyId, totalCasualties] : enemyCasualtiesMap) {
        if (totalCasualties > 0) {
            for (Faction& enemyFaction : allFactions) {
                if (enemyFaction.id == enemyId) {
                    killPopulation(map, enemyFaction, totalCasualties, &faction);
                    break;
                }
            }
        }
    }
}