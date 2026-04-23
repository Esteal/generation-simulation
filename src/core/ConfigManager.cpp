#include "ConfigManager.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool ConfigManager::saveConfig(const std::string& filename, const std::string& directory) {
    std::vector<std::string> searchPaths = {
        filename,                                 
        directory + filename,
        "../" + directory + filename,             
        "../../" + directory + filename,          
        "../../../" + directory + filename        
    };

    std::ifstream file;
    std::string loadedPath = "";

    for (const auto& path : searchPaths) {
        file.open(path);
        if (file.is_open()) {
            loadedPath = path;
            break;
        }
    }

    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier de configuration " << filename << std::endl;
        return false;
    }

    try {
        json j;

        j["city_manager"]["cohesion_bonus_per_city"] = currentConfig.cohesionBonusPerCity;
        j["city_manager"]["penalty_distance_from_mother_city_factor"] = currentConfig.penaltyDistanceFromMotherCityFactor;
        j["city_manager"]["carrying_capacity_base"] = currentConfig.carryingCapacityBase;
        j["city_manager"]["min_distance"] = currentConfig.minDistance;
        j["city_manager"]["number_of_factions"] = currentConfig.numberOfFactions;
        j["city_manager"]["stock_food"] = currentConfig.stockFood;
        j["city_manager"]["population_totale"] = currentConfig.populationTotale;
        j["city_manager"]["default_expand_migration_radius"] = currentConfig.defaultExpandMigrationRadius;
        j["city_manager"]["default_expand_plant_radius"] = currentConfig.defaultExpandPlantRadius;
        j["city_manager"]["default_expand_influence_radius"] = currentConfig.defaultExpandInfluenceRadius;
        j["city_manager"]["tech_base_level"] = static_cast<int>(currentConfig.techBaseLevel);


        // --- Warfare Manager ---
        j["warfare_manager"]["base_casualty_rate"] = currentConfig.baseCasualtyRate;
        j["warfare_manager"]["combat_range_bonus"] = currentConfig.combatRangeBonus;

        // --- Food Manager ---
        j["food_manager"]["consumption_rate"] = currentConfig.consumptionRate;
        j["food_manager"]["replant_chance"] = currentConfig.replantChance;
        j["food_manager"]["food_harvested"] = currentConfig.foodHarvested;

        // --- Ressource Manager ---
        j["ressource_manager"]["tree_chop_rate"] = currentConfig.treeChopRate;
        j["ressource_manager"]["mining_rate"] = currentConfig.miningRate;
        j["ressource_manager"]["max_wood_per_tree"] = currentConfig.maxWoodPerTree;
        j["ressource_manager"]["max_ore_per_vein"] = currentConfig.maxOrePerVein;

        // --- Tech Manager ---
        j["tech_manager"]["cost_wood_bronze"] = currentConfig.costWoodBronze;
        j["tech_manager"]["req_pop_bronze"] = currentConfig.reqPopBronze;
        j["tech_manager"]["req_colonies_bronze"] = currentConfig.reqColoniesBronze;
        j["tech_manager"]["cost_bronze_iron"] = currentConfig.costBronzeIron;
        j["tech_manager"]["cost_coal_iron"] = currentConfig.costCoalIron;
        j["tech_manager"]["req_pop_iron"] = currentConfig.reqPopIron;
        j["tech_manager"]["req_colonies_iron"] = currentConfig.reqColoniesIron;

        // --- Territory System ---
        j["territory_system"]["tension_factor"] = currentConfig.tensionFactor;

        // --- Thermal Erosion ---
        j["thermal_erosion"]["talus_angle"] = currentConfig.talusAngle;
        j["thermal_erosion"]["friction"] = currentConfig.friction;
        j["thermal_erosion"]["iterations_per_tick"] = currentConfig.iterationsPerTick;

        // --- Hydrolic Erosion ---
        j["hydrolic_erosion"]["k"] = currentConfig.K;
        j["hydrolic_erosion"]["m"] = currentConfig.M;
        j["hydrolic_erosion"]["n"] = currentConfig.n;

        // --- Agriculture ---
        j["agriculture"]["wheat_growth_speed"] = currentConfig.wheatGrowthSpeed;

        // --- Light ---
        j["light"]["radius"] = currentConfig.radius;
        j["light"]["num_directions"] = currentConfig.numDirections;

        // --- Hydrologie ---
        j["hydrologie"]["river_threshold"] = currentConfig.riverThreshold;
        j["hydrologie"]["drop_off"] = currentConfig.dropOff;

        // --- Minerals ---
        j["minerals"]["noise_frequency"] = currentConfig.noiseFrequency;
        j["minerals"]["base_spawn_chance"] = currentConfig.baseSpawnChance;
        
        j["minerals"]["vein_initial_chance"] = currentConfig.veinInitialChance;
        j["minerals"]["vein_decay_rate"] = currentConfig.veinDecayRate;
        
        j["minerals"]["offset_coal"] = currentConfig.offsetCoal;
        j["minerals"]["offset_iron"] = currentConfig.offsetIron;
        j["minerals"]["offset_gold"] = currentConfig.offsetGold;
        j["minerals"]["offset_bronze"] = currentConfig.offsetBronze;
        
        j["minerals"]["coal_min_altitude"] = currentConfig.coalMinAltitude;
        j["minerals"]["coal_max_altitude"] = currentConfig.coalMaxAltitude;
        j["minerals"]["coal_rarity"] = currentConfig.coalRarity;
        j["minerals"]["coal_biome_bonus"] = currentConfig.coalBiomeBonus;
        j["minerals"]["coal_biome_penalty"] = currentConfig.coalBiomePenalty;
        
        j["minerals"]["iron_min_altitude"] = currentConfig.ironMinAltitude;
        j["minerals"]["iron_max_altitude"] = currentConfig.ironMaxAltitude;
        j["minerals"]["iron_rarity"] = currentConfig.ironRarity;
        j["minerals"]["iron_biome_bonus"] = currentConfig.ironBiomeBonus;
        j["minerals"]["iron_biome_penalty"] = currentConfig.ironBiomePenalty;
        
        j["minerals"]["gold_min_altitude"] = currentConfig.goldMinAltitude;
        j["minerals"]["gold_max_altitude"] = currentConfig.goldMaxAltitude;
        j["minerals"]["gold_rarity"] = currentConfig.goldRarity;
        j["minerals"]["gold_biome_bonus"] = currentConfig.goldBiomeBonus;
        j["minerals"]["gold_biome_penalty"] = currentConfig.goldBiomePenalty;

        j["minerals"]["bronze_min_altitude"] = currentConfig.bronzeMinAltitude;
        j["minerals"]["bronze_max_altitude"] = currentConfig.bronzeMaxAltitude;
        j["minerals"]["bronze_rarity"] = currentConfig.bronzeRarity;
        j["minerals"]["bronze_biome_bonus"] = currentConfig.bronzeBiomeBonus;
        j["minerals"]["bronze_biome_penalty"] = currentConfig.bronzeBiomePenalty;

        // --- Faction Interface ---
        j["faction_interface"]["soil_bonus_multiplier"] = currentConfig.soilBonusMultiplier;
        j["faction_interface"]["soil_bonus_max"] = currentConfig.soilBonusMax;
        j["faction_interface"]["altitude_threshold"] = currentConfig.altitudeThreshold;
        j["faction_interface"]["altitude_malus_multiplier"] = currentConfig.altitudeMalusMultiplier;
        j["faction_interface"]["fertility_temperature_weight"] = currentConfig.fertilityTemperatureWeight;
        j["faction_interface"]["fertility_humidity_weight"] = currentConfig.fertilityHumidityWeight;
        j["faction_interface"]["fertility_light_weight"] = currentConfig.fertilityLightWeight;
        j["faction_interface"]["scan_radius"] = currentConfig.scanRadius;
        j["faction_interface"]["stone_age_water_bonus"] = currentConfig.stoneAgeWaterBonus;
        j["faction_interface"]["stone_age_wood_bonus"] = currentConfig.stoneAgeWoodBonus;
        j["faction_interface"]["bronze_age_wood_bonus"] = currentConfig.bronzeAgeWoodBonus;
        j["faction_interface"]["bronze_age_resource_bonus"] = currentConfig.bronzeAgeResourceBonus;
        j["faction_interface"]["iron_age_bronze_bonus"] = currentConfig.ironAgeBronzeBonus;
        j["faction_interface"]["iron_age_resource_bonus"] = currentConfig.ironAgeResourceBonus;
        j["faction_interface"]["min_migrants"] = currentConfig.minMigrants;
        j["faction_interface"]["growth_rate"] = currentConfig.growthRate;

        // --- General ---
        j["general"]["sea_level"] = currentConfig.seaLevel;

        // --- Vegetation ---
        j["vegetation"]["pollinization_chance"] = currentConfig.pollinizationChance;
        j["vegetation"]["growth_factor"] = currentConfig.growthFactor;
        j["vegetation"]["death_factor"] = currentConfig.deathFactor;
        j["vegetation"]["radius_cactus"] = currentConfig.radiusCactus;
        j["vegetation"]["radius_pine_tree"] = currentConfig.radiusPineTree;
        j["vegetation"]["radius_oak_tree"] = currentConfig.radiusOakTree;
        j["vegetation"]["radius_jungle_tree"] = currentConfig.radiusJungleTree;

        // --- Map Generator ---

        j["map_generator"]["altitude_frequency"] = currentConfig.altitudeFrequency;
        j["map_generator"]["altitude_octaves"] = currentConfig.altitudeOctaves;
        j["map_generator"]["altitude_lacunarity"] = currentConfig.altitudeLacunarity;
        j["map_generator"]["altitude_gain"] = currentConfig.altitudeGain;
        j["map_generator"]["temperature_frequency"] = currentConfig.temperatureFrequency;
        j["map_generator"]["humidity_frequency"] = currentConfig.humidityFrequency;
        j["map_generator"]["granular_frequency"] = currentConfig.granularFrequency;

        std::ofstream outFile(loadedPath);
        if (!outFile.is_open()) {
            std::cerr << "Erreur: Impossible d'ouvrir le fichier pour écriture " << loadedPath << std::endl;
            return false;
        }
        outFile << std::setw(4) << j << std::endl; // Indentation pour la lisibilité
        std::cout << "Configuration sauvegardee avec succes." << std::endl;
        return true;

    } catch (json::exception& e) {
        std::cerr << "Erreur lors de la sauvegarde de la configuration : " << e.what() << std::endl;
        return false;
    }
}

bool ConfigManager::loadConfig(const std::string& filename, const std::string& directory) {
    std::vector<std::string> searchPaths = {
        filename,                                 
        directory + filename,
        "../" + directory + filename,             
        "../../" + directory + filename,          
        "../../../" + directory + filename        
    };

    std::ifstream file;
    std::string loadedPath = "";

    for (const auto& path : searchPaths) {
        file.open(path);
        if (file.is_open()) {
            loadedPath = path;
            break;
        }
    }

    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier de configuration " << filename << std::endl;
        return false;
    }

    try {
        json j;
        file >> j;

        // La méthode .value("cle", valeur_par_defaut) évite les crashs si la clé manque dans le JSON
        
        if (j.contains("city_manager")) {   
            currentConfig.cohesionBonusPerCity = j["city_manager"].value("cohesion_bonus_per_city", 1.0f);
            currentConfig.penaltyDistanceFromMotherCityFactor = j["city_manager"].value("penalty_distance_from_mother_city_factor", 0.15f);
            currentConfig.carryingCapacityBase = j["city_manager"].value("carrying_capacity_base", 800);
            currentConfig.minDistance = j["city_manager"].value("min_distance", 20);
            currentConfig.numberOfFactions = j["city_manager"].value("number_of_factions", 5);
            currentConfig.stockFood = j["city_manager"].value("stock_food", 300);
            currentConfig.populationTotale = j["city_manager"].value("population_totale", 100);
            currentConfig.defaultExpandMigrationRadius = j["city_manager"].value("default_expand_migration_radius", 3);
            currentConfig.defaultExpandPlantRadius = j["city_manager"].value("default_expand_plant_radius", 2);
            currentConfig.defaultExpandInfluenceRadius = j["city_manager"].value("default_expand_influence_radius", 4);
            currentConfig.techBaseLevel = static_cast<TechLevel>(j["city_manager"].value("tech_base_level", 1));
        }

        if (j.contains("warfare_manager")) {
            currentConfig.baseCasualtyRate = j["warfare_manager"].value("base_casualty_rate", 0.2f);
            currentConfig.combatRangeBonus = j["warfare_manager"].value("combat_range_bonus", 1);
        }

        if (j.contains("food_manager")) {
            currentConfig.consumptionRate = j["food_manager"].value("consumption_rate", 0.8f);
            currentConfig.replantChance = j["food_manager"].value("replant_chance", 0.8f);
            currentConfig.foodHarvested = j["food_manager"].value("food_harvested", 100);
        }

        if (j.contains("ressource_manager")) {
            currentConfig.treeChopRate = j["ressource_manager"].value("tree_chop_rate", 0.5f);
            currentConfig.miningRate = j["ressource_manager"].value("mining_rate", 0.2f);
            currentConfig.maxWoodPerTree = j["ressource_manager"].value("max_wood_per_tree", 50.0f);
            currentConfig.maxOrePerVein = j["ressource_manager"].value("max_ore_per_vein", 300.0f);
        }

        if (j.contains("tech_manager")) {
            currentConfig.costWoodBronze = j["tech_manager"].value("cost_wood_bronze", 500);
            currentConfig.reqPopBronze = j["tech_manager"].value("req_pop_bronze", 1000);
            currentConfig.reqColoniesBronze = j["tech_manager"].value("req_colonies_bronze", 3);
            currentConfig.costBronzeIron = j["tech_manager"].value("cost_bronze_iron", 800);
            currentConfig.costCoalIron = j["tech_manager"].value("cost_coal_iron", 1000);
            currentConfig.reqPopIron = j["tech_manager"].value("req_pop_iron", 3000);
            currentConfig.reqColoniesIron = j["tech_manager"].value("req_colonies_iron", 6);
        }

        if(j.contains("territory_system")) {
            currentConfig.tensionFactor = j["territory_system"].value("tension_factor", 5);
        }

        if(j.contains("thermal_erosion")) {
            currentConfig.talusAngle = j["thermal_erosion"].value("talus_angle", 0.005f);
            currentConfig.friction = j["thermal_erosion"].value("friction", 0.5f);
            currentConfig.iterationsPerTick = j["thermal_erosion"].value("iterations_per_tick", 3);
        }

        if(j.contains("hydrolic_erosion")) {
            currentConfig.K = j["hydrolic_erosion"].value("k", 0.1f);
            currentConfig.M = j["hydrolic_erosion"].value("m", 0.5f);
            currentConfig.n = j["hydrolic_erosion"].value("n", 1.0f);
        }

        if(j.contains("agriculture")) {
            currentConfig.wheatGrowthSpeed = j["agriculture"].value("wheat_growth_speed", 0.3f);
        }

        if(j.contains("light")) {
            currentConfig.radius = j["light"].value("radius", 10);
            currentConfig.numDirections = j["light"].value("num_directions", 8);
        }

        if(j.contains("hydrologie")) {
            currentConfig.riverThreshold = j["hydrologie"].value("river_threshold", 50.0f);
            currentConfig.dropOff = j["hydrologie"].value("drop_off", 0.1f);
        }

        if(j.contains("minerals")) {
            currentConfig.noiseFrequency = j["minerals"].value("noise_frequency", 0.05f);
            currentConfig.baseSpawnChance = j["minerals"].value("base_spawn_chance", 0.05f);
            
            currentConfig.veinInitialChance = j["minerals"].value("vein_initial_chance", 0.60f);
            currentConfig.veinDecayRate = j["minerals"].value("vein_decay_rate", 0.20f);
            
            currentConfig.offsetCoal = j["minerals"].value("offset_coal", 0.0f);
            currentConfig.offsetIron = j["minerals"].value("offset_iron", 10000.0f);
            currentConfig.offsetGold = j["minerals"].value("offset_gold", 20000.0f);
            currentConfig.offsetBronze = j["minerals"].value("offset_bronze", 30000.0f);
            
            currentConfig.coalMinAltitude = j["minerals"].value("coal_min_altitude", 0.3f);
            currentConfig.coalMaxAltitude = j["minerals"].value("coal_max_altitude", 0.7f);
            currentConfig.coalRarity = j["minerals"].value("coal_rarity", 0.4f);
            currentConfig.coalBiomeBonus = j["minerals"].value("coal_biome_bonus", 1.5f);
            currentConfig.coalBiomePenalty = j["minerals"].value("coal_biome_penalty", 0.5f);
            
            currentConfig.ironMinAltitude = j["minerals"].value("iron_min_altitude", 0.5f);
            currentConfig.ironMaxAltitude = j["minerals"].value("iron_max_altitude", 0.9f);
            currentConfig.ironRarity = j["minerals"].value("iron_rarity", 0.2f);
            currentConfig.ironBiomeBonus = j["minerals"].value("iron_biome_bonus", 2.0f);
            currentConfig.ironBiomePenalty = j["minerals"].value("iron_biome_penalty", 0.8f);
            
            currentConfig.goldMinAltitude = j["minerals"].value("gold_min_altitude", 0.2f);
            currentConfig.goldMaxAltitude = j["minerals"].value("gold_max_altitude", 0.5f);
            currentConfig.goldRarity = j["minerals"].value("gold_rarity", 0.15f);
            currentConfig.goldBiomeBonus = j["minerals"].value("gold_biome_bonus", 1.2f);
            currentConfig.goldBiomePenalty = j["minerals"].value("gold_biome_penalty", 0.5f);

            currentConfig.bronzeMinAltitude = j["minerals"].value("bronze_min_altitude", 0.25f);
            currentConfig.bronzeMaxAltitude = j["minerals"].value("bronze_max_altitude", 0.65f);
            currentConfig.bronzeRarity = j["minerals"].value("bronze_rarity", 0.45f);
            currentConfig.bronzeBiomeBonus = j["minerals"].value("bronze_biome_bonus", 1.8f);
            currentConfig.bronzeBiomePenalty = j["minerals"].value("bronze_biome_penalty", 0.6f);
        }

        if(j.contains("faction_interface")) {
            currentConfig.soilBonusMultiplier = j["faction_interface"].value("soil_bonus_multiplier", 0.8f);
            currentConfig.soilBonusMax = j["faction_interface"].value("soil_bonus_max", 0.40f);
            currentConfig.altitudeThreshold = j["faction_interface"].value("altitude_threshold", 1.2f);
            currentConfig.altitudeMalusMultiplier = j["faction_interface"].value("altitude_malus_multiplier", 0.3f);
            currentConfig.fertilityTemperatureWeight = j["faction_interface"].value("fertility_temperature_weight", 0.25f);
            currentConfig.fertilityHumidityWeight = j["faction_interface"].value("fertility_humidity_weight", 0.35f);
            currentConfig.fertilityLightWeight = j["faction_interface"].value("fertility_light_weight", 0.10f);
            currentConfig.scanRadius = j["faction_interface"].value("scan_radius", 2);
            currentConfig.stoneAgeWaterBonus = j["faction_interface"].value("stone_age_water_bonus", 0.10f);
            currentConfig.stoneAgeWoodBonus = j["faction_interface"].value("stone_age_wood_bonus", 0.15f);
            currentConfig.bronzeAgeWoodBonus = j["faction_interface"].value("bronze_age_wood_bonus", 0.05f);
            currentConfig.bronzeAgeResourceBonus = j["faction_interface"].value("bronze_age_resource_bonus", 0.25f);
            currentConfig.ironAgeBronzeBonus = j["faction_interface"].value("iron_age_bronze_bonus", 0.10f);
            currentConfig.ironAgeResourceBonus = j["faction_interface"].value("iron_age_resource_bonus", 0.30f);
            currentConfig.minMigrants = j["faction_interface"].value("min_migrants", 50);
            currentConfig.growthRate = j["faction_interface"].value("growth_rate", 0.1f);
        }

        if(j.contains("general")) {
            currentConfig.seaLevel = j["general"].value("sea_level", 0.0f);
        }

        if(j.contains("vegetation")) {
            currentConfig.pollinizationChance = j["vegetation"].value("pollinization_chance", 0.1f);
            currentConfig.growthFactor = j["vegetation"].value("growth_factor", 0.5f);
            currentConfig.deathFactor = j["vegetation"].value("death_factor", 0.9f);
            currentConfig.radiusCactus = j["vegetation"].value("radius_cactus", 2);
            currentConfig.radiusPineTree = j["vegetation"].value("radius_pine_tree", 3);
            currentConfig.radiusOakTree = j["vegetation"].value("radius_oak_tree", 6);
            currentConfig.radiusJungleTree = j["vegetation"].value("radius_jungle_tree", 5);
        }

        if(j.contains("map_generator")) {
            currentConfig.altitudeFrequency = j["map_generator"].value("altitude_frequency", 0.008f);
            currentConfig.altitudeOctaves = j["map_generator"].value("altitude_octaves", 6);
            currentConfig.altitudeLacunarity = j["map_generator"].value("altitude_lacunarity", 2.0f);
            currentConfig.altitudeGain = j["map_generator"].value("altitude_gain", 0.25f);
            currentConfig.temperatureFrequency = j["map_generator"].value("temperature_frequency", 0.005f);
            currentConfig.humidityFrequency = j["map_generator"].value("humidity_frequency", 0.005f);
            currentConfig.granularFrequency = j["map_generator"].value("granular_frequency", 0.05f);
        }

        std::cout << "Configuration chargee avec succes." << std::endl;
        return true;
        
    } catch (json::parse_error& e) {
        std::cerr << "Erreur de syntaxe JSON : " << e.what() << std::endl;
        return false;
    }
}