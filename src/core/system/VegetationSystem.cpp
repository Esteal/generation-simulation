#include "VegetationSystem.h"
#include <cmath>
#include <algorithm>
#include <cstdlib>   

void VegetationSystem::process(Map& map, float deltaTime) 
{
    if(deltaTime > 0)
    {
        // évolution au cours du temps de la végétation
    }
    else 
    {
        sowSeeds(map);
    }
}

void VegetationSystem::sowSeeds(Map &map)
{
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());
    
    // Revoir la mousse quand la génération le permettra

    /*
    int nbJunleTrees = 0;
    int nbPineTrees = 0;
    int nbOakTrees = 0;
    int nbCactus = 0;
    int nbMoss = 0;
    int nbGrass = 0;
    int nbNone = 0;
    */

    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            Cell &cell = map.getGrid().get(x, y);
            
            if (cell.biome == BiomeIndex::OCEAN || cell.biome == BiomeIndex::BEACH || cell.biome == BiomeIndex::GLACE || cell.biome == BiomeIndex::RIVER) {
                cell.flora = FloraType::NONE;
                continue;
            }

            float temp = cell.temperature; 
            float hum = cell.humidity;     
            float light = cell.light;
            float sediment = cell.granular *4;
            float altitude = cell.bedrock + cell.granular;

            float oakProb = std::min({
                calculateSensivityToVegetation(temp, 0.55f, 0.2f),
                calculateSensivityToVegetation(hum, 0.70f, 0.3f),
                calculateSensivityToVegetation(light, 0.60f, 0.4f)
            }) * sediment;

            float pineProb = std::min({
                calculateSensivityToVegetation(temp, 0.25f, 0.2f),
                calculateSensivityToVegetation(hum, 0.55f, 0.3f)
            }) * sediment;


            float jungleProb = std::min({
                calculateSensivityToVegetation(temp, 0.7f, 0.3f),
                calculateSensivityToVegetation(hum, 0.8f, 0.2f)
            }) * sediment;

            float cactusProb = std::min({
                calculateSensivityToVegetation(temp, 0.20f, 0.85f),
                calculateSensivityToVegetation(hum, 0.15f, 0.10f),
                calculateSensivityToVegetation(light, 0.20f, 0.90f)
            });

            float mossProb = std::min({
                calculateSensivityToVegetation(hum, 0.40f, 0.90f),
                calculateSensivityToVegetation(light, 0.35f, 0.30f)
            });

            float grassProb = std::min({
                calculateSensivityToVegetation(temp, 0.40f, 0.50f),
                calculateSensivityToVegetation(hum, 0.40f, 0.40f)
            }) * sediment;
            
            float randVal = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            
            bool isHighAltitude = (altitude > 0.85f);

            if (mossProb > 0.01f && randVal < mossProb && cell.biome != BiomeIndex::DESERT) {
                cell.flora = FloraType::MOSS;
                //nbMoss++;
            } 
            else if (cactusProb > 0.01f && randVal < (cactusProb * 0.1f) && cell.biome == BiomeIndex::DESERT) { 
                cell.flora = FloraType::CACTUS;
                //nbCactus++;
            }
            else if (!isHighAltitude && jungleProb > 0.01f && randVal < jungleProb && cell.biome != BiomeIndex::DESERT) {
                cell.flora = FloraType::JUNGLE_TREE;
                //nbJunleTrees++;
            }
            else if (!isHighAltitude && pineProb > 0.01f && randVal < pineProb && cell.biome != BiomeIndex::DESERT) {
                cell.flora = FloraType::PINE_TREE;
                //nbPineTrees++;
            }
            else if (!isHighAltitude && oakProb > 0.01f && randVal < oakProb && cell.biome != BiomeIndex::DESERT) {
                cell.flora = FloraType::OAK_TREE;
                //nbOakTrees++;
            }
            else if (grassProb > 0.01f && randVal < grassProb && cell.biome != BiomeIndex::DESERT) {
                cell.flora = FloraType::GRASS;
                //nbGrass++;
            }
            else {
                cell.flora = FloraType::NONE; 
                //nbNone++;
            }
        }
    }
    /*
    std::cout << "Vegetation distribution: " << std::endl;
    std::cout << "  Jungle Trees: " << nbJunleTrees << std::endl;
    std::cout << "  Pine Trees: " << nbPineTrees << std::endl;
    std::cout << "  Oak Trees: " << nbOakTrees << std::endl;
    std::cout << "  Cactus: " << nbCactus << std::endl;
    std::cout << "  Moss: " << nbMoss << std::endl;
    std::cout << "  Grass: " << nbGrass << std::endl;
    std::cout << "  None: " << nbNone << std::endl;
    */
}


float VegetationSystem::calculateSensivityToVegetation( 
        float envValue, float optimalValue, float tolerance) const
{
    float diff = std::abs(envValue - optimalValue);
    return std::exp(-(std::pow(diff/tolerance, 2)));
}
