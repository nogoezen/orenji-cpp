#include "MerchantAI.h"
#include <algorithm>
#include <cmath>

MerchantAI::MerchantAI(std::shared_ptr<World> world, std::shared_ptr<TradingSystem> tradingSystem)
    : m_world(world), m_tradingSystem(tradingSystem)
{
}

void MerchantAI::update(float deltaTime)
{
    // Mise à jour des tendances du marché
    for (const auto &city : m_world->getCities())
    {
        for (const auto &good : m_tradingSystem->getAvailableGoods())
        {
            analyzeMarketTrend(good, city->getName());
        }
        analyzeCompetition(city->getName());
        analyzePoliticalStability(city->getName());
    }
}

std::vector<MerchantAI::TradeRoute> MerchantAI::findBestTradeRoutes(const std::string &currentCity)
{
    std::vector<TradeRoute> routes;

    for (const auto &city : m_world->getCities())
    {
        if (city->getName() != currentCity)
        {
            TradeRoute route;
            route.sourceCity = currentCity;
            route.destinationCity = city->getName();
            route.distance = calculateDistance(currentCity, city->getName());
            route.riskLevel = calculateRiskLevel(currentCity, city->getName());

            // Trouver les meilleures marchandises pour cette route
            for (const auto &good : m_tradingSystem->getAvailableGoods())
            {
                if (shouldBuyGood(good, currentCity) && shouldSellGood(good, city->getName()))
                {
                    route.goods.push_back(good);
                }
            }

            if (!route.goods.empty())
            {
                route.expectedProfit = calculateRouteProfit(currentCity, city->getName());
                routes.push_back(route);
            }
        }
    }

    // Trier les routes par profit attendu
    std::sort(routes.begin(), routes.end(),
              [](const TradeRoute &a, const TradeRoute &b)
              {
                  return a.expectedProfit > b.expectedProfit;
              });

    return routes;
}

float MerchantAI::calculateRouteProfit(const std::string &source, const std::string &destination)
{
    float totalProfit = 0.0f;

    for (const auto &good : m_tradingSystem->getAvailableGoods())
    {
        if (shouldBuyGood(good, source) && shouldSellGood(good, destination))
        {
            float buyPrice = m_tradingSystem->getGoodPrice(good, source);
            float sellPrice = m_tradingSystem->getGoodPrice(good, destination);
            float quantity = determineOptimalQuantity(good, source);

            float profit = (sellPrice - buyPrice) * quantity;
            float riskFactor = calculateRiskLevel(source, destination);

            totalProfit += profit * (1.0f - riskFactor);
        }
    }

    return totalProfit;
}

float MerchantAI::calculateRiskLevel(const std::string &source, const std::string &destination)
{
    float distanceRisk = calculateDistance(source, destination) / 1000.0f;
    float politicalRisk = (analyzePoliticalStability(source) + analyzePoliticalStability(destination)) / 2.0f;
    float competitionRisk = (analyzeCompetition(source) + analyzeCompetition(destination)) / 2.0f;

    return (distanceRisk * 0.3f + politicalRisk * 0.4f + competitionRisk * 0.3f);
}

bool MerchantAI::shouldBuyGood(const std::string &good, const std::string &city)
{
    float marketTrend = analyzeMarketTrend(good, city);
    float supplyDemandRatio = calculateSupplyDemandRatio(good, city);
    float priceVolatility = calculatePriceVolatility(good, city);

    // Conditions d'achat
    bool goodMarketTrend = marketTrend > 0.5f;
    bool goodSupplyDemand = supplyDemandRatio < 0.7f;
    bool stablePrice = priceVolatility < 0.3f;

    return goodMarketTrend && goodSupplyDemand && stablePrice;
}

bool MerchantAI::shouldSellGood(const std::string &good, const std::string &city)
{
    float marketTrend = analyzeMarketTrend(good, city);
    float supplyDemandRatio = calculateSupplyDemandRatio(good, city);
    float priceVolatility = calculatePriceVolatility(good, city);

    // Conditions de vente
    bool highDemand = supplyDemandRatio > 1.3f;
    bool goodPrice = marketTrend > 0.7f;
    bool stableMarket = priceVolatility < 0.4f;

    return highDemand && goodPrice && stableMarket;
}

int MerchantAI::determineOptimalQuantity(const std::string &good, const std::string &city)
{
    float marketTrend = analyzeMarketTrend(good, city);
    float supplyDemandRatio = calculateSupplyDemandRatio(good, city);
    float priceVolatility = calculatePriceVolatility(good, city);

    // Calcul de la quantité optimale basé sur plusieurs facteurs
    int baseQuantity = 100;
    float quantityMultiplier = (1.0f + marketTrend) * (1.0f - priceVolatility);

    return static_cast<int>(baseQuantity * quantityMultiplier);
}

float MerchantAI::analyzeMarketTrend(const std::string &good, const std::string &city)
{
    // Utiliser le cache si disponible
    std::string key = good + "_" + city;
    if (m_marketTrends.find(key) != m_marketTrends.end())
    {
        return m_marketTrends[key];
    }

    // Calculer la tendance du marché
    float currentPrice = m_tradingSystem->getGoodPrice(good, city);
    float basePrice = m_tradingSystem->getBasePrice(good);
    float priceRatio = currentPrice / basePrice;

    // Stocker dans le cache
    m_marketTrends[key] = priceRatio;

    return priceRatio;
}

float MerchantAI::analyzeCompetition(const std::string &city)
{
    // Utiliser le cache si disponible
    if (m_competitionLevels.find(city) != m_competitionLevels.end())
    {
        return m_competitionLevels[city];
    }

    // Analyser le niveau de concurrence
    float competition = 0.5f; // Valeur par défaut

    // TODO: Implémenter une analyse plus sophistiquée de la concurrence
    // basée sur le nombre de marchands, les prix moyens, etc.

    // Stocker dans le cache
    m_competitionLevels[city] = competition;

    return competition;
}

float MerchantAI::analyzePoliticalStability(const std::string &city)
{
    // Utiliser le cache si disponible
    if (m_politicalStability.find(city) != m_politicalStability.end())
    {
        return m_politicalStability[city];
    }

    // Analyser la stabilité politique
    float stability = 0.7f; // Valeur par défaut

    // TODO: Implémenter une analyse plus sophistiquée de la stabilité politique
    // basée sur les relations diplomatiques, les guerres, etc.

    // Stocker dans le cache
    m_politicalStability[city] = stability;

    return stability;
}

float MerchantAI::calculateDistance(const std::string &city1, const std::string &city2)
{
    // TODO: Implémenter le calcul de distance entre les villes
    return 100.0f; // Valeur temporaire
}

float MerchantAI::calculateSupplyDemandRatio(const std::string &good, const std::string &city)
{
    // TODO: Implémenter le calcul du ratio offre/demande
    return 1.0f; // Valeur temporaire
}

float MerchantAI::calculatePriceVolatility(const std::string &good, const std::string &city)
{
    // TODO: Implémenter le calcul de la volatilité des prix
    return 0.2f; // Valeur temporaire
}

float MerchantAI::calculateTradeVolume(const std::string &good, const std::string &city)
{
    // TODO: Implémenter le calcul du volume de commerce
    return 100.0f; // Valeur temporaire
}