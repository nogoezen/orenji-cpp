#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm> // Pour std::min, std::max, std::clamp
#include "Ship.h"
#include "Fleet.h"
#include "TradeGood.h"
#include "Character.h"

// Fonction clamp (au cas où std::clamp n'est pas disponible dans la version de C++ utilisée)
template <typename T>
T customClamp(const T &value, const T &min, const T &max)
{
    return std::min(std::max(value, min), max);
}

enum class PlayerMode
{
    LAND, // Mode terrestre (utilise Character)
    SEA   // Mode maritime (utilise Fleet/Ship)
};

class Player
{
private:
    std::string m_name;
    int m_gold;
    int m_level;
    int m_experience;

    // Flotte du joueur (remplace le navire unique)
    std::shared_ptr<Fleet> m_fleet;

    // Personnage du joueur
    std::shared_ptr<Character> m_character;

    // Inventaire du joueur (ID de l'article, quantité)
    std::map<int, int> m_inventory;

    // Position du joueur (x, y)
    int m_posX;
    int m_posY;

    // Mode actuel du joueur (terre ou mer)
    PlayerMode m_currentMode;

    // Système de réputation envers les nations
    std::map<std::string, int> m_reputation;

    // Added cargo management for trade goods
    struct CargoItem
    {
        int goodId;
        std::string name;
        int quantity;
        int unitWeight;
    };
    std::vector<CargoItem> m_cargo;

public:
    // Constructeur
    Player(const std::string &name, int gold = 1000, int level = 1, int experience = 0)
        : m_name(name),
          m_gold(gold),
          m_level(level),
          m_experience(experience),
          m_fleet(std::make_shared<Fleet>(name + "'s Fleet")),
          m_character(nullptr),
          m_posX(0),
          m_posY(0),
          m_currentMode(PlayerMode::LAND) {}

    // Constructeur avec personnage
    Player(const std::string &name, std::shared_ptr<Character> character, int gold = 1000)
        : m_name(name),
          m_gold(gold),
          m_level(character ? character->getLevel() : 1),
          m_experience(0),
          m_fleet(std::make_shared<Fleet>(name + "'s Fleet")),
          m_character(character),
          m_posX(0),
          m_posY(0),
          m_currentMode(PlayerMode::LAND)
    {
        // Récupérer les réputations du personnage
        if (character)
        {
            const auto &morality = character->getMorality();
            for (const auto &[nation, rep] : morality.reputation)
            {
                m_reputation[nation] = rep;
            }
        }
    }

    // Getters
    const std::string &getName() const { return m_name; }
    int getGold() const { return m_gold; }
    int getLevel() const { return m_level; }
    int getExperience() const { return m_experience; }
    const std::shared_ptr<Fleet> &getFleet() const { return m_fleet; }
    std::shared_ptr<Ship> getFlagship() const { return m_fleet ? m_fleet->getFlagship() : nullptr; }
    const std::map<int, int> &getInventory() const { return m_inventory; }
    int getPosX() const { return m_posX; }
    int getPosY() const { return m_posY; }
    PlayerMode getCurrentMode() const { return m_currentMode; }
    const std::map<std::string, int> &getReputation() const { return m_reputation; }

    // Getter pour le personnage
    const std::shared_ptr<Character> &getCharacter() const { return m_character; }

    // Setters et méthodes d'action
    void setName(const std::string &name) { m_name = name; }
    void addGold(int amount) { m_gold += amount; }
    void removeGold(int amount) { m_gold = std::max(0, m_gold - amount); }
    void addExperience(int amount);
    void setPosition(int x, int y)
    {
        m_posX = x;
        m_posY = y;
        // Mettre à jour la position de la flotte également
        if (m_fleet)
        {
            m_fleet->setPosition(x, y);
        }
    }
    void move(int deltaX, int deltaY)
    {
        m_posX += deltaX;
        m_posY += deltaY;
        // Mettre à jour la position de la flotte également
        if (m_fleet)
        {
            m_fleet->setPosition(m_posX, m_posY);
        }
    }

    // Définir le personnage
    void setCharacter(std::shared_ptr<Character> character)
    {
        m_character = character;

        // Mettre à jour les réputations
        if (character)
        {
            const auto &morality = character->getMorality();
            for (const auto &[nation, rep] : morality.reputation)
            {
                m_reputation[nation] = rep;
            }
        }
    }

    // Méthodes liées à la flotte
    void setFleet(std::shared_ptr<Fleet> fleet)
    {
        m_fleet = fleet;
        if (fleet)
        {
            // Synchroniser la position de la flotte avec celle du joueur
            fleet->setPosition(m_posX, m_posY);
        }
    }

    // Ajouter/supprimer des navires à la flotte
    bool addShipToFleet(std::shared_ptr<Ship> ship)
    {
        if (!m_fleet)
        {
            m_fleet = std::make_shared<Fleet>(m_name + "'s Fleet");
        }
        return m_fleet->addShip(ship);
    }

    bool removeShipFromFleet(size_t index)
    {
        return m_fleet ? m_fleet->removeShip(index) : false;
    }

    std::shared_ptr<Ship> getShipFromFleet(size_t index) const
    {
        return m_fleet ? m_fleet->getShip(index) : nullptr;
    }

    bool setFlagship(size_t index)
    {
        return m_fleet ? m_fleet->setFlagship(index) : false;
    }

    size_t getFleetSize() const
    {
        return m_fleet ? m_fleet->getSize() : 0;
    }

    // Création d'une nouvelle flotte
    void createNewFleet(const std::string &name)
    {
        m_fleet = std::make_shared<Fleet>(name);
        m_fleet->setPosition(m_posX, m_posY);
    }

    // Gestion du mode (terre/mer)
    void setMode(PlayerMode mode)
    {
        m_currentMode = mode;
    }

    void toggleMode()
    {
        // Basculer entre mode terrestre et maritime
        // Seulement si les deux modes sont disponibles
        if (m_character && m_fleet && m_fleet->getSize() > 0)
        {
            m_currentMode = (m_currentMode == PlayerMode::LAND) ? PlayerMode::SEA : PlayerMode::LAND;
        }
    }

    // Gestion de l'équipage
    void addCrew(int amount)
    {
        auto flagship = getFlagship();
        if (flagship)
        {
            flagship->addCrew(amount);
        }
    }

    void removeCrew(int amount)
    {
        auto flagship = getFlagship();
        if (flagship)
        {
            flagship->removeCrew(amount);
        }
    }

    int getCurrentCrew() const
    {
        return m_fleet ? m_fleet->getCurrentCrew() : 0;
    }

    int getTotalCrew() const
    {
        return m_fleet ? m_fleet->getTotalCrew() : 0;
    }

    bool hasMinimumCrew() const
    {
        return m_fleet && m_fleet->canNavigate();
    }

    // Gestion de la réputation
    void changeReputation(const std::string &nation, int amount)
    {
        m_reputation[nation] = customClamp(m_reputation[nation] + amount, -100, 100);
    }

    int getReputationWith(const std::string &nation) const
    {
        auto it = m_reputation.find(nation);
        return (it != m_reputation.end()) ? it->second : 0;
    }

    // Méthodes d'inventaire
    void addToInventory(int itemId, int quantity = 1);
    void removeFromInventory(int itemId, int quantity = 1);
    int getItemQuantity(int itemId) const;

    // Méthodes de cargaison (maintenant déléguées à la flotte)
    bool addToCargo(int goodId, const std::string &name, int quantity = 1, int unitWeight = 1);
    bool removeFromCargo(int goodId, int quantity = 1);
    int getTotalCargoCapacity() const { return m_fleet ? m_fleet->getTotalCargo() : 0; }
    int getUsedCargo() const { return m_fleet ? m_fleet->getUsedCargo() : 0; }
    int getRemainingCargoCapacity() const { return m_fleet ? m_fleet->getRemainingCargo() : 0; }

    // Méthodes pour obtenir les attributs selon le mode actuel
    std::string getCurrentEntityName() const
    {
        if (m_currentMode == PlayerMode::LAND)
        {
            return m_character ? m_character->getName() : m_name;
        }
        else
        {
            auto flagship = getFlagship();
            return flagship ? flagship->getName() : "Sans navire";
        }
    }

    int getSpeed() const
    {
        return (m_currentMode == PlayerMode::LAND) ? 5 : // Vitesse de base sur terre
                   (m_fleet ? m_fleet->getSpeed() : 0);
    }

    int getManeuverability() const
    {
        if (m_currentMode == PlayerMode::SEA)
        {
            auto flagship = getFlagship();
            return flagship ? flagship->getManeuverability() : 0;
        }
        return 0;
    }

    int getDurability() const
    {
        if (m_currentMode == PlayerMode::SEA)
        {
            auto flagship = getFlagship();
            return flagship ? flagship->getDurability() : 0;
        }
        return 0;
    }

    int getCombatPower() const
    {
        if (m_currentMode == PlayerMode::SEA)
        {
            auto flagship = getFlagship();
            return flagship ? flagship->getCombatPower() : 0;
        }
        return 0;
    }

    // Cargo management methods
    bool addCargo(int goodId, const std::string &name, int quantity, int unitWeight);
    bool removeCargo(int goodId, int quantity);
    int getCargoQuantity(int goodId) const;
    const std::vector<CargoItem> &getCargo() const { return m_cargo; }
    int getTotalCargoWeight() const;

    // Check if player has enough of a specific cargo
    bool hasCargo(int goodId, int quantity) const;
};