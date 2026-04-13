#include "MapRenderer.h"

void MapRenderer::render2D(const Map& map, Window& window, TextureManager& textureManager,
                         const Camera2D& camera, const size_t &windowWidth, 
                         const size_t &windowHeight, const int &mouseX, const int &mouseY,
                        const std::string &spritesheetId)
{
    int destSize = (int) camera.getZoom(); 
    int srcSize = 225;

    float camX = camera.getX();
    float camY = camera.getY();

    for (size_t y = 0; y < map.getHeight(); ++y) {
        for (size_t x = 0; x < map.getWidth(); ++x) {
            const Cell& cell = map.getGrid().get(x, y);
            
            size_t destX = x * destSize - camX;
            size_t destY = y * destSize - camY;

            if (destX + destSize < 0 || destX > windowWidth || 
                destY + destSize < 0 || destY > windowHeight) {
                continue; 
            }
            int srcX = 0; // colonne de la spritesheet
            int srcY = 0; // ligne de la spritesheet
            
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
                        
            textureManager.drawFrame(spritesheetId, window, 
                                     srcX, srcY, srcSize, srcSize, 
                                     destX, destY, destSize, destSize);
            
            // Dessiner les minéraux et la végétation par-dessus le biome
            if (cell.material != Material::NONE) {
                SDL_Rect materialRect;
                materialRect.x = destX;
                materialRect.y = destY;
                materialRect.w = destSize;
                materialRect.h = destSize;
                
                Uint8 r, g, b, a;
                getMaterialColor(cell.material, r, g, b, a);
                
                SDL_Renderer* renderer = &window.getRenderer();
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, r, g, b, a);
                SDL_RenderFillRect(renderer, &materialRect);
                
                // Optionnel: ajouter une bordure légère pour mieux distinguer
                SDL_SetRenderDrawColor(renderer, r / 2, g / 2, b / 2, 200);
                SDL_RenderDrawRect(renderer, &materialRect);
            }
        }
    }
renderFactions(window, camera, map.getFactions(), windowWidth, windowHeight);    
drawRectOnClick(window, map, mouseX, mouseY, camera);
}

void MapRenderer::getMaterialColor(const Material& material, Uint8& r, Uint8& g, Uint8& b, Uint8& a) const
{
    // Définit la couleur pour chaque type de matériau
    a = 180; // Transparence par défaut
    
    switch(material) {
        // Végétation
        case Material::GRASS:
            r = 34; g = 180; b = 65;  // Vert herbe
            a = 160;
            break;
        case Material::MOSS:
            r = 100; g = 120; b = 60;  // Vert foncé (mousse)
            a = 140;
            break;
        case Material::CACTUS:
            r = 139; g = 169; b = 19;  // Vert-jaune (cactus)
            a = 150;
            break;
        case Material::PINE_TREE:
            r = 34; g = 100; b = 40;  // Vert très foncé (pin)
            a = 170;
            break;
        case Material::OAK_TREE:
            r = 85; g = 140; b = 50;  // Vert moyen (chêne)
            a = 170;
            break;
        case Material::JUNGLE_TREE:
            r = 0; g = 150; b = 80;  // Vert jungle vif
            a = 175;
            break;
            
        // Minéraux
        case Material::BRONZE_ORE:
            r = 205; g = 127; b = 50;  // Orange-brun (bronze)
            a = 200;
            break;
        case Material::IRON_ORE:
            r = 100; g = 100; b = 100;  // Gris foncé (fer)
            a = 210;
            break;
        case Material::COAL_ORE:
            r = 50; g = 50; b = 50;  // Noir (charbon)
            a = 220;
            break;
        case Material::GOLD_ORE:
            r = 255; g = 215; b = 0;  // Or vif
            a = 200;
            break;
            
        // Agriculture
        case Material::WHEAT:
            r = 218; g = 165; b = 32;  // Doré (blé)
            a = 160;
            break;
            
        default:
        case Material::NONE:
            r = 0; g = 0; b = 0;
            a = 0;  // Transparent pour NONE
            break;
    }
}
void MapRenderer::renderFactions(Window &window, const Camera2D &camera, 
                                 const std::vector<Faction>& factions,
                                 const size_t &windowWidth, const size_t &windowHeight)
{
    SDL_Renderer* renderer = &window.getRenderer();
    int destSize = (int)camera.getZoom();
    float camX = camera.getX();
    float camY = camera.getY();

    for (const Faction& faction : factions) {
        
        Uint8 r = (faction.id * 85) % 255;
        Uint8 g = (faction.id * 140) % 255;
        Uint8 b = (faction.id * 200) % 255;

        for (const Settlement& settlement : faction.colonies) {
            
            int destX = (int)(settlement.x * destSize - camX);
            int destY = (int)(settlement.y * destSize - camY);

            if (destX + destSize < 0 || destX > windowWidth || 
                destY + destSize < 0 || destY > windowHeight) {
                continue; 
            }

            SDL_Rect cityRect;
            cityRect.w = destSize / 2;
            cityRect.h = destSize / 2;
            cityRect.x = destX + (destSize / 4);
            cityRect.y = destY + (destSize / 4);

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderFillRect(renderer, &cityRect);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &cityRect);
        }
    }
}

void MapRenderer::drawRectOnClick(Window &window, const Map &map,
                                  const int &mouseX, const int &mouseY,
                                  const Camera2D &camera)
{
    // Transformer les pixels de l'écran en Index de Grille
    
    float camX = camera.getX();
    float camY = camera.getY();
    int zoom = (int) camera.getZoom();
    size_t gridX = (size_t)((mouseX + camX) / zoom);
    size_t gridY = (size_t)((mouseY + camY) / zoom);

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

void MapRenderer::renderHeatmap(const Map& map, Window& window, const Camera2D& camera, 
                                const size_t &windowWidth, const size_t &windowHeight, 
                                HeatmapType type) 
{
    SDL_Renderer& renderer = window.getRenderer();
    
    int tileSize = (int)camera.getZoom(); 
    int width = map.getWidth();
    int height = map.getHeight();
    float camX = camera.getX();
    float camY = camera.getY();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {   
            
            int destX = static_cast<int>(x * tileSize - camX);
            int destY = static_cast<int>(y * tileSize - camY);

            if (destX + tileSize < 0 || destX > (int)windowWidth || 
                destY + tileSize < 0 || destY > (int)windowHeight) 
            {
                continue; 
            }

            Uint8 r = 0, g = 0, b = 0;
            Uint8 borderR = 0, borderG = 0, borderB = 0;

            if (type == HeatmapType::HUMIDITY) {
                float humidity = map.getGrid().get(x, y).humidity;
                r = static_cast<Uint8>(255 - humidity * (255 - 20));
                g = static_cast<Uint8>(240 - humidity * (240 - 50));
                b = static_cast<Uint8>(150 + humidity * (255 - 150));
            } 
            else if (type == HeatmapType::TEMPERATURE) {
                float temperature = map.getGrid().get(x, y).temperature;
                temperature = std::max(0.0f, std::min(1.0f, temperature));
                r = static_cast<Uint8>(temperature * 255);
                g = 50; 
                b = static_cast<Uint8>((1.0f - temperature) * 255);
            } 
            else if (type == HeatmapType::ALTITUDE) {
                float altitude = map.getAltitude(x, y);
                altitude = std::max(0.0f, std::min(1.0f, altitude));
                Uint8 colorValue = static_cast<Uint8>(altitude * 255);
                r = colorValue;
                g = colorValue; 
                b = colorValue;
                
                // Bordure spéciale pour l'altitude très sombre
                if (colorValue < 50)
                    borderR = borderG = borderB = 50;
            }
            else if (type == HeatmapType::LIGHT) {
                float light = map.getGrid().get(x, y).light;
                light = std::max(0.0f, std::min(1.0f, light)); 
                
                Uint8 colorValue = static_cast<Uint8>(light * 255);
                r = colorValue;
                g = colorValue;
                b = colorValue;

                if (colorValue < 30)
                    borderR = borderG = borderB = 40;
            }
            else if (type == HeatmapType::VEGETATION) {
                
                Material material = map.getGrid().get(x, y).material;
                
                switch (material) {
                    case Material::GRASS:       
                        r = 124; g = 204; b = 39;  break; // Vert pomme / clair
                    case Material::MOSS:        
                        r = 255;  g = 255; b = 255;  break; // Vert olive sombre (Fonds humides)
                    case Material::CACTUS:      
                        r = 255; g = 150; b = 0;   break; // Orange vif (Pour bien repérer les 5% d'apparition !)
                    case Material::PINE_TREE:   
                        r = 47;  g = 79;  b = 79;  break; // Gris-vert sombre (Taïga)
                    case Material::OAK_TREE:    
                        r = 34;  g = 139; b = 34;  break; // Vert forêt classique
                    case Material::JUNGLE_TREE: 
                        r = 0;   g = 80;  b = 0;   break; // Vert très sombre et profond
                    
                    case Material::NONE:
                    default:
                        r = 0; g = 0; b = 0;
                        break;
                }

                borderR = r; 
                borderG = g; 
                borderB = b;
            }

            SDL_Rect destRect = { destX, destY, tileSize, tileSize };

            SDL_SetRenderDrawColor(&renderer, r, g, b, 255);
            SDL_RenderFillRect(&renderer, &destRect);

            SDL_SetRenderDrawColor(&renderer, borderR, borderG, borderB, 255);
            SDL_RenderDrawRect(&renderer, &destRect);
        }
    }
}

bool MapRenderer::testRegression() {
    std::cout << "[Test] MapRenderer... ";
    
    Window win("Test Renderer", 100, 100);
    TextureManager tm;
    Camera2D cam; 
    Map m(2, 2); 
    
    MapRenderer renderer;
    
    // On force un rendu. Si le code dans render2D n'est pas protégé contre
    // les textures manquantes, ce test fera crasher le programme (Segfault).
    // Si ça passe, c'est que le code est good
    renderer.render2D(m, win, tm, cam, 100, 100, 0, 0, "test");
    
    std::cout << "OK" << std::endl;
    return true;
}