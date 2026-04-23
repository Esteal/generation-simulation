#ifndef VEGETATION_SYSTEM_H
#define VEGETATION_SYSTEM_H
#include "ISimulationSystem.h"
#include <unordered_map>

class VegetationSystem : public ISimulationSystem
{
private:
    float POLINIZATION_CHANCE; // 10% de chance de polinisation par seconde pour une cellule en stage 2
    float GROWTH_FACTOR;
    float DEATH_FACTOR;
    std::unordered_map<Material, int> RADIUS;
    
    void sowASeed(Cell &cell);
    void sowSeeds(Map &map);
    void polenization(Map &map, int x, int y, Material material);
    float calculateRate(const Material &material, const float &temp, const float &hum, 
                                          const float &light, const float &sediment) const;
    void evolveVegetation(Map &map, float deltaTime);
    float calculateSensivityToVegetation(float envValue, float tolerance, float optimalValue) const;
public:
    VegetationSystem();
    void process(Map& map, float deltaTime) override;


};

#endif