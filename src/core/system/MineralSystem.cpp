#include "MineralSystem.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>

MineralSystem::MineralSystem() {
    noiseGen.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noiseGen.SetFrequency(NOISE_FREQUENCY);
}

void MineralSystem::process(Map& map, float deltaTime) {
    if (deltaTime > 0.0f) return;

    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            
            float baseRand = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            
            if (baseRand < BASE_SPAWN_CHANCE) {
                
                Cell& cell = map.getGrid().get(x, y);
                Material chosenMineral = determineBestMineral(cell, x, y);

                if (chosenMineral != Material::NONE) {
                    spreadVein(map, x, y, chosenMineral, VEIN_INITIAL_CHANCE);
                }
            }
        }
    }
    displayMineralCount(map);
}

Material MineralSystem::determineBestMineral(const Cell& cell, float x, float y) const {

    if (cell.biome == BiomeIndex::OCEAN /*|| cell.biome == BiomeIndex::RIVER*/) return Material::NONE;

    float altitude = cell.bedrock + cell.granular;

    float coalBiomeFactor = (cell.biome == BiomeIndex::JUNGLE || cell.biome == BiomeIndex::FOREST) ? COAL_BIOME_BONUS : COAL_BIOME_PENALTY;
    float bronzeBiomeFactor = (cell.biome == BiomeIndex::PLAINS) ? BRONZE_BIOME_BONUS : BRONZE_BIOME_PENALTY;
    float ironBiomeFactor = (cell.biome == BiomeIndex::MOUNTAINS) ? IRON_BIOME_BONUS : IRON_BIOME_PENALTY;
    float goldBiomeFactor = (cell.biome == BiomeIndex::DESERT) ? GOLD_BIOME_BONUS : GOLD_BIOME_PENALTY;    
    
    float coalScore = calculateScore(altitude, COAL_MIN_ALTITUDE, COAL_MAX_ALTITUDE, COAL_RARITY, coalBiomeFactor, x, y, OFFSET_COAL);
    float bronzeScore = calculateScore(altitude, BRONZE_MIN_ALTITUDE, BRONZE_MAX_ALTITUDE, BRONZE_RARITY, bronzeBiomeFactor, x, y, OFFSET_BRONZE);
    float ironScore = calculateScore(altitude, IRON_MIN_ALTITUDE, IRON_MAX_ALTITUDE, IRON_RARITY, ironBiomeFactor, x, y, OFFSET_IRON);
    float goldScore = calculateScore(altitude, GOLD_MIN_ALTITUDE, GOLD_MAX_ALTITUDE, GOLD_RARITY, goldBiomeFactor, x, y, OFFSET_GOLD);

    if (ironScore == 0 && coalScore == 0 && goldScore == 0 && bronzeScore == 0) return Material::NONE;
    if (goldScore > 0.0f) return Material::GOLD_ORE;
    if (ironScore > 0.0f) return Material::IRON_ORE;
    if (bronzeScore > 0.0f) return Material::BRONZE_ORE;
    return Material::COAL_ORE;
}

float MineralSystem::calculateScore(float altitude, float minAltitude, float maxAltitude, 
                                    float rarity, float biomeFactor, float x, float y, float noiseOffset) const {
    if (altitude <= minAltitude || altitude >= maxAltitude) {
        return 0.0f;
    }

    float noiseValue = (noiseGen.GetNoise(x + noiseOffset, y + noiseOffset) + 1.0f) * 0.5f;

    if (noiseValue > (1.0f - (rarity * biomeFactor))) {
        return noiseValue * biomeFactor;
    }
    return 0.0f;
}

void MineralSystem::spreadVein(Map& map, int x, int y, Material mineral, float spawnChance) {
    if (spawnChance <= 0.0f) return;

    if (x < 0 || x >= static_cast<int>(map.getWidth()) || y < 0 || y >= static_cast<int>(map.getHeight())) return;

    Cell& cell = map.getGrid().get(x, y);

    if (cell.material != Material::NONE /*|| cell.biome == BiomeIndex::OCEAN*/) return;

    float randVal = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    if (randVal > spawnChance) return; // Le filon s'arrête dans cette direction

    cell.material = mineral;
    cell.pourcentageEvolution = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); 

    float nextChance = spawnChance - VEIN_DECAY_RATE; 

    spreadVein(map, x + 1, y, mineral, nextChance);
    spreadVein(map, x - 1, y, mineral, nextChance);
    spreadVein(map, x, y + 1, mineral, nextChance);
    spreadVein(map, x, y - 1, mineral, nextChance);
}

void MineralSystem::displayMineralCount(const Map& map) const {
    int coalCount = 0;
    int ironCount = 0;
    int goldCount = 0;
    int bronzeCount = 0;

    const Grid<Cell>& grid = map.getGrid();
    for (size_t y = 0; y < map.getHeight(); ++y) {
        for (size_t x = 0; x < map.getWidth(); ++x) {
            const Cell& cell = grid.get(x, y);

            switch (cell.material) {
                case Material::COAL_ORE:
                    coalCount++;
                    break;
                case Material::IRON_ORE:
                    ironCount++;
                    break;
                case Material::GOLD_ORE:
                    goldCount++;
                    break;
                case Material::BRONZE_ORE:
                    bronzeCount++;
                    break;
                default:
                    break;
            }
        }
    }

    std::cout << "=== RESSOURCES MINIÈRES ===" << std::endl;
    std::cout << "Charbon (COAL_ORE):   " << coalCount << std::endl;
    std::cout << "Fer (IRON_ORE):       " << ironCount << std::endl;
    std::cout << "Or (GOLD_ORE):        " << goldCount << std::endl;
    std::cout << "Bronze (BRONZE_ORE):  " << bronzeCount << std::endl;
    std::cout << "Total:                " << (coalCount + ironCount + goldCount + bronzeCount) << std::endl;
    std::cout << "=========================" << std::endl;
}