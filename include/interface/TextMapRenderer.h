/**
 * @file TextMapRenderer.h
 * @brief Déclaration de la classe TextMapRenderer pour l'affichage textuel de la carte.
 */

#ifndef TEXT_MAP_RENDERER_H
#define TEXT_MAP_RENDERER_H

#include "Map.h"
#include "BiomeIndex.h"
#include <iostream>
#include <algorithm>

/**
 * @class TextMapRenderer
 * @brief Gère le rendu de la carte sous forme de texte dans la console.
 * * Cette classe fournit des outils pour convertir les données de la carte (comme les biomes)
 * en caractères textuels et afficher une vue simplifiée de la carte directement dans le terminal.
 */
class TextMapRenderer {
public:
    /**
     * @brief Convertit un type de biome en un caractère d'affichage ASCII.
     * * Cette méthode est statique car elle agit comme une simple fonction utilitaire de conversion, 
     * indépendante de l'état d'une instance spécifique de la classe.
     * * @param biome Le biome à convertir.
     * @return char Le caractère représentant visuellement le biome (par exemple 'O' pour l'eau, 'M' pour la montagne).
     */
    static char biomeToChar(BiomeIndex biome);

    /**
     * @brief Affiche la carte des biomes dans la sortie standard (console).
     * * La méthode prend en compte des dimensions maximales pour s'assurer que 
     * * @param map La carte contenant les données des biomes à afficher.
     * @param maxWidth La largeur maximale autorisée pour l'affichage en nombre de caractères.
     * @param maxHeight La hauteur maximale autorisée pour l'affichage en nombre de lignes.
     */
    void printBiomeMap(const Map &map, size_t maxWidth = 120, size_t maxHeight = 40) const;


    /**
     * @brief Effectue un test de régression sur la classe TextMapRenderer.
     * * @return bool true si le test passe, false sinon.
     */
    static bool testRegression();
};

#endif // TEXT_MAP_RENDERER_H