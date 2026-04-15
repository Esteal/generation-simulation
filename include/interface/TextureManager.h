/**
 * @file TextureManager.h
 * @brief Déclaration de la classe TextureManager pour la gestion des ressources graphiques.
 */

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "Window.h"
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

/**
 * @class TextureManager
 * @brief Gestionnaire centralisé pour le chargement, le stockage et l'affichage des textures.
 * * Cette classe utilise une table de hachage pour stocker les textures SDL en mémoire. 
 * Elle garantit qu'une même image n'est chargée qu'une seule fois, optimisant ainsi 
 * l'utilisation de la mémoire et les performances de rendu.
 */
class TextureManager {
private:
    std::unordered_map<std::string, SDL_Texture*> textures; /**< Dictionnaire associant un identifiant textuel à un pointeur de texture SDL. */
    
public:
    /**
     * @brief Recherche le chemin absolu d'un fichier.
     * @param filePath Le chemin relatif du fichier.
     * @return Le chemin absolu du fichier ou nullptr en cas d'erreur.
     */
    std::string findPath(const std::string& filePath);

    /**
     * @brief Constructeur par défaut.
     */
    TextureManager() = default;

    /**
     * @brief Destructeur.
     * S'assure que toutes les textures sont correctement libérées de la mémoire à la destruction de l'objet.
     */
    ~TextureManager();

    /**
     * @brief Charge une texture à partir d'un fichier et l'associe à un identifiant unique. 
     * Si la texture est déjà chargée (identifiant existant), elle ne sera pas rechargée en mémoire.
     * * @param id L'identifiant unique pour la texture.
     * @param filePath Le chemin du fichier de l'image à charger.
     * @param window La référence à la fenêtre pour accéder au renderer.
     * @return true Si la texture a été chargée avec succès ou si elle était déjà en mémoire.
     * @return false Si une erreur est survenue lors du chargement de l'image.
     */
    bool load(const std::string& id, const std::string& filePath, Window& window);
    
    /**
     * @brief Dessine la texture associée à l'identifiant donné à une position et une taille spécifiées. 
     * Si la texture n'est pas trouvée, un message d'erreur est affiché.
     * * @param id L'identifiant unique de la texture à dessiner.
     * @param window La référence à la fenêtre pour accéder au renderer.
     * @param x La position en X où dessiner la texture.
     * @param y La position en Y où dessiner la texture.
     * @param w La largeur de la texture à dessiner.
     * @param h La hauteur de la texture à dessiner.
     */
    void draw(const std::string& id, Window& window, int x, int y, int w, int h);
    
    /**
     * @brief Dessine une portion spécifique d'une texture (idéal pour les spritesheets).
     * * @param id L'identifiant unique de la texture.
     * @param window La référence à la fenêtre pour accéder au renderer.
     * @param srcX Coordonnée X du point d'origine du découpage sur la texture source.
     * @param srcY Coordonnée Y du point d'origine du découpage sur la texture source.
     * @param srcW Largeur de la portion d'image à découper.
     * @param srcH Hauteur de la portion d'image à découper.
     * @param destX Coordonnée X de la position d'affichage sur la fenêtre.
     * @param destY Coordonnée Y de la position d'affichage sur la fenêtre.
     * @param destW Largeur finale de l'image dessinée à l'écran (permet d'étirer ou de rétrécir).
     * @param destH Hauteur finale de l'image dessinée à l'écran (permet d'étirer ou de rétrécir).
     */
    void drawFrame(const std::string& id, Window& window, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH);
    
    /**
     * @brief Libère toutes les textures chargées en mémoire vidéo et vide le dictionnaire.
     */
    void clean();

    /**
     * @brief Effectue un test de régression sur la classe TextureManager.
     * * @return bool true si le test passe, false sinon.
     */
    static bool testRegression();
};

#endif