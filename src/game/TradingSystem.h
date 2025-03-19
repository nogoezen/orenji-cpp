#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <random>
#include <functional>
#include "../models/TradeGood.h"
#include "../models/Player.h"
#include "../models/Fleet.h"
#include "../models/Ship.h"
#include "World.h"
#include "GameData.h"

// Structure for market prices
struct MarketPrice
{
    int goodId;
    std::string goodName;
    int basePrice;
    int buyPrice;
    int sellPrice;
    int available;
    int demand;
    std::string category;
};

// Structure for trade routes
struct TradeRoute
{
    int sourceId;
    std::string sourceName;
    int destId;
    std::string destName;
    float distance;
    float risk;
    std::vector<int> profitableGoods;
    int expectedProfit;
};

// Market class to handle trading in a specific location
class Market
{
private:
    int m_locationId;
    std::string m_locationName;
    int m_prosperity;
    std::vector<int> m_specialties;
    std::vector<int> m_demands;
    std::map<int, float> m_priceFactors;
    std::map<int, int> m_availability;
    float m_taxRate;

    // Fluctuation parameters
    std::mt19937 m_rng;
    float m_volatility;
    int m_lastUpdateDay;

public:
    Market(int locationId, const std::string &name, int prosperity,
           const std::vector<int> &specialties, const std::vector<int> &demands);

    // Initialize market with trade goods
    void initialize(const std::vector<TradeGood> &goods);

    // Update prices based on day cycle
    void updatePrices(int currentDay);

    // Get price information for all goods
    std::vector<MarketPrice> getAllPrices() const;

    // Get price for a specific good
    MarketPrice getPriceInfo(int goodId) const;

    // Buy goods from market
    bool buyGoods(Player &player, int goodId, int quantity);

    // Sell goods to market
    bool sellGoods(Player &player, int goodId, int quantity);

    // Check if a good is in demand
    bool isInDemand(int goodId) const;

    // Check if a good is a specialty
    bool isSpecialty(int goodId) const;

    // Get prosperity level
    int getProsperity() const { return m_prosperity; }

    // Get location info
    int getLocationId() const { return m_locationId; }
    std::string getLocationName() const { return m_locationName; }
};

// Trading system manager class
class TradingSystem
{
private:
    std::map<int, std::unique_ptr<Market>> m_markets;
    std::vector<TradeRoute> m_tradeRoutes;

    // Singleton instance
    static std::unique_ptr<TradingSystem> s_instance;

    // Private constructor for singleton
    TradingSystem();

public:
    // Get singleton instance
    static TradingSystem &getInstance();

    // Initialize all markets from game data
    void initialize();

    // Get market for a location
    Market *getMarket(int locationId);

    // Generate trade routes between markets
    void generateTradeRoutes();

    // Get all available trade routes
    std::vector<TradeRoute> getAvailableTradeRoutes(int sourceLocationId) const;

    // Find the most profitable trade routes
    std::vector<TradeRoute> findProfitableRoutes(int sourceLocationId, int maxRoutes = 5) const;

    // Calculate profit potential between two markets
    int calculateProfitPotential(int sourceId, int destId) const;

    // Update all markets
    void updateMarkets(int currentDay);

    // Get trade reputation with different kingdoms
    float getKingdomReputation(const std::string &kingdom) const;

    // Apply kingdom specific trade bonuses
    float getKingdomTradeBonus(const std::string &kingdom) const;
};