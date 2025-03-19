#include "FleetAI.h"
#include <algorithm>
#include <cmath>

FleetAI::FleetAI(std::shared_ptr<World> world)
    : m_world(world)
{
}

void FleetAI::update(float deltaTime)
{
    // Mise à jour des caches
    m_combatPowerCache.clear();
    m_threatLevelCache.clear();
}

FleetAI::NavigationPath FleetAI::findOptimalPath(const sf::Vector2f &start, const sf::Vector2f &end)
{
    NavigationPath path;
    path.waypoints.push_back(start);

    // Calcul de la distance directe
    float directDistance = calculateDistance(start, end);

    // Vérification des obstacles et des zones dangereuses
    sf::Vector2f currentPos = start;
    while (calculateDistance(currentPos, end) > 100.0f)
    {
        // Calcul du prochain waypoint en évitant les obstacles
        sf::Vector2f nextWaypoint = currentPos;
        float bestScore = std::numeric_limits<float>::max();

        // Test de plusieurs positions possibles
        for (float angle = 0.0f; angle < 360.0f; angle += 45.0f)
        {
            float rad = angle * 3.14159f / 180.0f;
            sf::Vector2f testPos = currentPos + sf::Vector2f(
                                                    std::cos(rad) * 100.0f,
                                                    std::sin(rad) * 100.0f);

            if (isPositionSafe(testPos))
            {
                float score = calculateDistance(testPos, end);
                if (score < bestScore)
                {
                    bestScore = score;
                    nextWaypoint = testPos;
                }
            }
        }

        path.waypoints.push_back(nextWaypoint);
        currentPos = nextWaypoint;
    }

    path.waypoints.push_back(end);

    // Calcul des métriques de la route
    path.totalDistance = 0.0f;
    for (size_t i = 0; i < path.waypoints.size() - 1; ++i)
    {
        path.totalDistance += calculateDistance(path.waypoints[i], path.waypoints[i + 1]);
    }

    // Estimation du temps en fonction de la vitesse moyenne d'un navire
    path.estimatedTime = path.totalDistance / 5.0f; // Vitesse moyenne de 5 unités par seconde

    // Calcul du niveau de risque
    path.riskLevel = 0.0f;
    for (const auto &waypoint : path.waypoints)
    {
        path.riskLevel += calculateTerrainEffect(waypoint);
    }
    path.riskLevel /= path.waypoints.size();

    return path;
}

FleetAI::CombatStrategy FleetAI::determineCombatStrategy(const Fleet &fleet, const Fleet &enemyFleet)
{
    CombatStrategy strategy;

    // Calcul des puissances de combat
    float ourPower = calculateCombatPower(fleet);
    float enemyPower = calculateEnemyThreat(enemyFleet);
    float powerRatio = ourPower / enemyPower;

    // Détermination de la stratégie basée sur le ratio de puissance
    if (powerRatio > 1.5f)
    {
        // Supériorité écrasante
        strategy.aggressionLevel = 1.0f;
        strategy.retreatThreshold = 0.2f;
        strategy.formationSpacing = 50.0f;
        strategy.preferRangedCombat = false;
    }
    else if (powerRatio > 1.2f)
    {
        // Légère supériorité
        strategy.aggressionLevel = 0.8f;
        strategy.retreatThreshold = 0.3f;
        strategy.formationSpacing = 75.0f;
        strategy.preferRangedCombat = true;
    }
    else if (powerRatio > 0.8f)
    {
        // Équilibre
        strategy.aggressionLevel = 0.6f;
        strategy.retreatThreshold = 0.4f;
        strategy.formationSpacing = 100.0f;
        strategy.preferRangedCombat = true;
    }
    else
    {
        // Infériorité
        strategy.aggressionLevel = 0.4f;
        strategy.retreatThreshold = 0.5f;
        strategy.formationSpacing = 150.0f;
        strategy.preferRangedCombat = true;
    }

    return strategy;
}

bool FleetAI::shouldEngageCombat(const Fleet &fleet, const Fleet &enemyFleet)
{
    float ourPower = calculateCombatPower(fleet);
    float enemyPower = calculateEnemyThreat(enemyFleet);
    float powerRatio = ourPower / enemyPower;

    // Facteurs influençant la décision
    float windEffect = calculateWindEffect(fleet.getPosition());
    float terrainEffect = calculateTerrainEffect(fleet.getPosition());

    // Conditions d'engagement
    bool hasAdvantage = powerRatio > 1.2f;
    bool goodPosition = windEffect > 0.7f && terrainEffect > 0.7f;
    bool hasEscapeRoute = isPathSafe(findOptimalPath(fleet.getPosition(), sf::Vector2f(0, 0)));

    return hasAdvantage && goodPosition && hasEscapeRoute;
}

bool FleetAI::shouldRetreat(const Fleet &fleet, const Fleet &enemyFleet)
{
    float ourPower = calculateCombatPower(fleet);
    float enemyPower = calculateEnemyThreat(enemyFleet);
    float powerRatio = ourPower / enemyPower;

    // Conditions de retraite
    bool severeDisadvantage = powerRatio < 0.7f;
    bool poorPosition = calculateWindEffect(fleet.getPosition()) < 0.3f;
    bool criticalDamage = fleet.getHealthPercentage() < 0.3f;

    return severeDisadvantage || (poorPosition && criticalDamage);
}

sf::Vector2f FleetAI::calculateOptimalPosition(const Fleet &fleet, const Fleet &enemyFleet)
{
    sf::Vector2f currentPos = fleet.getPosition();
    sf::Vector2f enemyPos = enemyFleet.getPosition();

    // Calcul de la direction du vent
    float windDir = calculateWindDirection(currentPos);

    // Calcul de la position optimale
    float distance = calculateDistance(currentPos, enemyPos);
    float optimalDistance = 200.0f; // Distance optimale pour le combat

    // Ajustement de la position en fonction du vent
    float angle = std::atan2(enemyPos.y - currentPos.y, enemyPos.x - currentPos.x);
    float windAngle = windDir * 3.14159f / 180.0f;

    // Position optimale tenant compte du vent et de la distance
    sf::Vector2f optimalPos = enemyPos + sf::Vector2f(
                                             std::cos(angle + windAngle) * optimalDistance,
                                             std::sin(angle + windAngle) * optimalDistance);

    return optimalPos;
}

float FleetAI::calculateWindEffect(const sf::Vector2f &position)
{
    // TODO: Implémenter le calcul de l'effet du vent
    return 0.8f; // Valeur temporaire
}

bool FleetAI::isPathSafe(const NavigationPath &path)
{
    for (const auto &waypoint : path.waypoints)
    {
        if (!isPositionSafe(waypoint))
        {
            return false;
        }
    }
    return true;
}

std::vector<Ship *> FleetAI::selectTargets(const Fleet &fleet, const Fleet &enemyFleet)
{
    std::vector<Ship *> targets;
    auto enemyShips = enemyFleet.getShips();

    // Trier les navires ennemis par priorité
    std::sort(enemyShips.begin(), enemyShips.end(),
              [](const Ship *a, const Ship *b)
              {
                  // Priorité aux navires les plus dangereux
                  float aScore = a->getAttackPower() * (1.0f - a->getHealthPercentage());
                  float bScore = b->getAttackPower() * (1.0f - b->getHealthPercentage());
                  return aScore > bScore;
              });

    // Sélectionner les cibles
    for (const auto &enemyShip : enemyShips)
    {
        if (targets.size() >= fleet.getShips().size())
        {
            break;
        }
        targets.push_back(enemyShip);
    }

    return targets;
}

float FleetAI::calculateCombatPower(const Fleet &fleet)
{
    std::string fleetId = fleet.getId();
    if (m_combatPowerCache.find(fleetId) != m_combatPowerCache.end())
    {
        return m_combatPowerCache[fleetId];
    }

    float totalPower = 0.0f;
    for (const auto &ship : fleet.getShips())
    {
        totalPower += ship->getAttackPower() * ship->getHealthPercentage();
    }

    m_combatPowerCache[fleetId] = totalPower;
    return totalPower;
}

float FleetAI::calculateEnemyThreat(const Fleet &enemyFleet)
{
    std::string fleetId = enemyFleet.getId();
    if (m_threatLevelCache.find(fleetId) != m_threatLevelCache.end())
    {
        return m_threatLevelCache[fleetId];
    }

    float threat = calculateCombatPower(enemyFleet);

    // Ajustement de la menace en fonction de la position et du vent
    float windEffect = calculateWindEffect(enemyFleet.getPosition());
    float terrainEffect = calculateTerrainEffect(enemyFleet.getPosition());

    threat *= (1.0f + windEffect * 0.5f + terrainEffect * 0.3f);

    m_threatLevelCache[fleetId] = threat;
    return threat;
}

float FleetAI::calculateDistance(const sf::Vector2f &pos1, const sf::Vector2f &pos2)
{
    float dx = pos2.x - pos1.x;
    float dy = pos2.y - pos1.y;
    return std::sqrt(dx * dx + dy * dy);
}

float FleetAI::calculateWindDirection(const sf::Vector2f &position)
{
    // TODO: Implémenter le calcul de la direction du vent
    return 0.0f; // Valeur temporaire
}

float FleetAI::calculateTerrainEffect(const sf::Vector2f &position)
{
    // TODO: Implémenter le calcul de l'effet du terrain
    return 0.8f; // Valeur temporaire
}

bool FleetAI::isPositionSafe(const sf::Vector2f &position)
{
    // TODO: Implémenter la vérification de sécurité de la position
    return true; // Valeur temporaire
}