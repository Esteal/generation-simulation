#ifndef GRID_H
#define GRID_H

#include <vector>
#include <iostream>
#include <cassert>

/**
 * @file Grid.h
 * @brief Déclaration de la classe Grid
 * 
 * Cette classe représente une grille de cellules générique, permettant de stocker et manipuler des données de type T.
 * Elle fournit des méthodes pour accéder et modifier les cellules, ainsi que pour obtenir les dimensions de la grille.
 */

template<typename T>
/**
 * @class Grid
 * @brief Représente une grille de cellules générique
 * 
 * Cette classe template permet de créer une grille de n'importe quel type de cellule.
 * Elle gère les dimensions de la grille et fournit des méthodes pour accéder et modifier les cellules.
 * La grille est stockée en mémoire sous forme linéaire et indexée en row-major order (Y * width + X).
 * 
 * @tparam T Le type de données stocké dans chaque cellule de la grille
 */
class Grid
{
private:
    size_t width;           /**< Largeur de la grille (nombre de colonnes) */
    size_t height;          /**< Hauteur de la grille (nombre de lignes) */
    std::vector<T> tab;     /**< Stockage linéaire des cellules en row-major order */

public:
    /**
     * @brief Constructeur par défaut
     * 
     * Initialise une grille vide avec une largeur et hauteur de 0.
     */
    Grid() : width(0), height(0) {}

    /**
     * @brief Constructeur avec dimensions
     * 
     * Initialise une grille avec les dimensions spécifiées.
     * 
     * @param w Largeur de la grille (nombre de colonnes)
     * @param h Hauteur de la grille (nombre de lignes)
     */
    Grid(size_t w, size_t h) : width(w), height(h), tab(w*h) {}

    /**
     * @brief Accède à une cellule en lecture/écriture
     * 
     * @param x Coordonnée X (colonne) de la cellule
     * @param y Coordonnée Y (ligne) de la cellule
     * @return Référence mutable à la cellule situé à (x, y)
     */
    T& get(size_t x, size_t y)
    {
        return tab[y * width + x];
    }

    /**
     * @brief Accède à une cellule en lecture seule
     * 
     * @param x Coordonnée X (colonne) de la cellule
     * @param y Coordonnée Y (ligne) de la cellule
     * @return Référence constante à la cellule situé à (x, y)
     */
    const T& get(size_t x, size_t y) const
    {
        return tab[y * width + x];
    }

    /**
     * @brief Modifie la valeur d'une cellule
     * 
     * @param x Coordonnée X (colonne) de la cellule
     * @param y Coordonnée Y (ligne) de la cellule
     * @param value Nouvelle valeur à assigner à la cellule
     */
    void set(size_t x, size_t y, const T& value)
    {
        tab[y * width + x] = value;
    }

    /**
     * @brief Opérateur d'accès en lecture/écriture
     * 
     * Fournit un accès pratique aux cellules de la grille.
     * 
     * @param x Coordonnée X (colonne) de la cellule
     * @param y Coordonnée Y (ligne) de la cellule
     * @return Référence mutable à la cellule situé à (x, y)
     */
    T& operator()(size_t x, size_t y)
    { 
        return tab[y * width + x]; 
    }

    /**
     * @brief Opérateur d'accès en lecture seule
     * 
     * @param x Coordonnée X (colonne) de la cellule
     * @param y Coordonnée Y (ligne) de la cellule
     * @return Référence constante à la cellule situé à (x, y)
     */
    const T& operator()(size_t x, size_t y) const 
    { 
        return tab[y * width + x]; 
    }

    /**
     * @brief Obtient la largeur de la grille
     * 
     * @return Largeur (nombre de colonnes) de la grille
     */
    size_t getWidth() const { return width; }

    /**
     * @brief Obtient la hauteur de la grille
     * 
     * @return Hauteur (nombre de lignes) de la grille
     */
    size_t getHeight() const { return height; }

    /**
     * @brief Fonction de test de régression pour la classe Grid
     * 
     * Teste les fonctionnalités principales de la classe Grid :
     * - Constructeurs et dimensions
     * - Accès et modification des cellules
     * - Opérateurs de parenthèse
     * - Copie profonde des données
     * - Généricité avec différents types
     * - Accès const
     * 
     * @return true si tous les tests sont passés, false sinon
     */
    static bool testRegression()
    {
        std::cout << "[Test] Grid (Template)... ";

        // 1. Test des constructeurs et dimensions
        Grid<int> empty;
        if (empty.getWidth() != 0 || empty.getHeight() != 0) {
            std::cerr << "\nErreur: Le constructeur vide n'initialise pas les dimensions a 0." << std::endl;
            return false;
        }

        size_t w = 10, h = 5;
        Grid<int> g(w, h);
        if (g.getWidth() != w || g.getHeight() != h) {
            std::cerr << "\nErreur: Le constructeur avec parametres n'initialise pas les bonnes dimensions." << std::endl;
            return false;
        }

        // 2. Test de l'accès et de l'opérateur parenthèse
        g.set(5, 2, 42);
        if (g.get(5, 2) != 42) {
            std::cerr << "\nErreur: Les methodes get() ou set() ne fonctionnent pas correctement." << std::endl;
            return false;
        }

        if (g(5, 2) != 42) {
            std::cerr << "\nErreur: L'operateur de lecture () ne renvoie pas la bonne valeur." << std::endl;
            return false;
        }

        // 3. Test de la modification via l'opérateur ()
        g(5, 2) = 100; 
        if (g.get(5, 2) != 100) {
            std::cerr << "\nErreur: L'operateur de modification () ne met pas a jour la valeur." << std::endl;
            return false;
        }

        // 4. Test de la copie (Deep Copy via le std::vector)
        Grid<int> copy = g; 
        if (copy.get(5, 2) != 100) {
            std::cerr << "\nErreur: La copie de la grille a echoue." << std::endl;
            return false;
        }
        
        copy.set(5, 2, 7);
        if (g.get(5, 2) != 100) { 
            std::cerr << "\nErreur: La copie n'est pas independante (Shallow Copy au lieu de Deep Copy)." << std::endl;
            return false;
        }

        // 5. Test de la généricité (avec un type complexe)
        Grid<std::string> msg(2, 2);
        msg.set(0, 0, "Hello");
        msg.set(1, 1, "World");
        if (msg.get(0, 0) != "Hello" || msg.get(1, 1) != "World") {
            std::cerr << "\nErreur: La genericite (template avec std::string) ne fonctionne pas correctement." << std::endl;
            return false;
        }

        // 6. Test des accès const
        const Grid<int>& const_g = g;
        if (const_g.get(5, 2) != 100 || const_g(5, 2) != 100) {
            std::cerr << "\nErreur: Les accesseurs constants renvoient des valeurs incorrectes." << std::endl;
            return false;
        }

        std::cout << "OK" << std::endl;
        return true;
    }
};

#endif