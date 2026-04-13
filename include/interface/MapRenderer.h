/**
 * @file MapRenderer.h
 * @brief Définition du rendu visuel en 2D de notre application.
 */

#ifndef MAP_RENDERER_H
#define MAP_RENDERER_H

#include "Map.h"
#include "Window.h"
#include "TextureManager.h"
#include "Camera2D.h"

enum class HeatmapType {
    HUMIDITY,
    TEMPERATURE,
    LIGHT,
    VEGETATION,
    ALTITUDE
};

/**
 * @class MapRenderer
 * @brief Gère le rendu visuel de la carte dans l'application.
 * * Cette classe s'occupe de dessiner la carte (Map) à l'écran en 2D, en tenant compte 
 * de la position de la caméra, des textures à appliquer, et de la position de la souris 
 * pour d'éventuelles interactions (comme la surbrillance d'une tuile).
 */
class MapRenderer {
private:
    /**
     * @brief Dessine un rectangle (surbrillance ou sélection) en fonction de la position de la souris.
     * * @param window La fenêtre de rendu cible.
     * @param map La carte contenant les données de l'environnement.
     * @param mouseX La position X actuelle de la souris à l'écran.
     * @param mouseY La position Y actuelle de la souris à l'écran.
     * @param camera La caméra 2D pour convertir les coordonnées de l'écran en coordonnées du monde.
     */
    void drawRectOnClick(Window &window, const Map &map,
                         const int &mouseX, const int &mouseY,
                         const Camera2D &camera);
    
    void renderSettlements(Window &window, const Camera2D &camera, const std::vector<Settlement> &settlements,
                            const size_t &windowWidth, const size_t &windowHeight);
    
    /**
     * @brief Obtient la couleur RGBA pour un type de matériau donné.
     * @param material Le type de matériau.
     * @param r Référence pour la composante rouge.
     * @param g Référence pour la composante verte.
     * @param b Référence pour la composante bleue.
     * @param a Référence pour la composante alpha (transparence).
     */
    void getMaterialColor(const Material& material, Uint8& r, Uint8& g, Uint8& b, Uint8& a) const;
                         
public:
    /**
     * @brief Constructeur par défaut.
     */
    MapRenderer() = default;

    /**
     * @brief Affiche la carte en 2D à l'écran.
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
     * @param spritesheetId L'identifiant de la spritesheet à utiliser pour le rendu des tuiles (par défaut "spritesheet").
     */
    void render2D(const Map& map, Window& window, TextureManager& textureManager,
                  const Camera2D& camera, const size_t &windowWidth, 
                  const size_t &windowHeight, const int &mouseX, const int &mouseY,
                  const std::string &spritesheetId);

    /**
     * @brief Affiche la carte sous forme de Heightmap (niveaux de gris) basée sur l'altitude.
     * @param map La carte contenant les données.
     * @param window La fenêtre de rendu.
     * @param camera La caméra pour la position et le zoom.
     * @param windowWidth Largeur de l'écran.
     * @param windowHeight Hauteur de l'écran.
     */
    void renderHeatmap(const Map& map, Window& window, const Camera2D& camera, 
                        const size_t &windowWidth, const size_t &windowHeight,
                        HeatmapType type);

    /**
     * @brief Effectue un test de régression sur la classe MapRenderer.
     * * @return bool true si le test passe, false sinon.
     */
    static bool testRegression();
};

#endif