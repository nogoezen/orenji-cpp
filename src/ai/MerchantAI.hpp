#pragma once

#include "../models/Player.hpp"
#include "../models/World.hpp"
#include "../core/TradingSystem.hpp"
#include <vector>
#include <memory>
#include <string>

class MerchantAI
{
public:
    struct TradeRoute
    {
        std::string sourceCity;
        std::string destinationCity;
        std::vector<std::string> goods;
        float expectedProfit;
        float riskLevel;
        int distance;
    };

    MerchantAI(std::shared_ptr<World> world, std::shared_ptr<TradingSystem> tradingSystem);
    ~MerchantAI() = default;

    // Méthodes principales
    void update(float deltaTime);
    std::vector<TradeRoute> findBestTradeRoutes(const std::string &currentCity);
    float calculateRouteProfit(const std::string &source, const std::string &destination);
    float calculateRiskLevel(const std::string &source, const std::string &destination);

    // Méthodes de décision
    bool shouldBuyGood(const std::string &good, const std::string &city);
    bool shouldSellGood(const std::string &good, const std::string &city);
    int determineOptimalQuantity(const std::string &good, const std::string &city);

    // Méthodes d'analyse
    float analyzeMarketTrend(const std::string &good, const std::string &city);
    float analyzeCompetition(const std::string &city);
    float analyzePoliticalStability(const std::string &city);

private:
    std::shared_ptr<World> m_world;
    std::shared_ptr<TradingSystem> m_tradingSystem;

    // Cache pour les calculs
    std::map<std::string, float> m_marketTrends;
    std::map<std::string, float> m_competitionLevels;
    std::map<std::string, float> m_politicalStability;

    // Méthodes utilitaires
    float calculateDistance(const std::string &city1, const std::string &city2);
    float calculateSupplyDemandRatio(const std::string &good, const std::string &city);
    float calculatePriceVolatility(const std::string &good, const std::string &city);

