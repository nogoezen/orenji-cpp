#pragma once

#include <string>
#include <vector>
#include <memory>
#include "TradingSystem.h"
#include "ConsoleUI.h"
#include "../models/Player.h"

// Class to handle trading UI
class TradingUI
{
private:
    Player &m_player;
    ConsoleUI &m_ui;

    // Internal methods for various screens
    void displayMarketInfo(Market *market);
    void displayTradeGoods(Market *market, bool buying);
    void displayPlayerCargo();
    void displayTradeRoutes(int locationId);
    void handleBuyGoods(Market *market);
    void handleSellGoods(Market *market);
    void displayPriceComparison(int goodId);
    void displayKingdomRelations();

public:
    TradingUI(Player &player, ConsoleUI &ui);
    ~TradingUI() = default;

    // Main menu for trading
    void showTradingMenu(int locationId);

    // Market screen (buy/sell)
    void showMarketScreen(int locationId);

    // Trade routes screen
    void showTradeRoutesScreen(int locationId);

    // Price comparison between markets
    void showPriceComparisonScreen(int locationId);

    // Kingdom trade relations screen
    void showKingdomRelationsScreen();
};