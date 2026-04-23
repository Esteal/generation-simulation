#include "IFactionSystem.h"
#include "ConfigManager.h"

IFactionSystem::IFactionSystem() {
    // Initialize static constants
    const GameConfig& cfg = ConfigManager::getInstance().getConfig();

    MIN_MIGRANTS = cfg.minMigrants;
    GROWTH_RATE = cfg.growthRate;
    
    SOIL_BONUS_MULTIPLIER = cfg.soilBonusMultiplier;
    SOIL_BONUS_MAX = cfg.soilBonusMax;
    ALTITUDE_THRESHOLD = cfg.altitudeThreshold;
    ALTITUDE_MALUS_MULTIPLIER = cfg.altitudeMalusMultiplier;
    FERTILITY_TEMPERATURE_WEIGHT = cfg.fertilityTemperatureWeight;
    FERTILITY_HUMIDITY_WEIGHT = cfg.fertilityHumidityWeight;
    FERTILITY_LIGHT_WEIGHT = cfg.fertilityLightWeight;
    SCAN_RADIUS = cfg.scanRadius;
    STONE_AGE_WATER_BONUS = cfg.stoneAgeWaterBonus;
    STONE_AGE_WOOD_BONUS = cfg.stoneAgeWoodBonus;
    BRONZE_AGE_WOOD_BONUS = cfg.bronzeAgeWoodBonus;
    BRONZE_AGE_RESOURCE_BONUS = cfg.bronzeAgeResourceBonus;
    IRON_AGE_BRONZE_BONUS = cfg.ironAgeBronzeBonus;
    IRON_AGE_RESOURCE_BONUS = cfg.ironAgeResourceBonus;
}

void IFactionSystem::resetCell(Cell& cell) {
    cell.material = Material::NONE;
    cell.stage = Stage::UNDEFINED;
    cell.attractivityLevel = 0.0f;
    cell.pourcentageEvolution = 0.0f;
}

float IFactionSystem::calculateTechAttractivity(const Map& map, int cx, int cy, TechLevel techLevel) {
    const Cell& centerCell = map.getGrid().get(cx, cy);

    if (centerCell.biome == BiomeIndex::OCEAN || 
        centerCell.biome == BiomeIndex::GLACE || 
        centerCell.biome == BiomeIndex::RIVER) {
        return 0.0f;
    }

    float soilBonus = std::min(centerCell.granular * SOIL_BONUS_MULTIPLIER, SOIL_BONUS_MAX); 

    float altitudeMalus = 0.0f;
    if (centerCell.bedrock > ALTITUDE_THRESHOLD) {
        altitudeMalus = (centerCell.bedrock - ALTITUDE_THRESHOLD) * ALTITUDE_MALUS_MULTIPLIER; 
    }

    float baseFertility = (centerCell.temperature * FERTILITY_TEMPERATURE_WEIGHT) + 
                          (centerCell.humidity * FERTILITY_HUMIDITY_WEIGHT) + 
                          (centerCell.light * FERTILITY_LIGHT_WEIGHT) + 
                          soilBonus - altitudeMalus;

    baseFertility = std::max(0.0f, std::min(baseFertility, 1.0f));
    
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());
    int scanRadius = SCAN_RADIUS; 
    
    int woodCount = 0;
    int waterCount = 0;
    int bronzeCoalCount = 0;
    int ironGoldCount = 0;

    for (int dy = -scanRadius; dy <= scanRadius; ++dy) {
        for (int dx = -scanRadius; dx <= scanRadius; ++dx) {
            if (dx == 0 && dy == 0) continue;

            int nx = cx + dx;
            int ny = cy + dy;

            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                const Cell& neighbor = map.getGrid().get(nx, ny);

                if (neighbor.biome == BiomeIndex::RIVER) waterCount++;
                
                if (neighbor.material == Material::OAK_TREE || 
                    neighbor.material == Material::PINE_TREE || 
                    neighbor.material == Material::JUNGLE_TREE) {
                    //std::cout << "  -> Arbre trouvé en " << nx << ", " << ny << " (Mat: " << static_cast<int>(neighbor.material) << ")\n";
                    woodCount++;
                }
                else if (neighbor.material == Material::BRONZE_ORE || neighbor.material == Material::COAL_ORE) {
                    bronzeCoalCount++;
                }
                else if (neighbor.material == Material::IRON_ORE || neighbor.material == Material::GOLD_ORE) {
                    ironGoldCount++;
                }
            }
        }
    }

    float finalScore = baseFertility;

    if (techLevel == TechLevel::UNDEFINED || techLevel == TechLevel::STONE_AGE) {
        finalScore += (waterCount * STONE_AGE_WATER_BONUS) + (woodCount * STONE_AGE_WOOD_BONUS);
    }
    else if (techLevel == TechLevel::BRONZE_AGE) {
        finalScore += (woodCount * BRONZE_AGE_WOOD_BONUS) + (bronzeCoalCount * BRONZE_AGE_RESOURCE_BONUS);
    }
    else if (techLevel == TechLevel::IRON_AGE) {
        finalScore += (bronzeCoalCount * IRON_AGE_BRONZE_BONUS) + (ironGoldCount * IRON_AGE_RESOURCE_BONUS);
    }
    /*
    std::cout << "[Attractivite " << cx << ", " << cy << "] TechLevel: " << static_cast<int>(techLevel) << "\n"
              << "  -> Env   : Temp=" << centerCell.temperature 
              << " | Hum=" << centerCell.humidity 
              << " | Light=" << centerCell.light << "\n"
              << "  -> Sol   : Granular=" << centerCell.granular 
              << " (Bonus: +" << soilBonus << ") | Bedrock=" << centerCell.bedrock 
              << " (Malus: -" << altitudeMalus << ")\n"
              << "  -> Fert. : " << baseFertility << "\n"
              << "  -> Ress. : Eau=" << waterCount 
              << " | Bois=" << woodCount 
              << " | Bronze/Charbon=" << bronzeCoalCount 
              << " | Fer/Or=" << ironGoldCount << "\n"
              << "  => SCORE FINAL : " << finalScore << "\n"
              << "----------------------------------------" << std::endl;
    */
    // On laisse le score dépasser 1.0f. Ainsi, une case moyennement fertile (0.6) 
    // mais entourée de fer (bonus 0.6) aura 1.2 et battra une prairie vide (1.0).
    // Dans la logique, après à voir les tests de regressions
    return finalScore; 
}

void IFactionSystem::killPopulation(Map& map, Faction& faction, int totalDeaths, Faction* conqueror) 
{
    if (totalDeaths <= 0 || faction.colonies.empty()) return;

    if (totalDeaths >= faction.populationTotale) {
        totalDeaths = faction.populationTotale;
    }

    int remainingDeaths = totalDeaths;
    int initialTotalPopulation = faction.populationTotale;
    
    bool capitalConquered = false;

    // --- DISTRIBUTION DES PERTES ---
    for (auto it = faction.colonies.begin(); it != faction.colonies.end(); ) {
        
        float populationRatio = static_cast<float>(it->population) / static_cast<float>(initialTotalPopulation);
        int deathsInThisCity = static_cast<int>(totalDeaths * populationRatio);

        if (std::next(it) == faction.colonies.end()) {
            deathsInThisCity = remainingDeaths;
        }

        deathsInThisCity = std::min(deathsInThisCity, it->population);
        
        it->population -= deathsInThisCity;
        remainingDeaths -= deathsInThisCity;

        // Si la ville est détruite
        if (it->population < MIN_MIGRANTS) {
            
            bool isCapital = (static_cast<int>(it->x) == faction.capitalX && static_cast<int>(it->y) == faction.capitalY);

            if (conqueror != nullptr) {
                //std::cout << "⚔️ VICTOIRE : La faction " << conqueror->id 
                  //        << " a conquis la ville en " << it->x << " / " << it->y 
                    //      << " (Anciennement Faction " << faction.id << ") !" << std::endl;
                
                // Transfert de la ville
                Settlement conqueredCity = *it;
                conqueredCity.factionId = conqueror->id;
                conqueredCity.population = MIN_MIGRANTS; 
                conqueror->colonies.push_back(conqueredCity);
                map.getGrid().get(it->x, it->y).faction = conqueror->id;

                if (isCapital) {
                    capitalConquered = true;
                }
            } 
            else {
                //std::cout << "💀 RUINE : La faction " << faction.id 
                  //        << " a perdu la ville en " << it->x << " / " << it->y 
                    //      << " par manque de ressources ou usure." << std::endl;
                
                // Destruction simple (mort de fin uniquement pour l'instant, à voir plus tard si manque de ressource)
                map.getGrid().get(it->x, it->y).isOccupied = false;
                map.getGrid().get(it->x, it->y).faction = 0;
            }

            it = faction.colonies.erase(it);

            // Si la capitale vient de tomber aux mains de l'ennemi, on arrête le massacre : 
            // la guerre est terminée
            if (capitalConquered) break; 
            
        } else {
            ++it;
        }
    }

    // --- GESTION DE LA CAPITALE ET DE L'EMPIRE ---
    
    // La capitale a été conquise
    if (capitalConquered && conqueror != nullptr) {
        std::cout << "👑 CHUTE DE L'EMPIRE : La capitale de la faction " << faction.id 
                  << " est tombee ! Toutes ses colonies se rendent a la faction " << conqueror->id << " !" << std::endl;
        
        // Toutes les villes restantes changentt de faction
        for (auto& remainingCity : faction.colonies) {
            remainingCity.factionId = conqueror->id;
            conqueror->colonies.push_back(remainingCity);
            map.getGrid().get(remainingCity.x, remainingCity.y).faction = conqueror->id;
        }
        faction.colonies.clear(); // La faction vaincue n'a plus aucune ville (la supression de la faction se fait dans la classe CivilisationManager)
    } 
    // Famine, ou ville mineure détruite (la faction survit s'il reste des villes)
    else if (!faction.colonies.empty()) {
        
        bool capitalExists = false;
        for (const auto& city : faction.colonies) {
            if (static_cast<int>(city.x) == faction.capitalX && static_cast<int>(city.y) == faction.capitalY) {
                capitalExists = true;
                break;
            }
        }
        
        // Si la capitale n'existe plus (morte de faim uniquement pour l'instant), on en désigne une nouvelle
        if (!capitalExists) {
            auto newCapitalIt = faction.colonies.begin();
            // On cherche la ville avec la plus grande population pour devenir la nouvelle capitale
            for (auto cit = faction.colonies.begin(); cit != faction.colonies.end(); ++cit) {
                if (cit->population > newCapitalIt->population) {
                    newCapitalIt = cit;
                }
            }
            faction.capitalX = static_cast<int>(newCapitalIt->x);
            faction.capitalY = static_cast<int>(newCapitalIt->y);
            
            std::cout << "🏛️ EXODE : La faction " << faction.id << " a perdu sa capitale ! "
                      << "Le nouveau centre de pouvoir est etabli en " << faction.capitalX << " / " << faction.capitalY << "." << std::endl;
        }
    }

    // --- RECALCUL DES POPULATIONS ---
    faction.populationTotale = 0;
    for (const Settlement& settlement : faction.colonies) {
        faction.populationTotale += settlement.population;
    }

    if (conqueror != nullptr) {
        conqueror->populationTotale = 0;
        for (const Settlement& settlement : conqueror->colonies) {
            conqueror->populationTotale += settlement.population;
        }
    }
}