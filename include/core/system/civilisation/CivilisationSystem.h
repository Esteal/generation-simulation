#ifndef CIVILISATIONSYSTEM_H
#define CIVILISATIONSYSTEM_H
#include "ISimulationSystem.h"
#include "IFactionSystem.h"
#include "Faction.h"
#include <vector>
#include <memory>

class CivilisationSystem : public ISimulationSystem {
private:
    std::vector<std::unique_ptr<IFactionSystem>> subSystems;
public:
    CivilisationSystem(Map& map);
    void process(Map& map, float deltaTime) override;
    void displayAveragePopulationPerCity(const Map& map);

};

#endif