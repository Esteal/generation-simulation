#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H

class Application;

class KeyboardHandler {
private:
    static inline int factionView = 0;
    static inline int currentFactionIndex = 0;
    static void focusOnNextCapital(Application& app);
public:
    /**
     * @brief Gère les événements utilisateurs.
     * * Capte les actions du clavier, de la souris et de la fenêtre (déplacement de caméra, zoom, pause, etc.).
     */
    static void keyboardInput(Application& app);
    static void displayControls();
    static void switchViewMode(Application& app, bool next);
};
#endif