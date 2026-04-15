/**
 * @file Window.h
 * @brief Déclaration de la classe Window gérant la fenêtre principale et le rendu SDL.
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

/**
 * @class Window
 * @brief Gère le cycle de vie de la fenêtre d'application et son moteur de rendu (renderer) via SDL2.
 * * Cette classe encapsule la création de la fenêtre SDL, l'initialisation du rendu graphique,
 * ainsi que les opérations de base de dessin et de gestion de l'écran (effacer, rafraîchir).
 */
class Window
{
private:
    SDL_Window* window;       /**< Pointeur vers la fenêtre SDL créée. */
    SDL_Renderer* renderer;   /**< Pointeur vers le moteur de rendu associé à la fenêtre. */
    bool isInitialized;       /**< Indique si la fenêtre et la SDL ont été initialisées avec succès. */

public:
    /**
     * @brief Indique si la fenêtre et le renderer ont été créés avec succès.
     * @return true si l'initialisation a réussi, false sinon.
     */
    bool getIsInitialized() const { return isInitialized; }

    /**
     * @brief Constructeur de la classe Window.
     * Initialise les sous-systèmes de la SDL, crée la fenêtre et le renderer associé.
     * * @param title Le titre qui s'affichera dans la barre de la fenêtre.
     * @param width La largeur de la fenêtre en pixels.
     * @param height La hauteur de la fenêtre en pixels.
     */
    Window(const std::string &title, const int &width, const int &height);

    /**
     * @brief Destructeur de la classe Window.
     * Libère proprement les ressources SDL (renderer, window) et quitte les sous-systèmes.
     */
    ~Window();

    /**
     * @brief Récupère une référence vers le moteur de rendu (renderer).
     * * @return SDL_Renderer& Une référence au renderer SDL actuel, utile pour dessiner à l'extérieur de la classe.
     */
    SDL_Renderer& getRenderer();

    /**
     * @brief Efface le contenu actuel de la fenêtre.
     * À appeler généralement au tout début de chaque itération de la boucle de rendu.
     */
    void clear();

    /**
     * @brief Met à jour l'écran avec les éléments dessinés depuis le dernier effacement.
     * À appeler à la fin de chaque itération de la boucle de rendu (l'équivalent de SDL_RenderPresent).
     */
    void present();

    /**
     * @brief Charge une image depuis un fichier et la convertit en texture exploitable par le renderer.
     * * @param filePath Le chemin vers le fichier image (ex: "assets/image.png").
     * @return SDL_Texture* Un pointeur vers la texture chargée, ou nullptr en cas d'échec.
     */
    SDL_Texture* loadTexture(const std::string& filePath);

    /**
     * @brief Dessine l'intégralité d'une texture à une position et une taille données sur l'écran.
     * * @param texture La texture SDL à dessiner.
     * @param x La position en X (coin supérieur gauche) sur la fenêtre.
     * @param y La position en Y (coin supérieur gauche) sur la fenêtre.
     * @param w La largeur d'affichage cible sur l'écran.
     * @param h La hauteur d'affichage cible sur l'écran.
     */
    void drawTexture(SDL_Texture* texture, int x, int y, int w, int h);

    /**
     * @brief Dessine une portion spécifique d'une texture vers une zone de destination sur l'écran.
     * Très utile pour isoler et afficher une frame précise à partir d'une feuille de sprites (spritesheet).
     * * @param texture La texture SDL source.
     * @param srcX La coordonnée X du coin supérieur gauche de la zone source à découper.
     * @param srcY La coordonnée Y du coin supérieur gauche de la zone source à découper.
     * @param srcW La largeur de la zone source à découper.
     * @param srcH La hauteur de la zone source à découper.
     * @param destX La position X sur la fenêtre (destination).
     * @param destY La position Y sur la fenêtre (destination).
     * @param destW La largeur d'affichage finale sur la fenêtre.
     * @param destH La hauteur d'affichage finale sur la fenêtre.
     */
    void drawTexture(SDL_Texture* texture, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH);

    /**
     * @brief Effectue un test de régression sur la classe Window.
     * * @return bool true si le test passe, false sinon.
     */
    static bool testRegression();

};

#endif