#pragma once

#include "../models/World.hpp"
#include "../models/Fleet.hpp"
#include <vector>
#include <memory>
#include <string>

class FleetAI
{
public:
    struct CombatStrategy
    {
        float aggressionLevel;   // Niveau d'agressivité (0.0 - 1.0)
        float retreatThreshold;  // Seuil de retraite
        float formationSpacing;  // Espacement entre les navires
        bool preferRangedCombat; // Préférence pour le combat à distance
    };

    struct NavigationPath
    {
        std::vector<sf::Vector2f> waypoints;
        float totalDistance;
        float estimatedTime;
        float riskLevel;
    };

    FleetAI(std::shared_ptr<World> world);
    ~FleetAI() = default;

    // Méthodes principales
    void update(float deltaTime);
    NavigationPath findOptimalPath(const sf::Vector2f &start, const sf::Vector2f &end);
    CombatStrategy determineCombatStrategy(const Fleet &fleet, const Fleet &enemyFleet);
    bool shouldEngageCombat(const Fleet &fleet, const Fleet &enemyFleet);
    bool shouldRetreat(const Fleet &fleet, const Fleet &enemyFleet);

    // Méthodes de navigation
    sf::Vector2f calculateOptimalPosition(const Fleet &fleet, const Fleet &enemyFleet);
    float calculateWindEffect(const sf::Vector2f &position);
    bool isPathSafe(const NavigationPath &path);

    // Méthodes de combat
    std::vector<Ship *> selectTargets(const Fleet &fleet, const Fleet &enemyFleet);
    float calculateCombatPower(const Fleet &fleet);
    float calculateEnemyThreat(const Fleet &enemyFleet);

private:
    std::shared_ptr<World> m_world;

    // Cache pour les calculs
    std::map<std::string, float> m_combatPowerCache;
    std::map<std::string, float> m_threatLevelCache;

    // Méthodes utilitaires
    float calculateDistance(const sf::Vector2f &pos1, const sf::Vector2f &pos2);
    float calculateWindDirection(const sf::Vector2f &position);
    float calculateTerrainEffect(const sf::Vector2f &position);
    bool isPositionSafe(const sf::Vector2f &position);
};
