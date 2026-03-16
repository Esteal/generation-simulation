#include "Application.h"

Application::Application(size_t width, size_t height, size_t mapWidth, size_t mapHeight, int seed) 
    : isRunning(true), map(mapWidth, mapHeight), 
    window("test", width, height),
    camera(), textureManager(), 
    width(width), height(height) 
{
    MapGenerator generator(seed);
    generator.generate(map, 0.7f, 0.01f, 0.01f);
    HydrologieSystem hydrologie(100.0f);
    hydrologie.process(map, 0.0f);
    textureManager.load("spritesheet", "spritesheet.png", window);
}

void Application::keyboardInput()   
{
    SDL_Event events;
    int mouseX, mouseY;
    while (SDL_PollEvent(&events)) {
        if (events.type == SDL_QUIT) {
            isRunning = false; 
        } else if (events.type == SDL_KEYDOWN) {
            if (events.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false; 
            }
            SDL_GetMouseState(&mouseX, &mouseY);
            if (events.key.keysym.sym == SDLK_x) {
                camera.zoomAt(camera.getZoom() + 0.1f, mouseX, mouseY);
            }
            else if (events.key.keysym.sym == SDLK_c) {
                camera.zoomAt(camera.getZoom() - 0.1f, mouseX, mouseY);
            }
        }
    }
    const Uint8* state = SDL_GetKeyboardState(NULL);
    
    
    if (state[SDL_SCANCODE_UP])    camera.move(0, -camera.getSpeed());
    if (state[SDL_SCANCODE_DOWN])  camera.move(0, camera.getSpeed());
    if (state[SDL_SCANCODE_LEFT])  camera.move(-camera.getSpeed(), 0);
    if (state[SDL_SCANCODE_RIGHT]) camera.move(camera.getSpeed(), 0);
}

void Application::run()
{
    int mouseX, mouseY;
    while(isRunning)
    {
        // texturing -> rendering -> affichage    
        keyboardInput();
        window.clear();
        SDL_GetMouseState(&mouseX, &mouseY);
        mapRenderer.render2D(map, window, textureManager, camera, width, height, mouseX, mouseY);
        window.present();
    }
}
