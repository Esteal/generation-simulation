#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H

/**
 * @file KeyboardHandler.h
 * @brief Gestion des entrées clavier/souris et commandes d'interface utilisateur.
 *
 * Fournit des utilitaires statiques pour capturer les actions utilisateur (clavier,
 * souris, fenêtre) et déclencher des comportements globaux de l'application
 * (déplacement de la caméra, changement de mode de vue, navigation entre capitales, affichage de l'aide).
 */

class Application;

/**
 * @class KeyboardHandler
 * @brief Gestionnaire statique des entrées utilisateur et raccourcis.
 *
 * Classe utilitaire ne produisant pas d'instances : toutes les méthodes sont statiques.
 * Elle conserve un petit état interne (mode de vue et index de faction) pour permettre
 * la navigation cyclique entre factions et le focus sur les capitales.
 */
class KeyboardHandler {
private:
    static inline int factionView = 0;          /**< Mode de vue actif pour les factions (0 = vue standard). */
    static inline int currentFactionIndex = 0;  /**< Index de la faction actuellement ciblée par la caméra/affichage. */

    /**
     * @brief Centre la caméra sur la capitale suivante.
     *
     * Méthode utilitaire interne qui parcourt la liste des factions accessibles via
     * l'objet Application et recentre la caméra sur la capitale suivante.
     *
     * @param app Référence à l'instance Application utilisée pour accéder aux données et à la caméra.
     */
    static void focusOnNextCapital(Application& app);

public:
    /**
     * @brief Traite les événements d'entrée utilisateur.
     *
     * Cette méthode doit être appelée depuis la boucle principale d'événements.
     * Elle gère :
     * - la lecture des touches/clique/souris,
     * - le déplacement et le zoom de la caméra,
     * - la bascule des modes de vue et l'exécution d'actions rapides (focus capitale, pause, etc.).
     *
     * @param app Référence à l'application courante sur laquelle appliquer les commandes.
     */
    static void keyboardInput(Application& app);

    /**
     * @brief Affiche l'aide des contrôles disponibles.
     *
     * Affiche (console/overlay selon implémentation) la liste des raccourcis clavier et
     * des commandes utilisateur pour l'interface.
     */
    static void displayControls();

    /**
     * @brief Change le mode de vue courant.
     *
     * Permet de passer cycliquement au mode de vue suivant ou précédent.
     *
     * @param app Référence à l'application courante.
     * @param next Si true passe au mode suivant, sinon passe au mode précédent.
     */
    static void switchViewMode(Application& app, bool next);
};

#endif