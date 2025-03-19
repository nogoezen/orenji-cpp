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

    // Les autres données comme les navires et l'équipage seraient chargées ici
}

// Conversion vers JSON
nlohmann::json Player::toJson() const
{
    nlohmann::json data;

    // Données de base
    data["name"] = m_name;
    data["gold"] = m_gold;
    data["level"] = m_level;
    data["experience"] = m_experience;

    // Position
    data["posX"] = m_posX;
    data["posY"] = m_posY;

    // Mode
    data["mode"] = static_cast<int>(m_currentMode);

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
    data["cargo"] = cargo;

    // Les autres données seraient ajoutées ici

    return data;
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