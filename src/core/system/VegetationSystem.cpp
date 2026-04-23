#include "VegetationSystem.h"
#include <cmath>
#include <algorithm>
#include "ConfigManager.h"

VegetationSystem::VegetationSystem() {

    ConfigManager& cfg = ConfigManager::getInstance();
    
    POLINIZATION_CHANCE = cfg.getConfig().pollinizationChance;
    GROWTH_FACTOR = cfg.getConfig().growthFactor;
    DEATH_FACTOR = cfg.getConfig().deathFactor;
    /*
    RADIUS = {
        {Material::GRASS, 2},
        {Material::MOSS, 6},
        {Material::CACTUS, 2},
        {Material::PINE_TREE, 3},
        {Material::OAK_TREE, 6},
        {Material::JUNGLE_TREE, 5}
    };
    */
    int radiusCactus = cfg.getConfig().radiusCactus;
    int radiusPine = cfg.getConfig().radiusPineTree;
    int radiusOak = cfg.getConfig().radiusOakTree;
    int radiusJungle = cfg.getConfig().radiusJungleTree;

    RADIUS[Material::GRASS] = 2;
    RADIUS[Material::MOSS] = 6;
    RADIUS[Material::CACTUS] = radiusCactus;
    RADIUS[Material::PINE_TREE] = radiusPine;
    RADIUS[Material::OAK_TREE] = radiusOak;
    RADIUS[Material::JUNGLE_TREE] = radiusJungle;
}

void VegetationSystem::process(Map& map, float deltaTime) 
{
    if(deltaTime > 0)
        evolveVegetation(map, deltaTime);
    else 
        sowSeeds(map);
}

void VegetationSystem::sowASeed(Cell &cell)
{

    if (cell.biome == BiomeIndex::OCEAN || cell.biome == BiomeIndex::GLACE || cell.biome == BiomeIndex::RIVER) {
        return ;
    }
    
    float temp = cell.temperature; 
    float hum = cell.humidity;     
    float light = cell.light;
    float sediment = cell.granular;
    float altitude = cell.bedrock + cell.granular;
    
    // TODO : Ajouter facteur d'altitude + angle de la pente
    // TODO 2 : optimiser en ne calculant que les probabilités des matériaux compatibles avec le biome et les conditions climatiques (ex: pas de cactus dans la neige
    float oakProb = calculateRate(Material::OAK_TREE, temp, hum, light, sediment);
    float pineProb = calculateRate(Material::PINE_TREE, temp, hum, light, sediment);
    float jungleProb = calculateRate(Material::JUNGLE_TREE, temp, hum, light, sediment);
    float cactusProb = calculateRate(Material::CACTUS, temp, hum, light, sediment);
    float mossProb = calculateRate(Material::MOSS, temp, hum, light, sediment);
    float grassProb = calculateRate(Material::GRASS, temp, hum, light, sediment);
    float randVal = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    
    bool isHighAltitude = (cell.biome == BiomeIndex::MOUNTAINS) ? (altitude > 0.90f) : (altitude > 0.80f);

    if (mossProb > 0.01f && randVal < mossProb && cell.biome != BiomeIndex::DESERT && cell.biome != BiomeIndex::SNOW) {
        cell.material = Material::MOSS;
        cell.pourcentageEvolution = mossProb;
        //nbMoss++;
    } 
    else if (cactusProb > 0.01f && randVal < (cactusProb * 0.1f) && cell.biome == BiomeIndex::DESERT) { 
        cell.material = Material::CACTUS;
        cell.pourcentageEvolution = cactusProb;
        //nbCactus++;
    }
    else if (!isHighAltitude && jungleProb > 0.01f && randVal < jungleProb && cell.biome == BiomeIndex::JUNGLE) {
        cell.material = Material::JUNGLE_TREE;
        cell.pourcentageEvolution = jungleProb;
        //nbJunleTrees++;
    }
    else if (!isHighAltitude && pineProb > 0.01f && randVal < pineProb && (cell.biome == BiomeIndex::SNOW || cell.biome == BiomeIndex::MOUNTAINS)) {
        cell.material = Material::PINE_TREE;
        cell.pourcentageEvolution = pineProb;
        //nbPineTrees++;
    }
    else if (!isHighAltitude && oakProb > 0.01f && randVal < oakProb && (cell.biome ==  BiomeIndex::PLAINS || cell.biome == BiomeIndex::FOREST)) {
        cell.material = Material::OAK_TREE;
        cell.pourcentageEvolution = oakProb;
        //nbOakTrees++;
    }
    else if (grassProb > 0.01f && randVal < grassProb && cell.biome != BiomeIndex::DESERT && cell.biome != BiomeIndex::SNOW) {
        cell.material = Material::GRASS;
        cell.pourcentageEvolution = grassProb;
        //nbGrass++;
    }
    else {
        //nbNone++;
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
            sowASeed(cell);
            if(cell.material != Material::NONE) {
               cell.stage = Stage::STAGE_1;
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

float VegetationSystem::calculateRate(const Material &material, const float &temp, const float &hum, const float &light, const float &sediment) const {

    switch(material) {
        case Material::MOSS: return std::min({
                calculateSensivityToVegetation(hum, 0.40f, 0.90f),
                calculateSensivityToVegetation(light, 0.35f, 0.30f)
            }) * 0;
        case Material::CACTUS: return std::min({
                calculateSensivityToVegetation(temp, 0.20f, 0.85f),
                calculateSensivityToVegetation(hum, 0.15f, 0.10f),
                calculateSensivityToVegetation(light, 0.20f, 0.90f),
            });
        case Material::JUNGLE_TREE: return std::min({
                calculateSensivityToVegetation(temp, 0.7f, 0.4f),
                calculateSensivityToVegetation(hum, 0.8f, 0.3f),
            }) * sediment;

        case Material::PINE_TREE: return std::min({
                calculateSensivityToVegetation(temp, 0.25f, 0.3f),
                calculateSensivityToVegetation(hum, 0.55f, 0.4f),
            }) * sediment;

        case Material::OAK_TREE: return std::min({
                calculateSensivityToVegetation(temp, 0.55f, 0.3f),
                calculateSensivityToVegetation(hum, 0.70f, 0.4f),
                calculateSensivityToVegetation(light, 0.60f, 0.5f),
            }) * sediment;

        case Material::GRASS: return std::min({
                calculateSensivityToVegetation(temp, 0.40f, 0.50f),
                calculateSensivityToVegetation(hum, 0.40f, 0.40f),
            }) * sediment * 0;
        default: return 0.0f;
    }
}


void VegetationSystem::evolveVegetation(Map &map, float deltaTime) 
{
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());

    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            Cell &cell = map.getGrid().get(x, y);
            
            if (cell.material == Material::NONE && cell.pourcentageEvolution < 0.0f) {
                continue;
            }
            
            bool isFlora = (cell.material == Material::MOSS || 
                            cell.material == Material::CACTUS || 
                            cell.material == Material::JUNGLE_TREE || 
                            cell.material == Material::PINE_TREE || 
                            cell.material == Material::OAK_TREE || 
                            cell.material == Material::GRASS);
            
            if (!isFlora)
                continue; 

            float growthRate = calculateRate(cell.material, cell.temperature, cell.humidity, cell.light, cell.granular);



            if(cell.stage == Stage::STAGE_1) {
                cell.pourcentageEvolution = std::min(cell.pourcentageEvolution + growthRate * deltaTime * GROWTH_FACTOR, 1.0f);
            }
            
            if(cell.pourcentageEvolution >= 1.0f || cell.stage == Stage::STAGE_2) {

                cell.stage = Stage::STAGE_2;
                cell.pourcentageEvolution -= growthRate * deltaTime * DEATH_FACTOR;
                float pollenChance = POLINIZATION_CHANCE * deltaTime;
                if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < pollenChance) {
                    polenization(map, x, y, cell.material);
                }
                if(cell.pourcentageEvolution < 0.4f)
                    cell.stage = Stage::STAGE_3;
                
            }
            if (cell.stage == Stage::STAGE_3) {
                cell.pourcentageEvolution -= growthRate * deltaTime * DEATH_FACTOR;
                if(cell.pourcentageEvolution <= 0.0f) {
                    cell.material = Material::NONE;
                    cell.stage = Stage::STAGE_4;
                    cell.pourcentageEvolution = -1.0f;
                }
            }
            if(cell.stage == Stage::STAGE_4) {
                cell.pourcentageEvolution += 0.1;
                if(cell.pourcentageEvolution >= 0.0f) {
                    cell.stage = Stage::UNDEFINED;
                    cell.pourcentageEvolution = 0.0f;
                }
            }
        }
    }
}



void VegetationSystem::polenization(Map &map, int x, int y, Material material) {
    // TODO : 
    int radius = RADIUS.at(material);
    int minX = std::max(0, x - radius);
    int maxX = std::min(static_cast<int>(map.getWidth()) - 1, x + static_cast<int>(radius));
    int minY = std::max(0, y - radius);
    int maxY = std::min(static_cast<int>(map.getHeight()) - 1, y + static_cast<int>(radius));

    bool isSameSpeciesNearby = false;
    for(int j = minY; j <= maxY; ++j) {
        for(int i = minX; i <= maxX; ++i) {
            if(i == x && j == y) continue;

            if(map.getGrid().get(i, j).material == material) {
                isSameSpeciesNearby = true;
                break;
            }
        }
    }

    for(int j = minY; j <= maxY; ++j) {
        for(int i = minX; i <= maxX; ++i) {
            if(i == x && j == y) continue;

            Cell &targetCell = map.getGrid().get(i, j);


            if (targetCell.biome == BiomeIndex::OCEAN || targetCell.biome == BiomeIndex::GLACE || targetCell.biome == BiomeIndex::RIVER) {
                continue ;
            }

            if(targetCell.material == Material::NONE && isSameSpeciesNearby) {
                float survivalRate = calculateRate(material, targetCell.temperature, targetCell.humidity, targetCell.light, targetCell.granular);
    
                if (survivalRate > 0.01f && (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) < survivalRate) {
                    targetCell.material = material;
                    targetCell.stage = Stage::STAGE_1;
                    targetCell.pourcentageEvolution = 0.0f;
                }
            }
        }
    }

}

float VegetationSystem::calculateSensivityToVegetation( 
        float envValue, float optimalValue, float tolerance) const
{
    float diff = std::abs(envValue - optimalValue);
    return std::exp(-(std::pow(diff/tolerance, 2)));
}
