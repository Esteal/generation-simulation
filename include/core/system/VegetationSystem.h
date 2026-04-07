#ifndef VEGETATION_SYSTEM_H
#define VEGETATION_SYSTEM_H
#include "ISimulationSystem.h"
#include <unordered_map>

class VegetationSystem : public ISimulationSystem
{
private:
    const float talusAngle = 0.5;
    std::unordered_map<Material, int> RADIUS = {
        {Material::GRASS, 4},
        {Material::MOSS, 8},
        {Material::CACTUS, 2},
        {Material::PINE_TREE, 3},
        {Material::OAK_TREE, 4},
        {Material::JUNGLE_TREE, 5}
     };
    void sowASeed(Cell &cell);
    void sowSeeds(Map &map);
    void polenization(Map &map, int x, int y, Material material);
    float calculateRate(const Material &material, const float &temp, const float &hum, 
                                          const float &light, const float &sediment) const;
    void evolveVegetation(Map &map, float deltaTime);
    float calculateSensivityToVegetation(float envValue, float tolerance, float optimalValue) const;
public:
    void process(Map& map, float deltaTime) override;


};

#endif