#pragma once

#include "../models/World.hpp"
#include "../models/City.hpp"
#include "../models/TradeGood.hpp"
#include <vector>
#include <memory>
#include <string>
#include <map>

class CityAI
{
public:
    struct ResourceProduction
    {
        std::string goodId;
        float productionRate;
        float consumptionRate;
        float stockLevel;
        float price;
        bool isExportable;
    };

    struct DevelopmentAction
    {
        enum class Type
        {
            BUILD_MARKET,
            BUILD_WAREHOUSE,
            BUILD_SHIPYARD,
            BUILD_FORT,
            IMPROVE_INFRASTRUCTURE,
            RECRUIT_GUARDS,
            TAX_ADJUSTMENT
        };

        Type type;
        float priority;
        float cost;
        std::map<std::string, float> benefits;
    };

    struct TradePolicy
    {
        std::string goodId;
        float importTax;
        float exportTax;
        bool allowImport;
        bool allowExport;
        float minStock;
        float maxStock;
    };

    CityAI(std::shared_ptr<World> world);
    ~CityAI() = default;

    // Méthodes principales
    void update(float deltaTime);
    std::vector<DevelopmentAction> evaluateDevelopmentActions(const std::string &city);
    bool executeDevelopmentAction(const std::string &city, const DevelopmentAction &action);
    void updateTradePolicies(const std::string &city);

    // Méthodes de gestion des ressources
    void updateResourceProduction(const std::string &city);
    float calculateResourceEfficiency(const std::string &city, const std::string &goodId);
    float calculateResourceDemand(const std::string &city, const std::string &goodId);
    void adjustPrices(const std::string &city);

    // Méthodes de développement
    float calculateInfrastructureLevel(const std::string &city);
    float calculateSecurityLevel(const std::string &city);
    float calculateEconomicHealth(const std::string &city);
    float calculatePopulationGrowth(const std::string &city);

    // Méthodes de gestion du commerce
    void optimizeTradeRoutes(const std::string &city);
    float calculateTradeVolume(const std::string &city, const std::string &goodId);
    float calculateMarketSaturation(const std::string &city, const std::string &goodId);

private:
    std::shared_ptr<World> m_world;

    // Cache pour les calculs
    std::map<std::string, float> m_infrastructureCache;
    std::map<std::string, float> m_securityCache;
    std::map<std::string, float> m_economicHealthCache;
    std::map<std::string, float> m_populationGrowthCache;

    // Méthodes utilitaires
    float calculateDistance(const sf::Vector2f &pos1, const sf::Vector2f &pos2);
    float calculateResourceAvailability(const std::string &city, const std::string &goodId);
    float calculateResourceQuality(const std::string &city, const std::string &goodId);
    float calculateInfrastructureCost(const std::string &city, DevelopmentAction::Type type);
    bool canAffordAction(const std::string &city, float cost);
    void updateCityStats(const std::string &city);
};
