#pragma once

#include "../models/World.hpp"
#include "../models/Kingdom.hpp"
#include "../models/City.hpp"
#include <vector>
#include <memory>
#include <string>
#include <map>

class DiplomaticAI
{
public:
    struct DiplomaticAction
    {
        enum class Type
        {
            DECLARE_WAR,
            MAKE_PEACE,
            FORM_ALLIANCE,
            BREAK_ALLIANCE,
            TRADE_AGREEMENT,
            EMBARGO,
            GIFT,
            DEMAND_TRIBUTE
        };

        Type type;
        std::string sourceKingdom;
        std::string targetKingdom;
        float priority;
        std::map<std::string, float> conditions;
    };

    struct TradeAgreement
    {
        std::string sourceKingdom;
        std::string targetKingdom;
        std::vector<std::string> goods;
        float taxRate;
        bool isExclusive;
        int duration;
    };

    DiplomaticAI(std::shared_ptr<World> world);
    ~DiplomaticAI() = default;

    // Méthodes principales
    void update(float deltaTime);
    std::vector<DiplomaticAction> evaluateDiplomaticActions(const std::string &kingdom);
    bool executeDiplomaticAction(const DiplomaticAction &action);
    float calculateRelations(const std::string &kingdom1, const std::string &kingdom2);

    // Méthodes de gestion des relations
    void updateRelations(const std::string &kingdom1, const std::string &kingdom2, float delta);
    float calculateTradeValue(const std::string &kingdom1, const std::string &kingdom2);
    float calculateMilitaryBalance(const std::string &kingdom1, const std::string &kingdom2);
    float calculateCulturalInfluence(const std::string &kingdom1, const std::string &kingdom2);

    // Méthodes de gestion des villes
    void updateCityRelations(const std::string &city, const std::string &kingdom);
    float calculateCityLoyalty(const std::string &city, const std::string &kingdom);
    bool shouldCityRevolt(const std::string &city, const std::string &kingdom);
    std::vector<std::string> getPotentialAllies(const std::string &kingdom);

private:
    std::shared_ptr<World> m_world;

    // Cache pour les calculs
    std::map<std::string, float> m_relationsCache;
    std::map<std::string, float> m_tradeValueCache;
    std::map<std::string, float> m_militaryBalanceCache;
    std::map<std::string, float> m_culturalInfluenceCache;

    // Méthodes utilitaires
    float calculateDistance(const sf::Vector2f &pos1, const sf::Vector2f &pos2);
    float calculateEconomicStrength(const std::string &kingdom);
    float calculateMilitaryStrength(const std::string &kingdom);
    float calculateCulturalStrength(const std::string &kingdom);
    bool areKingdomsAtWar(const std::string &kingdom1, const std::string &kingdom2);
    bool areKingdomsAllied(const std::string &kingdom1, const std::string &kingdom2);
};
