#ifndef AGRICULTURESYSTEM_H
#define AGRICULTURESYSTEM_H
#include "ISimulationSystem.h"

class AgricultureSystem : public ISimulationSystem {
private:
    const float WHEAT_GROWTH_SPEED = 0.3f; // Vitesse de croissance du blé (0.9f correspond à une croissance complète en ~1.1 secondes dans des conditions optimales)
public:
    void process(Map& map, float deltaTime) override;

};

#endif