#ifndef TECH_MANAGER_H
#define TECH_MANAGER_H

#include "IFactionSystem.h"

class TechManager : public IFactionSystem {
private:
    const int COST_WOOD_BRONZE = 500;
    const int REQ_POP_BRONZE = 1000;
    const int REQ_COLONIES_BRONZE = 3;

    const int COST_BRONZE_IRON = 800;
    const int COST_COAL_IRON = 1000;
    const int REQ_POP_IRON = 3000;
    const int REQ_COLONIES_IRON = 6;

    void applyTechBonus(Faction& faction);

public:
    void processFaction(Map& map, Faction& faction, float deltaTime) override;
};

#endif // TECH_MANAGER_H