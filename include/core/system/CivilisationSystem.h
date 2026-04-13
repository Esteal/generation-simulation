#ifndef CIVILISATIONSYSTEM_H
#define CIVILISATIONSYSTEM_H
#include "ISimulationSystem.h"
#include "Faction.h"
#include <vector>

class CivilisationSystem : public ISimulationSystem {
private:

    // --- Paramètres d'équilibrage de la simulation ---
    const int MIN_DISTANCE = 20; // Écart minimum en nombre de cases entre deux "capitales"
    const int numberOfFactions = 5; // Nombre de civilisations à créer
    const float GROWTH_RATE = 0.05f;           // +5% de population par seconde
    const int CARRYING_CAPACITY_BASE = 1000;   // Population max sur une case d'attractivité 1.0
    const int MIN_MIGRANTS = 50;               // Nombre minimum d'habitants pour fonder une ville

    void initializeCivilizations(Map& map);
    void evolveCivilizations(Map& map, float deltaTime);
public:
    void process(Map& map, float deltaTime) override;

};
#endif