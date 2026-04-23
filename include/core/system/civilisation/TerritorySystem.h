#ifndef TERRITORY_SYSTEM_H
#define TERRITORY_SYSTEM_H

#include "ISimulationSystem.h"

class TerritorySystem : public ISimulationSystem {
private:
    std::vector<std::pair<int, int>> claimedCells;
    int TENSION_FACTOR;             // Facteur d'augmentation de la tension

public:
    TerritorySystem();
    void process(Map& map, float deltaTime) override;
};

#endif