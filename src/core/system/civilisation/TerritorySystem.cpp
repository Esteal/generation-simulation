#include "TerritorySystem.h"
#include <cmath>
#include "ConfigManager.h"
TerritorySystem::TerritorySystem() {
    ConfigManager& cfg = ConfigManager::getInstance();
    TENSION_FACTOR = cfg.getConfig().tensionFactor;
}

void TerritorySystem::process(Map& map, float deltaTime) {
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());
    
    // Nettoyage chirurgical (O(N) où N est le nombre de cases réellement réclamées)
    for (const auto& pos : claimedCells) {
        // Sécurité au cas où la carte serait redimensionnée en cours de jeu
        if (pos.first >= 0 && pos.first < width && pos.second >= 0 && pos.second < height) {
            map.getGrid().get(pos.first, pos.second).faction = 0;
        }
    }
    claimedCells.clear(); 

    std::vector<Faction>& factions = map.getFactions();

    // BOucle pour projetter l'influence de chaque ville
    for (Faction& faction : factions) {
        int radius = faction.radiusInfluence;

        for (const Settlement& city : faction.colonies) {
            for (int dy = -radius; dy <= radius; ++dy) {
                for (int dx = -radius; dx <= radius; ++dx) {
                    
                    int nx = static_cast<int>(city.x) + dx;
                    int ny = static_cast<int>(city.y) + dy;

                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        
                        Cell& cell = map.getGrid().get(nx, ny);
                        
                        // Si la case est neutre, on l'annexe
                        if (cell.faction == 0) {
                            cell.faction = faction.id;
                            
                            // --- On mémorise la modification, opptimisation mémoire ---
                            claimedCells.push_back({nx, ny});
                        }
                        // La case appartient déjà à quelqu'un d'autre : on augmente la tension entre les 2 factions
                        else if (cell.faction != faction.id) {
                            
                            faction.relations[cell.faction].tension += TENSION_FACTOR * deltaTime;
                            faction.relations[cell.faction].tension = std::min(faction.relations[cell.faction].tension, 100.0f); 
                            // TODO : Cap à 100, définir plus tard différents différents paliers de tension (à voir avec config.json)

                            if (faction.relations[cell.faction].tension >= 100.0f && 
                                faction.relations[cell.faction].state == DiplomaticState::PEACE) 
                            {
                                faction.relations[cell.faction].state = DiplomaticState::WAR;
                                std::cout << "⚔️ DECLARATION DE GUERRE : Les frontieres des factions " 
                                          << faction.id << " et " << cell.faction 
                                          << " se touchent de trop pres !" << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
}