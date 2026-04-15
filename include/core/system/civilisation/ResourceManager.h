#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include "IFactionSystem.h"

class ResourceManager : public IFactionSystem {
private:
    // La vitesse à laquelle la ressource est prélevée (ex: 0.2 = 20% par itération)
    const float TREE_CHOP_RATE = 0.5f; 
    const float MINING_RATE = 0.2f;    

    // La quantité de ressource brute que contient une case pleine à 100% (1.0f)
    const float MAX_WOOD_PER_TREE = 50.0f;
    const float MAX_ORE_PER_VEIN = 300.0f;

public:
    void processFaction(Map& map, Faction& faction, float deltaTime) override;
};
#endif