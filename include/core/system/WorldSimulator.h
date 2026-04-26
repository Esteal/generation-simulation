/**
 * @file WorldSimulator.h
 * @brief Définition du gestionnaire global de la simulation du monde.
 */

#ifndef WORLDSIMULATOR_H
#define WORLDSIMULATOR_H

#include "Map.h"
#include "ISimulationSystem.h"
#include <vector>
#include <memory>

/**
 * @class WorldSimulator
 * @brief Orchestrateur de la simulation procédurale.
 * * Cette classe gère l'exécution séquentielle de tous les systèmes physiques et géologiques 
 * (érosion, hydrologie, végétation, etc.). Elle agit comme le "moteur" du monde, permettant 
 * de contrôler l'écoulement du temps (mise en pause, accélération) et d'appliquer ces 
 * règles sur la topologie de la carte.
 */
class WorldSimulator {
private:
    /** @brief Liste des systèmes de simulation à exécuter à chaque mise à jour. */
    std::vector<std::unique_ptr<ISimulationSystem>> systems;
    
    /** @brief État de la simulation (en pause ou en cours d'exécution). */
    bool isPaused;
    
    /** @brief Multiplicateur de vitesse de la simulation (ex: 2.0 pour avancer deux fois plus vite). */
    float simulationSpeed;

public:
    /**
     * @brief Constructeur par défaut.
     * Initialise la simulation en cours de lecture (non en pause) avec une vitesse de base de 1.0f.
     */
    WorldSimulator(bool paused = false, float speed = 1.0f) : isPaused(paused), simulationSpeed(speed) {};

    /**
     * @brief Ajoute un nouveau système à la chaîne de simulation.
     * * L'ordre d'ajout est important, car les systèmes seront exécutés séquentiellement 
     * dans l'ordre de cette liste .
     * @param system Pointeur vers le système de simulation à enregistrer.
     */
    void addSystem(std::unique_ptr<ISimulationSystem> system);

    /**
     * @brief Supprime un système de la chaîne de simulation.
     * * Permet de retirer un système spécifique de la liste d'exécution. 
     * @param system Pointeur vers le système de simulation à supprimer.
     */
    void removeSystem(ISimulationSystem* system);

    ISimulationSystem* getSystem(size_t index) { return systems[index].get(); }
    /**
     * @brief Modifie la vitesse globale de la simulation.
     * @param speed Le nouveau multiplicateur de vitesse.
     */
    void setSpeed(float speed);

    /**
     * @brief Modifie l'état de la simulation (pause/lecture).
     * @param paused True pour figer le monde, False pour relancer l'écoulement du temps.
     */
    void setPause(bool paused);

    /**
     * @brief Indique si la simulation est actuellement en pause.
     * @return True si la simulation est suspendue, False si elle est active.
     */
    bool isPause() const { return isPaused; };

    /**
     * @brief Met à jour le monde en exécutant tous les systèmes enregistrés.
     * * Si la simulation n'est pas en pause, cette méthode parcourt la liste des systèmes 
     * et appelle leur méthode process(). Le temps alloué à chaque système est calculé 
     * en multipliant le \p delta_time par la \p simulationSpeed.
     * * @param map La carte du monde à simuler et à modifier.
     * @param delta_time Le temps réel écoulé depuis la dernière mise à jour.
     */
    void update(Map& map, float delta_time);

    /**
     * @brief Effectue un test de régression pour valider le comportement de la carte.
     * @return true si tous les tests passent, false sinon.
     */
    static bool testRegression();
};

#endif // WORLDSIMULATOR_H