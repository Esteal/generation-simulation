#ifndef LIGHT_SYSTEM_H
#define LIGHT_SYSTEM_H

#include "ISimulationSystem.h"

class LightSystem : public ISimulationSystem {
private:
    int radius;
    int numDirections;

public:
    LightSystem(int radius = 10, int numDirections = 8) 
        : radius(radius), numDirections(numDirections) {}

    void process(Map& map, float deltaTime) override;
};

#endif