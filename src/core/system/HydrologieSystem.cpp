#include "system/HydrologieSystem.h"

HydrologieSystem::HydrologieSystem(float riverThreshold) 
    : riverThreshold(riverThreshold) {}

void HydrologieSystem::process(Map& map, float deltaTime) 
{
    int width = map.getWidth();
    int height = map.getHeight();

    vector<float> waterMap(width * height, 0.0f);
    vector<TerrainCell> landCells;
    landCells.reserve(width * height);
    
    initializeWaterMapAndLandCells(map, waterMap, landCells);
    sortLandCellsByAltitude(landCells);
    routeWater(map, waterMap, landCells);
    carveRivers(map, waterMap);
}    

void HydrologieSystem::initializeWaterMapAndLandCells(const Map& map, vector<float>& waterMap, vector<TerrainCell>& landCells) const
{
    int width = map.getWidth();
    int height = map.getHeight();

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            const Cell& cell = map.getGrid().get(x, y);
            int index = y * width + x;

            if (cell.biome != BiomeIndex::OCEAN && cell.biome != BiomeIndex::BEACH) {
                waterMap[index] = 1.0f;
                landCells.push_back({x, y, cell.altitude});
            }
        }
    }
}

void HydrologieSystem::sortLandCellsByAltitude(vector<TerrainCell>& landCells) const
{
    sort(landCells.begin(), landCells.end(), [](const TerrainCell& a, const TerrainCell& b) {
        return a.alt > b.alt; 
    });
}

void HydrologieSystem::routeWater(const Map& map, vector<float>& waterMap, const vector<TerrainCell>& landCells) const
{
    int width = map.getWidth();

    for (const TerrainCell& current : landCells) {
        int currentIndex = current.y * width + current.x;
        
        int lowestNeighborIndex = findLowestNeighborIndex(map, current.x, current.y, current.alt);

        if (lowestNeighborIndex != -1) {
            waterMap[lowestNeighborIndex] += waterMap[currentIndex];
        }
    }
}

int HydrologieSystem::findLowestNeighborIndex(const Map& map, int cx, int cy, float currentAlt) const
{
    int width = map.getWidth();
    int height = map.getHeight();

    float minAlt = currentAlt;
    int lowestIndex = -1;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;

            int nx = cx + dx;
            int ny = cy + dy;
            
            // Pour éviter de sortir de la grille
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                float neighborAlt = map.getGrid().get(nx, ny).altitude;
                
                if (neighborAlt < minAlt) {
                    minAlt = neighborAlt;
                    lowestIndex = ny * width + nx;
                }
            }
        }
    }

    return lowestIndex; 
}

void HydrologieSystem::carveRivers(Map& map, const vector<float>& waterMap) const
{
    int width = map.getWidth();
    int height = map.getHeight();

    vector<bool> riverMask(width * height, false);

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int index = y * width + x;
            Cell& cell = map.getGrid().get(x, y);

            if (waterMap[index] >= riverThreshold) {
                if (cell.biome != BiomeIndex::OCEAN && cell.biome != BiomeIndex::BEACH) {
                    riverMask[index] = true;
                }
            }
        }
    }

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            Cell& cell = map.getGrid().get(x, y);
            int index = y * width + x;
            if (riverMask[index]) {
                bool isConnected = false;

                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) continue;

                        int neighborIndex = (y + dy) * width + (x + dx);
                        BiomeIndex neighborBiome = map.getGrid().get(x + dx, y + dy).biome;
                        
                        if (riverMask[neighborIndex] || 
                            neighborBiome == BiomeIndex::OCEAN || 
                            neighborBiome == BiomeIndex::BEACH) {
                            isConnected = true;
                            break; 
                        }
                    }
                    if (isConnected) break;
                }

                if (!isConnected) {
                    riverMask[index] = false;
                }
            }
        }
    }

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int index = y * width + x;
            
            if (riverMask[index]) {
                map.getGrid().get(x, y).biome = BiomeIndex::RIVER;
            }
        }
    }
}