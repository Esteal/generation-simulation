#include "LightSystem.h"
#include <cmath>
#include <algorithm>
#include "ConfigManager.h"

LightSystem::LightSystem() {
    ConfigManager& cfg = ConfigManager::getInstance();
    radius = cfg.getConfig().radius;
    numDirections = cfg.getConfig().numDirections;
}

void LightSystem::process(Map& map, float deltaTime) {

    if (deltaTime != 0.0f) return;
    // Horizon-Based Ambient Occlusion (HBAO) 
    int width = map.getWidth();
    int height = map.getHeight();

    float dirs[8][2] = {
        {1, 0}, {1, 1}, {0, 1}, {-1, 1}, 
        {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
    };

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float currentAlt = map.getAltitude(x, y);
            float totalOcclusion = 0.0f;

            // Pour chaque direction (Nd)
            for (int d = 0; d < numDirections; ++d) {
                float dx = dirs[d][0];
                float dy = dirs[d][1];
                
                // On normalise le vecteur direction pour avoir des pas réguliers
                float length = std::sqrt(dx*dx + dy*dy);
                dx /= length;
                dy /= length;

                float maxSinHorizon = 0.0f;

                // Ray Marching : On avance pas à pas (Ns) jusqu'au rayon R
                for (int step = 1; step <= radius; ++step) {
                    int nx = static_cast<int>(x + dx * step);
                    int ny = static_cast<int>(y + dy * step);

                    if (nx < 0 || nx >= width || ny < 0 || ny >= height) break;

                    float neighborAlt = map.getAltitude(nx, ny);
                    float deltaZ = neighborAlt - currentAlt;

                    // Si alt voisin > alt courant => ombre
                    if (deltaZ > 0) {
                        float dist = static_cast<float>(step);
                        // Calcul de l'angle d'élévation
                        float angle = std::atan2(deltaZ, dist);
                        
                        // Atténuation W(theta) : Plus l'obstacle est loin, moins il fait d'ombre
                        float attenuation = std::max(0.0f, 1.0f - (dist / static_cast<float>(radius)));
                        
                        // On cherche le sin max (l'horizon le plus haut) dans cette direction
                        float sinElevation = std::sin(angle) * attenuation;
                        if (sinElevation > maxSinHorizon) {
                            maxSinHorizon = sinElevation;
                        }
                    }
                }
                
                totalOcclusion += maxSinHorizon;
            }

            // Calcul final de l'illumination (1.0 = Soleil, on soustrait la moyenne des ombres)
            // On divise par le nombre de directions pour faire la moyenne
            float ambientLight = 1.0f - (totalOcclusion / static_cast<float>(numDirections));
            
            map.getGrid().get(x, y).light = std::pow(std::max(0.0f, std::min(1.0f, ambientLight)), 2.0f);
        }
    }
}