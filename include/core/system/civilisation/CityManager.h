#ifndef CITY_MANAGER_H
#define CITY_MANAGER_H

#include "IFactionSystem.h"

class CityManager : public IFactionSystem 
{
private:
    const int CARRYING_CAPACITY_BASE = 800;   // Population max sur une case d'attractivité 1.0
    const int TENSION_FACTOR = 5;             // Facteur d'augmentation de la tension
    void expandRadius(Faction &faction);
public:
    void processFaction(Map& map, Faction& faction, float deltaTime) override;
};

#endif