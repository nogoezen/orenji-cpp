#include "DiplomaticAI.hpp"
#include <algorithm>
#include <cmath>

DiplomaticAI::DiplomaticAI(std::shared_ptr<World> world)
    : m_world(world)
{
}

void DiplomaticAI::update(float deltaTime)
{
    // Mise à jour des caches
    m_relationsCache.clear();
    m_tradeValueCache.clear();
    m_militaryBalanceCache.clear();
    m_culturalInfluenceCache.clear();

    // Mise à jour des relations entre tous les royaumes
    for (const auto &kingdom1 : m_world->getKingdoms())
    {
        for (const auto &kingdom2 : m_world->getKingdoms())
        {
            if (kingdom1->getName() != kingdom2->getName())
            {
                updateRelations(kingdom1->getName(), kingdom2->getName(), deltaTime);
            }
        }
    }
}

std::vector<DiplomaticAI::DiplomaticAction> DiplomaticAI::evaluateDiplomaticActions(const std::string &kingdom)
{
    std::vector<DiplomaticAction> actions;

    for (const auto &otherKingdom : m_world->getKingdoms())
    {
        if (otherKingdom->getName() == kingdom)
        {
            continue;
        }

        float relations = calculateRelations(kingdom, otherKingdom->getName());
        float militaryBalance = calculateMilitaryBalance(kingdom, otherKingdom->getName());
        float tradeValue = calculateTradeValue(kingdom, otherKingdom->getName());
        float culturalInfluence = calculateCulturalInfluence(kingdom, otherKingdom->getName());

        // Évaluation des actions possibles
        if (relations < -0.5f && militaryBalance > 1.2f)
        {
            DiplomaticAction action;
            action.type = DiplomaticAction::Type::DECLARE_WAR;
            action.sourceKingdom = kingdom;
            action.targetKingdom = otherKingdom->getName();
            action.priority = (1.0f - relations) * militaryBalance;
            action.conditions["military_advantage"] = militaryBalance;
            action.conditions["poor_relations"] = relations;
            actions.push_back(action);
        }
        else if (relations > 0.7f && !areKingdomsAllied(kingdom, otherKingdom->getName()))
        {
            DiplomaticAction action;
            action.type = DiplomaticAction::Type::FORM_ALLIANCE;
            action.sourceKingdom = kingdom;
            action.targetKingdom = otherKingdom->getName();
            action.priority = relations * (1.0f + tradeValue);
            action.conditions["good_relations"] = relations;
            action.conditions["trade_value"] = tradeValue;
            actions.push_back(action);
        }
        else if (relations > 0.5f && tradeValue > 0.3f)
        {
            DiplomaticAction action;
            action.type = DiplomaticAction::Type::TRADE_AGREEMENT;
            action.sourceKingdom = kingdom;
            action.targetKingdom = otherKingdom->getName();
            action.priority = relations * tradeValue;
            action.conditions["good_relations"] = relations;
            action.conditions["trade_value"] = tradeValue;
            actions.push_back(action);
        }
    }

    // Trier les actions par priorité
    std::sort(actions.begin(), actions.end(),
              [](const DiplomaticAction &a, const DiplomaticAction &b)
              {
                  return a.priority > b.priority;
              });

    return actions;
}

bool DiplomaticAI::executeDiplomaticAction(const DiplomaticAction &action)
{
    switch (action.type)
    {
    case DiplomaticAction::Type::DECLARE_WAR:
        // TODO: Implémenter la déclaration de guerre
        return true;

    case DiplomaticAction::Type::MAKE_PEACE:
        // TODO: Implémenter la conclusion de paix
        return true;

    case DiplomaticAction::Type::FORM_ALLIANCE:
        // TODO: Implémenter la formation d'alliance
        return true;

    case DiplomaticAction::Type::BREAK_ALLIANCE:
        // TODO: Implémenter la rupture d'alliance
        return true;

    case DiplomaticAction::Type::TRADE_AGREEMENT:
        // TODO: Implémenter l'accord commercial
        return true;

    case DiplomaticAction::Type::EMBARGO:
        // TODO: Implémenter l'embargo
        return true;

    case DiplomaticAction::Type::GIFT:
        // TODO: Implémenter le don
        return true;

    case DiplomaticAction::Type::DEMAND_TRIBUTE:
        // TODO: Implémenter la demande de tribut
        return true;

    default:
        return false;
    }
}

float DiplomaticAI::calculateRelations(const std::string &kingdom1, const std::string &kingdom2)
{
    std::string key = kingdom1 + "_" + kingdom2;
    if (m_relationsCache.find(key) != m_relationsCache.end())
    {
        return m_relationsCache[key];
    }

    float relations = 0.0f;

    // Facteurs influençant les relations
    float tradeValue = calculateTradeValue(kingdom1, kingdom2);
    float militaryBalance = calculateMilitaryBalance(kingdom1, kingdom2);
    float culturalInfluence = calculateCulturalInfluence(kingdom1, kingdom2);
    float distance = calculateDistance(
        m_world->getKingdom(kingdom1)->getCapitalPosition(),
        m_world->getKingdom(kingdom2)->getCapitalPosition());

    // Calcul des relations
    relations += tradeValue * 0.4f;
    relations += (militaryBalance - 1.0f) * 0.3f;
    relations += culturalInfluence * 0.3f;
    relations *= (1.0f - distance / 1000.0f); // Influence de la distance

    m_relationsCache[key] = relations;
    return relations;
}

void DiplomaticAI::updateRelations(const std::string &kingdom1, const std::string &kingdom2, float delta)
{
    float currentRelations = calculateRelations(kingdom1, kingdom2);
    float tradeValue = calculateTradeValue(kingdom1, kingdom2);
    float militaryBalance = calculateMilitaryBalance(kingdom1, kingdom2);

    // Mise à jour des relations basée sur les interactions
    float deltaRelations = 0.0f;
    deltaRelations += tradeValue * delta * 0.1f;
    deltaRelations += (militaryBalance - 1.0f) * delta * 0.05f;

    // TODO: Implémenter la mise à jour des relations dans le monde
}

float DiplomaticAI::calculateTradeValue(const std::string &kingdom1, const std::string &kingdom2)
{
    std::string key = kingdom1 + "_" + kingdom2;
    if (m_tradeValueCache.find(key) != m_tradeValueCache.end())
    {
        return m_tradeValueCache[key];
    }

    float tradeValue = 0.0f;
    float economicStrength1 = calculateEconomicStrength(kingdom1);
    float economicStrength2 = calculateEconomicStrength(kingdom2);

    // Calcul de la valeur commerciale
    tradeValue = std::min(economicStrength1, economicStrength2) /
                 std::max(economicStrength1, economicStrength2);

    m_tradeValueCache[key] = tradeValue;
    return tradeValue;
}

float DiplomaticAI::calculateMilitaryBalance(const std::string &kingdom1, const std::string &kingdom2)
{
    std::string key = kingdom1 + "_" + kingdom2;
    if (m_militaryBalanceCache.find(key) != m_militaryBalanceCache.end())
    {
        return m_militaryBalanceCache[key];
    }

    float strength1 = calculateMilitaryStrength(kingdom1);
    float strength2 = calculateMilitaryStrength(kingdom2);

    float balance = strength1 / strength2;
    m_militaryBalanceCache[key] = balance;
    return balance;
}

float DiplomaticAI::calculateCulturalInfluence(const std::string &kingdom1, const std::string &kingdom2)
{
    std::string key = kingdom1 + "_" + kingdom2;
    if (m_culturalInfluenceCache.find(key) != m_culturalInfluenceCache.end())
    {
        return m_culturalInfluenceCache[key];
    }

    float influence1 = calculateCulturalStrength(kingdom1);
    float influence2 = calculateCulturalStrength(kingdom2);

    float influence = influence1 / (influence1 + influence2);
    m_culturalInfluenceCache[key] = influence;
    return influence;
}

void DiplomaticAI::updateCityRelations(const std::string &city, const std::string &kingdom)
{
    float loyalty = calculateCityLoyalty(city, kingdom);

    // TODO: Implémenter la mise à jour des relations ville-royaume
}

float DiplomaticAI::calculateCityLoyalty(const std::string &city, const std::string &kingdom)
{
    // TODO: Implémenter le calcul de la loyauté des villes
    return 0.8f; // Valeur temporaire
}

bool DiplomaticAI::shouldCityRevolt(const std::string &city, const std::string &kingdom)
{
    float loyalty = calculateCityLoyalty(city, kingdom);
    float economicStrength = calculateEconomicStrength(kingdom);
    float militaryStrength = calculateMilitaryStrength(kingdom);

    return loyalty < 0.3f && economicStrength < 0.5f && militaryStrength < 0.7f;
}

std::vector<std::string> DiplomaticAI::getPotentialAllies(const std::string &kingdom)
{
    std::vector<std::string> allies;

    for (const auto &otherKingdom : m_world->getKingdoms())
    {
        if (otherKingdom->getName() != kingdom)
        {
            float relations = calculateRelations(kingdom, otherKingdom->getName());
            float militaryBalance = calculateMilitaryBalance(kingdom, otherKingdom->getName());

            if (relations > 0.6f && militaryBalance > 0.8f && militaryBalance < 1.2f)
            {
                allies.push_back(otherKingdom->getName());
            }
        }
    }

    return allies;
}

float DiplomaticAI::calculateDistance(const sf::Vector2f &pos1, const sf::Vector2f &pos2)
{
    float dx = pos2.x - pos1.x;
    float dy = pos2.y - pos1.y;
    return std::sqrt(dx * dx + dy * dy);
}

float DiplomaticAI::calculateEconomicStrength(const std::string &kingdom)
{
    // TODO: Implémenter le calcul de la force économique
    return 0.8f; // Valeur temporaire
}

float DiplomaticAI::calculateMilitaryStrength(const std::string &kingdom)
{
    // TODO: Implémenter le calcul de la force militaire
    return 0.8f; // Valeur temporaire
}

float DiplomaticAI::calculateCulturalStrength(const std::string &kingdom)
{
    // TODO: Implémenter le calcul de la force culturelle
    return 0.8f; // Valeur temporaire
}

bool DiplomaticAI::areKingdomsAtWar(const std::string &kingdom1, const std::string &kingdom2)
{
    // TODO: Implémenter la vérification de l'état de guerre
    return false; // Valeur temporaire
}

bool DiplomaticAI::areKingdomsAllied(const std::string &kingdom1, const std::string &kingdom2)
{
    // TODO: Implémenter la vérification des alliances
    return false; // Valeur temporaire
}
