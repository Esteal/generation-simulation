#ifndef TECH_MANAGER_H
#define TECH_MANAGER_H

#include "IFactionSystem.h"

class TechManager : public IFactionSystem {
private:
    int COST_WOOD_BRONZE;
    int REQ_POP_BRONZE;
    int REQ_COLONIES_BRONZE;

    int COST_BRONZE_IRON;
    int COST_COAL_IRON;
    int REQ_POP_IRON;
    int REQ_COLONIES_IRON;

    void applyTechBonus(Faction& faction);

public:
    TechManager();
    void processFaction(Map& map, Faction& faction, float deltaTime) override;
};

#endif