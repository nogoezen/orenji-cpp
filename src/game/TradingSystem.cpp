#include "TradingSystem.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>
#include "../utils/GameData.h"

// Initialize static instance
std::unique_ptr<TradingSystem> TradingSystem::s_instance = nullptr;

// Market constructor
Market::Market(int locationId, const std::string &name, int prosperity,
               const std::vector<int> &specialties, const std::vector<int> &demands)
    : m_locationId(locationId), m_locationName(name), m_prosperity(prosperity),
      m_specialties(specialties), m_demands(demands), m_taxRate(0.05f),
      m_volatility(0.1f), m_lastUpdateDay(0)
{
    // Initialize random number generator
    std::random_device rd;
    m_rng.seed(rd());
}

void Market::initialize(const std::vector<TradeGood> &goods)
{
    // Initialize price factors and availability for all goods
    for (const auto &good : goods)
    {
        int goodId = good.getId();

        // Base price factor based on prosperity
        float baseFactor = 1.0f + (m_prosperity - 5) * 0.05f;

        // Adjust for specialties (lower prices)
        if (std::find(m_specialties.begin(), m_specialties.end(), goodId) != m_specialties.end())
        {
            baseFactor *= 0.8f;
        }

        // Adjust for demands (higher prices)
        if (std::find(m_demands.begin(), m_demands.end(), goodId) != m_demands.end())
        {
            baseFactor *= 1.2f;
        }

        // Store price factor
        m_priceFactors[goodId] = baseFactor;

        // Initialize availability (higher for specialties, lower for demands)
        int baseAvailability = 50 + m_prosperity * 10;
        if (std::find(m_specialties.begin(), m_specialties.end(), goodId) != m_specialties.end())
        {
            baseAvailability *= 2;
        }
        if (std::find(m_demands.begin(), m_demands.end(), goodId) != m_demands.end())
        {
            baseAvailability /= 2;
        }

        m_availability[goodId] = baseAvailability;
    }
}

void Market::updatePrices(int currentDay)
{
    // Only update once per day
    if (m_lastUpdateDay == currentDay)
        return;

    // Update availability and price factors based on randomness
    std::uniform_real_distribution<float> dist(-m_volatility, m_volatility);

    for (auto &[goodId, factor] : m_priceFactors)
    {
        // Random fluctuation
        float fluctuation = 1.0f + dist(m_rng);
        factor *= fluctuation;

        // Ensure factors stay within reasonable bounds
        factor = std::max(0.5f, std::min(factor, 2.0f));
    }

    for (auto &[goodId, available] : m_availability)
    {
        // Random fluctuation in availability
        float fluctuation = 1.0f + dist(m_rng) * 0.5f;
        available = static_cast<int>(available * fluctuation);

        // Ensure availability stays within reasonable bounds
        available = std::max(10, std::min(available, 500));
    }

    m_lastUpdateDay = currentDay;
}

std::vector<MarketPrice> Market::getAllPrices() const
{
    std::vector<MarketPrice> prices;

    for (const auto &[goodId, factor] : m_priceFactors)
    {
        // Skip invalid goods
        if (goodId <= 0)
            continue;

        const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
        if (!good)
            continue;

        MarketPrice price;
        price.goodId = goodId;
        price.goodName = good->getName();
        price.category = good->getCategory();
        price.basePrice = good->getBasePrice();

        // Calculate buy and sell prices
        float buyFactor = factor * (1.0f + m_taxRate);
        float sellFactor = factor * (1.0f - m_taxRate);

        price.buyPrice = static_cast<int>(price.basePrice * buyFactor);
        price.sellPrice = static_cast<int>(price.basePrice * sellFactor);

        // Set availability
        auto it = m_availability.find(goodId);
        price.available = (it != m_availability.end()) ? it->second : 0;

        // Set demand level
        price.demand = 0;
        if (std::find(m_demands.begin(), m_demands.end(), goodId) != m_demands.end())
        {
            price.demand = 2; // High demand
        }
        else if (factor > 1.1f)
        {
            price.demand = 1; // Medium demand
        }

        prices.push_back(price);
    }

    return prices;
}

MarketPrice Market::getPriceInfo(int goodId) const
{
    MarketPrice price;
    price.goodId = goodId;

    const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
    if (!good)
    {
        // Return empty price info
        price.goodName = "Unknown";
        price.basePrice = 0;
        price.buyPrice = 0;
        price.sellPrice = 0;
        price.available = 0;
        price.demand = 0;
        price.category = "Unknown";
        return price;
    }

    price.goodName = good->getName();
    price.category = good->getCategory();
    price.basePrice = good->getBasePrice();

    // Get price factor
    float factor = 1.0f;
    auto factorIt = m_priceFactors.find(goodId);
    if (factorIt != m_priceFactors.end())
    {
        factor = factorIt->second;
    }

    // Calculate buy and sell prices
    float buyFactor = factor * (1.0f + m_taxRate);
    float sellFactor = factor * (1.0f - m_taxRate);

    price.buyPrice = static_cast<int>(price.basePrice * buyFactor);
    price.sellPrice = static_cast<int>(price.basePrice * sellFactor);

    // Set availability
    auto availIt = m_availability.find(goodId);
    price.available = (availIt != m_availability.end()) ? availIt->second : 0;

    // Set demand level
    price.demand = 0;
    if (std::find(m_demands.begin(), m_demands.end(), goodId) != m_demands.end())
    {
        price.demand = 2; // High demand
    }
    else if (factor > 1.1f)
    {
        price.demand = 1; // Medium demand
    }

    return price;
}

bool Market::buyGoods(Player &player, int goodId, int quantity)
{
    // Check if the good exists
    const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
    if (!good)
        return false;

    // Get price info
    MarketPrice price = getPriceInfo(goodId);

    // Check availability
    if (price.available < quantity)
        return false;

    // Calculate total cost
    int totalCost = price.buyPrice * quantity;

    // Check if player has enough gold
    if (player.getGold() < totalCost)
        return false;

    // Check if player has enough cargo space
    int unitWeight = good->getWeight();
    int totalWeight = unitWeight * quantity;

    if (player.getRemainingCargoCapacity() < totalWeight)
        return false;

    // Process the transaction
    player.removeGold(totalCost);
    player.addCargo(goodId, good->getName(), quantity, unitWeight);

    // Update availability
    m_availability[goodId] -= quantity;

    return true;
}

bool Market::sellGoods(Player &player, int goodId, int quantity)
{
    // Check if the good exists
    const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
    if (!good)
        return false;

    // Check if player has enough of the good
    if (!player.hasCargo(goodId, quantity))
        return false;

    // Get price info
    MarketPrice price = getPriceInfo(goodId);

    // Calculate total payment
    int totalPayment = price.sellPrice * quantity;

    // Process the transaction
    player.addGold(totalPayment);
    player.removeCargo(goodId, quantity);

    // Update availability
    m_availability[goodId] += quantity;

    return true;
}

bool Market::isInDemand(int goodId) const
{
    return std::find(m_demands.begin(), m_demands.end(), goodId) != m_demands.end();
}

bool Market::isSpecialty(int goodId) const
{
    return std::find(m_specialties.begin(), m_specialties.end(), goodId) != m_specialties.end();
}

// Trading system implementation
TradingSystem::TradingSystem()
{
    // Constructor
}

TradingSystem &TradingSystem::getInstance()
{
    if (!s_instance)
    {
        s_instance = std::unique_ptr<TradingSystem>(new TradingSystem());
    }
    return *s_instance;
}

void TradingSystem::initialize()
{
    // Clear existing markets
    m_markets.clear();

    // Get all cities from the world
    const auto &cities = World::getInstance().getCities();

    // Get all trade goods
    const auto &goodsMap = GameData::getInstance().getTradeGoods();
    std::vector<TradeGood> goods;
    for (const auto &[id, good] : goodsMap)
    {
        goods.push_back(good);
    }

    // Create markets for each city
    for (const auto &city : cities)
    {
        // Determine specialties and demands based on region
        std::vector<int> specialties;
        std::vector<int> demands;

        for (const auto &good : goods)
        {
            // Check if the good's specialty region matches the city region
            if (good.isSpecialtyRegion(city.region))
            {
                specialties.push_back(good.getId());
            }
            else
            {
                // Randomly select some goods that are in demand
                if (rand() % 10 < 3) // 30% chance to be in demand
                {
                    demands.push_back(good.getId());
                }
            }
        }

        // Create market
        auto market = std::make_unique<Market>(city.id, city.name, city.population / 10000, specialties, demands);

        // Initialize market with goods
        market->initialize(goods);

        // Add to map
        m_markets[city.id] = std::move(market);
    }

    // Generate trade routes
    generateTradeRoutes();

    std::cout << "Initialized " << m_markets.size() << " markets." << std::endl;
}

Market *TradingSystem::getMarket(int locationId)
{
    auto it = m_markets.find(locationId);
    if (it != m_markets.end())
    {
        return it->second.get();
    }
    return nullptr;
}

void TradingSystem::generateTradeRoutes()
{
    m_tradeRoutes.clear();

    // Get all cities
    const auto &cities = World::getInstance().getCities();

    // Generate routes between all pairs of cities
    for (const auto &sourceCity : cities)
    {
        for (const auto &destCity : cities)
        {
            // Skip self-routes
            if (sourceCity.id == destCity.id)
                continue;

            // Calculate distance
            float dx = static_cast<float>(destCity.posX - sourceCity.posX);
            float dy = static_cast<float>(destCity.posY - sourceCity.posY);
            float distance = std::sqrt(dx * dx + dy * dy);

            // Calculate risk based on distance
            float risk = std::min(0.8f, distance / 200.0f);

            // Find profitable goods
            std::vector<int> profitableGoods;
            int expectedProfit = calculateProfitPotential(sourceCity.id, destCity.id);

            // Create trade route
            TradeRoute route;
            route.sourceId = sourceCity.id;
            route.sourceName = sourceCity.name;
            route.destId = destCity.id;
            route.destName = destCity.name;
            route.distance = distance;
            route.risk = risk;
            route.profitableGoods = profitableGoods;
            route.expectedProfit = expectedProfit;

            m_tradeRoutes.push_back(route);
        }
    }
}

std::vector<TradeRoute> TradingSystem::getAvailableTradeRoutes(int sourceLocationId) const
{
    std::vector<TradeRoute> routes;

    for (const auto &route : m_tradeRoutes)
    {
        if (route.sourceId == sourceLocationId)
        {
            routes.push_back(route);
        }
    }

    return routes;
}

std::vector<TradeRoute> TradingSystem::findProfitableRoutes(int sourceLocationId, int maxRoutes) const
{
    // Get all routes from this location
    auto routes = getAvailableTradeRoutes(sourceLocationId);

    // Sort by expected profit
    std::sort(routes.begin(), routes.end(), [](const TradeRoute &a, const TradeRoute &b)
              { return a.expectedProfit > b.expectedProfit; });

    // Limit the number of routes
    if (routes.size() > static_cast<size_t>(maxRoutes))
    {
        routes.resize(maxRoutes);
    }

    return routes;
}

int TradingSystem::calculateProfitPotential(int sourceId, int destId) const
{
    // Get markets
    auto sourceIt = m_markets.find(sourceId);
    auto destIt = m_markets.find(destId);

    if (sourceIt == m_markets.end() || destIt == m_markets.end())
    {
        return 0;
    }

    const Market &source = *(sourceIt->second);
    const Market &dest = *(destIt->second);

    // Get prices for all goods
    std::vector<MarketPrice> sourcePrices = source.getAllPrices();

    int totalProfit = 0;

    // Calculate potential profit for each good
    for (const auto &price : sourcePrices)
    {
        // Skip if not available in source market
        if (price.available <= 0)
            continue;

        // Get destination price
        MarketPrice destPrice = dest.getPriceInfo(price.goodId);

        // Calculate profit per unit
        int profitPerUnit = destPrice.sellPrice - price.buyPrice;

        // If profitable, add to total
        if (profitPerUnit > 0)
        {
            // Limit by availability and a reasonable quantity (player's cargo space)
            int maxQuantity = std::min(price.available, 100);
            totalProfit += profitPerUnit * maxQuantity;
        }
    }

    return totalProfit;
}

void TradingSystem::updateMarkets(int currentDay)
{
    for (auto &[locationId, market] : m_markets)
    {
        market->updatePrices(currentDay);
    }
}

float TradingSystem::getKingdomReputation(const std::string &kingdom) const
{
    // In a real implementation, this would get the player's reputation with the specified kingdom
    // For now, return a random value between 0 and 10
    static std::map<std::string, float> reputations = {
        {"Espagne", 5.0f},
        {"Portugal", 7.5f},
        {"France", 3.2f},
        {"Angleterre", 4.8f},
        {"Venise", 6.3f},
        {"Empire Ottoman", 2.1f},
        {"Ming", 1.5f},
        {"Japon", 0.9f}};

    auto it = reputations.find(kingdom);
    return (it != reputations.end()) ? it->second : 0.0f;
}

float TradingSystem::getKingdomTradeBonus(const std::string &kingdom) const
{
    // Calculate trade bonus based on reputation
    float reputation = getKingdomReputation(kingdom);

    // Normalize to 0-1 range (assuming max reputation is 10)
    float normalizedRep = reputation / 10.0f;

    // Calculate bonus (0% to 25%)
    return normalizedRep * 0.25f;
}