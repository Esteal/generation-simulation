/**
 * @file MapRenderer2DIso.h
 * @brief Définition du rendu visuel en 2D isométric de notre application.
 */
#ifndef MAPRENDERER2DISO_H
#define MAPRENDERER2DISO_H
#include "MapRenderer.h"
#include "Chunk.h"
#include <memory>

/**
 * @class MapRenderer2DIso
 * @brief Gère le rendu visuel en 2D isométric de la carte dans l'application.
 * * Cette classe s'occupe de dessiner la carte (Map) à l'écran en 2D isométric, en tenant compte 
 * de la position de la caméra, des textures à appliquer, et de la position de la souris 
 * pour d'éventuelles interactions (comme la surbrillance d'une tuile mais ça c'est pas fait).
 */
class MapRenderer2DIso : public MapRenderer {

public:
/**
     * @brief Affiche la carte en 2D iso à l'écran.
     * * Cette méthode parcourt les éléments visibles de la carte et les dessine dans la fenêtre
     * en utilisant le gestionnaire de textures et la vue de la caméra.
     * * @param map La carte à rendre.
     * @param window La fenêtre dans laquelle le dessin est effectué.
     * @param textureManager Le gestionnaire contenant les ressources graphiques chargées.
     * @param camera La caméra 2D déterminant la zone visible actuelle.
     * @param windowWidth La largeur de la fenêtre (ou de la zone de rendu).
     * @param windowHeight La hauteur de la fenêtre (ou de la zone de rendu).
     * @param mouseX La position X de la souris, utile pour les effets de survol.
     * @param mouseY La position Y de la souris, utile pour les effets de survol.
     * @param spritesheetID L'ID du spritesheet à utiliser pour le rendu.
     */
    void render2D(const Map& map, Window& window, TextureManager& textureManager,
                  const Camera2D& camera, const size_t &windowWidth, 
                  const size_t &windowHeight, const int &mouseX, const int &mouseY,
                  const std::string &spritesheetID);


    /**
     * @brief Effectue un test de régression sur la classe MapRenderer2DIso.
     * * @return bool true si le test passe, false sinon.
     */
    static bool testRegression();
private:
    std::unique_ptr<Chunk> rootChunk;
};

#endif

