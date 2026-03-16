#include "MapGenerator.h"

MapGenerator::MapGenerator(int seed)
    : seed(seed)
{
    noiseAltitude.SetSeed(seed);
    noiseTemperature.SetSeed(seed + 1);
    noiseHumidity.SetSeed(seed + 2);

    noiseAltitude.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    // --- LES LIGNES À AJOUTER POUR RÉGLER LES RIVIÈRES ---
    noiseAltitude.SetFractalType(FastNoiseLite::FractalType_FBm);
    noiseAltitude.SetFractalOctaves(4);      // 4 couches de détails (crée des micro-vallées)
    noiseAltitude.SetFractalLacunarity(2.0f); // La fréquence des petits détails
    noiseAltitude.SetFractalGain(0.5f);

    noiseTemperature.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noiseHumidity.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
}

void MapGenerator::setSeed(int newSeed)
{
    seed = newSeed;
    noiseAltitude.SetSeed(newSeed);
    noiseTemperature.SetSeed(newSeed + 1);
    noiseHumidity.SetSeed(newSeed + 2);
}

void MapGenerator::generate(Map &map, float scaleAlt, float scaleTemp, float scaleHumid)
{   
    for(size_t y = 0; y < map.getHeight(); ++y)
    {
        for(size_t x = 0; x < map.getWidth(); ++x)
        {
            Cell& cell = map.getGrid().get(x, y);
            cell.altitude = noiseAltitude.GetNoise(x * scaleAlt, y * scaleAlt);
            cell.temperature = noiseTemperature.GetNoise(x * scaleTemp, y * scaleTemp);
            cell.humidity = noiseHumidity.GetNoise(x * scaleHumid, y * scaleHumid);
            cell.biome = determineBiome(cell.altitude, cell.temperature, cell.humidity);
        }
    }
}

BiomeIndex MapGenerator::determineBiome(float alt, float temp, float humid) const
{
    // 1. Définition du niveau de la mer
    float seaLevel = -0.1f; // Tout ce qui est en dessous est de l'eau

    // 2. Gestion des étendues d'eau
    if (alt < seaLevel) {
        if (temp < -0.3f) return BiomeIndex::GLACE; // Banquise
        return BiomeIndex::OCEAN; // Océan
    }

    // 3. Les littoraux (une fine bande juste au-dessus de l'eau)
    if (alt < seaLevel + 0.05f) {
        if (temp < -0.2f) return BiomeIndex::SNOW; // Plage gelée
        return BiomeIndex::BEACH;
    }

    // ---------------------------------------------------------
    // 4. LE GRADIENT THERMIQUE (L'astuce de réalisme)
    // Plus on monte haut au-dessus du niveau de la mer, plus il fait froid.
    // ---------------------------------------------------------
    float altitudeEffect = (alt - seaLevel) * 0.8f; // Le facteur 0.8 détermine la rudesse de la montagne
    float localTemp = temp - altitudeEffect;

    // ---------------------------------------------------------
    // 5. LA MATRICE DE WHITTAKER (Température vs Humidité)
    // ---------------------------------------------------------

    // Zone Glaciale / Polaire (Très froid)
    if (localTemp < -0.4f) {
        // En haute montagne ou aux pôles, l'humidité fait de la neige, sinon de la glace brute
        if (humid > 0.0f) return BiomeIndex::SNOW; 
        return BiomeIndex::MOUNTAINS; // Roche nue glacée
    }
    
    // Zone Boréale / Froide
    if (localTemp < 0.0f) {
        if (humid < -0.2f) return BiomeIndex::PLAINS;   // Steppe froide / Toundra sèche
        if (humid < 0.3f)  return BiomeIndex::FOREST;   // Taïga (Forêt de conifères)
        return BiomeIndex::SNOW;                        // Toundra très enneigée
    }

    // Zone Tempérée
    if (localTemp < 0.4f) {
        if (humid < -0.3f) return BiomeIndex::PLAINS;   // Prairie sèche
        if (humid < 0.4f)  return BiomeIndex::FOREST;   // Forêt tempérée classique
        return BiomeIndex::JUNGLE;                      // Forêt tempérée humide
    }

    // Zone Chaude / Tropicale (localTemp >= 0.4f)
    if (humid < -0.1f) return BiomeIndex::DESERT;       // Désert aride
    if (humid < 0.3f)  return BiomeIndex::PLAINS;       // Savane
    return BiomeIndex::JUNGLE;                          // Forêt tropicale humide
}