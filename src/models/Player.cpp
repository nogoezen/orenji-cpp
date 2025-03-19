#include "Player.h"
#include "../game/GameData.h"
#include <algorithm>

void Player::addExperience(int amount)
{
    m_experience += amount;
    // Simple système de leveling (à améliorer)
    int nextLevelThreshold = m_level * 1000;
    if (m_experience >= nextLevelThreshold)
    {
        m_level++;
        // Récompensez le joueur pour le niveau suivant
        addGold(m_level * 500);

        // Si en mode terrestre, mettre à jour aussi le niveau du personnage
        if (m_currentMode == PlayerMode::LAND && m_character)
        {
            m_character->setLevel(m_level);
        }
    }
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

    for (const auto &[goodId, quantity] : m_cargo)
    {
        const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
        if (good)
        {
            totalWeight += good->getWeight() * quantity;
        }
    }

    return totalWeight;
}

int Player::getRemainingCargoCapacity() const
{
    if (m_currentMode != PlayerMode::SEA || !m_currentShip)
        return 0;

    int totalCapacity = m_currentShip->getCargoCapacity();
    int usedCapacity = getTotalCargoWeight();

    return std::max(0, totalCapacity - usedCapacity);
}

// Cargo management implementation
bool Player::addCargo(int goodId, const std::string &name, int quantity, int unitWeight)
{
    // Calculate total weight
    int totalWeight = quantity * unitWeight;

    // Check if there's enough cargo capacity
    if (getRemainingCargoCapacity() < totalWeight)
    {
        return false;
    }

    // Check if this good is already in cargo
    for (auto &item : m_cargo)
    {
        if (item.goodId == goodId)
        {
            // Add to existing quantity
            item.quantity += quantity;
            return true;
        }
    }

    // Add new cargo item
    m_cargo.push_back({goodId, name, quantity, unitWeight});
    return true;
}

bool Player::removeCargo(int goodId, int quantity)
{
    for (auto it = m_cargo.begin(); it != m_cargo.end(); ++it)
    {
        if (it->goodId == goodId)
        {
            // Check if there's enough quantity
            if (it->quantity < quantity)
            {
                return false;
            }

            // Remove quantity
            it->quantity -= quantity;

            // Remove item if quantity is zero
            if (it->quantity <= 0)
            {
                m_cargo.erase(it);
            }

            return true;
        }
    }

    // Good not found
    return false;
}

int Player::getCargoQuantity(int goodId) const
{
    for (const auto &item : m_cargo)
    {
        if (item.goodId == goodId)
        {
            return item.quantity;
        }
    }

    return 0;
}

int Player::getTotalCargoWeight() const
{
    int totalWeight = 0;

    for (const auto &item : m_cargo)
    {
        totalWeight += item.quantity * item.unitWeight;
    }

    return totalWeight;
}

bool Player::hasCargo(int goodId, int quantity) const
{
    return getCargoQuantity(goodId) >= quantity;
}