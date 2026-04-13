/**
 * @file Map.h
 * @brief Définition de la classe représentant la carte du monde.
 */

#ifndef MAP_H
#define MAP_H

#include "Grid.h"
#include "Cell.h"
#include "Faction.h"

/**
 * @class Map
 * @brief Représente la carte du monde simulé.
 * * Cette classe agit comme un conteneur principal pour la grille de cellules (Grid). 
 * Elle fournit des interfaces d'accès simplifiées aux dimensions de la carte et 
 * permet de calculer dynamiquement des données topographiques.
 */
class Map
{
private:
    /** @brief Grille 2D contenant toutes les cellules (tuiles) du monde. */
    Grid<Cell> grid;
    std::vector<Faction> factions; /**< Liste de toutes les villes actives sur la carte */

public:
    /**
     * @brief Constructeur de la carte.
     * Initialise la grille interne avec les dimensions spécifiées.
     * @param width Largeur de la carte (en nombre de cellules).
     * @param height Hauteur de la carte (en nombre de cellules).
     */
    Map(size_t width, size_t height) : grid(width, height) {}
    
    /**
     * @brief Retourne la grille de la carte, accès en lecture et écriture.
     * @return Référence vers la grille (permet de modifier les cellules).
     */
    Grid<Cell>& getGrid() { return grid; }
    
    /**
     * @brief Retourne la grille de la carte, accès en lecture seule.
     * @return Référence constante vers la grille.
     */
    const Grid<Cell>& getGrid() const { return grid; }

    /**
     * @brief Récupère la largeur de la carte.
     * @return La largeur de la grille en nombre de cellules.
     */
    size_t getWidth() const { return grid.getWidth(); }

    /**
     * @brief Récupère la hauteur de la carte.
     * @return La hauteur de la grille en nombre de cellules.
     */
    size_t getHeight() const { return grid.getHeight(); }

    /**
     * @brief Calcule et retourne l'altitude géologique totale d'une cellule spécifique.
     * * L'altitude réelle (physique) correspond à l'empilement de la couche de roche mère 
     * (bedrock) et de la couche de sédiments/terre meuble (granular).
     * * @param x Coordonnée X de la cellule.
     * @param y Coordonnée Y de la cellule.
     * @return L'altitude totale de la cellule à ces coordonnées.
     */
    float getAltitude(size_t x, size_t y) const { return grid.get(x, y).bedrock + grid.get(x, y).granular; }


    std::vector<Faction>& getFactions() { return factions ;}
    const std::vector<Faction>& getFactions() const { return factions; }
    void setFactions(const std::vector<Faction>& newFactions) { factions = newFactions; }

    /**
     * @brief Effectue un test de régression pour valider le comportement de la carte.
     * @return true si tous les tests passent, false sinon.
     */
    static bool testRegression();
};

#endif // MAP_H