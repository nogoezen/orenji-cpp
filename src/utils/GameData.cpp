#include "GameData.h"
#include <iostream>

// Initialize static instance
std::unique_ptr<GameData> GameData::s_instance = nullptr;

void GameData::loadAllData()
{
    // Load all game data
    std::cout << "Loading game data..." << std::endl;

    loadTradeGoods();

    std::cout << "Game data loaded." << std::endl;
}

void GameData::loadTradeGoods()
{
    try
    {
        // Load trade goods from file
        nlohmann::json data = loadJsonFile("data/tradegoods.json");

        if (!data.is_array())
        {
            throw std::runtime_error("Trade goods data is not an array");
        }

        // Clear existing trade goods
        m_tradeGoods.clear();

        // Parse trade goods
        for (const auto &item : data)
        {
            // Create trade good using the JSON data directly
            TradeGood good(item);

            // Add to map
            m_tradeGoods[good.getId()] = good;
        }

        std::cout << "Loaded " << m_tradeGoods.size() << " trade goods." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading trade goods: " << e.what() << std::endl;
    }
}

const TradeGood *GameData::findTradeGoodById(int id) const
{
    auto it = m_tradeGoods.find(id);
    if (it != m_tradeGoods.end())
    {
        return &(it->second);
    }
    return nullptr;
}