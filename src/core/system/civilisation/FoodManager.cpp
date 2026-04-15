#include "FoodManager.h"
#include <algorithm>


void FoodManager::processFaction(Map &map, Faction &faction, float deltaTime){

        harvestCrops(map, faction);

        // consommer
        // ceux qui n'ont pas consommer meurt

        faction.stockFood -= faction.populationTotale * CONSOMMATION_RATE * deltaTime;
        if (faction.stockFood < 0) {
                        //std::cout << faction.stockFood << std::endl;

            float missingFood = std::abs(faction.stockFood);
            int unfedPeople = static_cast<int>(missingFood / (CONSOMMATION_RATE * deltaTime));            
            faction.stockFood = 0; 
            if (unfedPeople > 0) {
                //std::cout << "Famine ! " << unfedPeople << " citoyens de la faction " << faction.id << " vont mourir." << std::endl;
                killPopulation(map, faction, unfedPeople);
            }
            
        }

        // replanter : 
        int howManySeedToReplant = faction.populationTotale / FOOD_HARVESTED + 1;
        bool necessity = (faction.stockFood > faction.populationTotale * 2 * (1 + GROWTH_RATE));
       // std::cout << "Nb habitant : " << faction.populationTotale << " | Nb de graines : " << howManySeedToReplant << std::endl;
       if (!necessity) 
        sowSeeds(map, faction, howManySeedToReplant);
}

void FoodManager::harvestCrops(Map &map, Faction &faction) {
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());
    int maxRadiusToPlant = faction.radiusExploitation;

    std::vector<Candidate> ripeCrops;

    // 1. Repérer toutes les plantations prêtes à être récoltées
    for (const Settlement& settlement : faction.colonies) {
        for (int dy = -maxRadiusToPlant; dy <= maxRadiusToPlant; ++dy) {
            for (int dx = -maxRadiusToPlant; dx <= maxRadiusToPlant; ++dx) {
                int nx = settlement.x + dx;
                int ny = settlement.y + dy;

                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    Cell& cell = map.getGrid().get(nx, ny);

                    if (cell.material == Material::WHEAT && cell.stage == Stage::STAGE_3 && cell.faction == faction.id) {
                        // Utilisation de la distance de Manhattan (plus optimisé que sqrt)
                        float dist = abs(nx - faction.capitalX) + abs(ny - faction.capitalY);
                        ripeCrops.push_back({nx, ny, dist});
                    }
                }
            }
        }
    }

    // 2. Trier les candidats du plus proche au plus éloigné de la capitale
    std::sort(ripeCrops.begin(), ripeCrops.end(), [](const Candidate& a, const Candidate& b) {
        return a.attractivity < b.attractivity; // Tri croissant
    });

    // 3. Récolter dans l'ordre de priorité
    for (const auto& crop : ripeCrops) {
        Cell& cell = map.getGrid().get(crop.x, crop.y);
        
        // On revérifie l'état de la cellule pour éviter de récolter deux fois la même
        // si les rayons de deux villes se chevauchent.
        if (cell.material == Material::WHEAT && cell.stage == Stage::STAGE_3) {
            faction.stockFood += FOOD_HARVESTED;
            
            // 90% de chance de replanter une nouvelle graine
            if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < REPLANT_CHANCE) {
                sowSeed(cell, faction.id);
            } else {
                cell.material = Material::NONE;
                cell.stage = Stage::UNDEFINED;
                cell.pourcentageEvolution = 0.0f;
            } 
        }
    }
}

void FoodManager::sowSeeds(Map& map, Faction& faction, int seedsToPlant) 
{
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());
    int maxRadiusToPlant = faction.radiusExploitation;
    
    for (const Settlement& settlement : faction.colonies) {
        
        std::vector<Candidate> candidates;

        for (int dy = -maxRadiusToPlant; dy <= maxRadiusToPlant; ++dy) {
            for (int dx = -maxRadiusToPlant; dx <= maxRadiusToPlant; ++dx) {
                
                int nx = settlement.x + dx;
                int ny = settlement.y + dy;

                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    Cell& cell = map.getGrid().get(nx, ny);

                    if (!cell.isOccupied && 
                        (cell.material == Material::NONE || cell.material == Material::GRASS) &&
                        cell.biome != BiomeIndex::OCEAN && 
                        cell.biome != BiomeIndex::RIVER && 
                        cell.biome != BiomeIndex::GLACE) 
                    {
                        float agriculturalScore = (cell.granular * 0.5f) + (cell.humidity * 0.5f);

                        // Malus de distance
                        // TODO : refaire avec manathan, sqrt trop gourmand
                        /*
                        float distance = std::sqrt(dx*dx + dy*dy);
                        if (distance > 0.0f) {
                            agriculturalScore -= (distance * 0.05f); 
                        }*/

                        candidates.push_back({nx, ny, agriculturalScore});
                    }
                }
            }
        }

        std::sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
            return a.attractivity > b.attractivity;
        });

        int plantedCount = 0;
        for (const auto& candidate : candidates) {
            if (plantedCount >= seedsToPlant) break;
            
            sowSeed(map.getGrid().get(candidate.x, candidate.y), faction.id);
            plantedCount++;
        }
    }
}

void FoodManager::sowSeed(Cell& targetCell, int factionID)
{
        targetCell.material = Material::WHEAT;
        targetCell.stage = Stage::STAGE_1;
        targetCell.pourcentageEvolution = 0.0f;
        targetCell.faction = factionID;
}
