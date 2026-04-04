#include "WorldSimulator.h"
#include <iostream>
#include <algorithm>

void WorldSimulator::addSystem(ISimulationSystem* system) {
    systems.push_back(system);
}

void WorldSimulator::removeSystem(ISimulationSystem* system) {
    systems.erase(std::remove(systems.begin(), systems.end(), system), systems.end());
}

void WorldSimulator::setSpeed(float speed) {
    simulationSpeed = speed;
}

void WorldSimulator::setPause(bool paused) {
    isPaused = paused;
}

void WorldSimulator::update(Map& map, float delta_time) {
    if (!isPaused) {
        for (ISimulationSystem* system : systems) {
            system->process(map, delta_time);
        }
    }
}

// Classe tampon pour vérifier si la méthode process est bien appelée
class MockSystem : public ISimulationSystem {
public:
    int callCount = 0;
    void process(Map& map, float deltaTime) override { callCount++; }
};

bool WorldSimulator::testRegression() {
    std::cout << "[Test] WorldSimulator... ";
    
    WorldSimulator sim;
    
    // Test pause par défaut
    if (sim.isPause()) {
        std::cerr << "Erreur: La simulation ne devrait pas etre en pause au demarrage." << std::endl;
        return false;
    }
    
    MockSystem* mock = new MockSystem();
    sim.addSystem(mock);
    Map dummyMap(10, 10);
    
    // Test exécution normale
    sim.update(dummyMap, 1.0f);
    if (mock->callCount != 1) {
        std::cerr << "Erreur: Le systeme n'a pas ete mis a jour." << std::endl;
        return false;
    }
    
    // Test pause
    sim.setPause(true);
    sim.update(dummyMap, 1.0f);
    if (mock->callCount != 1) { // Ne doit pas s'incrémenter
        std::cerr << "Erreur: Le systeme a ete mis a jour malgre la pause." << std::endl;
        return false;
    }
    
    std::cout << "OK" << std::endl;
    return true;
}