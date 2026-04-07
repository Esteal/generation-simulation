#include "MapGenerator.h"


MapGenerator::MapGenerator(int seed)
    : seed(seed)
{
    noiseAltitude.SetSeed(seed);
    noiseTemperature.SetSeed(seed + 1);
    noiseHumidity.SetSeed(seed + 2);
    noiseGranular.SetSeed(seed + 3);

    noiseAltitude.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    noiseAltitude.SetFrequency(0.008f);

    noiseAltitude.SetFractalType(FastNoiseLite::FractalType_FBm);
    noiseAltitude.SetFractalOctaves(4);      // 4 couches de détails (crée des micro-vallées)
    noiseAltitude.SetFractalLacunarity(2.0f); // La fréquence des petits détails
    noiseAltitude.SetFractalGain(0.25f);
    
    noiseTemperature.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noiseTemperature.SetFrequency(0.005f);

    noiseHumidity.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noiseHumidity.SetFrequency(0.005f);
    
    noiseGranular.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noiseGranular.SetFrequency(0.05f); 
}

void MapGenerator::setSeed(int newSeed)
{
    seed = newSeed;
    noiseAltitude.SetSeed(newSeed);
    noiseTemperature.SetSeed(newSeed + 1);
    noiseHumidity.SetSeed(newSeed + 2);
}

void MapGenerator::generate(Map &map)
{  
    
    //std::vector<float> altitudeValues(map.getWidth() * map.getHeight()); 
    for(size_t y = 0; y < map.getHeight(); ++y)
    {
        for(size_t x = 0; x < map.getWidth(); ++x)
        {
            Cell& cell = map.getGrid().get((float)x, (float)y);
            float bonusRelief = 0.0f;
            float fx = (float)x;
             float fy = (float)y;
            //cell.bedrock = 1 - 2 * abs(noiseAltitude.GetNoise((float)x, (float)y)); // Valeurs entre 0 et 2, avec des montagnes plus hautes que les océans
            //float altNoise = noiseAltitude.GetNoise((float)x, (float)y);
            
            cell.bedrock = (1 + noiseAltitude.GetNoise((float)x, (float)y))/2.0f;
            cell.temperature = (1 + noiseTemperature.GetNoise((float)x, (float)y))/2.0f;
            cell.humidity = (1 + noiseHumidity.GetNoise((float)x, (float)y))/2.0f;
            cell.granular = (1 + noiseGranular.GetNoise((float)x, (float)y))/8.0f; 
            //cell.biome = determineBiome(cell.altitude, cell.temperature, cell.humidity);
            //altitudeValues[y * map.getWidth() + x] = cell.bedrock;
            // cell.biome = BiomeIndex::BEACH;
            cell.biome = determineBiome(cell.bedrock, cell.temperature, cell.humidity, cell.granular);
            
            switch (cell.biome) {
                case BiomeIndex::MOUNTAINS:
                    // On utilise un bruit de type "Ridge" (Slide 8) pour les pics
                    bonusRelief = 1.0f - std::abs(noiseAltitude.GetNoise(fx * 2.0f, fy * 2.0f));
                    cell.bedrock += bonusRelief * 0.5f; 
                    break;

                case BiomeIndex::DESERT:
                    // On peut simuler des dunes avec un bruit directionnel ou sinusoïdal (Marble)
                    bonusRelief = std::sin(fx * 0.2f + noiseAltitude.GetNoise(fx, fy) * 5.0f);
                    cell.bedrock += bonusRelief * 0.05f;
                    break;

                case BiomeIndex::OCEAN:
                    // On lisse le fond marin
                    cell.bedrock *= 0.8f; 
                    break;

                default:
                    // Bruit standard de turbulence pour les autres biomes
                    cell.bedrock += noiseAltitude.GetNoise(fx * 4.0f, fy * 4.0f) * 0.1f;
                    break;
            }
        }
    }
}

void MapGenerator::generateFromBiome(Map &map)
{
    for(size_t y = 0; y < map.getHeight(); ++y)
    {
        for(size_t x = 0; x < map.getWidth(); ++x)
        {
            Cell& cell = map.getGrid().get(x, y);
            float bonusRelief = 0.0f;
            float fx = (float)x;
            float fy = (float)y;

            switch (cell.biome) {
                case BiomeIndex::MOUNTAINS:
                    // On utilise un bruit de type "Ridge" (Slide 8) pour les pics
                    bonusRelief = 1.0f - std::abs(noiseAltitude.GetNoise(fx * 2.0f, fy * 2.0f));
                    cell.bedrock += bonusRelief * 0.5f; 
                    break;

                case BiomeIndex::DESERT:
                    // On peut simuler des dunes avec un bruit directionnel ou sinusoïdal (Marble)
                    bonusRelief = std::sin(fx * 0.2f + noiseAltitude.GetNoise(fx, fy) * 5.0f);
                    cell.bedrock += bonusRelief * 0.05f;
                    break;

                case BiomeIndex::OCEAN:
                    // On lisse le fond marin
                    cell.bedrock *= 0.8f; 
                    break;

                default:
                    // Bruit standard de turbulence pour les autres biomes
                    cell.bedrock += noiseAltitude.GetNoise(fx * 4.0f, fy * 4.0f) * 0.1f;
                    break;
            }
        }
    }
}

void MapGenerator::setBiome(Map &map)
{
    for(size_t y = 0; y < map.getHeight(); ++y)
    {
        for(size_t x = 0; x < map.getWidth(); ++x)
        {
            Cell& cell = map.getGrid().get((float)x, (float)y);
            cell.biome = determineBiome(map.getAltitude(x, y), cell.temperature, cell.humidity, cell.granular);
            if (cell.biome == BiomeIndex::OCEAN)
                cell.humidity = 1.0f;
        }
    }
}

BiomeIndex MapGenerator::determineBiome(float alt, float temp, float humid, float granular) const
{
    // =========================================================================
    // TABLEAU DE BORD DU CLIMAT (À modifier pour ajuster la génération)
    // =========================================================================
    
    // --- SEUILES GEOLOGIQUES --- => A revoir quand plus de biome
    const float rockyThreshold   = 0.01f; // Si granular < 0.01, la roche est à nu (Falaise/Montagne)
    const float fertileThreshold = 0.12f; // Si granular > 0.12, c'est un delta ou une vallée pleine de sédiment

    // --- RELIEF ---
    const float seaLevel      = 0.40f; // Altitude de l'océan (40% de la carte sous l'eau)
    const float beachWidth    = 0.05f; // Épaisseur de la bande de sable sur les côtes
    const float mountainLevel = 0.75f; // Altitude à partir de laquelle la roche remplace l'herbe

    // --- REFROIDISSEMENT ---
    const float altitudeCooling = 0.80f; // Vitesse à laquelle l'air se refroidit en montant en altitude

    // --- TEMPÉRATURES (Plus la valeur est basse, plus le monde est chaud) ---
    const float oceanFreezeTemp  = 0.0f; // Température max pour que la mer gèle (Banquise)
    const float plainsSnowTemp   = 0.0f; // Température max pour avoir de la neige en plaine
    const float beachFreezeTemp  = 0.0f; // Température max pour avoir du sable gelé
    const float mountainSnowTemp = 0.15f; // Température max pour que les pics aient des neiges éternelles
    const float desertMinTemp    = 0.55f; // Température min pour qu'une plaine sèche devienne un désert (Baisse = plus de déserts)

    // --- HUMIDITÉ ---
    const float forestMinHumid   = 0.45f; // Humidité min pour que les arbres poussent (Hausse = plus de plaines, moins de forêts)


    // =========================================================================
    // LOGIQUE MATHÉMATIQUE (Ne plus y toucher)
    // =========================================================================

    // 1. Le Gradient Thermique (Refroidissement par l'altitude)
    float altitudeEffect = 0.0f;
    if (alt > seaLevel) {
        altitudeEffect = (alt - seaLevel) * altitudeCooling; 
    }
    float localTemp = temp - altitudeEffect;

    // 2. Les Océans et Banquises
    if (alt < seaLevel) {
        if (localTemp < oceanFreezeTemp) return BiomeIndex::GLACE; 
        return BiomeIndex::OCEAN;
    }

    // 3. Les Littoraux
    if (alt < seaLevel + beachWidth) {
        if (localTemp < beachFreezeTemp) return BiomeIndex::SNOW; 
        return BiomeIndex::BEACH;
    }

    // 4. Les Hautes Montagnes
    if (alt > mountainLevel || granular < rockyThreshold) {
        if (localTemp < mountainSnowTemp) return BiomeIndex::SNOW; 
        return BiomeIndex::MOUNTAINS; 
    }

    // 5. Les Terres Continentales Froides
    if (localTemp < plainsSnowTemp) {
        return BiomeIndex::SNOW; // Froid extrême = Neige/Toundra
    }

    // 6. La Matrice de Végétation (Chaud et Tempéré)
    if (humid >= forestMinHumid || granular > fertileThreshold) {
        return BiomeIndex::FOREST; // Suffisamment humide = Forêt
    } else {
        // Trop sec pour une forêt, on regarde s'il fait très chaud
        if (localTemp > desertMinTemp) {
            return BiomeIndex::DESERT; // Aride
        } else {
            return BiomeIndex::PLAINS; // Steppe / Prairie
        }
    }
}

bool MapGenerator::testRegression() {
    std::cout << "[Test] MapGenerator... ";
    
    Map m1(50, 50);
    Map m2(50, 50);
    
    MapGenerator gen1(42); 
    MapGenerator gen2(42); 
    
    gen1.generate(m1);
    gen2.generate(m2);
    
    // la même graine doit produire la même carte
    if (m1.getAltitude(25, 25) != m2.getAltitude(25, 25)) {
        std::cerr << "Erreur: Le generateur n'est pas deterministe." << std::endl;
        return false;
    }
    
    // Test du changement de graine
    gen2.setSeed(99);
    gen2.generate(m2);
    if (m1.getAltitude(25, 25) == m2.getAltitude(25, 25)) {
        std::cerr << "Erreur: Le changement de seed n'affecte pas la generation." << std::endl;
        return false;
    }
    
    std::cout << "OK" << std::endl;
    return true;
}