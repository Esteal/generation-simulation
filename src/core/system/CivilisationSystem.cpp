#include "system/CivilisationSystem.h"
#include "system/WorldSimulator.h"

#include <algorithm>

struct Candidate {
    int x;
    int y;
    float attractivity;
};

void CivilisationSystem::process(Map& map, float deltaTime) 
{
    if(deltaTime > 0)
    {
        // évolution au cours du temps des civilisations
        evolveCivilizations(map, deltaTime);
    }
    else 
    {
        initializeCivilizations(map);
    }
}

void CivilisationSystem::initializeCivilizations(Map& map) 
{
    calculateAttractivity(map);
    int currentFactionId = 1;
    std::vector<Candidate> candidates;
    std::vector<Settlement> settlements;
    int width = static_cast<int>(map.getWidth());
    int height = static_cast<int>(map.getHeight());

    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            float attractivity = map.getGrid().get(x, y).attractivityLevel;
            if(attractivity > 0.0f) 
            {
                candidates.push_back({x, y, attractivity});
            }
        }
    }

    std::sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
        return a.attractivity > b.attractivity;
    });

    for (const Candidate& candidate : candidates) {
        if (settlements.size() >= static_cast<size_t>(numberOfFactions)) {
            break; 
        }

        bool isTooClose = false;

        for (const Settlement& existingSettlement : settlements) {
            // Utilisation de la distance de Manhattan (beaucoup plus rapide que Pythagore et suffisante sur une grille)
            int distance = std::abs(static_cast<int>(candidate.x) - static_cast<int>(existingSettlement.x)) +
                           std::abs(static_cast<int>(candidate.y) - static_cast<int>(existingSettlement.y));
            
            if (distance < MIN_DISTANCE) {
                isTooClose = true;
                break; 
            }
        }
        
        if (!isTooClose) {
            Settlement newCity;
            newCity.x = candidate.x;
            newCity.y = candidate.y;
            newCity.population = 100; // Population de départ
            newCity.factionId = currentFactionId++;
            std::cout << "Création de la civilisation " << newCity.factionId 
                      << " à (" << newCity.x << ", " << newCity.y << ") avec attractivité " 
                      << candidate.attractivity << std::endl;
            settlements.push_back(newCity);
            map.getGrid().get(candidate.x, candidate.y).isOccupied = true;
        }
    }
    map.setSettlements(settlements);
}


void CivilisationSystem::evolveCivilizations(Map& map, float deltaTime) 
{
    std::vector<Settlement>& settlements = map.getSettlements();
    std::vector<Settlement> newSettlements; // Liste d'attente pour les nouvelles villes

    for (Settlement& settlement : settlements) {
        
        // Lire le terrain sous la ville
        const Cell& currentCell = map.getGrid().get(settlement.x, settlement.y);
        
        // Calculer la limite de population (Carrying Capacity)
        int maxPopulation = static_cast<int>(CARRYING_CAPACITY_BASE * currentCell.attractivityLevel);

        // Appliquer la croissance démographique (indépendante du framerate grâce à deltaTime)
        settlement.population += static_cast<int>(settlement.population * GROWTH_RATE * deltaTime);

        // Gérer la surpopulation (Expansion)
        if (settlement.population > maxPopulation) {
            
            // Calculer le nombre d'habitants qui doivent partir
            int migrants = settlement.population - maxPopulation;
            settlement.population = maxPopulation; // La ville redescend à son maximum supportable

            // S'il y a assez de colons, on cherche une nouvelle case
            if (migrants >= MIN_MIGRANTS) {
                int bestX = -1;
                int bestY = -1;
                float bestScore = 0.0f;

                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) continue; // Ignorer le centre

                        int nx = settlement.x + dx;
                        int ny = settlement.y + dy;

                        // Vérifier qu'on ne sort pas de la carte
                        if (nx >= 0 && nx < map.getWidth() && ny >= 0 && ny < map.getHeight()) {
                            const Cell& neighborCell = map.getGrid().get(nx, ny);
                            
                            // Si la case est meilleure que ce qu'on a vu jusqu'ici
                            if (!neighborCell.isOccupied && neighborCell.attractivityLevel > bestScore) {
                                
                                // Vérifier qu'il n'y a pas déjà une ville sur cette case
                                bool isOccupied = false;
                                for (const auto& s : settlements) {
                                    if (s.x == nx && s.y == ny) { isOccupied = true; break; }
                                }
                                for (const auto& ns : newSettlements) { // Vérifier aussi la file d'attente
                                    if (ns.x == nx && ns.y == ny) { isOccupied = true; break; }
                                }

                                if (!isOccupied) {
                                    bestScore = neighborCell.attractivityLevel;
                                    bestX = nx;
                                    bestY = ny;
                                }
                            }
                        }
                    }
                }

                if (bestX != -1 && bestY != -1) {
                    Settlement newCity; 
                    newCity.x = bestX;
                    newCity.y = bestY;
                    newCity.population = migrants;
                    newCity.factionId = settlement.factionId;
                    std::cout << "Expansion de la civilisation " << newCity.factionId 
                              << " vers (" << newCity.x << ", " << newCity.y << ") avec population " 
                              << newCity.population << "Avec attractivité " << bestScore << std::endl;   
                    newSettlements.push_back(newCity);
                    map.getGrid().get(bestX, bestY).isOccupied = true;
                }
                // Si bestX reste -1, la région est saturée. Les migrants disparaissent (famine/mortalité).
            }
        }
    }

    // 6. Ajouter les nouvelles villes créées pendant ce tour à la carte globale
    if (!newSettlements.empty()) {
        settlements.insert(settlements.end(), newSettlements.begin(), newSettlements.end());
    }
}