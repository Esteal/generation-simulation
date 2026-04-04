
#include "KeyboardHandler.h"
#include "Application.h"
#include <iostream>


void KeyboardHandler::keyboardInput(Application& app)   
{
    SDL_Event events;
    int mouseX, mouseY;
    while (SDL_PollEvent(&events)) {
        if (events.type == SDL_QUIT) {
            app.isRunning = false; 
        } else if (events.type == SDL_KEYDOWN) {
            if (events.key.keysym.sym == SDLK_ESCAPE) {
                app.isRunning = false; 
            }
            SDL_GetMouseState(&mouseX, &mouseY);
            if (events.key.keysym.sym == SDLK_x) {
                app.camera.zoomAt(app.camera.getZoom() + 1.0f, mouseX, mouseY);
            }
            else if (events.key.keysym.sym == SDLK_c) {
                app.camera.zoomAt(app.camera.getZoom() - 1.0f, mouseX, mouseY);
            }
            else if (events.key.keysym.sym == SDLK_p)
            {
                app.worldSimulator.setPause(!app.worldSimulator.isPause());
                std::cout << "mise en pause du monde : " << app.worldSimulator.isPause() << std::endl;
            }
            else if (events.key.keysym.sym == SDLK_t)
            {
                app.textMapRenderer.printBiomeMap(app.map);
            }
            else if (events.key.keysym.sym == SDLK_b)
            {
                switchViewMode(app, true);
            }
            else if (events.key.keysym.sym == SDLK_n)
            {   switchViewMode(app, false);
            }
        }
    }
    const Uint8* state = SDL_GetKeyboardState(NULL);
    
    
    if (state[SDL_SCANCODE_UP])    app.camera.move(0, -app.camera.getSpeed());
    if (state[SDL_SCANCODE_DOWN])  app.camera.move(0, app.camera.getSpeed());
    if (state[SDL_SCANCODE_LEFT])  app.camera.move(-app.camera.getSpeed(), 0);
    if (state[SDL_SCANCODE_RIGHT]) app.camera.move(app.camera.getSpeed(), 0);
}

void KeyboardHandler::displayControls() {
    std::cout << "==========================================" << std::endl;
    std::cout << "        GUIDE DES COMMANDES - SIM          " << std::endl;
    std::cout << "==========================================" << std::endl;
    
    std::cout << "\n[ DEPLACEMENTS ]" << std::endl;
    std::cout << "  - Fleches : Deplacer la camera" << std::endl;
    
    std::cout << "\n[ ZOOM ]" << std::endl;
    std::cout << "  - Touche X : Zoom avant (+1.0)" << std::endl;
    std::cout << "  - Touche C : Zoom arriere (-1.0)" << std::endl;
    
    std::cout << "\n[ AFFICHAGE ]" << std::endl;
    std::cout << "  - Touche B : Mode de vue suivant" << std::endl;
    std::cout << "  - Touche N : Mode de vue precedent" << std::endl;
    std::cout << "  - Touche T : Afficher la carte textuelle" << std::endl;
    
    std::cout << "\n[ SYSTEME ]" << std::endl;
    std::cout << "  - Touche P : Pause / Reprise de la simulation" << std::endl;
    std::cout << "  - ECHAP    : Quitter l'application" << std::endl;
    
    std::cout << "------------------------------------------" << std::endl;
}
void KeyboardHandler::switchViewMode(Application& app, bool next) {
    float zoom = app.camera.getZoom();
    float camX = app.camera.getX();
    float camY = app.camera.getY();
    
    float winW = app.width; 
    float winH = app.height; 

    float worldX = 0.0f, worldY = 0.0f;

    if (app.currentViewMode == ViewMode::ISOMETRIC) { 
        worldX = (camX + 2.0f * camY + winH) / zoom;
        worldY = (2.0f * camY + winH - camX) / zoom;
    } else {
        worldX = (winW / 2.0f + camX) / zoom;
        worldY = (winH / 2.0f + camY) / zoom;
    }

    int maxMode = static_cast<int>(ViewMode::HEATMAP_ALTITUDE);
    int current = static_cast<int>(app.currentViewMode);
    
    if (next) {
        app.currentViewMode = static_cast<ViewMode>((current + 1) % (maxMode + 1));
    } else {
        app.currentViewMode = static_cast<ViewMode>((current - 1 + (maxMode + 1)) % (maxMode + 1));
    }
    //std::cout << "Changement de vue : Mode " << static_cast<int>(app.currentViewMode) << std::endl;

    float newCamX = 0.0f, newCamY = 0.0f;
    if (app.currentViewMode == ViewMode::ISOMETRIC) {
        newCamX = (worldX - worldY) * (zoom / 2.0f);
        newCamY = (worldX + worldY) * (zoom / 4.0f) - (winH / 2.0f);
    } else {
        newCamX = worldX * zoom - (winW / 2.0f);
        newCamY = worldY * zoom - (winH / 2.0f);
    }

    app.camera.setPosition(newCamX, newCamY);
}