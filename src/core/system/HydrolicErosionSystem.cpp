#include "HydrolicErosionSystem.h"
#include <iostream>
#include <algorithm>
#include <cmath>

void HydrolicErosionSystem::process(Map& map, float deltaTime)
{
    size_t width = map.getWidth();
    size_t height = map.getHeight();
    size_t numCells = width * height;
    
    int totalIterations = static_cast<int>(deltaTime);
    if (totalIterations <= 0) return;

    // On prépare nos buffers d'eau et de réception si nécessaire
    if (water.size() != numCells) {
        water.resize(numCells);
        receivers.resize(numCells);
    }

    updateSortedCells(map);

    for (int iter = 0; iter < totalIterations; ++iter) 
    {
        std::fill(water.begin(), water.end(), 1.0f);
        std::fill(receivers.begin(), receivers.end(), -1);

        calculateDrainage(map, sortedCells, water, receivers);
        applyIncision(map, sortedCells, water, receivers);
    }
}


void HydrolicErosionSystem::updateSortedCells(const Map& map) 
{
    size_t width = map.getWidth();
    size_t height = map.getHeight();
    size_t numCells = width * height;

    // On ne redimensionne le vecteur que si la taille de la carte a changé (rare)
    if (sortedCells.size() != numCells) {
        sortedCells.resize(numCells);
    }
    
    size_t i = 0;
    for (int y = 0; y < static_cast<int>(height); ++y) {
        for (int x = 0; x < static_cast<int>(width); ++x) {
            sortedCells[i++] = {x, y, map.getAltitude(x, y)};
        }
    }

    sort(sortedCells.begin(), sortedCells.end(), [](const CellData& a, const CellData& b) {
        return a.height > b.height;
    });
}

void HydrolicErosionSystem::calculateDrainage(const Map& map, const vector<CellData>& sortedCells, vector<float>& water, vector<int>& receivers) const 
{
    size_t width = map.getWidth();

    for (const auto& c : sortedCells) {
        int cx = c.x;
        int cy = c.y;
        size_t cIndex = static_cast<size_t>(cy) * width + static_cast<size_t>(cx);

        int lowestIndex = getLowestNeighborIndex(map, cx, cy);

        // Si lowestIndex est différent de -1, cela signifie que l'eau peut s'écouler
        if (lowestIndex != -1) {
            water[lowestIndex] += water[cIndex];
            receivers[cIndex] = lowestIndex; 
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