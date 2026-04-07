/**
 * @file Chunk.h
 * @brief Gestion hiérarchique des portions de la carte (BVH).
 */

#ifndef CHUNK_H
#define CHUNK_H

#include "Camera2D.h"  
#include "Map.h"     // Pour cam.getX()
#include "TextureManager.h" // Pour tm.drawFrame()
#include "Window.h"         // Pour passer l'objet window
#include "Cell.h"           // IMPORTANT : pour BiomeIndex et cell.biome
#include <algorithm>
#include <iostream>

class Chunk {
private:
    // --- Coordonnées dans la grille (indices logiques) ---
    int minX, maxX;
    int minY, maxY;

    // --- Boîte englobante en pixels (Axis-Aligned Bounding Box) ---
    float minPX, maxPX; // Bornes horizontales (gauche/droite)
    float minPY, maxPY; // Bornes verticales (haut/bas, incluant l'altitude)

    // --- Enfants dans l'arbre binaire ---
    Chunk* left;
    Chunk* right;

public:
    /**
     * @brief Constructeur récursif qui génère l'arbre de chunks.
     * @param x1 Indice X de début
     * @param x2 Indice X de fin
     * @param y1 Indice Y de début
     * @param y2 Indice Y de fin
     * @param map Référence vers la carte pour le calcul des altitudes
     */
    Chunk(int x1, int x2, int y1, int y2, const Map& map);

    /**
     * @brief Destructeur récursif libérant toute la hiérarchie.
     */
    ~Chunk();

    /**
     * @brief Indique si ce chunk est une feuille (pas d'enfants).
     */
    bool isLeaf() const { return left == nullptr && right == nullptr; }

    /**
     * @brief Vérifie si la boîte englobante du chunk est visible par la caméra.
     */
    bool isVisible(const Camera2D& camera, float windowWidth, float windowHeight) const;

    void debugPrint(int depth) const;

    void render(Window& window, TextureManager& tm, const Camera2D& cam, 
                  const Map& map, const std::string& spritesheetID,
                  int winW, int winH) ;

private:
    /**
     * @brief Calcule les limites spatiales (pixels) du chunk à partir de la carte.
     */
    void computeAABB(const Map& map);

    // Empêcher la copie de l'arbre pour éviter les doubles suppressions de pointeurs
    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;
};

#endif // CHUNK_H