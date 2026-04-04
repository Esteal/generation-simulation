#include "Camera2D.h"
#include "Map.h"
#include "MapGenerator.h"
#include "WorldSimulator.h"
#include "ISimulationSystem.h"
#include "ThermalErosionSystem.h"
#include "HydrolicErosionSystem.h"
#include "HydrologieSystem.h"
#include "Window.h"
#include "TextureManager.h"
#include "MapRenderer.h"
#include "MapRenderer2DIso.h"
#include "Application.h"
#include "Grid.h"

#include <iostream>

int main()
{
    std::cout << "=== DEBUT DES TESTS DE REGRESSION ===" << std::endl;
    std::cout << "Ne pas s'inquiter si on voit des messages d'erreur concernant des fichiers manquants ou des IDs inconnus, cela fait partie des tests pour voir si le programme crash ou non." << std::endl;
    int testsReussis = 0;
    int testsEchoues = 0;

    auto runTest = [&](bool (*testFunc)(), const std::string& name) {
        if (testFunc()) {
            testsReussis++;
        } else {
            std::cerr << ">>> ECHEC CRITIQUE : " << name << std::endl;
            testsEchoues++;
        }
    };

    // Core tests
    runTest(Map::testRegression, "Map");
    runTest(MapGenerator::testRegression, "MapGenerator");
    runTest(WorldSimulator::testRegression, "WorldSimulator");
    runTest(ISimulationSystem::testRegression, "ISimulationSystem");
    runTest(Grid<int>::testRegression, "Grid");
    
    // Simulation systems tests
    runTest(ThermalErosionSystem::testRegression, "ThermalErosionSystem");
    runTest(HydrolicErosionSystem::testRegression, "HydrolicErosionSystem");
    runTest(HydrologieSystem::testRegression, "HydrologieSystem");
    // runTest(VegetationSystem::testRegression, "VegetationSystem");
    // runTest(LightSystem::testRegression, "LightSystem");
    
    // Interface tests
    runTest(Camera2D::testRegression, "Camera2D");
    runTest(Window::testRegression, "Window");
    runTest(TextureManager::testRegression, "TextureManager");
    runTest(MapRenderer::testRegression, "MapRenderer");
    runTest(MapRenderer2DIso::testRegression, "MapRenderer2DIso");
    
    // Application test d'intégration
    runTest(Application::testRegression, "Application");

    std::cout << "=== BILAN ===" << std::endl;
    std::cout << "Reussis : " << testsReussis << " | Echoues : " << testsEchoues << std::endl;

    return 0; 
}