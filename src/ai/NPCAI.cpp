#include "NPCAI.h"
#include <algorithm>
#include <cmath>

NPCAI::NPCAI(std::shared_ptr<World> world)
    : m_world(world)
{
}

void NPCAI::update(float deltaTime)
{
    // Mise à jour de tous les NPCs
    for (const auto &npc : m_world->getCharacters())
    {
        if (!npc->isPlayer())
        {
            updateNPC(npc->getId());
        }
    }
}

void NPCAI::updateNPC(const std::string &npcId)
{
    updateNPCState(npcId);
    updateRelationships(npcId);
    updateSkills(npcId);

    // Évaluation et exécution des actions
    auto actions = evaluateActions(npcId);
    for (const auto &action : actions)
    {
        if (shouldExecuteAction(npcId, action))
        {
            executeAction(npcId, action);
        }
    }

    // Mise à jour des comportements spécifiques
    updateTravelBehavior(npcId);
    updateTradeBehavior(npcId);
    updateCombatBehavior(npcId);
    updateExplorationBehavior(npcId);
    updateDiplomaticBehavior(npcId);
    updatePersonalBehavior(npcId);
    updateFleetBehavior(npcId);
}

std::vector<NPCAI::NPCAction> NPCAI::evaluateActions(const std::string &npcId)
{
    std::vector<NPCAction> actions;
    auto npc = m_world->getCharacter(npcId);
    if (!npc)
    {
        return actions;
    }

    // Évaluation des actions de voyage
    for (const auto &city : m_world->getCities())
    {
        NPCAction action;
        action.type = NPCAction::Type::TRAVEL;
        action.target = city->getName();
        action.priority = calculateActionPriority(npcId, action);
        action.parameters["cost"] = calculateTravelCost(npcId, city->getName());
        actions.push_back(action);
    }

    // Évaluation des actions commerciales
    for (const auto &city : m_world->getCities())
    {
        NPCAction action;
        action.type = NPCAction::Type::TRADE;
        action.target = city->getName();
        action.priority = calculateActionPriority(npcId, action);
        action.parameters["value"] = calculateTradeValue(npcId, city->getName());
        actions.push_back(action);
    }

    // Évaluation des actions de combat
    if (npc->hasFleet())
    {
        NPCAction action;
        action.type = NPCAction::Type::COMBAT;
        action.priority = calculateActionPriority(npcId, action);
        action.parameters["strength"] = calculateCombatStrength(npcId);
        actions.push_back(action);
    }

    // Évaluation des actions d'exploration
    NPCAction exploreAction;
    exploreAction.type = NPCAction::Type::EXPLORE;
    exploreAction.priority = calculateActionPriority(npcId, exploreAction);
    actions.push_back(exploreAction);

    // Évaluation des actions diplomatiques
    for (const auto &otherNpc : m_world->getCharacters())
    {
        if (otherNpc->getId() != npcId)
        {
            NPCAction action;
            action.type = NPCAction::Type::DIPLOMATIC;
            action.target = otherNpc->getId();
            action.priority = calculateActionPriority(npcId, action);
            action.parameters["value"] = calculateDiplomaticValue(npcId, otherNpc->getId());
            actions.push_back(action);
        }
    }

    // Trier les actions par priorité
    std::sort(actions.begin(), actions.end(),
              [](const NPCAction &a, const NPCAction &b)
              {
                  return a.priority > b.priority;
              });

    return actions;
}

bool NPCAI::executeAction(const std::string &npcId, const NPCAction &action)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc)
    {
        return false;
    }

    switch (action.type)
    {
    case NPCAction::Type::TRAVEL:
        // TODO: Implémenter le voyage
        return true;

    case NPCAction::Type::TRADE:
        // TODO: Implémenter le commerce
        return true;

    case NPCAction::Type::COMBAT:
        // TODO: Implémenter le combat
        return true;

    case NPCAction::Type::EXPLORE:
        // TODO: Implémenter l'exploration
        return true;

    case NPCAction::Type::DIPLOMATIC:
        // TODO: Implémenter les actions diplomatiques
        return true;

    case NPCAction::Type::PERSONAL:
        // TODO: Implémenter les actions personnelles
        return true;

    case NPCAction::Type::FLEET_MANAGE:
        // TODO: Implémenter la gestion de flotte
        return true;

    default:
        return false;
    }
}

void NPCAI::updateNPCState(const std::string &npcId)
{
    auto &state = m_npcStates[npcId];
    auto npc = m_world->getCharacter(npcId);
    if (!npc)
    {
        return;
    }

    // Mise à jour de la santé
    state.health = npc->getHealth();

    // Mise à jour de la richesse
    state.wealth = npc->getWealth();

    // Mise à jour de la réputation
    state.reputation = npc->getReputation();

    // Mise à jour de l'influence
    state.influence = npc->getInfluence();

    // Mise à jour des compétences
    updateSkills(npcId);
}

void NPCAI::updateRelationships(const std::string &npcId)
{
    auto &state = m_npcStates[npcId];
    state.relationships.clear();

    // TODO: Implémenter la mise à jour des relations
}

void NPCAI::updateSkills(const std::string &npcId)
{
    auto &state = m_npcStates[npcId];
    auto npc = m_world->getCharacter(npcId);
    if (!npc)
    {
        return;
    }

    // TODO: Implémenter la mise à jour des compétences
}

NPCAI::NPCAction NPCAI::selectBestAction(const std::string &npcId)
{
    auto actions = evaluateActions(npcId);
    if (actions.empty())
    {
        return NPCAction();
    }

    return actions.front();
}

float NPCAI::calculateActionPriority(const std::string &npcId, const NPCAction &action)
{
    std::string key = npcId + std::to_string(static_cast<int>(action.type)) + action.target;
    if (m_actionPriorityCache.find(key) != m_actionPriorityCache.end())
    {
        return m_actionPriorityCache[key];
    }

    float priority = 0.5f;
    auto &state = m_npcStates[npcId];

    switch (action.type)
    {
    case NPCAction::Type::TRAVEL:
        priority = calculateTravelPriority(npcId, action);
        break;

    case NPCAction::Type::TRADE:
        priority = calculateTradePriority(npcId, action);
        break;

    case NPCAction::Type::COMBAT:
        priority = calculateCombatPriority(npcId, action);
        break;

    case NPCAction::Type::EXPLORE:
        priority = calculateExplorationPriority(npcId, action);
        break;

    case NPCAction::Type::DIPLOMATIC:
        priority = calculateDiplomaticPriority(npcId, action);
        break;

    case NPCAction::Type::PERSONAL:
        priority = calculatePersonalPriority(npcId, action);
        break;

    case NPCAction::Type::FLEET_MANAGE:
        priority = calculateFleetPriority(npcId, action);
        break;
    }

    // Ajuster la priorité en fonction de l'état du NPC
    if (state.health < 0.3f && action.type != NPCAction::Type::PERSONAL)
    {
        priority *= 0.5f;
    }

    if (state.wealth < 1000.0f && action.type != NPCAction::Type::TRADE)
    {
        priority *= 0.7f;
    }

    m_actionPriorityCache[key] = priority;
    return priority;
}

bool NPCAI::shouldExecuteAction(const std::string &npcId, const NPCAction &action)
{
    auto &state = m_npcStates[npcId];

    // Vérifier les conditions de base
    if (action.priority < 0.5f)
    {
        return false;
    }

    // Vérifier les conditions spécifiques selon le type d'action
    switch (action.type)
    {
    case NPCAction::Type::TRAVEL:
        return state.health > 0.5f && state.wealth > 500.0f;

    case NPCAction::Type::TRADE:
        return state.wealth > 100.0f;

    case NPCAction::Type::COMBAT:
        return state.health > 0.7f && state.wealth > 1000.0f;

    case NPCAction::Type::EXPLORE:
        return state.health > 0.6f && state.wealth > 800.0f;

    case NPCAction::Type::DIPLOMATIC:
        return state.reputation > 0.3f;

    case NPCAction::Type::PERSONAL:
        return true;

    case NPCAction::Type::FLEET_MANAGE:
        return state.wealth > 500.0f;

    default:
        return false;
    }
}

void NPCAI::updateTravelBehavior(const std::string &npcId)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc || !npc->hasFleet())
    {
        return;
    }

    // Vérifier si le NPC est déjà en voyage
    if (npc->isTraveling())
    {
        return;
    }

    // Trouver la meilleure destination
    std::string bestDestination;
    float bestValue = -1.0f;

    for (const auto &city : m_world->getCities())
    {
        float value = calculateTravelValue(npcId, city->getName());
        if (value > bestValue)
        {
            bestValue = value;
            bestDestination = city->getName();
        }
    }

    // Décider si le voyage est nécessaire
    if (bestValue > 0.7f)
    {
        NPCAction action;
        action.type = NPCAction::Type::TRAVEL;
        action.target = bestDestination;
        action.priority = bestValue;
        action.parameters["cost"] = calculateTravelCost(npcId, bestDestination);
        executeAction(npcId, action);
    }
}

void NPCAI::updateTradeBehavior(const std::string &npcId)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc)
    {
        return;
    }

    // Vérifier si le NPC est dans une ville
    auto currentCity = npc->getCurrentCity();
    if (!currentCity)
    {
        return;
    }

    // Analyser les opportunités commerciales
    for (const auto &city : m_world->getCities())
    {
        if (city->getName() == currentCity->getName())
        {
            continue;
        }

        float tradeValue = calculateTradeValue(npcId, city->getName());
        if (tradeValue > 0.6f)
        {
            NPCAction action;
            action.type = NPCAction::Type::TRADE;
            action.target = city->getName();
            action.priority = tradeValue;
            action.parameters["value"] = tradeValue;
            executeAction(npcId, action);
        }
    }
}

void NPCAI::updateCombatBehavior(const std::string &npcId)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc || !npc->hasFleet())
    {
        return;
    }

    // Vérifier les menaces potentielles
    for (const auto &otherNpc : m_world->getCharacters())
    {
        if (otherNpc->getId() == npcId || !otherNpc->hasFleet())
        {
            continue;
        }

        float threatLevel = calculateThreatLevel(npcId, otherNpc->getId());
        if (threatLevel > 0.7f)
        {
            NPCAction action;
            action.type = NPCAction::Type::COMBAT;
            action.target = otherNpc->getId();
            action.priority = threatLevel;
            action.parameters["strength"] = calculateCombatStrength(npcId);
            executeAction(npcId, action);
        }
    }
}

void NPCAI::updateExplorationBehavior(const std::string &npcId)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc || !npc->hasFleet())
    {
        return;
    }

    // Trouver les zones inexplorées les plus prometteuses
    std::vector<std::string> unexploredAreas = findUnexploredAreas(npcId);
    for (const auto &area : unexploredAreas)
    {
        float explorationValue = calculateExplorationValue(npcId, area);
        if (explorationValue > 0.6f)
        {
            NPCAction action;
            action.type = NPCAction::Type::EXPLORE;
            action.target = area;
            action.priority = explorationValue;
            action.parameters["value"] = explorationValue;
            executeAction(npcId, action);
        }
    }
}

void NPCAI::updateDiplomaticBehavior(const std::string &npcId)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc)
    {
        return;
    }

    // Analyser les relations diplomatiques
    for (const auto &otherNpc : m_world->getCharacters())
    {
        if (otherNpc->getId() == npcId)
        {
            continue;
        }

        float diplomaticValue = calculateDiplomaticValue(npcId, otherNpc->getId());
        if (diplomaticValue > 0.6f)
        {
            NPCAction action;
            action.type = NPCAction::Type::DIPLOMATIC;
            action.target = otherNpc->getId();
            action.priority = diplomaticValue;
            action.parameters["value"] = diplomaticValue;
            executeAction(npcId, action);
        }
    }
}

void NPCAI::updatePersonalBehavior(const std::string &npcId)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc)
    {
        return;
    }

    // Vérifier les besoins personnels
    auto &state = m_npcStates[npcId];

    // Si la santé est basse
    if (state.health < 0.3f)
    {
        NPCAction action;
        action.type = NPCAction::Type::PERSONAL;
        action.priority = 0.8f;
        action.parameters["health"] = state.health;
        executeAction(npcId, action);
    }

    // Si la richesse est basse
    if (state.wealth < 1000.0f)
    {
        NPCAction action;
        action.type = NPCAction::Type::PERSONAL;
        action.priority = 0.7f;
        action.parameters["wealth"] = state.wealth;
        executeAction(npcId, action);
    }
}

void NPCAI::updateFleetBehavior(const std::string &npcId)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc || !npc->hasFleet())
    {
        return;
    }

    // Vérifier l'état de la flotte
    auto fleet = npc->getFleet();
    if (!fleet)
    {
        return;
    }

    // Si la flotte a besoin de réparations
    if (fleet->getHealth() < 0.5f)
    {
        NPCAction action;
        action.type = NPCAction::Type::FLEET_MANAGE;
        action.priority = 0.8f;
        action.parameters["health"] = fleet->getHealth();
        executeAction(npcId, action);
    }

    // Si la flotte a besoin de ravitaillement
    if (fleet->getSupplies() < 0.3f)
    {
        NPCAction action;
        action.type = NPCAction::Type::FLEET_MANAGE;
        action.priority = 0.7f;
        action.parameters["supplies"] = fleet->getSupplies();
        executeAction(npcId, action);
    }
}

float NPCAI::calculateDistance(const sf::Vector2f &pos1, const sf::Vector2f &pos2)
{
    float dx = pos2.x - pos1.x;
    float dy = pos2.y - pos1.y;
    return std::sqrt(dx * dx + dy * dy);
}

float NPCAI::calculateTravelCost(const std::string &npcId, const std::string &destination)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc || !npc->hasFleet())
    {
        return 0.0f;
    }

    float cost = 0.0f;
    auto fleet = npc->getFleet();
    auto destinationCity = m_world->getCity(destination);
    if (!destinationCity)
    {
        return 0.0f;
    }

    // Coût de base basé sur la distance
    float distance = calculateDistance(npc->getPosition(), destinationCity->getPosition());
    cost += distance * 0.1f; // 0.1 unités par unité de distance

    // Coût de maintenance de la flotte
    cost += fleet->getShipCount() * 10.0f; // 10 unités par navire

    // Coût des provisions
    float suppliesNeeded = distance / 100.0f; // Provisions nécessaires basées sur la distance
    cost += suppliesNeeded * 5.0f;            // 5 unités par unité de provisions

    // Ajustement selon l'état de la flotte
    if (fleet->getHealth() < 0.5f)
    {
        cost *= 1.5f; // 50% plus cher si la flotte est endommagée
    }

    return cost;
}

float NPCAI::calculateTradeValue(const std::string &npcId, const std::string &city)
{
    auto npc = m_world->getCharacter(npcId);
    auto targetCity = m_world->getCity(city);
    if (!npc || !targetCity)
    {
        return 0.0f;
    }

    float value = 0.5f;

    // Facteurs de valeur commerciale
    float distanceFactor = 1.0f - (calculateDistance(npc->getPosition(), targetCity->getPosition()) / 1000.0f);
    float wealthFactor = m_npcStates[npcId].wealth / 10000.0f;
    float reputationFactor = m_npcStates[npcId].reputation;

    // Vérifier les biens disponibles
    float goodsValue = 0.0f;
    if (npc->hasFleet())
    {
        auto fleet = npc->getFleet();
        for (const auto &cargo : fleet->getCargo())
        {
            float marketPrice = targetCity->getMarketPrice(cargo.goodId);
            float basePrice = m_world->getGoodBasePrice(cargo.goodId);
            goodsValue += (marketPrice - basePrice) * cargo.quantity;
        }
    }

    // Calculer la valeur finale
    value = (distanceFactor * 0.2f + wealthFactor * 0.2f + reputationFactor * 0.2f + goodsValue * 0.4f);
    return std::max(0.0f, std::min(1.0f, value));
}

float NPCAI::calculateCombatStrength(const std::string &npcId)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc || !npc->hasFleet())
    {
        return 0.0f;
    }

    float strength = 0.5f;
    auto fleet = npc->getFleet();

    // Facteurs de force militaire
    float shipCountFactor = fleet->getShipCount() / 10.0f; // Normalisé par rapport à 10 navires
    float healthFactor = fleet->getHealth();
    float suppliesFactor = fleet->getSupplies();
    float crewFactor = fleet->getCrewCount() / fleet->getMaxCrewCount();

    // Calculer la force finale
    strength = (shipCountFactor * 0.3f + healthFactor * 0.3f + suppliesFactor * 0.2f + crewFactor * 0.2f);
    return std::max(0.0f, std::min(1.0f, strength));
}

float NPCAI::calculateExplorationValue(const std::string &npcId, const std::string &area)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc || !npc->hasFleet())
    {
        return 0.0f;
    }

    float value = 0.5f;

    // Facteurs de valeur d'exploration
    float distanceFactor = 1.0f - (calculateDistance(npc->getPosition(), m_world->getAreaPosition(area)) / 1000.0f);
    float wealthFactor = m_npcStates[npcId].wealth / 10000.0f;
    float healthFactor = m_npcStates[npcId].health;

    // Vérifier si la zone est déjà explorée
    float explorationProgress = m_world->getAreaExplorationProgress(area);
    float unexploredFactor = 1.0f - explorationProgress;

    // Calculer la valeur finale
    value = (distanceFactor * 0.3f + wealthFactor * 0.2f + healthFactor * 0.2f + unexploredFactor * 0.3f);
    return std::max(0.0f, std::min(1.0f, value));
}

float NPCAI::calculateDiplomaticValue(const std::string &npcId, const std::string &target)
{
    auto npc = m_world->getCharacter(npcId);
    auto targetNpc = m_world->getCharacter(target);
    if (!npc || !targetNpc)
    {
        return 0.0f;
    }

    float value = 0.5f;

    // Facteurs de valeur diplomatique
    float reputationFactor = m_npcStates[npcId].reputation;
    float influenceFactor = m_npcStates[npcId].influence;
    float relationshipFactor = calculateRelationshipFactor(npcId, target);
    float wealthFactor = m_npcStates[npcId].wealth / 10000.0f;

    // Calculer la valeur finale
    value = (reputationFactor * 0.3f + influenceFactor * 0.3f + relationshipFactor * 0.2f + wealthFactor * 0.2f);
    return std::max(0.0f, std::min(1.0f, value));
}

void NPCAI::updateNPCStats(const std::string &npcId)
{
    // TODO: Implémenter la mise à jour des statistiques
}

float NPCAI::calculateTravelPriority(const std::string &npcId, const NPCAction &action)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc)
    {
        return 0.0f;
    }

    float priority = 0.5f;
    auto &state = m_npcStates[npcId];

    // Facteurs de priorité
    float distanceFactor = 1.0f - (calculateDistance(npc->getPosition(), m_world->getCity(action.target)->getPosition()) / 1000.0f);
    float wealthFactor = state.wealth / 10000.0f;
    float healthFactor = state.health;

    // Calculer la priorité finale
    priority = (distanceFactor * 0.4f + wealthFactor * 0.3f + healthFactor * 0.3f);
    return std::max(0.0f, std::min(1.0f, priority));
}

float NPCAI::calculateTradePriority(const std::string &npcId, const NPCAction &action)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc)
    {
        return 0.0f;
    }

    float priority = 0.5f;
    auto &state = m_npcStates[npcId];

    // Facteurs de priorité
    float wealthFactor = state.wealth / 10000.0f;
    float tradeValue = calculateTradeValue(npcId, action.target);
    float reputationFactor = state.reputation;

    // Calculer la priorité finale
    priority = (wealthFactor * 0.3f + tradeValue * 0.4f + reputationFactor * 0.3f);
    return std::max(0.0f, std::min(1.0f, priority));
}

float NPCAI::calculateCombatPriority(const std::string &npcId, const NPCAction &action)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc || !npc->hasFleet())
    {
        return 0.0f;
    }

    float priority = 0.5f;
    auto &state = m_npcStates[npcId];

    // Facteurs de priorité
    float healthFactor = state.health;
    float strengthFactor = calculateCombatStrength(npcId);
    float threatLevel = calculateThreatLevel(npcId, action.target);

    // Calculer la priorité finale
    priority = (healthFactor * 0.3f + strengthFactor * 0.3f + threatLevel * 0.4f);
    return std::max(0.0f, std::min(1.0f, priority));
}

float NPCAI::calculateExplorationPriority(const std::string &npcId, const NPCAction &action)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc || !npc->hasFleet())
    {
        return 0.0f;
    }

    float priority = 0.5f;
    auto &state = m_npcStates[npcId];

    // Facteurs de priorité
    float healthFactor = state.health;
    float wealthFactor = state.wealth / 10000.0f;
    float explorationValue = calculateExplorationValue(npcId, action.target);

    // Calculer la priorité finale
    priority = (healthFactor * 0.3f + wealthFactor * 0.3f + explorationValue * 0.4f);
    return std::max(0.0f, std::min(1.0f, priority));
}

float NPCAI::calculateDiplomaticPriority(const std::string &npcId, const NPCAction &action)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc)
    {
        return 0.0f;
    }

    float priority = 0.5f;
    auto &state = m_npcStates[npcId];

    // Facteurs de priorité
    float reputationFactor = state.reputation;
    float influenceFactor = state.influence;
    float diplomaticValue = calculateDiplomaticValue(npcId, action.target);

    // Calculer la priorité finale
    priority = (reputationFactor * 0.3f + influenceFactor * 0.3f + diplomaticValue * 0.4f);
    return std::max(0.0f, std::min(1.0f, priority));
}

float NPCAI::calculatePersonalPriority(const std::string &npcId, const NPCAction &action)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc)
    {
        return 0.0f;
    }

    float priority = 0.5f;
    auto &state = m_npcStates[npcId];

    // Facteurs de priorité
    float healthFactor = state.health;
    float wealthFactor = state.wealth / 10000.0f;

    // Calculer la priorité finale
    priority = (healthFactor * 0.5f + wealthFactor * 0.5f);
    return std::max(0.0f, std::min(1.0f, priority));
}

float NPCAI::calculateFleetPriority(const std::string &npcId, const NPCAction &action)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc || !npc->hasFleet())
    {
        return 0.0f;
    }

    float priority = 0.5f;
    auto &state = m_npcStates[npcId];
    auto fleet = npc->getFleet();

    // Facteurs de priorité
    float healthFactor = fleet->getHealth();
    float suppliesFactor = fleet->getSupplies();
    float wealthFactor = state.wealth / 10000.0f;

    // Calculer la priorité finale
    priority = ((1.0f - healthFactor) * 0.4f + (1.0f - suppliesFactor) * 0.4f + wealthFactor * 0.2f);
    return std::max(0.0f, std::min(1.0f, priority));
}

float NPCAI::calculateThreatLevel(const std::string &npcId, const std::string &targetId)
{
    auto npc = m_world->getCharacter(npcId);
    auto target = m_world->getCharacter(targetId);
    if (!npc || !target || !npc->hasFleet() || !target->hasFleet())
    {
        return 0.0f;
    }

    float threat = 0.5f;

    // Facteurs de menace
    float distanceFactor = 1.0f - (calculateDistance(npc->getPosition(), target->getPosition()) / 1000.0f);
    float strengthRatio = calculateCombatStrength(targetId) / calculateCombatStrength(npcId);
    float relationshipFactor = calculateRelationshipFactor(npcId, targetId);

    // Calculer le niveau de menace
    threat = (distanceFactor * 0.3f + strengthRatio * 0.4f + relationshipFactor * 0.3f);
    return std::max(0.0f, std::min(1.0f, threat));
}

float NPCAI::calculateRelationshipFactor(const std::string &npcId, const std::string &targetId)
{
    auto &state = m_npcStates[npcId];

    // Vérifier si le NPC a une relation avec la cible
    auto it = std::find(state.relationships.begin(), state.relationships.end(), targetId);
    if (it == state.relationships.end())
    {
        return 0.5f; // Relation neutre par défaut
    }

    // Calculer la relation basée sur l'historique des interactions
    float relationship = 0.5f;

    // Facteurs positifs
    if (state.tradeHistory.find(targetId) != state.tradeHistory.end())
    {
        relationship += 0.1f * state.tradeHistory[targetId].successfulTrades;
    }
    if (state.combatHistory.find(targetId) != state.combatHistory.end())
    {
        relationship -= 0.2f * state.combatHistory[targetId].conflicts;
    }
    if (state.diplomaticHistory.find(targetId) != state.diplomaticHistory.end())
    {
        relationship += 0.15f * state.diplomaticHistory[targetId].alliances;
    }

    // Ajuster selon la réputation mutuelle
    auto targetState = m_npcStates.find(targetId);
    if (targetState != m_npcStates.end())
    {
        relationship += (targetState->second.reputation - 0.5f) * 0.1f;
    }

    return std::max(0.0f, std::min(1.0f, relationship));
}

std::vector<std::string> NPCAI::findUnexploredAreas(const std::string &npcId)
{
    std::vector<std::string> unexploredAreas;
    auto npc = m_world->getCharacter(npcId);
    if (!npc)
    {
        return unexploredAreas;
    }

    // Obtenir toutes les zones du monde
    auto allAreas = m_world->getAreas();

    // Filtrer les zones inexplorées
    for (const auto &area : allAreas)
    {
        float explorationProgress = m_world->getAreaExplorationProgress(area);
        if (explorationProgress < 0.3f) // Zone considérée comme inexplorée si moins de 30% explorée
        {
            // Vérifier si la zone est accessible
            float distance = calculateDistance(npc->getPosition(), m_world->getAreaPosition(area));
            if (distance < 1000.0f) // Zone accessible si à moins de 1000 unités
            {
                unexploredAreas.push_back(area);
            }
        }
    }

    // Trier les zones par valeur d'exploration
    std::sort(unexploredAreas.begin(), unexploredAreas.end(),
              [this, npcId](const std::string &a, const std::string &b)
              {
                  return calculateExplorationValue(npcId, a) > calculateExplorationValue(npcId, b);
              });

    return unexploredAreas;
}

float NPCAI::calculateTravelValue(const std::string &npcId, const std::string &destination)
{
    auto npc = m_world->getCharacter(npcId);
    if (!npc)
    {
        return 0.0f;
    }

    float value = 0.5f;

    // Facteurs de valeur
    float distanceFactor = 1.0f - (calculateDistance(npc->getPosition(), m_world->getCity(destination)->getPosition()) / 1000.0f);
    float tradeValue = calculateTradeValue(npcId, destination);
    float explorationValue = calculateExplorationValue(npcId, destination);

    // Calculer la valeur finale
    value = (distanceFactor * 0.3f + tradeValue * 0.4f + explorationValue * 0.3f);
    return std::max(0.0f, std::min(1.0f, value));
}