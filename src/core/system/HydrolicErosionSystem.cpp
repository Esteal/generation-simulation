#include "HydrolicErosionSystem.h"
#include <iostream>
#include <algorithm>
#include <cmath>

void HydrolicErosionSystem::process(Map& map, float deltaTime)
{
    size_t width = map.getWidth();
    size_t height = map.getHeight();
    size_t numCells = width * height;

    for (int iter = 0; iter < static_cast<int>(deltaTime); ++iter) 
    {
        vector<CellData> sortedCells = sortCellsByAltitude(map);
        vector<float> water(numCells, 1.0f);
        vector<int> receivers(numCells, -1); 
        calculateDrainage(map, sortedCells, water, receivers);
        applyIncision(map, sortedCells, water, receivers);
    }
}

vector<CellData> HydrolicErosionSystem::sortCellsByAltitude(const Map& map) const 
{
    size_t width = map.getWidth();
    size_t height = map.getHeight();
    vector<CellData> sortedCells;
    sortedCells.reserve(width * height);
    
    for (int y = 0; y < static_cast<int>(height); ++y) {
        for (int x = 0; x < static_cast<int>(width); ++x) {
            sortedCells.push_back({x, y, map.getAltitude(x, y)});
        }
    }

    sort(sortedCells.begin(), sortedCells.end(), [](const CellData& a, const CellData& b) {
        return a.height > b.height;
    });

    return sortedCells;
}

void HydrolicErosionSystem::calculateDrainage(const Map& map, const vector<CellData>& sortedCells, vector<float>& water, vector<int>& receivers) const 
{
    size_t width = map.getWidth();
    size_t height = map.getHeight();

    int dx[] = {-1, 1, 0, 0, -1, 1, -1, 1};
    int dy[] = {0, 0, -1, 1, -1, -1, 1, 1};

    for (const auto& c : sortedCells) {
        int cx = c.x;
        int cy = c.y;
        size_t cIndex = static_cast<size_t>(cy) * width + static_cast<size_t>(cx);
        float currentHeight = map.getAltitude(cx, cy);

        int lowestX = cx;
        int lowestY = cy;
        float lowestHeight = currentHeight;

        for (int i = 0; i < 8; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (nx >= 0 && nx < static_cast<int>(width) && ny >= 0 && ny < static_cast<int>(height)) {
                float nHeight = map.getAltitude(nx, ny);
                if (nHeight < lowestHeight) {
                    lowestHeight = nHeight;
                    lowestX = nx;
                    lowestY = ny;
                }
            }
        }

        if (lowestX != cx || lowestY != cy) {
            size_t lowestIndex = static_cast<size_t>(lowestY) * width + static_cast<size_t>(lowestX);
            water[lowestIndex] += water[cIndex];
            receivers[cIndex] = static_cast<int>(lowestIndex); 
        }
    }
}

void HydrolicErosionSystem::applyIncision(Map& map, const vector<CellData>& sortedCells, const vector<float>& water, const vector<int>& receivers) const 
{
    size_t width = map.getWidth();

    for (const auto& c : sortedCells) {
        int cx = c.x;
        int cy = c.y;
        size_t cIndex = static_cast<size_t>(cy) * width + static_cast<size_t>(cx);
        
        int receiverIndex = receivers[cIndex];
        if (receiverIndex == -1 || map.getAltitude(cx, cy) <= seaLevel) continue;   

        int rx = receiverIndex % static_cast<int>(width);
        int ry = receiverIndex / static_cast<int>(width);

        Cell& currentCell = map.getGrid().get(cx, cy);
        // Cell& receiverCell = map.getGrid().get(rx, ry);
        
        float slope = map.getAltitude(cx, cy) - map.getAltitude(rx, ry);
        
        float A = water[cIndex] * 0.05f; 
        float S = slope;

        float streamPower = K * pow(A, m) * pow(S, n);

        float maxErosion = slope * 0.9f;
        float erodeAmount = min(streamPower, maxErosion);

        float granularErosion = min(currentCell.granular, erodeAmount);
        currentCell.granular -= granularErosion;
        erodeAmount -= granularErosion;

        currentCell.bedrock -= erodeAmount;
    }
}

bool HydrolicErosionSystem::testRegression() {
    std::cout << "[Test] HydrolicErosionSystem... ";
    
    Map m(5, 5);
    // On crée une pente descendante de gauche (x=0) vers la droite (x=4)
    for(size_t x = 0; x < 5; ++x) {
        for(size_t y = 0; y < 5; ++y) {
            m.getGrid().get(x, y).bedrock = (float)(10 - x); // Altitude diminue avec X
            m.getGrid().get(x, y).granular = 1.0f; // Couche de terre meuble
        }
    }
    
    float altAvant = m.getAltitude(2, 2); // Point au milieu de la pente
    
    HydrolicErosionSystem sys;
    
    for(int i = 0; i < 10; ++i) {
        sys.process(m, 1.0f);
    }
    
    float altApres = m.getAltitude(2, 2);
    
    if (altApres >= altAvant) {
        std::cerr << "Erreur: L'eau n'a pas erode la pente." << std::endl;
        return false;
    }
    
    std::cout << "OK" << std::endl;
    return true;
}