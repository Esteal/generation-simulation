#ifndef IS_SIMULATION_SYSTEM_H
#define IS_SIMULATION_SYSTEM_H
#include "Map.h"

class ISimulationSystem {
public:
    virtual ~ISimulationSystem() = default;
    virtual void process(Map& map, float deltaTime) = 0;
};

#endif