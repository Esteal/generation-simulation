#include "MapRenderer2DIso.h"

#include "MapRenderer2DIso.h"
#include <algorithm>

void MapRenderer2DIso::render2D(const Map& map, Window& window, TextureManager& textureManager,
                         const Camera2D& camera, const size_t &windowWidth, 
                         const size_t &windowHeight, const int &mouseX, const int &mouseY, const std::string &spritesheetID)
{
    int srcSize = 64;

    float tileW = camera.getZoom();
    float tileH = tileW / 2.0f;
    float destSize = tileW;

    float camX = camera.getX();
    float camY = camera.getY();
    float offsetX = windowWidth / 2.0f;

    float seaLevel = 0.40f; 
    float elevationScale = 120.0f; 
    
    // passer à tileH / 4 si le gap est trop visible.
    int step = static_cast<int>(tileH / 2.0f);
    if (step < 1) step = 1; 

    for (int y = 0; y < static_cast<int>(map.getHeight()); ++y) {
        for (int x = 0; x < static_cast<int>(map.getWidth()); ++x) {
            const Cell& cell = map.getGrid().get(x, y);

            
            float currentAlt = map.getAltitude(x, y);
            float elevation = 0.0f;
            if (currentAlt > seaLevel) {
                elevation = (currentAlt - seaLevel) * elevationScale;
            }

            int destX = (int)((x - y) * (tileW / 2.0f) - camX + offsetX);
            int baseDestY = (int)((x + y) * (tileH / 2.0f) - camY);
            int finalDestY = (int)(baseDestY - elevation);

            float minNeighborElev = elevation;
            
            if (x + 1 < static_cast<int>(map.getWidth())) {
                float alt = map.getAltitude(x + 1, y);
                float nElev = (alt > seaLevel) ? (alt - seaLevel) * elevationScale : 0.0f;
                if (nElev < minNeighborElev) minNeighborElev = nElev;
            }
            if (y + 1 < static_cast<int>(map.getHeight())) {
                float alt = map.getAltitude(x, y + 1);
                float nElev = (alt > seaLevel) ? (alt - seaLevel) * elevationScale : 0.0f;
                if (nElev < minNeighborElev) minNeighborElev = nElev;
            }

            // Culling optimisé (on prend en compte la base du mur pour ne pas le faire disparaître trop tôt en haut de l'écran)
            int bottomY = (int)(baseDestY - minNeighborElev);
            if (destX + destSize < 0 || destX > static_cast<int>(windowWidth) || 
                bottomY + destSize < 0 || finalDestY > static_cast<int>(windowHeight)) {
                continue; 
            }

            int srcX = 0; 
            int srcY = 0; 
            switch (cell.biome) {
                case BiomeIndex::OCEAN:     srcX = 0 * srcSize; break;
                case BiomeIndex::GLACE:     srcX = 1 * srcSize; break;
                case BiomeIndex::RIVER:     srcX = 1 * srcSize; break;
                case BiomeIndex::DESERT:    srcX = 2 * srcSize; break;
                case BiomeIndex::BEACH:     srcX = 2 * srcSize; break;                
                case BiomeIndex::PLAINS:    srcX = 3 * srcSize; break;
                case BiomeIndex::FOREST:    srcX = 4 * srcSize; break;
                case BiomeIndex::MOUNTAINS: srcX = 5 * srcSize; break;
                case BiomeIndex::SNOW:      srcX = 6 * srcSize; break;
                default:                    srcX = 0; break;
            }

            // DESSIN DES "facades" 
            if (elevation > minNeighborElev) {
                 //Utilisation temporaire de texture MOUNTAINS
                 int dirtSrcX = 5 * srcSize; 
                 int dirtSrcY = 0;

                for (float e = minNeighborElev; e < elevation; e += step) {
                    int fillY = (int)(baseDestY - e);
                    textureManager.drawFrame(spritesheetID, window, 
                                             dirtSrcX, dirtSrcY, srcSize, srcSize, 
                                             destX, fillY, tileW, tileW);
                }
            }
                        
            // Le "toit" de la notr Cell
            textureManager.drawFrame(spritesheetID, window, 
                                     srcX, srcY, srcSize, srcSize, 
                                     destX, finalDestY, tileW, tileW);
        }
    }
}

bool MapRenderer2DIso::testRegression() {
    std::cout << "[Test] MapRenderer2DIso... ";
    
    Window win("Test Renderer Iso", 100, 100);
    TextureManager tm;
    Camera2D cam; 
    Map m(2, 2); 
    
    MapRenderer2DIso rendererIso;
    
    // Vérification de non-crash
    rendererIso.render2D(m, win, tm, cam, 100, 100, 0, 0, "test");
    
    std::cout << "OK" << std::endl;
    return true;
}