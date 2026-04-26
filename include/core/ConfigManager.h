/**
 * @file ConfigManager.h
 * @brief Chargement, stockage et accès centralisé à la configuration du jeu.
 *
 * Définit la structure GameConfig qui regroupe toutes les options configurables
 * (valeurs par défaut) et la classe singleton ConfigManager responsable du
 * chargement/sauvegarde et de l'accès global à ces paramètres.
 */

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include "dependencies/json.hpp"
#include "model/Faction.h"

/**
 * @struct GameConfig
 * @brief Conteneur de toutes les variables de configuration du jeu.
 *
 * Les champs sont initialisés avec des valeurs par défaut. Ils peuvent être
 * écrasés lors du chargement d'un fichier JSON via ConfigManager::loadConfig().
 */
struct GameConfig {
    // --- General ---
    float seaLevel = 0.0f;                   /**< Niveau de la mer / seuil d'inondation global. */

    // --- Agriculture ---
    float wheatGrowthSpeed = 0.3f;           /**< Vitesse de croissance du blé (fraction par seconde). */

    // --- Hydrolic Erosion ---
    float K = 0.1f;                          /**< Coefficient d'érosion hydraulique (K). */
    float M = 0.5f;                          /**< Exposant/multiplicateur du modèle d'érosion (M). */
    float n = 1.0f;                          /**< Autre paramètre du modèle d'érosion (n). */

    // --- Thermal Erosion ---
    float talusAngle = 0.005f;               /**< Seuil d'angle de talus pour l'érosion thermique. */
    float friction = 0.5f;                   /**< Coefficient de friction appliqué lors de l'érosion thermique. */
    int iterationsPerTick = 3;               /**< Nombre d'itérations d'érosion thermique par tick. */

    // --- Hydrologie ---
    float riverThreshold = 50.0f;            /**< Seuil de débit pour qu'une cellule devienne une rivière. */
    float dropOff = 0.1f;                    /**< Pourcentage d'eau s'écoule vers les cellules plus basses par itération. */

    // --- Minerals ---
    float noiseFrequency = 0.05f;            /**< Fréquence du bruit procédural pour la distribution des minerais. */
    float baseSpawnChance = 0.05f;           /**< Probabilité de base de démarrage d'un filon. */

    float veinInitialChance = 0.60f;         /**< Probabilité initiale d'extension d'un filon au premier voisin. */
    float veinDecayRate = 0.20f;             /**< Taux de décroissance de la probabilité de propagation. */

    float offsetCoal = 0.0f;                 /**< Offset d'échantillonnage bruit pour le charbon. */
    float offsetIron = 10000.0f;             /**< Offset d'échantillonnage bruit pour le fer. */
    float offsetGold = 20000.0f;             /**< Offset d'échantillonnage bruit pour l'or. */
    float offsetBronze = 30000.0f;           /**< Offset d'échantillonnage bruit pour le bronze. */

    // --- Charbon (Coal) ---
    float coalMinAltitude = 0.3f;            /**< Altitude minimale favorable au charbon (0..1). */
    float coalMaxAltitude = 0.7f;            /**< Altitude maximale favorable au charbon (0..1). */
    float coalRarity = 0.4f;                 /**< Rareté relative du charbon (0.0 rare -> 1.0 commun). */
    float coalBiomeBonus = 1.5f;             /**< Bonus d'aptitude dans les biomes favorables. */
    float coalBiomePenalty = 0.5f;           /**< Pénalité d'aptitude dans les biomes défavorables. */

    // --- Fer (Iron) ---
    float ironMinAltitude = 0.5f;            /**< Altitude minimale favorable au fer (0..1). */
    float ironMaxAltitude = 0.9f;            /**< Altitude maximale favorable au fer (0..1). */
    float ironRarity = 0.2f;                 /**< Rareté relative du fer. */
    float ironBiomeBonus = 2.0f;             /**< Bonus d'aptitude pour le fer selon le biome. */
    float ironBiomePenalty = 0.8f;           /**< Pénalité d'aptitude pour le fer selon le biome. */

    // --- Or (Gold) ---
    float goldMinAltitude = 0.2f;            /**< Altitude minimale favorable à l'or (0..1). */
    float goldMaxAltitude = 0.5f;            /**< Altitude maximale favorable à l'or (0..1). */
    float goldRarity = 0.15f;                /**< Rareté relative de l'or. */
    float goldBiomeBonus = 1.2f;             /**< Bonus d'aptitude pour l'or selon le biome. */
    float goldBiomePenalty = 0.5f;           /**< Pénalité d'aptitude pour l'or selon le biome. */

    // --- Bronze ---
    float bronzeMinAltitude = 0.25f;         /**< Altitude minimale favorable au bronze (0..1). */
    float bronzeMaxAltitude = 0.65f;         /**< Altitude maximale favorable au bronze (0..1). */
    float bronzeRarity = 0.45f;              /**< Rareté relative du bronze. */
    float bronzeBiomeBonus = 1.8f;           /**< Bonus d'aptitude pour le bronze selon le biome. */
    float bronzeBiomePenalty = 0.6f;         /**< Pénalité d'aptitude pour le bronze selon le biome. */

    // --- Light ---
    int radius = 10;                         /**< Rayon d'analyse d'occlusion pour le calcul de la lumière (en cellules). */
    int numDirections = 8;                   /**< Nombre de directions échantillonnées pour l'ombrage (ex: 8 = octogonal). */

    // --- Vegetation ---
    float pollinizationChance = 0.1f;        /**< Probabilité par seconde de pollinisation effective. */
    float growthFactor = 0.5f;               /**< Facteur global appliqué aux taux de croissance végétale. */
    float deathFactor = 0.9f;                /**< Facteur global appliqué aux taux de mortalité végétale. */
    int radiusCactus = 2;                    /**< Rayon de dispersion pour cactus. */
    int radiusPineTree = 3;                  /**< Rayon de dispersion pour pins. */
    int radiusOakTree = 6;                   /**< Rayon de dispersion pour chênes. */
    int radiusJungleTree = 5;                /**< Rayon de dispersion pour arbres de jungle. */

    // --- Faction Interface (IFactionSystem) ---
    float soilBonusMultiplier = 0.8f;        /**< Multiplicateur du bonus lié à la fertilité du sol. */
    float soilBonusMax = 0.40f;              /**< Valeur maximale du bonus de fertilité. */
    float altitudeThreshold = 1.2f;          /**< Seuil d'altitude pour appliquer des malus. */
    float altitudeMalusMultiplier = 0.3f;    /**< Multiplicateur appliqué aux pénalités d'altitude. */
    float fertilityTemperatureWeight = 0.25f;/**< Poids de la température dans le calcul de fertilité. */
    float fertilityHumidityWeight = 0.35f;   /**< Poids de l'humidité dans le calcul de fertilité. */
    float fertilityLightWeight = 0.10f;      /**< Poids de la lumière dans le calcul de fertilité. */
    int scanRadius = 2;                      /**< Rayon de balayage pour rechercher sites d'expansion. */
    float stoneAgeWaterBonus = 0.10f;        /**< Bonus d'attractivité lié à l'eau pour l'Âge de pierre. */
    float stoneAgeWoodBonus = 0.15f;         /**< Bonus d'attractivité lié au bois pour l'Âge de pierre. */
    float bronzeAgeWoodBonus = 0.05f;        /**< Bonus lié au bois à l'Âge du bronze. */
    float bronzeAgeResourceBonus = 0.25f;    /**< Bonus lié aux ressources à l'Âge du bronze. */
    float ironAgeBronzeBonus = 0.10f;        /**< Bonus lié à la disponibilité du bronze à l'Âge du fer. */
    float ironAgeResourceBonus = 0.30f;      /**< Bonus lié aux ressources à l'Âge du fer. */
    int minMigrants = 50;                    /**< Nombre minimal d'habitants requis pour fonder une nouvelle colonie. */
    float growthRate = 0.1f;                 /**< Taux de croissance démographique de référence (multiplicateur). */

    // --- City Manager ---
    int carryingCapacityBase = 800;          /**< Capacité de charge de base d'une colonie. */
    float cohesionBonusPerCity = 1.0f;       /**< Bonus de cohésion apporté par chaque colonie supplémentaire. */
    float penaltyDistanceFromMotherCityFactor = 0.15f; /**< Pénalité liée à la distance depuis la ville mère. */

    // --- Warfare Manager ---
    float baseCasualtyRate = 0.2f;           /**< Taux de pertes de base en cas de conflit (fraction par tick). */
    int combatRangeBonus = 1;                /**< Bonus de portée en combat (cases supplémentaires). */

    // --- Food Manager ---
    float consumptionRate = 0.8f;            /**< Nourriture consommée par habitant par seconde. */
    float replantChance = 0.8f;              /**< Probabilité de replanter après récolte. */
    int foodHarvested = 100;                 /**< Quantité de nourriture récoltée par champ mûr. */

    // --- Resource Manager ---
    float treeChopRate = 0.5f;               /**< Fraction de bois prélevée par itération (ex: 0.5 = 50%). */
    float miningRate = 0.2f;                 /**< Fraction de minerai prélevée par itération. */
    float maxWoodPerTree = 50.0f;            /**< Quantité maximale de bois présente dans un arbre mature. */
    float maxOrePerVein = 300.0f;            /**< Quantité maximale de minerai dans un gisement exploitable. */

    // --- Tech Manager ---
    int costWoodBronze = 500;                /**< Bois requis pour passer à l'Âge du Bronze. */
    int reqPopBronze = 1000;                 /**< Population minimale pour débloquer le Bronze. */
    int reqColoniesBronze = 3;               /**< Nombre minimal de colonies pour le Bronze. */
    int costBronzeIron = 800;                /**< Bronze requis pour passer à l'Âge du Fer. */
    int costCoalIron = 1000;                 /**< Charbon requis pour passer à l'Âge du Fer. */
    int reqPopIron = 3000;                   /**< Population minimale pour débloquer le Fer. */
    int reqColoniesIron = 6;                 /**< Nombre minimal de colonies pour le Fer. */

    // --- Tension / Territory ---
    int tensionFactor = 5;                   /**< Facteur d'augmentation de la tension en cas de conflit. */

    // --- Civilisation System ---
    int minDistance = 20;                    /**< Distance minimale entre deux capitales (en cases). */
    int numberOfFactions = 5;                /**< Nombre de factions/civilisations à initialiser. */
    int stockFood = 300;                     /**< Stock initial de nourriture par faction (unités). */
    int populationTotale = 100;              /**< Population initiale par faction. */
    int defaultExpandMigrationRadius = 3;    /**< Rayon par défaut de migration. */
    int defaultExpandPlantRadius = 2;        /**< Rayon par défaut d'exploitation des ressources. */
    int defaultExpandInfluenceRadius = 4;    /**< Rayon par défaut d'influence diplomatique. */
    TechLevel techBaseLevel = TechLevel::STONE_AGE; /**< Niveau technologique initial des factions. */

    // --- Map Generator ---
    float altitudeFrequency = 0.008f;        /**< Fréquence du bruit pour l'altitude. */
    int altitudeOctaves = 6;                 /**< Nombre d'octaves pour le bruit d'altitude. */
    float altitudeLacunarity = 2.0f;         /**< Lacunarité du bruit d'altitude. */
    float altitudeGain = 0.25f;              /**< Gain (persistence) du bruit d'altitude. */
    float temperatureFrequency = 0.005f;     /**< Fréquence pour la carte de température. */
    float humidityFrequency = 0.005f;        /**< Fréquence pour la carte d'humidité. */
    float granularFrequency = 0.05f;         /**< Fréquence pour la carte de sédiments/granularité. */
};

/**
 * @class ConfigManager
 * @brief Singleton fournissant l'accès global à la configuration du jeu.
 *
 * Responsible de charger et sauvegarder les GameConfig depuis/vers des fichiers JSON,
 * et de fournir des accesseurs en lecture et écriture aux autres systèmes.
 */
class ConfigManager {
private:
    GameConfig currentConfig;                /**< Configuration courante utilisée par la simulation. */

    /** Constructeur privé pour garantir l'unicité (pattern Singleton). */
    ConfigManager() = default;

public:
    /**
     * @brief Retourne l'instance unique du ConfigManager.
     *
     * Utilise l'initialisation statique locale pour garantir la construction sûre
     * et paresseuse de l'instance (thread-safe depuis C++11).
     *
     * @return ConfigManager& Référence à l'instance unique.
     */
    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }

    /**
     * @brief Charge la configuration depuis un fichier JSON.
     *
     * Le fichier est lu depuis le répertoire donné (par défaut "config/") et parsé
     * avec la bibliothèque nlohmann::json. Les champs présents dans le JSON remplacent
     * les valeurs par défaut de GameConfig.
     *
     * @param filename Nom du fichier (ex: "game_config.json").
     * @param directory Répertoire contenant le fichier (par défaut "config/").
     * @return true si le chargement et la désérialisation ont réussi, false sinon.
     */
    bool loadConfig(const std::string& filename, const std::string& directory = "config/");

    /**
     * @brief Accès en lecture à la configuration courante.
     *
     * @return const GameConfig& Référence constante vers la configuration utilisée.
     */
    const GameConfig& getConfig() const { return currentConfig; }

    /**
     * @brief Accès en lecture/écriture à la configuration courante.
     *
     * Utile pour modifier dynamiquement des paramètres durant l'exécution (tests, debug).
     *
     * @return GameConfig& Référence mutable vers la configuration.
     */
    GameConfig& getMutableConfig() { return currentConfig; }

    /**
     * @brief Sauvegarde la configuration courante dans un fichier JSON.
     *
     * Sérialise GameConfig avec nlohmann::json et écrit le résultat dans
     * directory/filename. Remplace le fichier existant si nécessaire.
     *
     * @param filename Nom du fichier de destination.
     * @param directory Répertoire de destination (par défaut "config/").
     * @return true si l'écriture a réussi, false sinon.
     */
    bool saveConfig(const std::string& filename, const std::string& directory = "config/");
};

#endif