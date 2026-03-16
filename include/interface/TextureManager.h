#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "Window.h"
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include <iostream>

using namespace std;

class TextureManager {
private:
    unordered_map<string, SDL_Texture*> textures;
public:
    TextureManager() = default;
    ~TextureManager();
    /**
     * @brief Charge une texture à partir d'un fichier et l'associe à un identifiant unique. Si la texture est déjà chargée, elle ne sera pas rechargée en mémoire.
     * 
     * @param id L'identifiant unique pour la texture
     * @param filePath Le chemin du fichier de l'image à charger
     * @param window La référence à la fenêtre pour accéder au renderer
     * @return true 
     * @return false 
     */
    bool load(const string& id, const string& filePath, Window& window);
    
    /**
     * @brief Dessine la texture associée à l'identifiant donné à une position et une taille spécifiées sur la fenêtre. Si la texture n'est pas trouvée, un message d'erreur est affiché.
     * 
     * @param id L'identifiant unique pour la texture
     * @param window La référence à la fenêtre pour accéder au renderer
     * @param x La position en x où dessiner la texture
     * @param y La position en y où dessiner la texture
     * @param w La largeur de la texture à dessiner
     * @param h La hauteur de la texture à dessiner
     */
    void draw(const string& id, Window& window, int x, int y, int w, int h);
    
    /**
     * @brief Dessine une portion spécifique d'une texture (idéal pour les spritesheets).     * 
     * @param id L'identifiant unique pour la texture
     * @param window La référence à la fenêtre pour accéder au renderer
     * @param srcX Coordonnée X du point d'origine du découpage sur la texture source.
     * @param srcY Coordonnée Y du point d'origine du découpage sur la texture source.
     * @param srcW Largeur de la portion d'image à découper.
     * @param srcH Hauteur de la portion d'image à découper.
     * @param destX Coordonnée X de la position d'affichage sur la fenêtre.
     * @param destY Coordonnée Y de la position d'affichage sur la fenêtre.
     * @param destW Largeur finale de l'image dessinée à l'écran (permet d'étirer ou rétrécir).
     * @param destH Hauteur finale de l'image dessinée à l'écran (permet d'étirer ou rétrécir).
     */
    void drawFrame(const string& id, Window& window, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH);
    
    /**
     * @brief Libère toutes les textures chargées en mémoire et vide la map des textures.
     * 
     */
    void clean();
};

#endif