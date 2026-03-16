#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H
#include "Map.h"
#include "FastNoiseLite.h"
class MapGenerator
{
private:
    FastNoiseLite noiseAltitude;
    FastNoiseLite noiseTemperature;
    FastNoiseLite noiseHumidity;
    int seed;
    BiomeIndex determineBiome(float altitude, float temperature, float humidity) const;
public:
    MapGenerator(int seed = 0);
    /**
     * @brief Génère une carte en utilisant des bruits de Perlin pour l'altitude, la température et l'humidité. 
     * L'échelle : plus le chiffre est petit, plus les continents seront grands. 
     * @param map la carte à remplir
     * @param scaleAlt l'échelle pour l'altitude
     * @param scaleTemp l'échelle pour la température, elle varie plus doucement que les autres
     * @param scaleHumid l'échelle pour l'humidité
     */
    void generate(Map& map, float scaleAlt = 0.01f, float scaleTemp = 0.01f, float scaleHumid = 0.015f);
    void setSeed(int newSeed);
};

#endif