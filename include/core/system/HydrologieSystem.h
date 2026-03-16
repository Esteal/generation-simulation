#ifndef HYDROLOGIESYSTEM_H
#define HYDROLOGIESYSTEM_H

#include "ISimulationSystem.h"
#include <algorithm>
using namespace std;

struct TerrainCell
{
    size_t x;
    size_t y;
    float alt;
};


class HydrologieSystem : public ISimulationSystem 
{
private :
    float riverThreshold;
    void initializeWaterMapAndLandCells(const Map& map, vector<float>& waterMap, vector<TerrainCell>& landCells) const;
    void sortLandCellsByAltitude(vector<TerrainCell>& landCells) const;
    void routeWater(const Map& map, vector<float>& waterMap, const vector<TerrainCell>& landCells) const;
    void carveRivers(Map& map, const vector<float>& waterMap) const;
    int findLowestNeighborIndex(const Map& map, int cx, int cy, float currentAlt) const;

public:
    HydrologieSystem(float riverThreshold = 40.0f);
    void process(Map& map, float deltaTime) override;
};

#endif