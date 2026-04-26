/**
 * @file Chunk.h
 * @brief Gestion hiérarchique des portions de la carte (BVH).
 *
 * La classe Chunk représente une partition rectangulaire de la carte utilisée pour
 * accélérer le rendu et les tests de visibilité (AABB). L'arbre est construit
 * récursivement et chaque nœud contient une boîte englobante en pixels ainsi que
 * des pointeurs vers deux enfants (ou nul si feuille).
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
    int minX;   /**< Indice X minimal (inclus) de la zone couverte par ce chunk. */
    int maxX;   /**< Indice X maximal (inclus) de la zone couverte par ce chunk. */
    int minY;   /**< Indice Y minimal (inclus) de la zone couverte par ce chunk. */
    int maxY;   /**< Indice Y maximal (inclus) de la zone couverte par ce chunk. */

    // --- Boîte englobante en pixels (Axis-Aligned Bounding Box) ---
    float minPX; /**< Borne gauche de la boîte englobante en pixels. */
    float maxPX; /**< Borne droite de la boîte englobante en pixels. */
    float minPY; /**< Borne supérieure/inférieure dépendant du système de coordonnées (pixels) ; inclut altitude. */
    float maxPY; /**< Borne opposée de minPY (pixels). */

    // --- Enfants dans l'arbre binaire ---
    Chunk* left;   /**< Pointeur vers le sous-chunk gauche (ou nullptr si feuille). */
    Chunk* right;  /**< Pointeur vers le sous-chunk droit (ou nullptr si feuille). */

    float seaLevel; /**< Niveau de la mer utilisé pour ce chunk (même unité que les coordonnées en pixels). */

public:
    /**
     * @brief Constructeur récursif qui génère l'arbre de chunks.
     *
     * Sépare la région fournie en deux enfants jusqu'à atteindre une condition de feuille.
     * computeAABB(map) est appelé pour initialiser la boîte englobante en pixels.
     *
     * @param x1 Indice X de début (inclus).
     * @param x2 Indice X de fin (inclus).
     * @param y1 Indice Y de début (inclus).
     * @param y2 Indice Y de fin (inclus).
     * @param map Référence vers la carte servant à déterminer les altitudes/valeurs nécessaires au calcul des limites.
     */
    Chunk(int x1, int x2, int y1, int y2, const Map& map);

    /**
     * @brief Destructeur récursif libérant toute la hiérarchie.
     *
     * Supprime proprement les enfants pour éviter les fuites mémoire.
     */
    ~Chunk();

    /**
     * @brief Indique si ce chunk est une feuille (pas d'enfants).
     * @return true si le chunk n'a pas d'enfants, false sinon.
     */
    bool isLeaf() const { return left == nullptr && right == nullptr; }

    /**
     * @brief Vérifie si la boîte englobante du chunk est visible par la caméra.
     *
     * Effectue un test AABB simple entre la boîte du chunk et la fenêtre de vue
     * fournie par la caméra (position + dimensions).
     *
     * @param camera Référence à la caméra 2D courante.
     * @param windowWidth Largeur de la fenêtre (pixels) utilisée pour le test de visibilité.
     * @param windowHeight Hauteur de la fenêtre (pixels) utilisée pour le test de visibilité.
     * @return true si le chunk intersecte la fenêtre de visualisation, false sinon.
     */
    bool isVisible(const Camera2D& camera, float windowWidth, float windowHeight) const;

    /**
     * @brief Affiche la structure de l'arbre pour debug.
     *
     * Imprime récursivement la hiérarchie des chunks avec un niveau d'indentation
     * correspondant à `depth`.
     *
     * @param depth Niveau d'indentation / profondeur pour l'affichage.
     */
    void debugPrint(int depth) const;

    /**
     * @brief Rend (ou ordonne le rendu) du contenu du chunk.
     *
     * Si le chunk est visible, la méthode dessine les tuiles correspondantes ou délègue
     * l'appel aux enfants. Utilise TextureManager pour dessiner depuis un spritesheet.
     *
     * @param window Référence à la fenêtre de rendu.
     * @param tm Référence au gestionnaire de textures / sprites.
     * @param cam Référence à la caméra pour convertir coordonnées/visibilité.
     * @param map Référence à la carte contenant les cellules à dessiner.
     * @param spritesheetID Identifiant de la spritesheet à utiliser pour le rendu.
     * @param winW Largeur de la fenêtre en pixels (utile pour optimisation du rendu).
     * @param winH Hauteur de la fenêtre en pixels (utile pour optimisation du rendu).
     */
    void render(Window& window, TextureManager& tm, const Camera2D& cam,
                const Map& map, const std::string& spritesheetID,
                int winW, int winH);

private:
    /**
     * @brief Calcule les limites spatiales (pixels) du chunk à partir de la carte.
     *
     * Parcourt la zone couverte par le chunk dans la Map pour déterminer les
     * bornes min/max en pixels (X et Y) et initialise seaLevel si nécessaire.
     *
     * @param map Référence à la carte permettant de convertir indices en coordonnées pixels/altitude.
     */
    void computeAABB(const Map& map);

    // Empêcher la copie de l'arbre pour éviter les doubles suppressions de pointeurs
    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;
};

#endif // CHUNK_H