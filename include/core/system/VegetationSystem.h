#ifndef VEGETATION_SYSTEM_H
#define VEGETATION_SYSTEM_H
#include "ISimulationSystem.h"

class VegetationSystem : public ISimulationSystem
{
private:
    float talusAngle = 0.5;
    float minHumidity = 0.4;
    void sowSeeds(Map &map);
    float calculateSensivityToVegetation(float envValue, float tolerance, float optimalValue) const;
public:
    void process(Map& map, float deltaTime) override;


};

#endif