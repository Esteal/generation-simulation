#ifndef MAP_RENDERER_H
#define MAP_RENDERER_H

#include "Map.h"
#include "Window.h"
#include "TextureManager.h"
#include "Camera2D.h"

class MapRenderer {
private:
    void drawRectOnClick(Window &window, const Map &map,
                         const int &mouseX, const int &mouseY,
                         const Camera2D &camera);
public:
    MapRenderer() = default;
    void render2D(const Map& map, Window& window, TextureManager& textureManager,
                  const Camera2D& camera, const size_t &windowWidth, 
                  const size_t &windowHeight, const int &mouseX, const int &mouseY);
};

#endif
