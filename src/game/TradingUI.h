#pragma once

#include "../models/Player.h"
#include "../core/TradingSystem.h"
#include "../ui/GuiUI.h"
#include <vector>
#include <string>

// Class to handle trading UI
class TradingUI
{
private:
    Player &m_player;
    GuiUI &m_ui;
    TradingSystem &m_tradingSystem;
    int m_currentCityId;

    // Internal methods for various screens
    void displayMarketInfo(int cityId);
    void displayPlayerInventory();
    void displayPriceHistory(int cityId, int goodId);
    void displayEvents();

    // Méthodes d'achat et de vente
    bool buyGoods(int cityId);
    bool sellGoods(int cityId);
    bool exchangeWithCaravan();

    // Méthodes de navigation
    void showRouteOptions(int cityId);
    void createTradeRoute(int cityId);
    void manageTradeRoutes();

    // Méthodes utilitaires
    std::string formatPrice(int price) const;
    std::string getGoodName(int goodId) const;
    std::string getCityName(int cityId) const;

public:
    TradingUI(Player &player, GuiUI &ui, TradingSystem &tradingSystem);
    ~TradingUI() = default;

    // Main menu for trading
    void showTradeMenu(int cityId);

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

    void updatePrices(int days);
};