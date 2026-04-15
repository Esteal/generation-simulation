#include "system/AgricultureSystem.h"

void AgricultureSystem::process(Map& map, float deltaTime) {
    if (deltaTime <= 0.0f) return;

    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            Cell& cell = map.getGrid().get(x, y);

            if (cell.material == Material::WHEAT) {
                float growthMultiplier = cell.humidity * cell.light * 2.0f; 
                
                cell.pourcentageEvolution += WHEAT_GROWTH_SPEED * growthMultiplier * deltaTime;

                if (cell.stage == Stage::STAGE_1 && cell.pourcentageEvolution >= 0.5f) {
                    cell.stage = Stage::STAGE_2;
                }
                else if (cell.stage == Stage::STAGE_2 && cell.pourcentageEvolution >= 1.0f) {
                    cell.stage = Stage::STAGE_3;
                    if(cell.pourcentageEvolution > 3.0f) {
                        //récolte pourrie
                        cell.stage = Stage::UNDEFINED;
                        cell.material = Material::NONE;
                        cell.pourcentageEvolution = 0.0f;
                        cell.faction = 0;
                    }
                }
            }
        }
    }
}