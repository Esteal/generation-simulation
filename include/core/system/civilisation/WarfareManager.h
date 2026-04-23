#ifndef WARFARE_MANAGER_H
#define WARFARE_MANAGER_H

#include "IFactionSystem.h"

class WarfareManager : public IFactionSystem {
private:
    // --- Paramètres d'équilibrage de la guerre ---
    float BASE_CASUALTY_RATE;
    int COMBAT_RANGE_BONUS;      // Bonus de portée pour attaquer

    // Récupère le multiplicateur de force selon l'âge
    float getTechMultiplier(TechLevel techLevel) const;

public:
    WarfareManager();
    void processFaction(Map& map, Faction& faction, float deltaTime) override;
};

#endif