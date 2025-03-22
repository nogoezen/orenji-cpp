#include "Player.hpp"
#include "../data/GameData.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cmath>

namespace Orenji
{
    Player::Player()
        : m_id(-1), m_name(""), m_faction(""), m_money(0), m_level(1), m_experience(0), m_x(0.0f), m_y(0.0f), m_activeShipIndex(-1)
    {
        // Initialiser les statistiques à zéro
        m_stats.citiesVisited = 0;
        m_stats.transactionsMade = 0;
        m_stats.shipsDestroyed = 0;
        m_stats.distanceTraveled = 0;
        m_stats.goodsTraded = 0;
        m_stats.battlesWon = 0;
        m_stats.battlesLost = 0;
    }

    Player::Player(int id, const std::string &name, const std::string &faction)
        : m_id(id), m_name(name), m_faction(faction), m_money(1000) // Capital de départ
          ,
          m_level(1), m_experience(0), m_x(0.0f), m_y(0.0f), m_activeShipIndex(-1)
    {
        // Initialiser les statistiques à zéro
        m_stats.citiesVisited = 0;
        m_stats.transactionsMade = 0;
        m_stats.shipsDestroyed = 0;
        m_stats.distanceTraveled = 0;
        m_stats.goodsTraded = 0;
        m_stats.battlesWon = 0;
        m_stats.battlesLost = 0;

        // Initialiser la réputation envers sa propre faction
        m_reputations[faction] = 100;
    }

    Player::Player(const nlohmann::json &json)
        : m_id(-1), m_name(""), m_faction(""), m_money(0), m_level(1), m_experience(0), m_x(0.0f), m_y(0.0f), m_activeShipIndex(-1)
    {
        // Initialiser les statistiques à zéro
        m_stats.citiesVisited = 0;
        m_stats.transactionsMade = 0;
        m_stats.shipsDestroyed = 0;
        m_stats.distanceTraveled = 0;
        m_stats.goodsTraded = 0;
        m_stats.battlesWon = 0;
        m_stats.battlesLost = 0;

        // Charger les propriétés de base à partir du JSON
        if (json.contains("id") && json["id"].is_number())
            m_id = json["id"];

        if (json.contains("name") && json["name"].is_string())
            m_name = json["name"];

        if (json.contains("faction") && json["faction"].is_string())
            m_faction = json["faction"];

        if (json.contains("money") && json["money"].is_number())
            m_money = json["money"];

        if (json.contains("level") && json["level"].is_number())
            m_level = json["level"];

        if (json.contains("experience") && json["experience"].is_number())
            m_experience = json["experience"];

        if (json.contains("x") && json["x"].is_number())
            m_x = json["x"];

        if (json.contains("y") && json["y"].is_number())
            m_y = json["y"];

        if (json.contains("activeShipIndex") && json["activeShipIndex"].is_number())
            m_activeShipIndex = json["activeShipIndex"];

        // Charger l'inventaire
        if (json.contains("inventory") && json["inventory"].is_object())
        {
            for (auto it = json["inventory"].begin(); it != json["inventory"].end(); ++it)
            {
                int itemId = std::stoi(it.key());
                int quantity = it.value();
                m_inventory[itemId] = quantity;
            }
        }

        // Charger les navires
        if (json.contains("ships") && json["ships"].is_array())
        {
            for (const auto &shipJson : json["ships"])
            {
                m_ships.emplace_back(shipJson);
            }
        }

        // Charger les compétences
        if (json.contains("skills") && json["skills"].is_object())
        {
            for (auto it = json["skills"].begin(); it != json["skills"].end(); ++it)
            {
                m_skills[it.key()] = it.value();
            }
        }

        // Charger les réputations
        if (json.contains("reputations") && json["reputations"].is_object())
        {
            for (auto it = json["reputations"].begin(); it != json["reputations"].end(); ++it)
            {
                m_reputations[it.key()] = it.value();
            }
        }

        // Charger les statistiques
        if (json.contains("stats") && json["stats"].is_object())
        {
            const auto &stats = json["stats"];

            if (stats.contains("citiesVisited") && stats["citiesVisited"].is_number())
                m_stats.citiesVisited = stats["citiesVisited"];

            if (stats.contains("transactionsMade") && stats["transactionsMade"].is_number())
                m_stats.transactionsMade = stats["transactionsMade"];

            if (stats.contains("shipsDestroyed") && stats["shipsDestroyed"].is_number())
                m_stats.shipsDestroyed = stats["shipsDestroyed"];

            if (stats.contains("distanceTraveled") && stats["distanceTraveled"].is_number())
                m_stats.distanceTraveled = stats["distanceTraveled"];

            if (stats.contains("goodsTraded") && stats["goodsTraded"].is_number())
                m_stats.goodsTraded = stats["goodsTraded"];

            if (stats.contains("battlesWon") && stats["battlesWon"].is_number())
                m_stats.battlesWon = stats["battlesWon"];

            if (stats.contains("battlesLost") && stats["battlesLost"].is_number())
                m_stats.battlesLost = stats["battlesLost"];
        }

        // Charger les quêtes actives
        if (json.contains("activeQuests") && json["activeQuests"].is_object())
        {
            for (auto it = json["activeQuests"].begin(); it != json["activeQuests"].end(); ++it)
            {
                int questId = std::stoi(it.key());
                int progress = it.value();
                m_activeQuests[questId] = progress;
            }
        }

        // Charger les quêtes terminées
        if (json.contains("completedQuests") && json["completedQuests"].is_array())
        {
            for (const auto &questId : json["completedQuests"])
            {
                if (questId.is_number())
                    m_completedQuests.push_back(questId);
            }
        }
    }

    nlohmann::json Player::toJson() const
    {
        nlohmann::json json;

        // Propriétés de base
        json["id"] = m_id;
        json["name"] = m_name;
        json["faction"] = m_faction;
        json["money"] = m_money;
        json["level"] = m_level;
        json["experience"] = m_experience;
        json["x"] = m_x;
        json["y"] = m_y;
        json["activeShipIndex"] = m_activeShipIndex;

        // Inventaire
        nlohmann::json inventory = nlohmann::json::object();
        for (const auto &item : m_inventory)
        {
            inventory[std::to_string(item.first)] = item.second;
        }
        json["inventory"] = inventory;

        // Navires
        nlohmann::json ships = nlohmann::json::array();
        for (const auto &ship : m_ships)
        {
            ships.push_back(ship.toJson());
        }
        json["ships"] = ships;

        // Compétences
        nlohmann::json skills = nlohmann::json::object();
        for (const auto &skill : m_skills)
        {
            skills[skill.first] = skill.second;
        }
        json["skills"] = skills;

        // Réputations
        nlohmann::json reputations = nlohmann::json::object();
        for (const auto &reputation : m_reputations)
        {
            reputations[reputation.first] = reputation.second;
        }
        json["reputations"] = reputations;

        // Statistiques
        nlohmann::json stats = nlohmann::json::object();
        stats["citiesVisited"] = m_stats.citiesVisited;
        stats["transactionsMade"] = m_stats.transactionsMade;
        stats["shipsDestroyed"] = m_stats.shipsDestroyed;
        stats["distanceTraveled"] = m_stats.distanceTraveled;
        stats["goodsTraded"] = m_stats.goodsTraded;
        stats["battlesWon"] = m_stats.battlesWon;
        stats["battlesLost"] = m_stats.battlesLost;
        json["stats"] = stats;

        // Quêtes actives
        nlohmann::json activeQuests = nlohmann::json::object();
        for (const auto &quest : m_activeQuests)
        {
            activeQuests[std::to_string(quest.first)] = quest.second;
        }
        json["activeQuests"] = activeQuests;

        // Quêtes terminées
        nlohmann::json completedQuests = nlohmann::json::array();
        for (int questId : m_completedQuests)
        {
            completedQuests.push_back(questId);
        }
        json["completedQuests"] = completedQuests;

        return json;
    }

    void Player::addMoney(int amount)
    {
        m_money += amount;
    }

    bool Player::spendMoney(int amount)
    {
        if (m_money >= amount)
        {
            m_money -= amount;
            return true;
        }
        return false;
    }

    bool Player::addExperience(int exp)
    {
        m_experience += exp;

        int expForNextLevel = getExperienceForNextLevel();

        // Vérifier si le joueur a gagné un niveau
        if (m_experience >= expForNextLevel)
        {
            m_level++;
            return true;
        }

        return false;
    }

    int Player::getExperienceForNextLevel() const
    {
        // Formule simple : 100 * niveau actuel
        return 100 * m_level;
    }

    void Player::addItem(int itemId, int quantity)
    {
        if (quantity <= 0)
            return;

        m_inventory[itemId] += quantity;
    }

    bool Player::removeItem(int itemId, int quantity)
    {
        if (quantity <= 0)
            return true;

        auto it = m_inventory.find(itemId);
        if (it != m_inventory.end() && it->second >= quantity)
        {
            it->second -= quantity;

            // Si la quantité devient nulle, supprimer l'entrée
            if (it->second == 0)
                m_inventory.erase(it);

            return true;
        }

        return false;
    }

    bool Player::hasItem(int itemId, int quantity) const
    {
        auto it = m_inventory.find(itemId);
        return (it != m_inventory.end() && it->second >= quantity);
    }

    int Player::getItemQuantity(int itemId) const
    {
        auto it = m_inventory.find(itemId);
        if (it != m_inventory.end())
            return it->second;
        return 0;
    }

    int Player::addShip(const Ship &ship)
    {
        m_ships.push_back(ship);

        // Si c'est le premier navire, le définir comme actif
        if (m_ships.size() == 1 && m_activeShipIndex == -1)
            m_activeShipIndex = 0;

        return static_cast<int>(m_ships.size() - 1);
    }

    bool Player::removeShip(int index)
    {
        if (index >= 0 && index < static_cast<int>(m_ships.size()))
        {
            // Si le navire à supprimer est le navire actif
            if (index == m_activeShipIndex)
            {
                // Si c'est le seul navire
                if (m_ships.size() == 1)
                    m_activeShipIndex = -1;
                else
                    // Définir le navire précédent comme actif
                    m_activeShipIndex = (index > 0) ? index - 1 : 0;
            }
            // Si le navire à supprimer est avant le navire actif, ajuster l'index
            else if (index < m_activeShipIndex)
            {
                m_activeShipIndex--;
            }

            m_ships.erase(m_ships.begin() + index);
            return true;
        }

        return false;
    }

    Ship *Player::getShip(int index)
    {
        if (index >= 0 && index < static_cast<int>(m_ships.size()))
            return &m_ships[index];
        return nullptr;
    }

    const Ship *Player::getShip(int index) const
    {
        if (index >= 0 && index < static_cast<int>(m_ships.size()))
            return &m_ships[index];
        return nullptr;
    }

    Ship *Player::getActiveShip()
    {
        if (m_activeShipIndex >= 0 && m_activeShipIndex < static_cast<int>(m_ships.size()))
            return &m_ships[m_activeShipIndex];
        return nullptr;
    }

    const Ship *Player::getActiveShip() const
    {
        if (m_activeShipIndex >= 0 && m_activeShipIndex < static_cast<int>(m_ships.size()))
            return &m_ships[m_activeShipIndex];
        return nullptr;
    }

    bool Player::setActiveShip(int index)
    {
        if (index >= 0 && index < static_cast<int>(m_ships.size()))
        {
            m_activeShipIndex = index;
            return true;
        }
        return false;
    }

    void Player::setSkill(const std::string &skillName, int level)
    {
        m_skills[skillName] = level;
    }

    int Player::getSkillLevel(const std::string &skillName) const
    {
        auto it = m_skills.find(skillName);
        if (it != m_skills.end())
            return it->second;
        return 0;
    }

    void Player::setReputation(const std::string &faction, int value)
    {
        // Limiter la réputation entre -100 et 100
        m_reputations[faction] = std::min(100, std::max(-100, value));
    }

    int Player::getReputation(const std::string &faction) const
    {
        auto it = m_reputations.find(faction);
        if (it != m_reputations.end())
            return it->second;
        return 0;
    }

    void Player::addQuest(int questId, int progress)
    {
        // Vérifier que la quête n'est pas déjà terminée
        if (std::find(m_completedQuests.begin(), m_completedQuests.end(), questId) == m_completedQuests.end())
        {
            // Limiter la progression entre 0 et 100
            m_activeQuests[questId] = std::min(100, std::max(0, progress));
        }
    }

    bool Player::updateQuestProgress(int questId, int progress)
    {
        auto it = m_activeQuests.find(questId);
        if (it != m_activeQuests.end())
        {
            // Limiter la progression entre 0 et 100
            it->second = std::min(100, std::max(0, progress));

            // Si la progression atteint 100%, terminer la quête
            if (it->second >= 100)
                completeQuest(questId);

            return true;
        }
        return false;
    }

    bool Player::completeQuest(int questId)
    {
        auto it = m_activeQuests.find(questId);
        if (it != m_activeQuests.end())
        {
            // Supprimer la quête des quêtes actives
            m_activeQuests.erase(it);

            // Ajouter la quête aux quêtes terminées si elle n'y est pas déjà
            if (std::find(m_completedQuests.begin(), m_completedQuests.end(), questId) == m_completedQuests.end())
                m_completedQuests.push_back(questId);

            return true;
        }
        return false;
    }

    bool Player::isQuestActive(int questId) const
    {
        return m_activeQuests.find(questId) != m_activeQuests.end();
    }

    bool Player::isQuestCompleted(int questId) const
    {
        return std::find(m_completedQuests.begin(), m_completedQuests.end(), questId) != m_completedQuests.end();
    }

    int Player::getQuestProgress(int questId) const
    {
        auto it = m_activeQuests.find(questId);
        if (it != m_activeQuests.end())
            return it->second;

        // Si la quête est terminée, retourner 100%
        if (isQuestCompleted(questId))
            return 100;

        return -1; // Quête non trouvée
    }

    void Player::incrementStat(const std::string &statName, int value)
    {
        if (statName == "citiesVisited")
            m_stats.citiesVisited += value;
        else if (statName == "transactionsMade")
            m_stats.transactionsMade += value;
        else if (statName == "shipsDestroyed")
            m_stats.shipsDestroyed += value;
        else if (statName == "distanceTraveled")
            m_stats.distanceTraveled += value;
        else if (statName == "goodsTraded")
            m_stats.goodsTraded += value;
        else if (statName == "battlesWon")
            m_stats.battlesWon += value;
        else if (statName == "battlesLost")
            m_stats.battlesLost += value;
    }

    int Player::getStat(const std::string &statName) const
    {
        if (statName == "citiesVisited")
            return m_stats.citiesVisited;
        else if (statName == "transactionsMade")
            return m_stats.transactionsMade;
        else if (statName == "shipsDestroyed")
            return m_stats.shipsDestroyed;
        else if (statName == "distanceTraveled")
            return m_stats.distanceTraveled;
        else if (statName == "goodsTraded")
            return m_stats.goodsTraded;
        else if (statName == "battlesWon")
            return m_stats.battlesWon;
        else if (statName == "battlesLost")
            return m_stats.battlesLost;

        return 0;
    }
}
