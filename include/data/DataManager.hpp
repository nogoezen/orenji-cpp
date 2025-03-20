#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <type_traits>
#include <optional>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "../models/Ship.h"
#include "../models/Fleet.h"
#include "../models/Character.h"
#include "../models/World.h"
#include "../models/TradeGood.h"
#include "../models/Player.h"

namespace Orenji
{
    /**
     * @brief Gestionnaire central des données du jeu
     *
     * Classe singleton qui gère le chargement, la sauvegarde et l'accès
     * à toutes les données du jeu (flottes, navires, personnages, etc.)
     */
    class DataManager
    {
    public:
        /**
         * @brief Obtient l'instance unique du DataManager
         * @return Référence vers l'instance
         */
        static DataManager &getInstance();

        /**
         * @brief Initialise le gestionnaire de données
         * @param basePath Chemin de base pour les fichiers de données
         * @return true si l'initialisation a réussi, false sinon
         */
        bool initialize(const std::string &basePath = "resources/data/");

        /**
         * @brief Charge toutes les données du jeu
         * @return true si le chargement a réussi, false sinon
         */
        bool loadAllData();

        /**
         * @brief Sauvegarde toutes les données du jeu
         * @return true si la sauvegarde a réussi, false sinon
         */
        bool saveAllData();

        /**
         * @brief Charge les données d'un type spécifique
         * @tparam T Type de données à charger
         * @param filename Nom du fichier à charger
         * @return true si le chargement a réussi, false sinon
         */
        template <typename T>
        bool loadData(const std::string &filename);

        /**
         * @brief Sauvegarde les données d'un type spécifique
         * @tparam T Type de données à sauvegarder
         * @param filename Nom du fichier pour la sauvegarde
         * @return true si la sauvegarde a réussi, false sinon
         */
        template <typename T>
        bool saveData(const std::string &filename);

        /**
         * @brief Obtient une entité par son ID
         * @tparam T Type d'entité à obtenir
         * @param id Identifiant de l'entité
         * @return Pointeur partagé vers l'entité ou nullptr si non trouvée
         */
        template <typename T>
        std::shared_ptr<T> getEntityById(int id);

        /**
         * @brief Obtient des entités qui correspondent à un critère
         * @tparam T Type d'entité à filtrer
         * @param filterFunc Fonction de filtrage
         * @return Vecteur de pointeurs partagés vers les entités filtrées
         */
        template <typename T>
        std::vector<std::shared_ptr<T>> getEntitiesWhere(std::function<bool(const T &)> filterFunc);

        /**
         * @brief Ajoute une nouvelle entité
         * @tparam T Type d'entité à ajouter
         * @param entity Entité à ajouter
         * @return Identifiant de la nouvelle entité
         */
        template <typename T>
        int addEntity(const T &entity);

        /**
         * @brief Met à jour une entité existante
         * @tparam T Type d'entité à mettre à jour
         * @param entity Entité avec les nouvelles données
         * @return true si la mise à jour a réussi, false sinon
         */
        template <typename T>
        bool updateEntity(const T &entity);

        /**
         * @brief Supprime une entité par son ID
         * @tparam T Type d'entité à supprimer
         * @param id Identifiant de l'entité
         * @return true si la suppression a réussi, false sinon
         */
        template <typename T>
        bool deleteEntity(int id);

        /**
         * @brief Obtient tous les personnages
         * @return Vecteur de tous les personnages
         */
        std::vector<std::shared_ptr<Character>> getAllCharacters() const;

        /**
         * @brief Obtient tous les navires
         * @return Vecteur de tous les navires
         */
        std::vector<std::shared_ptr<Ship>> getAllShips() const;

        /**
         * @brief Obtient toutes les flottes
         * @return Vecteur de toutes les flottes
         */
        std::vector<std::shared_ptr<Fleet>> getAllFleets() const;

        /**
         * @brief Obtient tous les biens commerciaux
         * @return Vecteur de tous les biens commerciaux
         */
        std::vector<std::shared_ptr<TradeGood>> getAllTradeGoods() const;

        /**
         * @brief Obtient le monde actuel
         * @return Référence au monde actuel
         */
        World &getWorld();

        /**
         * @brief Obtient le joueur actuel
         * @return Référence au joueur actuel
         */
        Player &getPlayer();

    private:
        // Constructeur privé (singleton)
        DataManager();
        ~DataManager();

        // Interdire la copie et l'assignation
        DataManager(const DataManager &) = delete;
        DataManager &operator=(const DataManager &) = delete;

        // Instance singleton
        static std::unique_ptr<DataManager> s_instance;

        // Chemin de base pour les fichiers de données
        std::string m_basePath;

        // Collections de données
        std::unordered_map<int, std::shared_ptr<Character>> m_characters;
        std::unordered_map<int, std::shared_ptr<Ship>> m_ships;
        std::unordered_map<int, std::shared_ptr<Fleet>> m_fleets;
        std::unordered_map<int, std::shared_ptr<TradeGood>> m_tradeGoods;
        std::shared_ptr<World> m_world;
        std::shared_ptr<Player> m_player;

        // Fonctions utilitaires pour JSON
        nlohmann::json entityToJson(const Character &character) const;
        nlohmann::json entityToJson(const Ship &ship) const;
        nlohmann::json entityToJson(const Fleet &fleet) const;
        nlohmann::json entityToJson(const TradeGood &tradeGood) const;
        nlohmann::json entityToJson(const World &world) const;
        nlohmann::json entityToJson(const Player &player) const;

        std::shared_ptr<Character> jsonToCharacter(const nlohmann::json &json) const;
        std::shared_ptr<Ship> jsonToShip(const nlohmann::json &json) const;
        std::shared_ptr<Fleet> jsonToFleet(const nlohmann::json &json) const;
        std::shared_ptr<TradeGood> jsonToTradeGood(const nlohmann::json &json) const;
        std::shared_ptr<World> jsonToWorld(const nlohmann::json &json) const;
        std::shared_ptr<Player> jsonToPlayer(const nlohmann::json &json) const;

        // Génération de nouveaux IDs
        int generateNextId(const std::unordered_map<int, std::shared_ptr<Character>> &collection) const;
        int generateNextId(const std::unordered_map<int, std::shared_ptr<Ship>> &collection) const;
        int generateNextId(const std::unordered_map<int, std::shared_ptr<Fleet>> &collection) const;
        int generateNextId(const std::unordered_map<int, std::shared_ptr<TradeGood>> &collection) const;
    };

    // Implémentations des templates
    template <typename T>
    std::shared_ptr<T> DataManager::getEntityById(int id)
    {
        if constexpr (std::is_same_v<T, Character>)
        {
            auto it = m_characters.find(id);
            return (it != m_characters.end()) ? it->second : nullptr;
        }
        else if constexpr (std::is_same_v<T, Ship>)
        {
            auto it = m_ships.find(id);
            return (it != m_ships.end()) ? it->second : nullptr;
        }
        else if constexpr (std::is_same_v<T, Fleet>)
        {
            auto it = m_fleets.find(id);
            return (it != m_fleets.end()) ? it->second : nullptr;
        }
        else if constexpr (std::is_same_v<T, TradeGood>)
        {
            auto it = m_tradeGoods.find(id);
            return (it != m_tradeGoods.end()) ? it->second : nullptr;
        }
        else if constexpr (std::is_same_v<T, World>)
        {
            return m_world;
        }
        else if constexpr (std::is_same_v<T, Player>)
        {
            return m_player;
        }
        return nullptr;
    }

    template <typename T>
    std::vector<std::shared_ptr<T>> DataManager::getEntitiesWhere(std::function<bool(const T &)> filterFunc)
    {
        std::vector<std::shared_ptr<T>> result;

        if constexpr (std::is_same_v<T, Character>)
        {
            for (const auto &[id, entity] : m_characters)
            {
                if (filterFunc(*entity))
                {
                    result.push_back(entity);
                }
            }
        }
        else if constexpr (std::is_same_v<T, Ship>)
        {
            for (const auto &[id, entity] : m_ships)
            {
                if (filterFunc(*entity))
                {
                    result.push_back(entity);
                }
            }
        }
        else if constexpr (std::is_same_v<T, Fleet>)
        {
            for (const auto &[id, entity] : m_fleets)
            {
                if (filterFunc(*entity))
                {
                    result.push_back(entity);
                }
            }
        }
        else if constexpr (std::is_same_v<T, TradeGood>)
        {
            for (const auto &[id, entity] : m_tradeGoods)
            {
                if (filterFunc(*entity))
                {
                    result.push_back(entity);
                }
            }
        }

        return result;
    }

    template <typename T>
    int DataManager::addEntity(const T &entity)
    {
        if constexpr (std::is_same_v<T, Character>)
        {
            int id = generateNextId(m_characters);
            auto newEntity = std::make_shared<Character>(entity);
            newEntity->setId(id);
            m_characters[id] = newEntity;
            return id;
        }
        else if constexpr (std::is_same_v<T, Ship>)
        {
            int id = generateNextId(m_ships);
            auto newEntity = std::make_shared<Ship>(entity);
            newEntity->setId(id);
            m_ships[id] = newEntity;
            return id;
        }
        else if constexpr (std::is_same_v<T, Fleet>)
        {
            int id = generateNextId(m_fleets);
            auto newEntity = std::make_shared<Fleet>(entity);
            newEntity->setId(id);
            m_fleets[id] = newEntity;
            return id;
        }
        else if constexpr (std::is_same_v<T, TradeGood>)
        {
            int id = generateNextId(m_tradeGoods);
            auto newEntity = std::make_shared<TradeGood>(entity);
            newEntity->setId(id);
            m_tradeGoods[id] = newEntity;
            return id;
        }
        else if constexpr (std::is_same_v<T, World>)
        {
            m_world = std::make_shared<World>(entity);
            return 0;
        }
        else if constexpr (std::is_same_v<T, Player>)
        {
            m_player = std::make_shared<Player>(entity);
            return 0;
        }
        return -1;
    }

    template <typename T>
    bool DataManager::updateEntity(const T &entity)
    {
        if constexpr (std::is_same_v<T, Character>)
        {
            int id = entity.getId();
            if (m_characters.find(id) != m_characters.end())
            {
                m_characters[id] = std::make_shared<Character>(entity);
                return true;
            }
        }
        else if constexpr (std::is_same_v<T, Ship>)
        {
            int id = entity.getId();
            if (m_ships.find(id) != m_ships.end())
            {
                m_ships[id] = std::make_shared<Ship>(entity);
                return true;
            }
        }
        else if constexpr (std::is_same_v<T, Fleet>)
        {
            int id = entity.getId();
            if (m_fleets.find(id) != m_fleets.end())
            {
                m_fleets[id] = std::make_shared<Fleet>(entity);
                return true;
            }
        }
        else if constexpr (std::is_same_v<T, TradeGood>)
        {
            int id = entity.getId();
            if (m_tradeGoods.find(id) != m_tradeGoods.end())
            {
                m_tradeGoods[id] = std::make_shared<TradeGood>(entity);
                return true;
            }
        }
        else if constexpr (std::is_same_v<T, World>)
        {
            m_world = std::make_shared<World>(entity);
            return true;
        }
        else if constexpr (std::is_same_v<T, Player>)
        {
            m_player = std::make_shared<Player>(entity);
            return true;
        }
        return false;
    }

    template <typename T>
    bool DataManager::deleteEntity(int id)
    {
        if constexpr (std::is_same_v<T, Character>)
        {
            return m_characters.erase(id) > 0;
        }
        else if constexpr (std::is_same_v<T, Ship>)
        {
            return m_ships.erase(id) > 0;
        }
        else if constexpr (std::is_same_v<T, Fleet>)
        {
            return m_fleets.erase(id) > 0;
        }
        else if constexpr (std::is_same_v<T, TradeGood>)
        {
            return m_tradeGoods.erase(id) > 0;
        }
        return false;
    }

    template <typename T>
    bool DataManager::loadData(const std::string &filename)
    {
        std::string fullPath = m_basePath + filename;

        try
        {
            std::ifstream file(fullPath);
            if (!file.is_open())
            {
                return false;
            }

            nlohmann::json json;
            file >> json;
            file.close();

            if constexpr (std::is_same_v<T, Character>)
            {
                m_characters.clear();
                for (const auto &item : json)
                {
                    auto character = jsonToCharacter(item);
                    if (character)
                    {
                        m_characters[character->getId()] = character;
                    }
                }
            }
            else if constexpr (std::is_same_v<T, Ship>)
            {
                m_ships.clear();
                for (const auto &item : json)
                {
                    auto ship = jsonToShip(item);
                    if (ship)
                    {
                        m_ships[ship->getId()] = ship;
                    }
                }
            }
            else if constexpr (std::is_same_v<T, Fleet>)
            {
                m_fleets.clear();
                for (const auto &item : json)
                {
                    auto fleet = jsonToFleet(item);
                    if (fleet)
                    {
                        m_fleets[fleet->getId()] = fleet;
                    }
                }
            }
            else if constexpr (std::is_same_v<T, TradeGood>)
            {
                m_tradeGoods.clear();
                for (const auto &item : json)
                {
                    auto tradeGood = jsonToTradeGood(item);
                    if (tradeGood)
                    {
                        m_tradeGoods[tradeGood->getId()] = tradeGood;
                    }
                }
            }
            else if constexpr (std::is_same_v<T, World>)
            {
                m_world = jsonToWorld(json);
            }
            else if constexpr (std::is_same_v<T, Player>)
            {
                m_player = jsonToPlayer(json);
            }
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Erreur lors du chargement de " << fullPath << ": " << e.what() << std::endl;
            return false;
        }
    }

    template <typename T>
    bool DataManager::saveData(const std::string &filename)
    {
        std::string fullPath = m_basePath + filename;

        try
        {
            nlohmann::json json;

            if constexpr (std::is_same_v<T, Character>)
            {
                for (const auto &[id, character] : m_characters)
                {
                    json.push_back(entityToJson(*character));
                }
            }
            else if constexpr (std::is_same_v<T, Ship>)
            {
                for (const auto &[id, ship] : m_ships)
                {
                    json.push_back(entityToJson(*ship));
                }
            }
            else if constexpr (std::is_same_v<T, Fleet>)
            {
                for (const auto &[id, fleet] : m_fleets)
                {
                    json.push_back(entityToJson(*fleet));
                }
            }
            else if constexpr (std::is_same_v<T, TradeGood>)
            {
                for (const auto &[id, tradeGood] : m_tradeGoods)
                {
                    json.push_back(entityToJson(*tradeGood));
                }
            }
            else if constexpr (std::is_same_v<T, World>)
            {
                json = entityToJson(*m_world);
            }
            else if constexpr (std::is_same_v<T, Player>)
            {
                json = entityToJson(*m_player);
            }

            std::ofstream file(fullPath);
            if (!file.is_open())
            {
                return false;
            }

            file << json.dump(4); // Indentation de 4 espaces pour lisibilité
            file.close();
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Erreur lors de la sauvegarde de " << fullPath << ": " << e.what() << std::endl;
            return false;
        }
    }

} // namespace Orenji