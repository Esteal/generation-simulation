/**
 * @file MapGenerator.h
 * @brief Définition du générateur de carte procédurale.
 */

#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#include "Map.h"
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
    /** @brief Générateur de bruit dédié à la topologie (roche mère / bedrock). */
    FastNoiseLite noiseAltitude;
    
    /** @brief Générateur de bruit dédié à la carte des températures. */
    FastNoiseLite noiseTemperature;
    
    /** @brief Générateur de bruit dédié à la carte d'humidité (précipitations). */
    FastNoiseLite noiseHumidity;
    
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
     * @brief Regénère ou adapte le terrain en se basant sur une carte de biomes préexistante.
     * * Utile si la carte a été chargée depuis un fichier ou modifiée manuellement, 
     * afin de recalculer des altitudes et températures cohérentes avec les biomes actuels.
     * * @param[in,out] map La carte à traiter.
     */
    void generateFromBiome(Map &map);

    /**
     * @brief Recalcule et assigne les biomes de toutes les cellules de la carte.
     * * Doit être appelée après la génération initiale, ou après le passage 
     * des systèmes d'érosion qui modifient la terre (granular) et la roche.
     * * @param[in,out] map La carte dont les biomes doivent être mis à jour.
     */
    void setBiome(Map &map);

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