#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>
#include "JsonLoader.h"
#include "../models/TradeGood.h"

// Class to manage game data
class GameData
{
private:
    // Singleton instance
    static std::unique_ptr<GameData> s_instance;

    // Trade goods by ID
    std::unordered_map<int, TradeGood> m_tradeGoods;

    // Private constructor (singleton)
    GameData() {}

    // Load trade goods from file
    void loadTradeGoods();

public:
    // Get singleton instance
    static GameData &getInstance()
    {
        if (!s_instance)
        {
            s_instance = std::unique_ptr<GameData>(new GameData());
        }
        return *s_instance;
    }

    // Load all game data
    void loadAllData();

    // Find a trade good by ID
    const TradeGood *findTradeGoodById(int id) const;

    // Get all trade goods
    const std::unordered_map<int, TradeGood> &getTradeGoods() const
    {
        return m_tradeGoods;
    }

    // Delegate to JsonLoader
    nlohmann::json loadJsonFile(const std::string &filePath, bool useCache = true)
    {
        return JsonLoader::loadJsonFile(filePath, useCache);
    }
};