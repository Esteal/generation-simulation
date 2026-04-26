/**
 * @file MapGenerator.h
 * @brief Définition du générateur de carte procédurale.
 */

#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include "model/Map.h"
#include "FastNoiseLite.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>

/**
 * @class MapGenerator
 * @brief Générateur de monde procédural utilisant des fonctions de bruit (Perlin/Simplex).
 * * Cette classe est responsable de la création initiale du terrain. Elle utilise 
 * différentes couches de bruit (FastNoiseLite) pour générer l'altitude de la roche 
 * mère, la température et l'humidité, et en déduit la répartition des biomes.
 */
class MapGenerator
{
private:

    float seaLevel; /**< Seuil d'altitude définissant le niveau de la mer, configurable via GameConfig. */

    /** @brief Générateur de bruit dédié à la topologie (roche mère / bedrock). */
    FastNoiseLite noiseAltitude;
    
    /** @brief Générateur de bruit dédié à la carte des températures. */
    FastNoiseLite noiseTemperature;
    
    /** @brief Générateur de bruit dédié à la carte d'humidité (précipitations). */
    FastNoiseLite noiseHumidity;
    
    FastNoiseLite noiseGranular; /**< Générateur de bruit dédié à la distribution initiale de sédiments (granular). */

    /** @brief Graine aléatoire (seed) garantissant que le même nombre générera toujours le même monde. */
    int seed;

    /**
     * @brief Détermine le biome d'une cellule en croisant ses données physiques et climatiques.
     * * Utilise les seuils de température, d'humidité, l'altitude de la roche et 
     * l'épaisseur de terre pour définir l'écosystème local.
     * * @param altitude L'altitude totale (ou roche mère) de la cellule.
     * @param temperature La température locale normalisée.
     * @param humidity L'humidité locale normalisée.
     * @param granular La quantité de sédiments ou de terre meuble sur la cellule.
     * @return L'index du biome correspondant (BiomeIndex).
     */
    BiomeIndex determineBiome(float altitude, float temperature, float humidity, float granular) const;

public:
    /**
     * @brief Constructeur du générateur de carte.
     * @param seed La graine aléatoire de base (0 par défaut).
     */
    MapGenerator(int seed = 0);

    /**
     * @brief Génère le terrain initial de la carte.
     * * Remplit la grille avec des valeurs issues des bruits de Perlin pour l'altitude, 
     * la température et l'humidité. Plus l'échelle interne des bruits est petite, 
     * plus les continents et les variations climatiques seront vastes.
     * * @param[out] map La carte à remplir avec les données générées.
     */
    void generate(Map& map);

    /**
     * @brief Applique la classification des biomes à chaque cellule de la carte.
     * * Après la génération initiale du terrain, cette méthode parcourt chaque cellule
     * pour déterminer son biome en fonction de ses caractéristiques physiques et climatiques.
     * * @param[in,out] map La carte à modifier en assignant les biomes
     */
    void setBiome(Map& map);

    /**
     * @brief Modifie la graine de génération et réinitialise les moteurs de bruit.
     * @param newSeed La nouvelle graine à appliquer pour les prochaines générations.
     */
    void setSeed(int newSeed);

    /**
     * @brief Effectue un test de régression pour valider le comportement de la carte.
     * @return true si tous les tests passent, false sinon.
     */
    static bool testRegression();
};

#endif // MAP_GENERATOR_H