#include "system/HydrologieSystem.h"
#include <algorithm>

HydrologieSystem::HydrologieSystem(float riverThreshold) 
    : riverThreshold(riverThreshold) {}

void HydrologieSystem::process(Map& map, float deltaTime) 
{
    if(deltaTime != 0.0f) return;
    int width = map.getWidth();
    int height = map.getHeight();

    vector<float> waterMap(width * height, 0.0f);
    vector<TerrainCell> landCells;
    landCells.reserve(width * height);
    
    initializeWaterMapAndLandCells(map, waterMap, landCells);
    sortLandCellsByAltitude(landCells);
    routeWater(map, waterMap, landCells);
    carveRivers(map, waterMap);
    diffuseHumidity(map);
}    

void HydrologieSystem::initializeWaterMapAndLandCells(const Map& map, vector<float>& waterMap, vector<TerrainCell>& landCells) const
{
    size_t width = map.getWidth();
    size_t height = map.getHeight();

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            const Cell& cell = map.getGrid().get(x, y);
            int index = (int)y * (int)width + (int)x;

            if (cell.biome != BiomeIndex::OCEAN && cell.biome != BiomeIndex::BEACH && cell.biome!= BiomeIndex::GLACE) {
                waterMap[index] = 1.0f;
                landCells.push_back({x, y, cell.bedrock + cell.granular});
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
                float neighborAlt = map.getAltitude(nx, ny);
                
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
    size_t width = map.getWidth();
    size_t height = map.getHeight();

    vector<bool> riverMask(width * height, false);

    for (size_t y = 1; y < height - 1; ++y) {
        for (size_t x = 1; x < width - 1; ++x) {
            int index = (int)y * (int)width + (int)x;
            Cell& cell = map.getGrid().get((int)x, (int)y);

            if (waterMap[index] >= riverThreshold) {
                if (cell.biome != BiomeIndex::OCEAN && cell.biome != BiomeIndex::BEACH && cell.biome != BiomeIndex::GLACE) {
                    riverMask[index] = true;
                }
            }
        }
    }

    for (size_t y = 1; y < height - 1; ++y) {
        for (size_t x = 1; x < width - 1; ++x) {
            // Cell& cell = map.getGrid().get(x, y);
            int index = (int)y * (int)width + (int)x;
            if (riverMask[index]) {
                bool isConnected = false;

                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) continue;

                        int neighborIndex = (y + dy) * width + (x + dx);
                        BiomeIndex neighborBiome = map.getGrid().get(x + dx, y + dy).biome;
                        
                        // On considère que la rivière est connectée si elle touche une cellule déjà marquée comme rivière ou un biome d'eau (océan, plage, glace)
                        if (riverMask[neighborIndex] || 
                            neighborBiome == BiomeIndex::OCEAN || 
                            neighborBiome == BiomeIndex::BEACH || 
                            neighborBiome == BiomeIndex::GLACE) {
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

    for (size_t y = 1; y < height - 1; ++y) {
        for (size_t x = 1; x < width - 1; ++x) {
            int index = (int)y * (int)width + (int)x;
            
            if (riverMask[index]) {
                Cell& cell = map.getGrid().get((int)x, (int)y);
                cell.biome = BiomeIndex::RIVER;
                cell.humidity = 1.0f;
            }
        }
    }
}


void HydrologieSystem::diffuseHumidity(Map& map) const
{
    // Distance Transform
    int width = map.getWidth();
    int height = map.getHeight();

    // Passe 1 : Balayage de Haut en Bas, de Gauche à Droite
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float maxHum = map.getGrid().get(x, y).humidity;
            
            // voisin de Gauche
            if (x > 0) 
                maxHum = std::max(maxHum, map.getGrid().get(x - 1, y).humidity - dropoff);
            // voisin du Haut
            if (y > 0) 
                maxHum = std::max(maxHum, map.getGrid().get(x, y - 1).humidity - dropoff);
            // voisin Haut-Gauche 
            if (x > 0 && y > 0) 
                maxHum = std::max(maxHum, map.getGrid().get(x - 1, y - 1).humidity - diagDropoff);
            // voisin Haut-Droite
            if (x < width - 1 && y > 0) 
                maxHum = std::max(maxHum, map.getGrid().get(x + 1, y - 1).humidity - diagDropoff);
            map.getGrid().get(x, y).humidity = maxHum;
        }
    }

    // Passe 2 : Balayage de Bas en Haut, de Droite à Gauche
    for (int y = height - 1; y >= 0; --y) {
        for (int x = width - 1; x >= 0; --x) {
            float maxHum = map.getGrid().get(x, y).humidity;
            
            // voisin de Droite
            if (x < width - 1) 
                maxHum = std::max(maxHum, map.getGrid().get(x + 1, y).humidity - dropoff);
            // voisin du Bas
            if (y < height - 1) 
                maxHum = std::max(maxHum, map.getGrid().get(x, y + 1).humidity - dropoff);
            // voisin Bas-Droite 
            if (x < width - 1 && y < height - 1) 
                maxHum = std::max(maxHum, map.getGrid().get(x + 1, y + 1).humidity - diagDropoff);

            // voisin Bas-Gauche
            if (x > 0 && y < height - 1) 
                maxHum = std::max(maxHum, map.getGrid().get(x - 1, y + 1).humidity - diagDropoff);
            map.getGrid().get(x, y).humidity = maxHum;
        }
    }
}


bool HydrologieSystem::testRegression() {
    std::cout << "[Test] HydrologieSystem... ";
    
    Map m(3, 3);
    for(size_t x = 0; x < 3; ++x) {
        for(size_t y = 0; y < 3; ++y) {
            m.getGrid().get(x, y).bedrock = 10.0f;
            m.getGrid().get(x, y).granular = 0.0f;
            m.getGrid().get(x, y).biome = BiomeIndex::PLAINS;
        }
    }
    
    m.getGrid().get(1, 1).bedrock = 2.0f;
    
    BiomeIndex biomeAvant = m.getGrid().get(1, 1).biome;
    m.getGrid().get(0, 1).biome = BiomeIndex::OCEAN;

    HydrologieSystem sys(2.0f);
    sys.process(m, 0.0f);
    
    BiomeIndex biomeApres = m.getGrid().get(1, 1).biome;
    
    if (biomeAvant == biomeApres) {
        std::cerr << "Erreur: L'accumulation d'eau n'a pas modifie le biome de la cuvette." << std::endl;
        return false;
    }
    
    std::cout << "OK" << std::endl;
    return true ;
}