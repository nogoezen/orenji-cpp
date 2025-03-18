#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm> // Pour std::min, std::max, std::clamp
#include "Ship.h"
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
    SEA   // Mode maritime (utilise Ship)
};

class Player
{
private:
    std::string m_name;
    int m_gold;
    int m_level;
    int m_experience;

    // Navire actuel du joueur
    std::shared_ptr<Ship> m_currentShip;

    // Personnage du joueur
    std::shared_ptr<Character> m_character;

    // Inventaire du joueur (ID de l'article, quantité)
    std::map<int, int> m_inventory;

    // Cargaison du navire (ID du bien commercial, quantité)
    std::map<int, int> m_cargo;

    // Position du joueur (x, y)
    int m_posX;
    int m_posY;

    // Mode actuel du joueur (terre ou mer)
    PlayerMode m_currentMode;

    // Équipage du navire
    int m_currentCrew;
    int m_maxCrew;

    // Système de réputation envers les nations
    std::map<std::string, int> m_reputation;

public:
    // Constructeur
    Player(const std::string &name, int gold = 1000, int level = 1, int experience = 0)
        : m_name(name),
          m_gold(gold),
          m_level(level),
          m_experience(experience),
          m_currentShip(nullptr),
          m_character(nullptr),
          m_posX(0),
          m_posY(0),
          m_currentMode(PlayerMode::LAND),
          m_currentCrew(0),
          m_maxCrew(0) {}

    // Constructeur avec personnage
    Player(const std::string &name, std::shared_ptr<Character> character, int gold = 1000)
        : m_name(name),
          m_gold(gold),
          m_level(character ? character->getLevel() : 1),
          m_experience(0),
          m_currentShip(nullptr),
          m_character(character),
          m_posX(0),
          m_posY(0),
          m_currentMode(PlayerMode::LAND),
          m_currentCrew(0),
          m_maxCrew(0)
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
    const std::shared_ptr<Ship> &getCurrentShip() const { return m_currentShip; }
    const std::map<int, int> &getInventory() const { return m_inventory; }
    const std::map<int, int> &getCargo() const { return m_cargo; }
    int getPosX() const { return m_posX; }
    int getPosY() const { return m_posY; }
    PlayerMode getCurrentMode() const { return m_currentMode; }
    int getCurrentCrew() const { return m_currentCrew; }
    int getMaxCrew() const { return m_maxCrew; }
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
    }
    void move(int deltaX, int deltaY)
    {
        m_posX += deltaX;
        m_posY += deltaY;
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

    // Méthodes liées au navire
    void setShip(std::shared_ptr<Ship> ship)
    {
        m_currentShip = ship;
        if (ship)
        {
            m_maxCrew = ship->getMaxCrew();
            // Définir un équipage initial minimum requis
            m_currentCrew = ship->getRequiredCrew();
        }
        else
        {
            m_maxCrew = 0;
            m_currentCrew = 0;
        }
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
        if (m_character && m_currentShip)
        {
            m_currentMode = (m_currentMode == PlayerMode::LAND) ? PlayerMode::SEA : PlayerMode::LAND;
        }
    }

    // Gestion de l'équipage
    void addCrew(int amount)
    {
        m_currentCrew = std::min(m_currentCrew + amount, m_maxCrew);
    }

    void removeCrew(int amount)
    {
        m_currentCrew = std::max(0, m_currentCrew - amount);
    }

    bool hasMinimumCrew() const
    {
        return m_currentShip && (m_currentCrew >= m_currentShip->getRequiredCrew());
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

    // Méthodes de cargaison
    bool addToCargo(int goodId, int quantity = 1);
    void removeFromCargo(int goodId, int quantity = 1);
    int getCargoQuantity(int goodId) const;
    int getTotalCargoWeight() const;
    int getRemainingCargoCapacity() const;

    // Méthodes pour obtenir les attributs selon le mode actuel
    std::string getCurrentEntityName() const
    {
        return (m_currentMode == PlayerMode::LAND) ? (m_character ? m_character->getName() : m_name) : (m_currentShip ? m_currentShip->getName() : "Sans navire");
    }

    int getSpeed() const
    {
        return (m_currentMode == PlayerMode::LAND) ? 5 : // Vitesse de base sur terre
                   (m_currentShip ? m_currentShip->getSpeed() : 0);
    }

    int getManeuverability() const
    {
        return (m_currentMode == PlayerMode::SEA && m_currentShip) ? m_currentShip->getManeuverability() : 0;
    }

    int getDurability() const
    {
        return (m_currentMode == PlayerMode::SEA && m_currentShip) ? m_currentShip->getDurability() : 0;
    }

    int getCombatPower() const
    {
        return (m_currentMode == PlayerMode::SEA && m_currentShip) ? m_currentShip->getCombatPower() : 0;
    }
};