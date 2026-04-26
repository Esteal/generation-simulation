#ifndef VEGETATION_SYSTEM_H
#define VEGETATION_SYSTEM_H

#include "ISimulationSystem.h"
#include <unordered_map>

/**
 * @file VegetationSystem.h
 * @brief Gestion de la végétation : croissance, pollinisation et mortalité.
 *
 * Le VegetationSystem simule l'évolution de la végétation (herbes, mousses, arbres, cactus, etc.)
 * en tenant compte des conditions locales (température, humidité, lumière, sédiments) et des
 * interactions biologiques (pollinisation, dispersion). Les modifications affectent les cellules
 * (Cell::material, Cell::stage, etc.) et sont consommées par d'autres systèmes (ResourceManager, Agriculture, ...).
 */

/**
 * @class VegetationSystem
 * @brief Système de simulation de la végétation.
 *
 * Responsabilités :
 * - ensemencer et propager naturellement les plantes ;
 * - faire évoluer les stades de croissance (maturation, sénescence) ;
 * - appliquer la mortalité liée aux conditions environnementales ;
 * - calculer les taux de croissance/mortalité suivant le type de végétation et l'environnement.
 */
class VegetationSystem : public ISimulationSystem
{
private:
    float POLINIZATION_CHANCE;                 /**< Probabilité par seconde de pollinisation effective pour une cellule en STAGE_2 (0.0..1.0). */
    float GROWTH_FACTOR;                       /**< Facteur global appliqué aux taux de croissance calculés. */
    float DEATH_FACTOR;                        /**< Facteur global appliqué aux taux de mortalité calculés. */
    std::unordered_map<Material, int> RADIUS;  /**< Rayon (en cellules) utilisé pour la dispersion/ensemencement par Material. */

    /**
     * @brief Tente d'ensemencer une graine dans la cellule fournie.
     *
     * Initialise le matériel végétal ou avance le stade si les conditions locales sont favorables.
     *
     * @param cell Référence à la cellule cible.
     */
    void sowASeed(Cell &cell);

    /**
     * @brief Parcourt la map et ensemence des cellules selon la logique de dispersion.
     *
     * Utilise RADIUS et POLINIZATION_CHANCE pour distribuer de nouvelles pousses autour
     * des plantes existantes.
     *
     * @param map Référence à la carte de simulation.
     */
    void sowSeeds(Map &map);

    /**
     * @brief Propage le pollen depuis la cellule (x,y) contenant `material`.
     *
     * Tente de faire germer des semences dans les cellules voisines en appliquant
     * POLINIZATION_CHANCE et en vérifiant les contraintes environnementales.
     *
     * @param map Référence à la carte.
     * @param x Coordonnée X de la cellule émettrice.
     * @param y Coordonnée Y de la cellule émettrice.
     * @param material Type de végétation émettant le pollen.
     */
    void polenization(Map &map, int x, int y, Material material);

    /**
     * @brief Calcule un taux de croissance net pour un type de végétation selon l'environnement.
     *
     * Combine température, humidité, lumière et sédiment avec les paramètres propres
     * à `material` et renvoie un taux continu (positif => croissance, négatif => déclin).
     *
     * @param material Type de végétation considéré.
     * @param temp Température locale.
     * @param hum Humidité locale.
     * @param light Quantité de lumière locale.
     * @param sediment Épaisseur des sédiments / granularité locale.
     * @return float Taux de croissance net (sera multiplié par deltaTime lors de l'application).
     */
    float calculateRate(const Material &material, const float &temp, const float &hum, 
                        const float &light, const float &sediment) const;

    /**
     * @brief Applique l'évolution de la végétation sur l'ensemble de la carte.
     *
     * Met à jour les stades, applique la mortalité, et déclenche la pollinisation/dispersion.
     *
     * @param map Référence à la carte.
     * @param deltaTime Temps écoulé depuis la dernière mise à jour (en secondes).
     */
    void evolveVegetation(Map &map, float deltaTime);

    /**
     * @brief Calcule la sensibilité d'une plante à une variable environnementale.
     *
     * Renvoie une valeur normalisée [0..1] indiquant la compatibilité entre envValue
     * et la valeur optimale, en tenant compte d'une tolérance.
     *
     * @param envValue Valeur mesurée (ex: température).
     * @param tolerance Tolérance autour de la valeur optimale.
     * @param optimalValue Valeur optimale attendue.
     * @return float Sensibilité normalisée (0.0 = inadapté, 1.0 = optimal).
     */
    float calculateSensivityToVegetation(float envValue, float tolerance, float optimalValue) const;

public:
    /**
     * @brief Constructeur.
     *
     * Initialise les paramètres de simulation et les rayons par Material.
     */
    VegetationSystem();

    /**
     * @brief Exécute une étape de simulation de la végétation.
     *
     * Délègue à evolveVegetation() et sowSeeds() pour appliquer croissance et dispersion.
     *
     * @param map Référence à la carte de simulation.
     * @param deltaTime Temps écoulé depuis la dernière mise à jour (en secondes).
     *
     * @override Implémente ISimulationSystem::process().
     */
    void process(Map& map, float deltaTime) override;
};

#endif