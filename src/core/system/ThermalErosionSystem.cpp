#include "ThermalErosionSystem.h"
#include "ConfigManager.h"

ThermalErosionSystem::ThermalErosionSystem() {
    ConfigManager& cfg = ConfigManager::getInstance();
    
    talusAngle = cfg.getConfig().talusAngle;
    friction = cfg.getConfig().friction;
    iterationsPerTick = cfg.getConfig().iterationsPerTick;
    seaLevel = cfg.getConfig().seaLevel;
}

void ThermalErosionSystem::process(Map& map, float deltaTime) 
{
    size_t width = map.getWidth();
    size_t height = map.getHeight();

    int totalIterations = ((int) deltaTime)* iterationsPerTick;
    if (totalIterations <= 0) totalIterations = 1;

    for (int iter = 0; iter < totalIterations; ++iter) 
    {
        for (int y = 0; y < (int) height; ++y) {
            for (int x = 0; x < (int) width; ++x) {
                if (map.getAltitude(x, y) > seaLevel)
                    processCell(map, x, y);
            }
        }
    }
}

void ThermalErosionSystem::processCell(Map& map, int x, int y) const
{
    float maxDiff = 0.0f;
    int lowestX = x;
    int lowestY = y;

    findSteepestNeighbor(map, x, y, maxDiff, lowestX, lowestY);

    // si la pente dépasse l'angle de repos, on déclenche un "éboulement"
    if (maxDiff > talusAngle) {
        Cell& currentCell = map.getGrid().get(x, y);
        Cell& lowestCell = map.getGrid().get(lowestX, lowestY);
        
        transferMaterial(currentCell, lowestCell, maxDiff);
    }
}


void ThermalErosionSystem::transferMaterial(Cell& currentCell, Cell& lowestCell, float maxDiff) const
{
    // Calcul de la quantité totale de matière devant s'effondrer
    float transferAmount = (maxDiff - talusAngle) * friction;

    // Les sédiments glissent en premier
    float granularTransfer = std::min(currentCell.granular, transferAmount);
    currentCell.granular -= granularTransfer;
    lowestCell.granular += granularTransfer;
    
    transferAmount -= granularTransfer;

    // Si la terre n'a pas suffi à adoucir la falaise, la roche mère s'effondre
    if (transferAmount > 0.0f) {
        currentCell.bedrock -= transferAmount;
        lowestCell.granular += transferAmount; // La roche tombe sous forme de "gravats" (la notion de masse volumique dont on avait parler tkt)
    }
}

bool ThermalErosionSystem::testRegression() {
    std::cout << "[Test] ThermalErosionSystem... ";
    
    Map m(3, 3);
    // On crée un terrain plat à 0 d'altitude
    for(size_t x = 0; x < 3; ++x) {
        for(size_t y = 0; y < 3; ++y) {
            m.getGrid().get(x, y).bedrock = 0.0f;
            m.getGrid().get(x, y).granular = 0.0f;
        }
    }
    
    // On dresse un pic artificiel au centre (impossible à maintenir avec la gravité)
    m.getGrid().get(1, 1).bedrock = 20.0f; 
    float altAvant = m.getAltitude(1, 1);
    
    ThermalErosionSystem sys;
    sys.process(m, 1.0f);
    
    float altApres = m.getAltitude(1, 1);
    
    // Le pic doit avoir perdu de l'altitude au profit de ses voisins
    if (altApres >= altAvant) {
        std::cerr << "Erreur: Le pic central ne s'est pas effondre sous son propre poids." << std::endl;
        return false;
    }
    
    // Vérification qu'un voisin a bien reçu de la terre (granular)
    if (m.getGrid().get(0, 1).granular == 0.0f && m.getGrid().get(1, 0).granular == 0.0f) {
        std::cerr << "Erreur: La matiere eboulee n'a pas ete transferee aux voisins." << std::endl;
        return false;
    }
    
    std::cout << "OK" << std::endl;
    return true;
}