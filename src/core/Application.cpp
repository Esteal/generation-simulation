#include "Application.h"
#include "KeyboardHandler.h"
#include "system/civilisation/TerritorySystem.h"
#include <iostream>
#include <Chunk.h>

Application::Application(size_t width, size_t height, size_t mapWidth, size_t mapHeight, int seed) 
    : isRunning(true),
    window("Generation Simulation", width, height),
    textureManager(), 
    camera(),
    width(width), height(height),
    map(mapWidth, mapHeight), 
    worldSimulator(false), generator(seed)
{
    generator.generate(map);

    // algos de simulation ayant besoin de temps (deltaTime) qui s'écoule pour marquer le terrain
    worldSimulator.addSystem(std::make_unique<HydrolicErosionSystem>());
    worldSimulator.addSystem(std::make_unique<ThermalErosionSystem>());
    //std::cout << "Application de l'érosion hydraulique..." << std::endl;
    worldSimulator.update(map, 10.0f);
    
    generator.setBiome(map);
    // algos ayant besoin de temps (deltaTime) à 0 pour s'exécuter une seule fois au lancement
    worldSimulator.addSystem(std::make_unique<HydrologieSystem>());
    worldSimulator.addSystem(std::make_unique<LightSystem>());
    worldSimulator.addSystem(std::make_unique<MineralSystem>());
    worldSimulator.addSystem(std::make_unique<VegetationSystem>());
    worldSimulator.update(map, 0.0f);


    worldSimulator.removeSystem(worldSimulator.getSystem(0)); // Retire l'érosion hydraulique après la phase de génération initiale
    worldSimulator.removeSystem(worldSimulator.getSystem(0)); // Retire l'érosion thermique après la phase de génération initiale

    for(int i = 0; i < 20; ++i)
        worldSimulator.update(map, 1.0f);

    
    // algos de simulation qui n'initialise rien, modifie seulement le terrain
    worldSimulator.addSystem(std::make_unique<TerritorySystem>());
    worldSimulator.addSystem(std::make_unique<CivilisationSystem>(map));
    worldSimulator.addSystem(std::make_unique<AgricultureSystem>());

    textureManager.load("sprite2D", "spritesheet2D.png", window);
    textureManager.load("sprite2DIso", "spritesheet2DIso.png", window);
}


void Application::simulate(Uint32 &currentTime, Uint32 &lastUpdateTime)
{
    currentTime = SDL_GetTicks();
    if(currentTime - lastUpdateTime >= 100)
    {
        worldSimulator.update(map, 1);
        //generator.setBiome(map);
        //worldSimulator.update(map, 0.0f);
        lastUpdateTime = currentTime;
        ++age;
        cout << "Années écoulées : " << age << endl;
    }
}

void Application::run()
{
    int mouseX, mouseY;
    Uint32 lastUpdateTime = SDL_GetTicks();
    Uint32 currentTime;
    KeyboardHandler::displayControls();
    while(isRunning)
    {
        KeyboardHandler::keyboardInput(*this);
        window.clear();
        SDL_GetMouseState(&mouseX, &mouseY);

        switch (currentViewMode) {
            case ViewMode::ISOMETRIC:
                mapRendererIso.render2D(map, window, textureManager, camera, width, height, mouseX, mouseY, "sprite2DIso");
                break;
            case ViewMode::NORMAL_2D:
                mapRenderer.render2D(map, window, textureManager, camera, width, height, mouseX, mouseY, "sprite2D");
                break;
            case ViewMode::HEATMAP_HUMIDITY:
                mapRenderer.renderHeatmap(map, window, camera, width, height, HeatmapType::HUMIDITY);
                break;
            case ViewMode::HEATMAP_TEMPERATURE:
                mapRenderer.renderHeatmap(map, window, camera, width, height, HeatmapType::TEMPERATURE);
                break;
            case ViewMode::HEATMAP_ALTITUDE:
                mapRenderer.renderHeatmap(map, window, camera, width, height, HeatmapType::ALTITUDE);
                break;
            case ViewMode::HEATMAP_LIGHT:
                mapRenderer.renderHeatmap(map, window, camera, width, height, HeatmapType::LIGHT);
                break;
            case ViewMode::HEATMAP_VEGETATION:
                mapRenderer.renderHeatmap(map, window, camera, width, height, HeatmapType::VEGETATION);
                break;
            default:
                mapRendererIso.render2D(map, window, textureManager, camera, width, height, mouseX, mouseY, "sprite2DIso");
                break;
        }
        
        window.present();
        
        if(/*false*/!worldSimulator.isPause())
            simulate(currentTime, lastUpdateTime);
    }
}

bool Application::testRegression()
{
    std::cout << "[Test] Application (Integration globale)... ";
    
    try {
        Application app(100, 100, 10, 10, 55);
        
        Uint32 currentTime = 1000;
        Uint32 lastUpdateTime = 0;
        
        app.simulate(currentTime, lastUpdateTime);
        
        KeyboardHandler::keyboardInput(app);
        
        std::cout << "OK" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "\nErreur critique lors du test d'integration : " << e.what() << std::endl;
        return false;
    } catch (...) {
        std::cerr << "\nErreur inconnue lors du test d'integration." << std::endl;
        return false;
    }
}
