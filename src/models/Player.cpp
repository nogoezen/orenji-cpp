#include "Player.h"
#include "../data/GameData.h"
#include <algorithm>
#include <iostream>
#include <sstream>

// Constructeur à partir de JSON
Player::Player(const nlohmann::json &data)
    : m_name("Joueur"), m_gold(1000), m_level(1), m_experience(0),
      m_cargoCapacity(100), m_currentCargoWeight(0)
{
    // Charger les données de base
    if (data.contains("name") && data["name"].is_string())
    {
        m_name = data["name"];
    }

    if (data.contains("gold") && data["gold"].is_number())
    {
        m_gold = data["gold"];
    }

    if (data.contains("level") && data["level"].is_number())
    {
        m_level = data["level"];
    }

    if (data.contains("experience") && data["experience"].is_number())
    {
        m_experience = data["experience"];
    }

    // Charger la position
    if (data.contains("posX") && data["posX"].is_number())
    {
        m_posX = data["posX"];
    }

    if (data.contains("posY") && data["posY"].is_number())
    {
        m_posY = data["posY"];
    }

    // Charger le mode
    if (data.contains("mode") && data["mode"].is_number())
    {
        m_currentMode = static_cast<PlayerMode>(data["mode"].get<int>());
    }

    // Charger l'inventaire
    if (data.contains("cargo") && data["cargo"].is_array())
    {
        for (const auto &item : data["cargo"])
        {
            if (item.contains("id") && item.contains("name") &&
                item.contains("quantity") && item.contains("unitWeight"))
            {
                int id = item["id"];
                std::string name = item["name"];
                int quantity = item["quantity"];
                int unitWeight = item["unitWeight"];

                m_cargo.emplace(id, CargoItem(id, name, quantity, unitWeight));
                m_currentCargoWeight += quantity * unitWeight;
            }
        }
    }

    // Initialiser la flotte
    m_fleet = std::make_shared<Fleet>(m_name + "'s Fleet");

    // Charger les compétences commerciales
    if (data.contains("tradeSkills") && data["tradeSkills"].is_object())
    {
        m_tradeSkills.fromJson(data["tradeSkills"]);
    }

    // Charger les réputations des villes
    if (data.contains("cityReputations") && data["cityReputations"].is_object())
    {
        for (auto it = data["cityReputations"].begin(); it != data["cityReputations"].end(); ++it)
        {
            int cityId = std::stoi(it.key());
            m_cityReputations[cityId] = it.value().get<float>();
        }
    }

    // Charger les réputations des royaumes
    if (data.contains("kingdomReputations") && data["kingdomReputations"].is_object())
    {
        for (auto it = data["kingdomReputations"].begin(); it != data["kingdomReputations"].end(); ++it)
        {
            m_kingdomReputations[it.key()] = it.value().get<float>();
        }
    }

    // Charger les routes commerciales
    if (data.contains("tradeRoutes") && data["tradeRoutes"].is_array())
    {
        for (const auto &routeData : data["tradeRoutes"])
        {
            m_tradeRoutes.push_back(TradeRoute::fromJson(routeData));
        }
    }
}

// Conversion vers JSON
nlohmann::json Player::toJson() const
{
    nlohmann::json j;

    // Données de base
    j["name"] = m_name;
    j["gold"] = m_gold;
    j["level"] = m_level;
    j["experience"] = m_experience;

    // Position
    j["posX"] = m_posX;
    j["posY"] = m_posY;

    // Mode
    j["mode"] = static_cast<int>(m_currentMode);

    // Inventaire
    nlohmann::json cargo = nlohmann::json::array();
    for (const auto &[id, item] : m_cargo)
    {
        nlohmann::json cargoItem;
        cargoItem["id"] = item.id;
        cargoItem["name"] = item.name;
        cargoItem["quantity"] = item.quantity;
        cargoItem["unitWeight"] = item.unitWeight;
        cargo.push_back(cargoItem);
    }
    j["cargo"] = cargo;

    // Sauvegarder les compétences commerciales
    j["tradeSkills"] = m_tradeSkills.toJson();

    // Sauvegarder les réputations des villes
    nlohmann::json cityReps;
    for (const auto &[cityId, reputation] : m_cityReputations)
    {
        cityReps[std::to_string(cityId)] = reputation;
    }
    j["cityReputations"] = cityReps;

    // Sauvegarder les réputations des royaumes
    nlohmann::json kingdomReps;
    for (const auto &[kingdom, reputation] : m_kingdomReputations)
    {
        kingdomReps[kingdom] = reputation;
    }
    j["kingdomReputations"] = kingdomReps;

    // Sauvegarder les routes commerciales
    nlohmann::json routes = nlohmann::json::array();
    for (const auto &route : m_tradeRoutes)
    {
        routes.push_back(route.toJson());
    }
    j["tradeRoutes"] = routes;

    return j;
}

void Player::addExperience(int experience)
{
    m_experience += experience;

    // Vérifier si le joueur peut monter de niveau
    int expForNextLevel = m_level * 100; // Formule simplifiée

    while (m_experience >= expForNextLevel)
    {
        m_experience -= expForNextLevel;
        m_level++;
        expForNextLevel = m_level * 100;

        std::cout << "Niveau supérieur! Vous êtes maintenant niveau " << m_level << std::endl;
    }
}

bool Player::canAddCargo(int goodId, int quantity) const
{
    // Obtenir le bien commercial
    const auto *good = GameData::getInstance().findTradeGoodById(goodId);
    if (!good)
        return false;

    // Calculer le poids supplémentaire
    int unitWeight = 1; // Poids par défaut
    int additionalWeight = quantity * unitWeight;

    // Vérifier si le joueur a assez de place
    return (m_currentCargoWeight + additionalWeight <= m_cargoCapacity);
}

bool Player::addCargo(int goodId, const std::string &name, int quantity, int unitWeight)
{
    // Vérifier si le joueur a assez de place
    int additionalWeight = quantity * unitWeight;
    if (m_currentCargoWeight + additionalWeight > m_cargoCapacity)
    {
        return false;
    }

    // Ajouter ou mettre à jour l'élément dans l'inventaire
    auto it = m_cargo.find(goodId);
    if (it != m_cargo.end())
    {
        // Mettre à jour la quantité
        it->second.quantity += quantity;
    }
    else
    {
        // Ajouter un nouvel élément
        m_cargo.emplace(goodId, CargoItem(goodId, name, quantity, unitWeight));
    }

    // Mettre à jour le poids total
    m_currentCargoWeight += additionalWeight;

    return true;
}

bool Player::addCargo(int goodId, int quantity)
{
    // Obtenir les informations sur le bien commercial
    const auto *good = GameData::getInstance().findTradeGoodById(goodId);
    if (!good)
        return false;

    // Utiliser la méthode principale
    return addCargo(goodId, good->getName(), quantity, 1); // Poids unitaire par défaut
}

bool Player::hasCargo(int goodId, int quantity) const
{
    // Vérifier si le joueur possède ce bien
    auto it = m_cargo.find(goodId);
    if (it == m_cargo.end())
    {
        return false;
    }

    // Vérifier si la quantité est suffisante
    return it->second.quantity >= quantity;
}

bool Player::removeCargo(int goodId, int quantity)
{
    // Vérifier si le joueur possède ce bien
    auto it = m_cargo.find(goodId);
    if (it == m_cargo.end() || it->second.quantity < quantity)
    {
        return false;
    }

    // Mettre à jour la quantité
    it->second.quantity -= quantity;

    // Mettre à jour le poids total
    m_currentCargoWeight -= quantity * it->second.unitWeight;

    // Supprimer l'élément si la quantité est nulle
    if (it->second.quantity <= 0)
    {
        m_cargo.erase(it);
    }

    return true;
}

void Player::addToInventory(int itemId, int quantity)
{
    if (quantity <= 0)
        return;

    m_inventory[itemId] += quantity;
}

void Player::removeFromInventory(int itemId, int quantity)
{
    if (quantity <= 0)
        return;

    auto it = m_inventory.find(itemId);
    if (it != m_inventory.end())
    {
        it->second = std::max(0, it->second - quantity);

        // Supprimez l'entrée si la quantité est tombée à 0
        if (it->second == 0)
        {
            m_inventory.erase(it);
        }
    }
}

int Player::getItemQuantity(int itemId) const
{
    auto it = m_inventory.find(itemId);
    return (it != m_inventory.end()) ? it->second : 0;
}

bool Player::addToCargo(int goodId, const std::string &name, int quantity, int unitWeight)
{
    // Vérifier si nous sommes en mode maritime
    if (m_currentMode != PlayerMode::SEA)
    {
        return false;
    }

    if (quantity <= 0)
        return true;

    // Vérifier si nous avons une flotte et au moins un navire
    auto flagship = getFlagship();
    if (!flagship)
        return false;

    // Vérifier si nous avons assez d'espace dans le navire amiral
    if (flagship->getRemainingCargo() < quantity * unitWeight)
    {
        // S'il n'y a pas assez d'espace, chercher un autre navire dans la flotte
        bool cargoAdded = false;
        size_t fleetSize = getFleetSize();

        for (size_t i = 0; i < fleetSize; ++i)
        {
            std::shared_ptr<Ship> ship = getShipFromFleet(i);
            if (ship && ship != flagship && ship->getRemainingCargo() >= quantity * unitWeight)
            {
                // Ajouter la cargaison à ce navire
                cargoAdded = ship->addCargo(goodId, name, quantity, unitWeight);
                if (cargoAdded)
                    break;
            }
        }

        return cargoAdded;
    }

    // Ajouter au navire amiral
    return flagship->addCargo(goodId, name, quantity, unitWeight);
}

bool Player::removeFromCargo(int goodId, int quantity)
{
    if (quantity <= 0)
        return true;

    // Vérifier si nous avons une flotte et au moins un navire
    if (!m_fleet || getFleetSize() == 0)
        return false;

    int remainingToRemove = quantity;
    size_t fleetSize = getFleetSize();

    // Parcourir tous les navires pour retirer la quantité demandée
    for (size_t i = 0; i < fleetSize && remainingToRemove > 0; ++i)
    {
        std::shared_ptr<Ship> ship = getShipFromFleet(i);
        if (ship)
        {
            int quantityInShip = ship->getCargoQuantity(goodId);
            if (quantityInShip > 0)
            {
                int toRemove = std::min(remainingToRemove, quantityInShip);
                ship->removeCargo(goodId, toRemove);
                remainingToRemove -= toRemove;
            }
        }
    }

    return (remainingToRemove == 0);
}

int Player::getCargoQuantity(int goodId) const
{
    if (!m_fleet)
        return 0;

    int totalQuantity = 0;
    size_t fleetSize = getFleetSize();

    // Somme des quantités dans tous les navires
    for (size_t i = 0; i < fleetSize; ++i)
    {
        std::shared_ptr<Ship> ship = getShipFromFleet(i);
        if (ship)
        {
            totalQuantity += ship->getCargoQuantity(goodId);
        }
    }

    return totalQuantity;
}

int Player::getTotalCargoWeight() const
{
    int totalWeight = 0;
    for (const auto &[goodId, item] : m_cargo)
    {
        totalWeight += item.unitWeight * item.quantity;
    }
    return totalWeight;
}

float Player::getCityReputation(int cityId) const
{
    auto it = m_cityReputations.find(cityId);
    return (it != m_cityReputations.end()) ? it->second : 0.0f;
}

float Player::getKingdomReputation(const std::string &kingdom) const
{
    auto it = m_kingdomReputations.find(kingdom);
    return (it != m_kingdomReputations.end()) ? it->second : 0.0f;
}

void Player::improveTradeSkill(const std::string &skillName, int amount)
{
    if (skillName == "negotiation")
    {
        m_tradeSkills.negotiation += amount;
        m_tradeSkills.negotiation = std::min(10, m_tradeSkills.negotiation); // Maximum de 10
    }
    else if (skillName == "logistics")
    {
        m_tradeSkills.logistics += amount;
        m_tradeSkills.logistics = std::min(10, m_tradeSkills.logistics);
    }
    else if (skillName == "smuggling")
    {
        m_tradeSkills.smuggling += amount;
        m_tradeSkills.smuggling = std::min(10, m_tradeSkills.smuggling);
    }
    else if (skillName == "influence")
    {
        m_tradeSkills.influence += amount;
        m_tradeSkills.influence = std::min(10, m_tradeSkills.influence);
    }
}

void Player::setCityReputation(int cityId, float reputation)
{
    m_cityReputations[cityId] = std::clamp(reputation, -100.0f, 100.0f);
}

void Player::setKingdomReputation(const std::string &kingdom, float reputation)
{
    m_kingdomReputations[kingdom] = std::clamp(reputation, -100.0f, 100.0f);
}

void Player::updateCityReputation(int cityId, float amount)
{
    auto it = m_cityReputations.find(cityId);
    if (it != m_cityReputations.end())
    {
        it->second = std::clamp(it->second + amount, -100.0f, 100.0f);
    }
    else
    {
        m_cityReputations[cityId] = std::clamp(amount, -100.0f, 100.0f);
    }
}

void Player::updateKingdomReputation(const std::string &kingdom, float amount)
{
    auto it = m_kingdomReputations.find(kingdom);
    if (it != m_kingdomReputations.end())
    {
        it->second = std::clamp(it->second + amount, -100.0f, 100.0f);
    }
    else
    {
        m_kingdomReputations[kingdom] = std::clamp(amount, -100.0f, 100.0f);
    }
}

int Player::addTradeRoute(const TradeRoute &route)
{
    m_tradeRoutes.push_back(route);
    return m_tradeRoutes.size() - 1; // Retourne l'index de la nouvelle route
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