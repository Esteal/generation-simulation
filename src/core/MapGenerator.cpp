#include "MapGenerator.h"
#include "ConfigManager.h"


MapGenerator::MapGenerator(int seed)
    : seed(seed)
{
    const GameConfig& cfg = ConfigManager::getInstance().getConfig();
    seaLevel = cfg.seaLevel;

    noiseAltitude.SetSeed(seed);
    noiseTemperature.SetSeed(seed + 1);
    noiseHumidity.SetSeed(seed + 2);
    noiseGranular.SetSeed(seed + 3);

    noiseAltitude.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noiseAltitude.SetFrequency(cfg.altitudeFrequency);
    noiseAltitude.SetFractalType(FastNoiseLite::FractalType_FBm);
    noiseAltitude.SetFractalOctaves(cfg.altitudeOctaves);      
    noiseAltitude.SetFractalLacunarity(cfg.altitudeLacunarity); 
    noiseAltitude.SetFractalGain(cfg.altitudeGain);
    
    noiseTemperature.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noiseTemperature.SetFrequency(cfg.temperatureFrequency);

    noiseHumidity.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noiseHumidity.SetFrequency(cfg.humidityFrequency);
    
    noiseGranular.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noiseGranular.SetFrequency(cfg.granularFrequency);
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
    for(size_t y = 0; y < map.getHeight(); ++y)
    {
        for(size_t x = 0; x < map.getWidth(); ++x)
        {
            Cell& cell = map.getGrid().get((float)x, (float)y);
            float fx = (float)x;
            float fy = (float)y;

            // 1. LE CONTINENT DE BASE
            float baseNoise = (1.0f + noiseAltitude.GetNoise(fx, fy)) / 2.0f;

            // 2. LES MONTAGNES SPECTACULAIRES
            float mountainNoise = 1.0f - std::abs(noiseAltitude.GetNoise(fx * 2.5f, fy * 2.5f));
            mountainNoise = std::pow(mountainNoise, 3.0f); 
            cell.bedrock = baseNoise + (mountainNoise * baseNoise * 1.5f);

            // 3. CONTRASTE GLOBAL
            cell.bedrock = std::pow(cell.bedrock, 1.2f);

            // =======================================================
            // 4. LISSAGE DU LITTORAL (Correction de l'effet falaise)
            // =======================================================
            float coastZone = 0.06f; // L'épaisseur de la côte qui sera lissée (à ajuster selon vos goûts)
            
            // Si la terre est juste au-dessus du niveau de l'eau...
            if (cell.bedrock > seaLevel && cell.bedrock < seaLevel + coastZone) {
                
                // 't' va de 0 (pile au niveau de l'eau) à 1 (fin de la plage)
                float t = (cell.bedrock - seaLevel) / coastZone; 
                
                // On met 't' au carré. Cela force la pente à devenir parfaitement 
                // plate (tangente) au moment où elle touche l'eau.
                float smoothTransition = t * t; 
                
                // On applique la nouvelle altitude lissée
                cell.bedrock = seaLevel + (smoothTransition * coastZone);
            }
            // =======================================================

            // On s'assure que le bedrock reste gérable
            cell.bedrock = std::max(0.0f, cell.bedrock);

            // Température, humidité et sédiments...
            cell.temperature = (1 + noiseTemperature.GetNoise(fx, fy))/2.0f;
            cell.humidity = (1 + noiseHumidity.GetNoise(fx, fy))/2.0f;
            cell.granular = (1 + noiseGranular.GetNoise(fx, fy))/8.0f; 
        }
    }
}

void MapGenerator::setBiome(Map& map)
{
    for(size_t y = 0; y < map.getHeight(); ++y)
    {
        for(size_t x = 0; x < map.getWidth(); ++x)
        {
            Cell& cell = map.getGrid().get((float)x, (float)y);
            cell.biome = determineBiome(cell.bedrock, cell.temperature, cell.humidity, cell.granular);
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