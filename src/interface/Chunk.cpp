#include "Chunk.h"
#include <algorithm> // Pour std::min et std::max

Chunk::Chunk(int x1, int x2, int y1, int y2, const Map& map)
    : minX(x1), maxX(x2), minY(y1), maxY(y2), left(nullptr), right(nullptr) 
{
    // 1. On calcule les limites visuelles (AABB) pour CE morceau de carte
    this->computeAABB(map);

    // 2. On calcule la taille de la zone
    int width = x2 - x1 + 1;
    int height = y2 - y1 + 1;

    // 3. CONDITION D'ARRÊT : Si la zone est <= 1x1, on s'arrête (Feuille)
    if (width <= 1 && height <= 1) {
        return; 
    }

    // 4. DIVISION : On coupe en deux sur l'axe le plus long
    if (width >= height) {
        int midX = x1 + (width / 2) - 1;
        left = new Chunk(x1, midX, y1, y2, map);
        right = new Chunk(midX + 1, x2, y1, y2, map);
    } else {
        int midY = y1 + (height / 2) - 1;
        left = new Chunk(x1, x2, y1, midY, map);
        right = new Chunk(x1, x2, midY + 1, y2, map);
    }
}

// Destructeur récursif : Supprimer la racine suffit à supprimer tout l'arbre
Chunk::~Chunk() {
    delete left;  // Appelle le destructeur de l'enfant gauche
    delete right; // Appelle le destructeur de l'enfant droit
}

void Chunk::computeAABB(const Map& map) {
    // Initialisation avec des valeurs extrêmes (neutres)
    minPX = 1e9f; maxPX = -1e9f;
    minPY = 1e9f; maxPY = -1e9f;

    // Constantes de rendu (doivent être identiques à celles de ton Renderer)
    const float tileW = 64.0f; // Taille de la tuile en pixels (doit correspondre à ta spritesheet)
    const float tileH = tileW / 2.0f;
    const float elevationScale = 800.0f;
    const float seaLevel = 0.40f;

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            // Position ISOMÉTRIQUE de base au sol
            float isoX = (x - y) * (tileW / 2.0f);
            float isoY = (x + y) * (tileH / 2.0f);
            
            // Calcul de l'élévation à partir de Map.h
            float alt = map.getAltitude(x, y);
            float elev = (alt > seaLevel) ? (alt - seaLevel) * elevationScale : 0.0f;

            // On définit les 4 bords visuels de cette tuile précise
            float tileLeft   = isoX;
            float tileRight  = isoX + tileW;
            float tileTop    = isoY - elev;     // Le point le plus haut (sommet)
            float tileBottom = isoY + tileH;    // Le point le plus bas (sol)

            // Mise à jour de l'AABB du Chunk global
            if (tileLeft   < minPX) minPX = tileLeft;
            if (tileRight  > maxPX) maxPX = tileRight;
            if (tileTop    < minPY) minPY = tileTop;
            if (tileBottom > maxPY) maxPY = tileBottom;
        }
    }
}

bool Chunk::isVisible(const Camera2D& camera, float windowWidth, float windowHeight) const {
    float baseTileW = 64.0f; // Doit être identique à tileW dans computeAABB et au rendu
    float scale = camera.getZoom() / baseTileW;

    
    float scaledMinPX = minPX * scale;
    float scaledMaxPX = maxPX * scale;
    float scaledMinPY = minPY * scale;
    float scaledMaxPY = maxPY * scale;


    float screenMinX = scaledMinPX - camera.getX() + (windowWidth / 2.0f);
    float screenMaxX = scaledMaxPX - camera.getX() + (windowWidth / 2.0f);
    float screenMinY = scaledMinPY - camera.getY();
    float screenMaxY = scaledMaxPY - camera.getY();

    if (screenMaxX < 0 || screenMinX > windowWidth || screenMaxY < 0 || screenMinY > windowHeight) {
        return false; 
    }
    return true; 
}

void Chunk::render(Window& window, TextureManager& tm, const Camera2D& cam, 
                  const Map& map, const std::string& spritesheetID,
                  int winW, int winH) 
{
   
    if (!isVisible(cam, winW, winH)) {
        return; 
    }

    
    if (isLeaf()) {
        float tileW = cam.getZoom();
        float tileH = tileW / 2.0f;
        float scale = cam.getZoom() / 64.0f; 
        float elevationScale = 700.0f * scale; 
        float seaLevel = 0.40f;
        int step = std::max(1, (int)(tileH / 2.0f));
        float offsetX = winW / 2.0f;

        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
              
                const Cell& cell = map.getGrid().get(x, y);
                float currentAlt = map.getAltitude(x, y);
                float elevation = (currentAlt > seaLevel) ? (currentAlt - seaLevel) * elevationScale : 0.0f;

                int destX = (int)((x - y) * (tileW / 2.0f) - cam.getX() + offsetX);
                int baseDestY = (int)((x + y) * (tileH / 2.0f) - cam.getY());
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

            
            int srcX = 0; 
            int srcY = 0; 
            int srcSize = 64; 
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

        
            if (elevation > minNeighborElev) {
                int dirtSrcX = 5 * srcSize; // Texture de flanc (ex: Montagne/Terre)
                for (float e = minNeighborElev; e < elevation; e += step) {
                    int fillY = (int)(baseDestY - e);
                    tm.drawFrame(spritesheetID, window, dirtSrcX, 0, srcSize, srcSize, 
                             destX, fillY, tileW, tileW);
                }
            }
        
        
            tm.drawFrame(spritesheetID, window, srcX, srcY, srcSize, srcSize, 
                     destX, finalDestY, tileW, tileW);
            }
        }
        
    } 
    
        if (left) left->render(window, tm, cam, map, spritesheetID, winW, winH);
        if (right) right->render(window, tm, cam, map, spritesheetID, winW, winH);
    }


void Chunk::debugPrint(int depth) const {
    for(int i=0; i<depth; ++i) std::cout << "  "; // Indentation
    
    std::cout << "Chunk [" << minX << "," << minY << "] to [" << maxX << "," << maxY << "] "
              << "AABB: (" << minPX << "," << minPY << ") -> (" << maxPX << "," << maxPY << ")" 
              << (isLeaf() ? " [LEAF]" : "") << std::endl;

    if (left) left->debugPrint(depth + 1);
    if (right) right->debugPrint(depth + 1);
}