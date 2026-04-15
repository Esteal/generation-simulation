/**
 * @file Application.h
 * @brief Déclaration de la classe principale Application.
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include "TextureManager.h"
#include "Camera2D.h"
#include "MapRenderer2DIso.h"
#include "TextMapRenderer.h"

#include "MapGenerator.h"
#include "system/WorldSimulator.h"
#include "system/HydrologieSystem.h"
#include "system/HydrolicErosionSystem.h"
#include "system/ThermalErosionSystem.h"
#include "system/LightSystem.h"
#include "system/VegetationSystem.h"
#include "system/civilisation/CivilisationSystem.h"
#include "system/MineralSystem.h"
#include "system/AgricultureSystem.h"

enum class ViewMode {
        ISOMETRIC,
        NORMAL_2D,
        HEATMAP_VEGETATION,
        HEATMAP_HUMIDITY,
        HEATMAP_TEMPERATURE,
        HEATMAP_LIGHT,
        HEATMAP_ALTITUDE
    };

/**
 * @class Application
 * @brief Classe principale orchestrant l'ensemble de la simulation.
 * * Cette classe agit comme le chef d'orchestre du projet. Elle initialise
 * la fenêtre, la carte, le générateur de terrain, le simulateur physique
 * et les systèmes de rendu. Elle contient également la boucle principale
 * du programme qui gère les événements clavier, la mise à 
 * jour de la simulation et le rendu graphique.
 */
class Application
{
friend class KeyboardHandler;
private:
    bool isRunning;                     /**< Indique si la boucle principale est en cours d'exécution. */
    
    Window window;                      /**< Gestionnaire de la fenêtre SDL. */
    MapRenderer2DIso mapRendererIso;    /**< Moteur de rendu 2D isométrique. */
    MapRenderer mapRenderer;            /**< Moteur de rendu 2D classique. */
    ViewMode currentViewMode = ViewMode::ISOMETRIC; /**< Mode de vue actuel (2D, isométrique, heatmap, etc.). */
    TextureManager textureManager;      /**< Gestionnaire des textures et ressources graphiques. */
    Camera2D camera;                    /**< Caméra permettant de naviguer dans la carte. */
    size_t width, height;               /**< Dimensions de la fenêtre de rendu. */

    Map map;                            /**< La carte topographique contenant toutes les données du terrain. */
    WorldSimulator worldSimulator;      /**< Gestionnaire d'exécution des différents systèmes géologiques. */
    MapGenerator generator;             /**< Générateur procédural du terrain initial. */
    TextMapRenderer textMapRenderer;    /**< Moteur de rendu textuel (console) de la carte. */
    size_t age = 0;                     /**< Compteur d'années ou de cycles écoulés dans la simulation. */

public:
    /**
     * @brief Constructeur de l'application.
     * * Initialise l'environnement, génère la carte initiale et applique 
     * les premières phases de simulation.
     * * @param width Largeur de la fenêtre d'affichage (en pixels).
     * @param height Hauteur de la fenêtre d'affichage (en pixels).
     * @param mapWidth Largeur de la carte générée (en nombre de cellules).
     * @param mapHeight Hauteur de la carte générée (en nombre de cellules).
     * @param seed Graine de génération procédurale pour assurer un terrain déterministe.
     */
    Application(size_t width, size_t height, size_t mapWidth, size_t mapHeight, int seed);
    
    /**
     * @brief Lance la boucle principale de l'application.
     * * Gère en continu les entrées utilisateur, la mise à jour de la logique de simulation 
     * et le rendu graphique à l'écran, jusqu'à la demande de fermeture de la fenêtre.
     */
    void run();
    
    /**
     * @brief Met à jour l'état de la simulation dans le temps.
     * * Gère la fréquence des mises à jour géologiques en fonction du temps écoulé.
     * * @param[in,out] currentTime Le temps actuel (en millisecondes depuis le lancement de la SDL).
     * @param[in,out] lastUpdateTime Le moment de la dernière mise à jour de la simulation.
     */
    void simulate(Uint32 &currentTime, Uint32 &lastUpdateTime);
        
    /**
     * @brief Test de régression de la classe Application (Test d'intégration).
     * * Instancie une application complète avec une petite carte de test pour vérifier
     * que tous les systèmes parviennent à communiquer sans erreur critique ou fuite mémoire.
     * * @return true si l'instanciation et la simulation réussissent, false sinon.
     */
    static bool testRegression();
};

#endif