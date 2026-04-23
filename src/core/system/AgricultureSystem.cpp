#include "system/AgricultureSystem.h"
#include "ConfigManager.h"

AgricultureSystem::AgricultureSystem() {
    ConfigManager& cfg = ConfigManager::getInstance();
    WHEAT_GROWTH_SPEED = cfg.getConfig().wheatGrowthSpeed;
}

void AgricultureSystem::process(Map& map, float deltaTime) {
    if (deltaTime <= 0.0f) return;

    int heigh = static_cast<int>(map.getHeight());
    int width = static_cast<int>(map.getHeight());

    for (int y = 0; y < heigh; ++y) {
        for (int x = 0; x < width; ++x) {
            Cell& cell = map.getGrid().get(x, y);

            if (cell.material == Material::WHEAT) {
                float growthMultiplier = cell.humidity * cell.light * 2.0f; 
                
                cell.pourcentageEvolution += WHEAT_GROWTH_SPEED * growthMultiplier * deltaTime;
                
                if (cell.stage == Stage::STAGE_1 && cell.pourcentageEvolution >= 0.5f) {
                    cell.stage = Stage::STAGE_2;
                }
                else if (cell.stage == Stage::STAGE_2 && cell.pourcentageEvolution >= 1.0f) {
                    cell.stage = Stage::STAGE_3;
                }

                if(cell.pourcentageEvolution > 5.0f) {
                        // TODO : revoir le coefficient et le mettre dans le json
                        //récolte pourrie
                        //std::cout << "Récolte pourrie en " << x << ", " << y << " (Pourcentage: " << cell.pourcentageEvolution << ")\n";
                        cell.stage = Stage::UNDEFINED;
                        cell.material = Material::NONE;
                        cell.pourcentageEvolution = 0.0f;
                        cell.faction = 0;
                    }
            }
        }
    }
}