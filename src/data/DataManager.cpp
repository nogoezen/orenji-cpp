#include "../../include/data/DataManager.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>

namespace Orenji
{
    std::unique_ptr<DataManager> DataManager::s_instance = nullptr;

    DataManager::DataManager()
        : m_basePath("resources/data/"),
          m_world(nullptr),
          m_player(nullptr)
    {
    }

    DataManager::~DataManager()
    {
        // Sauvegarde automatique des données si nécessaire
        try
        {
            saveAllData();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Erreur lors de la sauvegarde des données à la fermeture: " << e.what() << std::endl;
        }
    }

    DataManager &DataManager::getInstance()
    {
        if (!s_instance)
        {
            s_instance = std::unique_ptr<DataManager>(new DataManager());
        }
        return *s_instance;
    }

    bool DataManager::initialize(const std::string &basePath)
    {
        m_basePath = basePath;

        // Créer le répertoire de données s'il n'existe pas
        try
        {
            std::filesystem::create_directories(basePath);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Erreur lors de la création du répertoire de données: " << e.what() << std::endl;
            return false;
        }

        // Initialiser avec des valeurs par défaut si les fichiers n'existent pas
        if (!std::filesystem::exists(basePath + "characters.json"))
        {
            m_characters.clear();
        }

        if (!std::filesystem::exists(basePath + "ships.json"))
        {
            m_ships.clear();
        }

        if (!std::filesystem::exists(basePath + "fleets.json"))
        {
            m_fleets.clear();
        }

        if (!std::filesystem::exists(basePath + "trade_goods.json"))
        {
            m_tradeGoods.clear();
        }

        if (!std::filesystem::exists(basePath + "world.json"))
        {
            m_world = std::make_shared<World>();
        }

        if (!std::filesystem::exists(basePath + "player.json"))
        {
            m_player = std::make_shared<Player>();
        }

        return true;
    }

    bool DataManager::loadAllData()
    {
        bool success = true;

        success &= loadData<Character>("characters.json");
        success &= loadData<Ship>("ships.json");
        success &= loadData<Fleet>("fleets.json");
        success &= loadData<TradeGood>("trade_goods.json");
        success &= loadData<World>("world.json");
        success &= loadData<Player>("player.json");

        return success;
    }

    bool DataManager::saveAllData()
    {
        bool success = true;

        success &= saveData<Character>("characters.json");
        success &= saveData<Ship>("ships.json");
        success &= saveData<Fleet>("fleets.json");
        success &= saveData<TradeGood>("trade_goods.json");
        success &= saveData<World>("world.json");
        success &= saveData<Player>("player.json");

        return success;
    }

    // Méthodes d'accès aux collections
    std::vector<std::shared_ptr<Character>> DataManager::getAllCharacters() const
    {
        std::vector<std::shared_ptr<Character>> result;
        result.reserve(m_characters.size());

        for (const auto &[id, character] : m_characters)
        {
            result.push_back(character);
        }

        return result;
    }

    std::vector<std::shared_ptr<Ship>> DataManager::getAllShips() const
    {
        std::vector<std::shared_ptr<Ship>> result;
        result.reserve(m_ships.size());

        for (const auto &[id, ship] : m_ships)
        {
            result.push_back(ship);
        }

        return result;
    }

    std::vector<std::shared_ptr<Fleet>> DataManager::getAllFleets() const
    {
        std::vector<std::shared_ptr<Fleet>> result;
        result.reserve(m_fleets.size());

        for (const auto &[id, fleet] : m_fleets)
        {
            result.push_back(fleet);
        }

        return result;
    }

    std::vector<std::shared_ptr<TradeGood>> DataManager::getAllTradeGoods() const
    {
        std::vector<std::shared_ptr<TradeGood>> result;
        result.reserve(m_tradeGoods.size());

        for (const auto &[id, tradeGood] : m_tradeGoods)
        {
            result.push_back(tradeGood);
        }

        return result;
    }

    World &DataManager::getWorld()
    {
        if (!m_world)
        {
            m_world = std::make_shared<World>();
        }
        return *m_world;
    }

    Player &DataManager::getPlayer()
    {
        if (!m_player)
        {
            m_player = std::make_shared<Player>();
        }
        return *m_player;
    }

    // Fonctions de génération d'ID
    int DataManager::generateNextId(const std::unordered_map<int, std::shared_ptr<Character>> &collection) const
    {
        int maxId = 0;
        for (const auto &[id, _] : collection)
        {
            maxId = std::max(maxId, id);
        }
        return maxId + 1;
    }

    int DataManager::generateNextId(const std::unordered_map<int, std::shared_ptr<Ship>> &collection) const
    {
        int maxId = 0;
        for (const auto &[id, _] : collection)
        {
            maxId = std::max(maxId, id);
        }
        return maxId + 1;
    }

    int DataManager::generateNextId(const std::unordered_map<int, std::shared_ptr<Fleet>> &collection) const
    {
        int maxId = 0;
        for (const auto &[id, _] : collection)
        {
            maxId = std::max(maxId, id);
        }
        return maxId + 1;
    }

    int DataManager::generateNextId(const std::unordered_map<int, std::shared_ptr<TradeGood>> &collection) const
    {
        int maxId = 0;
        for (const auto &[id, _] : collection)
        {
            maxId = std::max(maxId, id);
        }
        return maxId + 1;
    }

    // Fonctions de sérialisation JSON
    nlohmann::json DataManager::entityToJson(const Character &character) const
    {
        nlohmann::json json;

        json["id"] = character.getId();
        json["name"] = character.getName();

        // Ajouter tous les autres attributs de Character
        json["age"] = character.getAge();
        json["health"] = character.getHealth();
        json["maxHealth"] = character.getMaxHealth();
        json["strength"] = character.getStrength();
        json["dexterity"] = character.getDexterity();
        json["intelligence"] = character.getIntelligence();
        json["charisma"] = character.getCharisma();
        json["experience"] = character.getExperience();
        json["level"] = character.getLevel();

        // Autres propriétés selon le modèle de données

        return json;
    }

    nlohmann::json DataManager::entityToJson(const Ship &ship) const
    {
        nlohmann::json json;

        json["id"] = ship.getId();
        json["name"] = ship.getName();

        // Ajouter tous les autres attributs de Ship
        json["type"] = static_cast<int>(ship.getType());
        json["health"] = ship.getHealth();
        json["maxHealth"] = ship.getMaxHealth();
        json["speed"] = ship.getSpeed();
        json["maxSpeed"] = ship.getMaxSpeed();
        json["capacity"] = ship.getCapacity();
        json["maxCapacity"] = ship.getMaxCapacity();
        json["crewCapacity"] = ship.getCrewCapacity();
        json["maxCrewCapacity"] = ship.getMaxCrewCapacity();

        // Position
        json["position"]["x"] = ship.getPosition().x;
        json["position"]["y"] = ship.getPosition().y;

        // Rotation
        json["rotation"] = ship.getRotation();

        // Équipage
        nlohmann::json crewArray = nlohmann::json::array();
        for (const auto &crewMember : ship.getCrew())
        {
            if (crewMember)
            {
                nlohmann::json crewJson;
                crewJson["id"] = crewMember->getId();
                crewJson["role"] = static_cast<int>(crewMember->getRole());
                crewArray.push_back(crewJson);
            }
        }
        json["crew"] = crewArray;

        // Cargaison
        nlohmann::json cargoArray = nlohmann::json::array();
        for (const auto &[tradeGoodId, quantity] : ship.getCargo())
        {
            nlohmann::json cargoJson;
            cargoJson["goodId"] = tradeGoodId;
            cargoJson["quantity"] = quantity;
            cargoArray.push_back(cargoJson);
        }
        json["cargo"] = cargoArray;

        return json;
    }

    nlohmann::json DataManager::entityToJson(const Fleet &fleet) const
    {
        nlohmann::json json;

        json["id"] = fleet.getId();
        json["name"] = fleet.getName();

        // Ajouter tous les autres attributs de Fleet
        json["ownerId"] = fleet.getOwnerId();

        // Position
        json["position"]["x"] = fleet.getPosition().x;
        json["position"]["y"] = fleet.getPosition().y;

        // Navires
        nlohmann::json shipsArray = nlohmann::json::array();
        for (const auto &shipId : fleet.getShipIds())
        {
            shipsArray.push_back(shipId);
        }
        json["ships"] = shipsArray;

        return json;
    }

    nlohmann::json DataManager::entityToJson(const TradeGood &tradeGood) const
    {
        nlohmann::json json;

        json["id"] = tradeGood.getId();
        json["name"] = tradeGood.getName();

        // Ajouter tous les autres attributs de TradeGood
        json["basePrice"] = tradeGood.getBasePrice();
        json["weight"] = tradeGood.getWeight();
        json["rarity"] = static_cast<int>(tradeGood.getRarity());
        json["category"] = static_cast<int>(tradeGood.getCategory());

        return json;
    }

    nlohmann::json DataManager::entityToJson(const World &world) const
    {
        nlohmann::json json;

        // Ajouter tous les attributs du monde
        json["name"] = world.getName();
        json["width"] = world.getWidth();
        json["height"] = world.getHeight();

        // Villes
        nlohmann::json citiesArray = nlohmann::json::array();
        for (const auto &city : world.getCities())
        {
            nlohmann::json cityJson;
            cityJson["id"] = city.getId();
            cityJson["name"] = city.getName();
            cityJson["position"]["x"] = city.getPosition().x;
            cityJson["position"]["y"] = city.getPosition().y;
            cityJson["population"] = city.getPopulation();
            cityJson["wealth"] = city.getWealth();

            // Marchés
            nlohmann::json marketsArray = nlohmann::json::array();
            for (const auto &[goodId, marketData] : city.getMarket())
            {
                nlohmann::json marketJson;
                marketJson["goodId"] = goodId;
                marketJson["price"] = marketData.price;
                marketJson["supply"] = marketData.supply;
                marketJson["demand"] = marketData.demand;
                marketsArray.push_back(marketJson);
            }
            cityJson["market"] = marketsArray;

            citiesArray.push_back(cityJson);
        }
        json["cities"] = citiesArray;

        // Autres données du monde

        return json;
    }

    nlohmann::json DataManager::entityToJson(const Player &player) const
    {
        nlohmann::json json;

        json["id"] = player.getId();
        json["name"] = player.getName();

        // Ajouter tous les autres attributs du joueur
        json["money"] = player.getMoney();
        json["reputation"] = player.getReputation();
        json["experience"] = player.getExperience();
        json["level"] = player.getLevel();

        // Flottes
        nlohmann::json fleetsArray = nlohmann::json::array();
        for (const auto &fleetId : player.getFleetIds())
        {
            fleetsArray.push_back(fleetId);
        }
        json["fleets"] = fleetsArray;

        // Quêtes
        nlohmann::json questsArray = nlohmann::json::array();
        for (const auto &quest : player.getQuests())
        {
            nlohmann::json questJson;
            questJson["id"] = quest.getId();
            questJson["title"] = quest.getTitle();
            questJson["description"] = quest.getDescription();
            questJson["status"] = static_cast<int>(quest.getStatus());
            questJson["reward"] = quest.getReward();
            questsArray.push_back(questJson);
        }
        json["quests"] = questsArray;

        return json;
    }

    // Fonctions de désérialisation JSON
    std::shared_ptr<Character> DataManager::jsonToCharacter(const nlohmann::json &json) const
    {
        auto character = std::make_shared<Character>();

        character->setId(json["id"]);
        character->setName(json["name"]);

        // Récupérer tous les autres attributs
        if (json.contains("age"))
            character->setAge(json["age"]);
        if (json.contains("health"))
            character->setHealth(json["health"]);
        if (json.contains("maxHealth"))
            character->setMaxHealth(json["maxHealth"]);
        if (json.contains("strength"))
            character->setStrength(json["strength"]);
        if (json.contains("dexterity"))
            character->setDexterity(json["dexterity"]);
        if (json.contains("intelligence"))
            character->setIntelligence(json["intelligence"]);
        if (json.contains("charisma"))
            character->setCharisma(json["charisma"]);
        if (json.contains("experience"))
            character->setExperience(json["experience"]);
        if (json.contains("level"))
            character->setLevel(json["level"]);

        return character;
    }

    std::shared_ptr<Ship> DataManager::jsonToShip(const nlohmann::json &json) const
    {
        auto ship = std::make_shared<Ship>();

        ship->setId(json["id"]);
        ship->setName(json["name"]);

        // Récupérer tous les autres attributs
        if (json.contains("type"))
            ship->setType(static_cast<Ship::Type>(json["type"]));
        if (json.contains("health"))
            ship->setHealth(json["health"]);
        if (json.contains("maxHealth"))
            ship->setMaxHealth(json["maxHealth"]);
        if (json.contains("speed"))
            ship->setSpeed(json["speed"]);
        if (json.contains("maxSpeed"))
            ship->setMaxSpeed(json["maxSpeed"]);
        if (json.contains("capacity"))
            ship->setCapacity(json["capacity"]);
        if (json.contains("maxCapacity"))
            ship->setMaxCapacity(json["maxCapacity"]);
        if (json.contains("crewCapacity"))
            ship->setCrewCapacity(json["crewCapacity"]);
        if (json.contains("maxCrewCapacity"))
            ship->setMaxCrewCapacity(json["maxCrewCapacity"]);

        // Position
        if (json.contains("position"))
        {
            sf::Vector2f position;
            position.x = json["position"]["x"];
            position.y = json["position"]["y"];
            ship->setPosition(position);
        }

        // Rotation
        if (json.contains("rotation"))
            ship->setRotation(json["rotation"]);

        // Équipage (les références seront résolues après)
        if (json.contains("crew"))
        {
            for (const auto &crewJson : json["crew"])
            {
                int characterId = crewJson["id"];
                auto role = static_cast<Ship::CrewRole>(crewJson["role"]);

                // On ajoute une référence à résoudre plus tard
                // Dans une implémentation complète, il faudrait stocker ces références
                // et les résoudre après le chargement de toutes les entités
                auto character = getEntityById<Character>(characterId);
                if (character)
                {
                    ship->addCrewMember(character, role);
                }
            }
        }

        // Cargaison
        if (json.contains("cargo"))
        {
            for (const auto &cargoJson : json["cargo"])
            {
                int goodId = cargoJson["goodId"];
                int quantity = cargoJson["quantity"];

                auto tradeGood = getEntityById<TradeGood>(goodId);
                if (tradeGood)
                {
                    ship->addCargo(goodId, quantity);
                }
            }
        }

        return ship;
    }

    std::shared_ptr<Fleet> DataManager::jsonToFleet(const nlohmann::json &json) const
    {
        auto fleet = std::make_shared<Fleet>();

        fleet->setId(json["id"]);
        fleet->setName(json["name"]);

        // Récupérer tous les autres attributs
        if (json.contains("ownerId"))
            fleet->setOwnerId(json["ownerId"]);

        // Position
        if (json.contains("position"))
        {
            sf::Vector2f position;
            position.x = json["position"]["x"];
            position.y = json["position"]["y"];
            fleet->setPosition(position);
        }

        // Navires (les références seront résolues après)
        if (json.contains("ships"))
        {
            for (const auto &shipId : json["ships"])
            {
                // On ajoute une référence à résoudre plus tard
                fleet->addShip(shipId);
            }
        }

        return fleet;
    }

    std::shared_ptr<TradeGood> DataManager::jsonToTradeGood(const nlohmann::json &json) const
    {
        auto tradeGood = std::make_shared<TradeGood>();

        tradeGood->setId(json["id"]);
        tradeGood->setName(json["name"]);

        // Récupérer tous les autres attributs
        if (json.contains("basePrice"))
            tradeGood->setBasePrice(json["basePrice"]);
        if (json.contains("weight"))
            tradeGood->setWeight(json["weight"]);
        if (json.contains("rarity"))
            tradeGood->setRarity(static_cast<TradeGood::Rarity>(json["rarity"]));
        if (json.contains("category"))
            tradeGood->setCategory(static_cast<TradeGood::Category>(json["category"]));

        return tradeGood;
    }

    std::shared_ptr<World> DataManager::jsonToWorld(const nlohmann::json &json) const
    {
        auto world = std::make_shared<World>();

        // Récupérer tous les attributs du monde
        if (json.contains("name"))
            world->setName(json["name"]);
        if (json.contains("width"))
            world->setWidth(json["width"]);
        if (json.contains("height"))
            world->setHeight(json["height"]);

        // Villes
        if (json.contains("cities"))
        {
            std::vector<World::City> cities;
            for (const auto &cityJson : json["cities"])
            {
                World::City city;
                city.setId(cityJson["id"]);
                city.setName(cityJson["name"]);

                // Position
                sf::Vector2f position;
                position.x = cityJson["position"]["x"];
                position.y = cityJson["position"]["y"];
                city.setPosition(position);

                city.setPopulation(cityJson["population"]);
                city.setWealth(cityJson["wealth"]);

                // Marchés
                if (cityJson.contains("market"))
                {
                    for (const auto &marketJson : cityJson["market"])
                    {
                        int goodId = marketJson["goodId"];
                        World::MarketData marketData;
                        marketData.price = marketJson["price"];
                        marketData.supply = marketJson["supply"];
                        marketData.demand = marketJson["demand"];
                        city.addMarketData(goodId, marketData);
                    }
                }

                world->addCity(city);
            }
        }

        // Autres données du monde

        return world;
    }

    std::shared_ptr<Player> DataManager::jsonToPlayer(const nlohmann::json &json) const
    {
        auto player = std::make_shared<Player>();

        player->setId(json["id"]);
        player->setName(json["name"]);

        // Récupérer tous les autres attributs
        if (json.contains("money"))
            player->setMoney(json["money"]);
        if (json.contains("reputation"))
            player->setReputation(json["reputation"]);
        if (json.contains("experience"))
            player->setExperience(json["experience"]);
        if (json.contains("level"))
            player->setLevel(json["level"]);

        // Flottes
        if (json.contains("fleets"))
        {
            for (const auto &fleetId : json["fleets"])
            {
                player->addFleet(fleetId);
            }
        }

        // Quêtes
        if (json.contains("quests"))
        {
            for (const auto &questJson : json["quests"])
            {
                Player::Quest quest;
                quest.setId(questJson["id"]);
                quest.setTitle(questJson["title"]);
                quest.setDescription(questJson["description"]);
                quest.setStatus(static_cast<Player::QuestStatus>(questJson["status"]));
                quest.setReward(questJson["reward"]);
                player->addQuest(quest);
            }
        }

        return player;
    }

} // namespace Orenji