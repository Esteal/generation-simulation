#ifndef WARFARE_MANAGER_H
#define WARFARE_MANAGER_H

#include "IFactionSystem.h"

class WarfareManager : public IFactionSystem {
private:
    // --- Paramètres d'équilibrage de la guerre ---
    const float BASE_CASUALTY_RATE = 0.2f; // % de morts de base par itération
    const int COMBAT_RANGE_BONUS = 1;      // Bonus de portée pour attaquer

    // Récupère le multiplicateur de force selon l'âge
    float getTechMultiplier(TechLevel techLevel) const;

public:
    void processFaction(Map& map, Faction& faction, float deltaTime) override;
};

#endif