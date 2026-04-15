/**
 * @file Camera2D.h
 * @brief Définition de la camera du monde.
 */
#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "Window.h"

/**
 * @class Camera2D
 * @brief Représente une caméra 2D pour la navigation dans le monde du jeu.
 * * Cette classe gère la position (x, y), le niveau de zoom et la vitesse de déplacement 
 * de la caméra, permettant de manipuler la vue dans un environnement 2D.
 */
class Camera2D {
private:
    float x, y;      /**< Le centre de la vue dans le monde (coordonnées X et Y). */
    float zoom;      /**< Le niveau de zoom actuel de la caméra. */
    float speed;     /**< La vitesse de déplacement de la caméra. */
    
public:
    /**
     * @brief Constructeur par défaut de Camera2D.
     * Initialise la position, le zoom et la vitesse avec des valeurs par défaut.
     */
    Camera2D();

    /**
     * @brief Destructeur de Camera2D.
     */
    ~Camera2D();

    /**
     * @brief Récupère la position X de la caméra.
     * @return La coordonnée X du centre de la vue.
     */
    float getX() const { return x; }

    /**
     * @brief Récupère la position Y de la caméra.
     * @return La coordonnée Y du centre de la vue.
     */
    float getY() const { return y; }

    /**
     * @brief Récupère la vitesse de la caméra.
     * @return La vitesse de déplacement actuelle.
     */
    float getSpeed() const { return speed; }

    /**
     * @brief Récupère le niveau de zoom de la caméra.
     * @return Le facteur de zoom actuel.
     */
    float getZoom() const { return zoom; }

    /**
     * @brief Déplace la caméra en ajoutant des valeurs à ses coordonnées.
     * @param deltaX La distance de déplacement sur l'axe X.
     * @param deltaY La distance de déplacement sur l'axe Y.
     */
    void move(float deltaX, float deltaY);

    /**
     * @brief Modifie le niveau de zoom global de la caméra.
     * @param newZoom Le nouveau facteur de zoom à appliquer.
     */
    void setZoom(float newZoom);

    /**
     * @brief Effectue un zoom ciblé en fonction de la position de la souris.
     * @param newZoom Le nouveau facteur de zoom à appliquer.
     * @param mouseX La position X de la souris (ou du point focal) sur l'écran.
     * @param mouseY La position Y de la souris (ou du point focal) sur l'écran.
     */
    void zoomAt(float newZoom, int mouseX, int mouseY);
    
    void setPosition(float newX, float newY);

    /** 
    * @brief Test de régression de la classe Camera2D.
     * Vérifie le bon fonctionnement de l'initialisation et des méthodes de modification.
     * @return true si tous les tests passent, false sinon.
     */
    static bool testRegression();
};

#endif