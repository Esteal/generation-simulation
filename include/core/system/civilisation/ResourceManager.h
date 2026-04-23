#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include "IFactionSystem.h"

class ResourceManager : public IFactionSystem {
private:
    // La vitesse à laquelle la ressource est prélevée (ex: 0.2 = 20% par itération)
    float TREE_CHOP_RATE; 
    float MINING_RATE;    

    // La quantité de ressource brute que contient une case pleine à 100% (1.0f)
    float MAX_WOOD_PER_TREE;
    float MAX_ORE_PER_VEIN;

public:
    ResourceManager();
    void processFaction(Map& map, Faction& faction, float deltaTime) override;
};
#endif