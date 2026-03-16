#include "MapRenderer.h"

void MapRenderer::render2D(const Map& map, Window& window, TextureManager& textureManager,
                         const Camera2D& camera, const size_t &windowWidth, 
                         const size_t &windowHeight, const int &mouseX, const int &mouseY)
{
    int destSize = (int) camera.getZoom(); 
    int srcSize = 225;

    float camX = camera.getX();
    float camY = camera.getY();

    for (int y = 0; y < map.getHeight(); ++y) {
        for (int x = 0; x < map.getWidth(); ++x) {
            const Cell& cell = map.getGrid().get(x, y);
            
            int destX = x * destSize - (int)camX;
            int destY = y * destSize - (int)camY;

            if (destX + destSize < 0 || destX > windowWidth || 
                destY + destSize < 0 || destY > windowHeight) {
                continue; 
            }
            int srcX = 0; // colonne de la spritesheet
            int srcY = 0; // ligne de la spritesheet
            
            switch (cell.biome) {
                case BiomeIndex::OCEAN:     srcX = 0 * srcSize; break;
                case BiomeIndex::RIVER:     srcX = 1 * srcSize; break;
                case BiomeIndex::BEACH:     srcX = 2 * srcSize; break;                
                case BiomeIndex::PLAINS:    srcX = 3 * srcSize; break;
                case BiomeIndex::FOREST:    srcX = 4 * srcSize; break;
                case BiomeIndex::MOUNTAINS: srcX = 5 * srcSize; break;
                case BiomeIndex::SNOW:      srcX = 6 * srcSize; break;
                default:                    srcX = 0; break;
            }
                        
            textureManager.drawFrame("spritesheet", window, 
                                     srcX, srcY, srcSize, srcSize, 
                                     destX, destY, destSize, destSize);
        }
    }
    drawRectOnClick(window, map, mouseX, mouseY, camera);
}

void MapRenderer::drawRectOnClick(Window &window, const Map &map,
                                  const int &mouseX, const int &mouseY,
                                  const Camera2D &camera)
{
    // Transformer les pixels de l'écran en Index de Grille
    

    float camX = camera.getX();
    float camY = camera.getY();
    int zoom = (int) camera.getZoom();
    int gridX = (int)((mouseX + camX) / zoom);
    int gridY = (int)((mouseY + camY) / zoom);

    if (gridX >= 0 && gridX < map.getWidth() && gridY >= 0 && gridY < map.getHeight()) {
        
        SDL_Rect highlightRect;
        highlightRect.x = (gridX * zoom) - (int) camX;
        highlightRect.y = (gridY * zoom) - (int) camY;
        highlightRect.w = zoom;
        highlightRect.h = zoom;

        SDL_Renderer* renderer = &window.getRenderer();
        
        //Pour avoir la case en transparent
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100); 
        SDL_RenderFillRect(renderer, &highlightRect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
        SDL_RenderDrawRect(renderer, &highlightRect);
    }
}