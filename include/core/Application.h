#ifndef APPLICATION_H
#define APPLICATION_H

#include "MapGenerator.h"
#include "Window.h"
#include "system/HydrologieSystem.h"
#include "TextureManager.h"
#include "Camera2D.h"
#include "MapRenderer.h"

class Application
{
private:
    bool isRunning;
    Map map;
    Window window;
    MapRenderer mapRenderer;
    TextureManager textureManager;
    Camera2D camera;
    size_t width, height;
    
public:
    Application(size_t width, size_t height, size_t mapWidth, size_t mapHeight, int seed);
    void run();
    void keyboardInput();
};

#endif