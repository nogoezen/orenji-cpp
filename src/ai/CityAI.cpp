#include "CityAI.h"
#include <algorithm>
#include <cmath>

CityAI::CityAI(std::shared_ptr<World> world)
    : m_world(world)
{
}

void CityAI::update(float deltaTime)
{
    // Mise à jour des caches
    m_infrastructureCache.clear();
    m_securityCache.clear();
    m_economicHealthCache.clear();
    m_populationGrowthCache.clear();

    // Mise à jour de toutes les villes
    for (const auto &city : m_world->getCities())
    {
        updateResourceProduction(city->getName());
        updateTradePolicies(city->getName());
        updateCityStats(city->getName());
    }
}

std::vector<CityAI::DevelopmentAction> CityAI::evaluateDevelopmentActions(const std::string &city)
{
    std::vector<DevelopmentAction> actions;
    float infrastructureLevel = calculateInfrastructureLevel(city);
    float securityLevel = calculateSecurityLevel(city);
    float economicHealth = calculateEconomicHealth(city);

    // Évaluation des actions de développement possibles
    if (infrastructureLevel < 0.7f)
    {
        DevelopmentAction action;
        action.type = DevelopmentAction::Type::IMPROVE_INFRASTRUCTURE;
        action.priority = (1.0f - infrastructureLevel) * economicHealth;
        action.cost = calculateInfrastructureCost(city, action.type);
        action.benefits["infrastructure"] = 0.2f;
        action.benefits["economic_growth"] = 0.1f;
        actions.push_back(action);
    }

    if (securityLevel < 0.6f)
    {
        DevelopmentAction action;
        action.type = DevelopmentAction::Type::RECRUIT_GUARDS;
        action.priority = (1.0f - securityLevel) * economicHealth;
        action.cost = calculateInfrastructureCost(city, action.type);
        action.benefits["security"] = 0.3f;
        action.benefits["stability"] = 0.2f;
        actions.push_back(action);
    }

    if (economicHealth > 0.7f)
    {
        DevelopmentAction action;
        action.type = DevelopmentAction::Type::BUILD_MARKET;
        action.priority = economicHealth * infrastructureLevel;
        action.cost = calculateInfrastructureCost(city, action.type);
        action.benefits["trade_capacity"] = 0.4f;
        action.benefits["economic_growth"] = 0.2f;
        actions.push_back(action);
    }

    // Trier les actions par priorité
    std::sort(actions.begin(), actions.end(),
              [](const DevelopmentAction &a, const DevelopmentAction &b)
              {
                  return a.priority > b.priority;
              });

    return actions;
}

bool CityAI::executeDevelopmentAction(const std::string &city, const DevelopmentAction &action)
{
    if (!canAffordAction(city, action.cost))
    {
        return false;
    }

    // TODO: Implémenter l'exécution des actions de développement
    return true;
}

void CityAI::updateTradePolicies(const std::string &city)
{
    for (const auto &good : m_world->getTradeGoods())
    {
        float demand = calculateResourceDemand(city, good->getId());
        float supply = calculateResourceAvailability(city, good->getId());
        float saturation = calculateMarketSaturation(city, good->getId());

        // Ajustement des politiques commerciales
        TradePolicy policy;
        policy.goodId = good->getId();
        policy.allowImport = demand > supply * 1.2f;
        policy.allowExport = supply > demand * 1.5f;
        policy.importTax = (demand / supply) * 0.1f;
        policy.exportTax = (supply / demand) * 0.1f;
        policy.minStock = demand * 0.5f;
        policy.maxStock = demand * 2.0f;

        // TODO: Implémenter la mise à jour des politiques commerciales
    }
}

void CityAI::updateResourceProduction(const std::string &city)
{
    for (const auto &good : m_world->getTradeGoods())
    {
        float efficiency = calculateResourceEfficiency(city, good->getId());
        float demand = calculateResourceDemand(city, good->getId());
        float quality = calculateResourceQuality(city, good->getId());

        ResourceProduction production;
        production.goodId = good->getId();
        production.productionRate = efficiency * quality;
        production.consumptionRate = demand;
        production.stockLevel = 0.0f; // TODO: Implémenter le suivi des stocks
        production.price = good->getBasePrice() * (1.0f + (demand / production.productionRate));
        production.isExportable = production.productionRate > production.consumptionRate * 1.2f;

        // TODO: Implémenter la mise à jour de la production
    }
}

float CityAI::calculateResourceEfficiency(const std::string &city, const std::string &goodId)
{
    float infrastructureLevel = calculateInfrastructureLevel(city);
    float resourceQuality = calculateResourceQuality(city, goodId);
    float marketSaturation = calculateMarketSaturation(city, goodId);

    return infrastructureLevel * resourceQuality * (1.0f - marketSaturation);
}

float CityAI::calculateResourceDemand(const std::string &city, const std::string &goodId)
{
    float population = m_world->getCity(city)->getPopulation();
    float economicHealth = calculateEconomicHealth(city);
    float baseDemand = m_world->getTradeGood(goodId)->getDemandFactor();

    return population * baseDemand * economicHealth;
}

void CityAI::adjustPrices(const std::string &city)
{
    for (const auto &good : m_world->getTradeGoods())
    {
        float demand = calculateResourceDemand(city, good->getId());
        float supply = calculateResourceAvailability(city, good->getId());
        float basePrice = good->getBasePrice();

        float newPrice = basePrice * (1.0f + (demand - supply) / (demand + supply));
        // TODO: Implémenter la mise à jour des prix
    }
}

float CityAI::calculateInfrastructureLevel(const std::string &city)
{
    if (m_infrastructureCache.find(city) != m_infrastructureCache.end())
    {
        return m_infrastructureCache[city];
    }

    float level = 0.5f; // Niveau de base
    // TODO: Implémenter le calcul du niveau d'infrastructure

    m_infrastructureCache[city] = level;
    return level;
}

float CityAI::calculateSecurityLevel(const std::string &city)
{
    if (m_securityCache.find(city) != m_securityCache.end())
    {
        return m_securityCache[city];
    }

    float level = 0.5f; // Niveau de base
    // TODO: Implémenter le calcul du niveau de sécurité

    m_securityCache[city] = level;
    return level;
}

float CityAI::calculateEconomicHealth(const std::string &city)
{
    if (m_economicHealthCache.find(city) != m_economicHealthCache.end())
    {
        return m_economicHealthCache[city];
    }

    float health = 0.5f; // Santé économique de base
    // TODO: Implémenter le calcul de la santé économique

    m_economicHealthCache[city] = health;
    return health;
}

float CityAI::calculatePopulationGrowth(const std::string &city)
{
    if (m_populationGrowthCache.find(city) != m_populationGrowthCache.end())
    {
        return m_populationGrowthCache[city];
    }

    float growth = 0.0f;
    float economicHealth = calculateEconomicHealth(city);
    float securityLevel = calculateSecurityLevel(city);
    float infrastructureLevel = calculateInfrastructureLevel(city);

    growth = (economicHealth + securityLevel + infrastructureLevel) / 3.0f;
    m_populationGrowthCache[city] = growth;
    return growth;
}

void CityAI::optimizeTradeRoutes(const std::string &city)
{
    // TODO: Implémenter l'optimisation des routes commerciales
}

float CityAI::calculateTradeVolume(const std::string &city, const std::string &goodId)
{
    float demand = calculateResourceDemand(city, goodId);
    float supply = calculateResourceAvailability(city, goodId);
    float infrastructureLevel = calculateInfrastructureLevel(city);

    return std::min(demand, supply) * infrastructureLevel;
}

float CityAI::calculateMarketSaturation(const std::string &city, const std::string &goodId)
{
    float supply = calculateResourceAvailability(city, goodId);
    float demand = calculateResourceDemand(city, goodId);

    return supply / (supply + demand);
}

float CityAI::calculateDistance(const sf::Vector2f &pos1, const sf::Vector2f &pos2)
{
    float dx = pos2.x - pos1.x;
    float dy = pos2.y - pos1.y;
    return std::sqrt(dx * dx + dy * dy);
}

float CityAI::calculateResourceAvailability(const std::string &city, const std::string &goodId)
{
    // TODO: Implémenter le calcul de la disponibilité des ressources
    return 100.0f; // Valeur temporaire
}

float CityAI::calculateResourceQuality(const std::string &city, const std::string &goodId)
{
    // TODO: Implémenter le calcul de la qualité des ressources
    return 0.8f; // Valeur temporaire
}

float CityAI::calculateInfrastructureCost(const std::string &city, DevelopmentAction::Type type)
{
    float baseCost = 1000.0f;
    float infrastructureLevel = calculateInfrastructureLevel(city);
    float economicHealth = calculateEconomicHealth(city);

    switch (type)
    {
    case DevelopmentAction::Type::BUILD_MARKET:
        return baseCost * 2.0f * (1.0f + infrastructureLevel);
    case DevelopmentAction::Type::BUILD_WAREHOUSE:
        return baseCost * 1.5f * (1.0f + infrastructureLevel);
    case DevelopmentAction::Type::BUILD_SHIPYARD:
        return baseCost * 3.0f * (1.0f + infrastructureLevel);
    case DevelopmentAction::Type::BUILD_FORT:
        return baseCost * 4.0f * (1.0f + infrastructureLevel);
    case DevelopmentAction::Type::IMPROVE_INFRASTRUCTURE:
        return baseCost * (1.0f + infrastructureLevel);
    case DevelopmentAction::Type::RECRUIT_GUARDS:
        return baseCost * 0.5f * (1.0f + economicHealth);
    case DevelopmentAction::Type::TAX_ADJUSTMENT:
        return 0.0f;
    default:
        return baseCost;
    }
}

bool CityAI::canAffordAction(const std::string &city, float cost)
{
    // TODO: Implémenter la vérification des ressources financières
    return true; // Valeur temporaire
}

void CityAI::updateCityStats(const std::string &city)
{
    // TODO: Implémenter la mise à jour des statistiques de la ville
}