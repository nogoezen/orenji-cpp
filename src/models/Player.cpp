#include "Player.h"
#include "../data/GameData.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cmath>

// Constructeur à partir de JSON
Player::Player(const nlohmann::json &data)
    : m_name("Joueur"), m_gold(1000), m_level(1), m_experience(0),
      m_posX(0), m_posY(0), m_currentMode(PlayerMode::LAND),
      m_cargoCapacity(100), m_currentCargoWeight(0), m_fleet(std::make_shared<Fleet>("Fleet")),
      m_character(std::make_shared<Character>("Joueur"))
{
    if (data.contains("name"))
        m_name = data["name"];

    if (data.contains("gold"))
        m_gold = data["gold"];

    if (data.contains("level"))
        m_level = data["level"];

    if (data.contains("experience"))
        m_experience = data["experience"];

    if (data.contains("posX"))
        m_posX = data["posX"];

    if (data.contains("posY"))
        m_posY = data["posY"];

    if (data.contains("mode"))
        m_currentMode = static_cast<PlayerMode>(data["mode"]);

    if (data.contains("debt"))
        m_debt = data["debt"];

    if (data.contains("debtInterestRate"))
        m_debtInterestRate = data["debtInterestRate"];

    if (data.contains("lastDebtPaymentDay"))
        m_lastDebtPaymentDay = data["lastDebtPaymentDay"];

    if (data.contains("cargo"))
    {
        for (const auto &item : data["cargo"].items())
        {
            const auto &cargo = item.value();
            int id = std::stoi(item.key());
            std::string name = cargo["name"];
            int quantity = cargo["quantity"];
            int unitWeight = cargo.contains("unitWeight") ? cargo["unitWeight"] : 1;

            m_cargo[id] = CargoItem(id, name, quantity, unitWeight);
            m_currentCargoWeight += quantity * unitWeight;
        }
    }

    if (data.contains("cargoCapacity"))
        m_cargoCapacity = data["cargoCapacity"];

    if (data.contains("inventory"))
    {
        for (const auto &item : data["inventory"].items())
        {
            int id = std::stoi(item.key());
            int quantity = item.value();
            m_inventory[id] = quantity;
        }
    }

    if (data.contains("character") && data["character"].is_object())
    {
        m_character = std::make_shared<Character>(data["character"]);
    }

    if (data.contains("fleet") && data["fleet"].is_object())
    {
        m_fleet = std::make_shared<Fleet>(data["fleet"]);
    }

    // Chargement des compétences commerciales
    if (data.contains("tradeSkills") && data["tradeSkills"].is_object())
    {
        m_tradeSkills.fromJson(data["tradeSkills"]);
    }

    // Chargement des compétences d'amirauté
    if (data.contains("admiraltySkills") && data["admiraltySkills"].is_object())
    {
        m_admiraltySkills.fromJson(data["admiraltySkills"]);
    }

    // Chargement des réputations des villes
    if (data.contains("cityReputations") && data["cityReputations"].is_object())
    {
        for (const auto &item : data["cityReputations"].items())
        {
            int cityId = std::stoi(item.key());
            float reputation = item.value();
            m_cityReputations[cityId] = reputation;
        }
    }

    // Chargement des réputations des royaumes
    if (data.contains("kingdomReputations") && data["kingdomReputations"].is_object())
    {
        for (const auto &item : data["kingdomReputations"].items())
        {
            std::string kingdom = item.key();
            float reputation = item.value();
            m_kingdomReputations[kingdom] = reputation;
        }
    }

    // Chargement des routes commerciales
    if (data.contains("tradeRoutes") && data["tradeRoutes"].is_array())
    {
        for (const auto &routeData : data["tradeRoutes"])
        {
            TradeRoute route = TradeRoute::fromJson(routeData);
            m_tradeRoutes.push_back(route);
        }
    }
}

// Conversion vers JSON
nlohmann::json Player::toJson() const
{
    nlohmann::json data;
    data["name"] = m_name;
    data["gold"] = m_gold;
    data["level"] = m_level;
    data["experience"] = m_experience;
    data["posX"] = m_posX;
    data["posY"] = m_posY;
    data["mode"] = static_cast<int>(m_currentMode);
    data["debt"] = m_debt;
    data["debtInterestRate"] = m_debtInterestRate;
    data["lastDebtPaymentDay"] = m_lastDebtPaymentDay;

    // Cargo
    nlohmann::json cargo;
    for (const auto &item : m_cargo)
    {
        nlohmann::json cargoItem;
        cargoItem["name"] = item.second.name;
        cargoItem["quantity"] = item.second.quantity;
        cargoItem["unitWeight"] = item.second.unitWeight;
        cargo[std::to_string(item.first)] = cargoItem;
    }
    data["cargo"] = cargo;
    data["cargoCapacity"] = m_cargoCapacity;

    // Inventaire
    nlohmann::json inventory;
    for (const auto &item : m_inventory)
    {
        inventory[std::to_string(item.first)] = item.second;
    }
    data["inventory"] = inventory;

    // Personnage
    if (m_character)
    {
        data["character"] = m_character->toJson();
    }

    // Flotte
    if (m_fleet)
    {
        data["fleet"] = m_fleet->toJson();
    }

    // Compétences commerciales
    data["tradeSkills"] = m_tradeSkills.toJson();

    // Compétences d'amirauté
    data["admiraltySkills"] = m_admiraltySkills.toJson();

    // Réputations des villes
    nlohmann::json cityReputations;
    for (const auto &rep : m_cityReputations)
    {
        cityReputations[std::to_string(rep.first)] = rep.second;
    }
    data["cityReputations"] = cityReputations;

    // Réputations des royaumes
    nlohmann::json kingdomReputations;
    for (const auto &rep : m_kingdomReputations)
    {
        kingdomReputations[rep.first] = rep.second;
    }
    data["kingdomReputations"] = kingdomReputations;

    // Routes commerciales
    nlohmann::json tradeRoutes = nlohmann::json::array();
    for (const auto &route : m_tradeRoutes)
    {
        tradeRoutes.push_back(route.toJson());
    }
    data["tradeRoutes"] = tradeRoutes;

    return data;
}

// Méthode pour ajouter de l'expérience et monter de niveau
void Player::addExperience(int experience)
{
    m_experience += experience;

    // Vérifier si le joueur monte de niveau
    int experienceNeeded = m_level * 1000; // Formule simple: niveau * 1000
    if (m_experience >= experienceNeeded)
    {
        // Monter de niveau
        m_level++;
        m_experience -= experienceNeeded;

        // Avantages liés au niveau
        m_cargoCapacity += 10; // +10 de capacité de cargo par niveau
    }
}

// Méthodes d'inventaire général
void Player::addToInventory(int itemId, int quantity)
{
    m_inventory[itemId] += quantity;
}

void Player::removeFromInventory(int itemId, int quantity)
{
    if (m_inventory.find(itemId) != m_inventory.end())
    {
        m_inventory[itemId] = std::max(0, m_inventory[itemId] - quantity);
        if (m_inventory[itemId] == 0)
        {
            m_inventory.erase(itemId);
        }
    }
}

int Player::getItemQuantity(int itemId) const
{
    auto it = m_inventory.find(itemId);
    return it != m_inventory.end() ? it->second : 0;
}

// Méthodes de cargaison (mode maritime)
bool Player::addToCargo(int goodId, const std::string &name, int quantity, int unitWeight)
{
    // Vérifier si le poids total ne dépasse pas la capacité
    int totalWeight = m_currentCargoWeight + (quantity * unitWeight);
    if (totalWeight > m_cargoCapacity)
    {
        return false;
    }

    // Ajouter ou mettre à jour la cargaison
    auto it = m_cargo.find(goodId);
    if (it != m_cargo.end())
    {
        it->second.quantity += quantity;
    }
    else
    {
        m_cargo[goodId] = CargoItem(goodId, name, quantity, unitWeight);
    }

    // Mettre à jour le poids total
    m_currentCargoWeight = totalWeight;

    return true;
}

bool Player::removeFromCargo(int goodId, int quantity)
{
    auto it = m_cargo.find(goodId);
    if (it == m_cargo.end() || it->second.quantity < quantity)
    {
        return false;
    }

    // Mettre à jour la quantité
    it->second.quantity -= quantity;

    // Mettre à jour le poids total
    m_currentCargoWeight -= quantity * it->second.unitWeight;

    // Supprimer l'entrée si la quantité est nulle
    if (it->second.quantity == 0)
    {
        m_cargo.erase(it);
    }

    return true;
}

int Player::getCargoQuantity(int goodId) const
{
    auto it = m_cargo.find(goodId);
    return it != m_cargo.end() ? it->second.quantity : 0;
}

// Méthodes de cargaison (mode terrestre)
bool Player::canAddCargo(int goodId, int quantity) const
{
    auto it = m_cargo.find(goodId);
    int unitWeight = (it != m_cargo.end()) ? it->second.unitWeight : 1;
    int totalWeight = m_currentCargoWeight + (quantity * unitWeight);
    return totalWeight <= m_cargoCapacity;
}

bool Player::addCargo(int goodId, const std::string &name, int quantity, int unitWeight)
{
    return addToCargo(goodId, name, quantity, unitWeight);
}

bool Player::addCargo(int goodId, int quantity)
{
    auto it = m_cargo.find(goodId);
    if (it == m_cargo.end())
    {
        return false;
    }
    return addCargo(goodId, it->second.name, quantity, it->second.unitWeight);
}

bool Player::hasCargo(int goodId, int quantity) const
{
    auto it = m_cargo.find(goodId);
    return it != m_cargo.end() && it->second.quantity >= quantity;
}

bool Player::removeCargo(int goodId, int quantity)
{
    return removeFromCargo(goodId, quantity);
}

int Player::getTotalCargoWeight() const
{
    return m_currentCargoWeight;
}

// Méthodes pour les compétences commerciales
void Player::improveTradeSkill(const std::string &skillName, int amount)
{
    if (skillName == "negotiation")
    {
        m_tradeSkills.negotiation = std::min(100, m_tradeSkills.negotiation + amount);
    }
    else if (skillName == "logistics")
    {
        m_tradeSkills.logistics = std::min(100, m_tradeSkills.logistics + amount);
    }
    else if (skillName == "smuggling")
    {
        m_tradeSkills.smuggling = std::min(100, m_tradeSkills.smuggling + amount);
    }
    else if (skillName == "influence")
    {
        m_tradeSkills.influence = std::min(100, m_tradeSkills.influence + amount);
    }
}

// Méthodes pour les compétences d'amirauté
void Player::improveAdmiraltySkill(const std::string &skillName, int amount)
{
    if (skillName == "navigation")
    {
        m_admiraltySkills.navigation = std::min(100, m_admiraltySkills.navigation + amount);
    }
    else if (skillName == "tactics")
    {
        m_admiraltySkills.tactics = std::min(100, m_admiraltySkills.tactics + amount);
    }
    else if (skillName == "leadership")
    {
        m_admiraltySkills.leadership = std::min(100, m_admiraltySkills.leadership + amount);
    }
    else if (skillName == "resourcefulness")
    {
        m_admiraltySkills.resourcefulness = std::min(100, m_admiraltySkills.resourcefulness + amount);
    }
    else if (skillName == "crewManagement")
    {
        m_admiraltySkills.crewManagement = std::min(100, m_admiraltySkills.crewManagement + amount);
    }
    else if (skillName == "shipMaintenance")
    {
        m_admiraltySkills.shipMaintenance = std::min(100, m_admiraltySkills.shipMaintenance + amount);
    }
}

// Méthodes pour les réputations des villes
float Player::getCityReputation(int cityId) const
{
    auto it = m_cityReputations.find(cityId);
    return it != m_cityReputations.end() ? it->second : 0.0f;
}

void Player::setCityReputation(int cityId, float reputation)
{
    m_cityReputations[cityId] = std::clamp(reputation, -100.0f, 100.0f);
}

void Player::updateCityReputation(int cityId, float amount)
{
    float currentRep = getCityReputation(cityId);
    setCityReputation(cityId, currentRep + amount);
}

// Méthodes pour les réputations des royaumes
float Player::getKingdomReputation(const std::string &kingdom) const
{
    auto it = m_kingdomReputations.find(kingdom);
    return it != m_kingdomReputations.end() ? it->second : 0.0f;
}

void Player::setKingdomReputation(const std::string &kingdom, float reputation)
{
    m_kingdomReputations[kingdom] = std::clamp(reputation, -100.0f, 100.0f);
}

void Player::updateKingdomReputation(const std::string &kingdom, float amount)
{
    float currentRep = getKingdomReputation(kingdom);
    setKingdomReputation(kingdom, currentRep + amount);
}

// Gestion des routes commerciales
int Player::addTradeRoute(const TradeRoute &route)
{
    m_tradeRoutes.push_back(route);
    return m_tradeRoutes.size() - 1; // Retourner l'index de la nouvelle route
}

bool Player::removeTradeRoute(size_t index)
{
    if (index >= m_tradeRoutes.size())
    {
        return false;
    }

    m_tradeRoutes.erase(m_tradeRoutes.begin() + index);
    return true;
}

// Implémentation de TradeRoute::toJson
nlohmann::json TradeRoute::toJson() const
{
    nlohmann::json j;
    j["sourceCity"] = sourceCity;
    j["destinationCity"] = destinationCity;

    nlohmann::json goods = nlohmann::json::array();
    for (const auto &pair : tradedGoods)
    {
        nlohmann::json good;
        good["id"] = pair.first;
        good["quantity"] = pair.second;
        goods.push_back(good);
    }
    j["tradedGoods"] = goods;

    j["tripDuration"] = tripDuration;
    j["securityLevel"] = securityLevel;
    j["profitMargin"] = profitMargin;
    j["currentTime"] = currentTime;

    return j;
}

// Implémentation de TradeRoute::fromJson
TradeRoute TradeRoute::fromJson(const nlohmann::json &data)
{
    TradeRoute route;

    if (data.contains("sourceCity"))
        route.sourceCity = data["sourceCity"];

    if (data.contains("destinationCity"))
        route.destinationCity = data["destinationCity"];

    if (data.contains("tradedGoods") && data["tradedGoods"].is_array())
    {
        for (const auto &goodData : data["tradedGoods"])
        {
            if (goodData.contains("id") && goodData.contains("quantity"))
            {
                int id = goodData["id"];
                int quantity = goodData["quantity"];
                route.tradedGoods.push_back(std::make_pair(id, quantity));
            }
        }
    }

    if (data.contains("tripDuration"))
        route.tripDuration = data["tripDuration"];

    if (data.contains("securityLevel"))
        route.securityLevel = data["securityLevel"];

    if (data.contains("profitMargin"))
        route.profitMargin = data["profitMargin"];

    if (data.contains("currentTime"))
        route.currentTime = data["currentTime"];

    return route;
}