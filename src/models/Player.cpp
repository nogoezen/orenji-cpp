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

bool Player::addToCargo(int goodId, int quantity)
{
    // Vérifier si nous sommes en mode maritime
    if (m_currentMode != PlayerMode::SEA)
    {
        return false;
    }

    if (quantity <= 0)
        return true;

    // Vérifier si nous avons un navire
    if (!m_currentShip)
        return false;

    // Obtenir le bien commercial pour connaître son poids
    const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
    if (!good)
        return false;

    // Calculer le poids total du cargo après l'ajout
    int currentWeight = getTotalCargoWeight();
    int addedWeight = good->getWeight() * quantity;
    int totalWeight = currentWeight + addedWeight;

    // Vérifier si nous avons assez d'espace et d'équipage minimum
    if (totalWeight > m_currentShip->getCargoCapacity() || !hasMinimumCrew())
    {
        return false;
    }

    // Ajouter au cargo
    m_cargo[goodId] += quantity;
    return true;
}

void Player::removeFromCargo(int goodId, int quantity)
{
    if (quantity <= 0)
        return;

    auto it = m_cargo.find(goodId);
    if (it != m_cargo.end())
    {
        it->second = std::max(0, it->second - quantity);

        // Supprimez l'entrée si la quantité est tombée à 0
        if (it->second == 0)
        {
            m_cargo.erase(it);
        }
    }
}

int Player::getCargoQuantity(int goodId) const
{
    auto it = m_cargo.find(goodId);
    return (it != m_cargo.end()) ? it->second : 0;
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