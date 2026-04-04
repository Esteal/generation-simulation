#include "Map.h"
#include <iostream>

bool Map::testRegression() {
    std::cout << "[Test] Map... ";
    
    // Test d'initialisation
    Map m(10, 20);
    if (m.getWidth() != 10 || m.getHeight() != 20) {
        std::cerr << "Erreur: Dimensions incorrectes." << std::endl;
        return false;
    }
    
    // Test de modification et calcul d'altitude
    m.getGrid().get(5, 5).bedrock = 15.0f;
    m.getGrid().get(5, 5).granular = 5.0f;
    
    if (m.getAltitude(5, 5) != 20.0f) {
        std::cerr << "Erreur: Calcul d'altitude (bedrock + granular) incorrect." << std::endl;
        return false;
    }
    
    std::cout << "OK" << std::endl;
    return true;
}