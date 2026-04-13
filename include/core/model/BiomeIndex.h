/**
 * @file BiomeIndex.h
 * @brief Définition de l'énumération des biomes.
 */

#ifndef BIOME_INDEX_H
#define BIOME_INDEX_H

#include <cstdint>

/**
 * @enum BiomeIndex
 * @brief Représente les différents types d'écosystèmes (biomes) générés sur la carte.
 * * Cette énumération utilise un entier non signé de 8 bits (uint8_t) afin 
 * d'optimiser l'empreinte mémoire de chaque cellule dans les grands mondes.
 */
enum class BiomeIndex : uint8_t
{
    OCEAN = 0,  /**< Étendue d'eau profonde (généralement sous le niveau de la mer). */
    PLAINS,     /**< Plaines herbeuses avec un climat tempéré. */
    DESERT,     /**< Zone aride, chaude et très peu humide (sable). */
    MOUNTAINS,  /**< Reliefs élevés ou falaises rocheuses (terre lavée par l'érosion). */
    FOREST,     /**< Zone tempérée riche en sédiments avec une forte densité d'arbres. */
    BEACH,      /**< Zone côtière sablonneuse, transition entre l'océan et les terres. */
    SNOW,       /**< Terres continentales ou sommets montagneux très froids (neige/toundra). */
    JUNGLE,     /**< Forêt tropicale dense, combinant forte chaleur et forte humidité. */
    RIVER,      /**< Lit d'un cours d'eau douce généré par la simulation hydrologique. */
    GLACE       /**< Étendue d'eau gelée (océan polaire ou banquise). */
};  

#endif // BIOME_INDEX_H