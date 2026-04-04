#include "system/ISimulationSystem.h"
#include <iostream>

void ISimulationSystem::findSteepestNeighbor(const Map& map, int x, int y, float& outMaxDiff, int& outLowestX, int& outLowestY) const
{
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());
    
    float currentHeight = map.getAltitude(x, y);

    int dx[] = {-1, 1, 0, 0}; 
    int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        // Vérification des bords de la carte
        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
            float neighborHeight = map.getAltitude(nx, ny);
            float diff = currentHeight - neighborHeight;
            
            if (diff > outMaxDiff) {
                outMaxDiff = diff;
                outLowestX = nx;
                outLowestY = ny;
            }
        }
    }
}


void ISimulationSystem::calculateAttractivity(Map& map) {
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Cell& cell = map.getGrid().get(x, y);
            // Calcul de l'attractivité basé sur les facteurs environnementaux
            if(cell.biome == BiomeIndex::OCEAN || cell.biome == BiomeIndex::GLACE || cell.biome == BiomeIndex::RIVER) {
                cell.attractivityLevel = 0.0f;
            } else {
                cell.attractivityLevel = std::min((cell.temperature * 0.3f) + (cell.humidity * 0.3f) + (cell.light * 0.2f) + ((cell.bedrock + cell.granular) * 0.2f), 1.0f);
            }
        }
    }
}

void ISimulationSystem::calculateAttractivityForCell(Map& map, int x, int y) {
    Cell& cell = map.getGrid().get(x, y);
    if(cell.biome == BiomeIndex::OCEAN || cell.biome == BiomeIndex::GLACE || cell.biome == BiomeIndex::RIVER) {
        cell.attractivityLevel = 0.0f;
    } else {
        cell.attractivityLevel = std::min((cell.temperature * 0.3f) + (cell.humidity * 0.3f) + (cell.light * 0.2f) + ((cell.bedrock + cell.granular) * 0.2f), 1.0f);
    }
}

// Classe tampon pour tester l'interface abstraite
class DummySimulationSystem : public ISimulationSystem {
public:
    void process(Map& map, float deltaTime) override {}
};

bool ISimulationSystem::testRegression() {
    std::cout << "[Test] ISimulationSystem... ";
    
    Map m(3, 3);
    
    for(size_t x = 0; x < 3; ++x) {
        for(size_t y = 0; y < 3; ++y) {
            m.getGrid().get(x,y).bedrock = 10.0f;
            m.getGrid().get(x,y).granular = 0.0f;
        }
    }
    
    m.getGrid().get(1, 0).bedrock = 5.0f; 
    
    DummySimulationSystem dummySys;
    float maxDiff = 0.0f;
    int lowestX = -1, lowestY = -1;
    
    // On cherche le voisin le plus bas depuis le centre (1,1)
    dummySys.findSteepestNeighbor(m, 1, 1, maxDiff, lowestX, lowestY);
    
    if (lowestX != 1 || lowestY != 0 || maxDiff != 5.0f) {
        std::cerr << "Erreur: findSteepestNeighbor n'a pas trouve la bonne pente." << std::endl;
        return false;
    }
    
    std::cout << "OK" << std::endl;
    return true;
}