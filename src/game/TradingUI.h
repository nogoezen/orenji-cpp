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
    TradingSystem &m_tradingSystem;

    // Internal methods for various screens
    void displayMarketInfo(int cityId);
    void displayTradeGoods(int cityId, bool buying);
    void displayPlayerCargo();
    void displayTradeRoutes(int cityId);
    void handleBuyGoods(int cityId);
    void handleSellGoods(int cityId);
    void displayPriceComparison(int cityId, int goodId);
    void displayKingdomRelations();

    // Nouvelles méthodes
    void displayActiveEvents();
    void displayTradeSkills();
    void displayPriceHistory(int cityId, int goodId);
    void displayProfitableRoutes(int cityId);
    void createTradeRoute(int sourceCity);
    void manageExistingRoutes();
    void improveTradeSkills();
    void displayBlackMarket(int cityId);

public:
    TradingUI(Player &player, ConsoleUI &ui, TradingSystem &tradingSystem);
    ~TradingUI() = default;

    // Main menu for trading
    void showTradingMenu(int cityId);

    // Market screen (buy/sell)
    void showMarketScreen(int cityId);

    // Trade routes screen
    void showTradeRoutesScreen(int cityId);

    // Price comparison between markets
    void showPriceComparisonScreen(int cityId);

    // Kingdom trade relations screen
    void showKingdomRelationsScreen();

    // Nouvelles méthodes publiques
    void showTradeEventsScreen();
    void showTradeSkillsScreen();
    void showBlackMarketScreen(int cityId);
};