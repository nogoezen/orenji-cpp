#include "World.h"
#include "../utils/JsonLoader.h"
#include "../data/GameData.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>

// Implémentation de City::toJson
nlohmann::json City::toJson() const
{
    nlohmann::json j;
    j["id"] = id;
    j["name"] = name;
    j["x"] = x;
    j["y"] = y;
    j["region"] = region;
    j["description"] = description;
    j["population"] = population;

    nlohmann::json goods = nlohmann::json::array();
    for (int goodId : availableGoods)
    {
        goods.push_back(goodId);
    }
    j["availableGoods"] = goods;

    nlohmann::json ships = nlohmann::json::array();
    for (int shipId : availableShips)
    {
        ships.push_back(shipId);
    }
    j["availableShips"] = ships;

    return j;
}

// Implémentation de City::fromJson
City City::fromJson(const nlohmann::json &json)
{
    City city;
    city.id = json["id"];
    city.name = json["name"];
    city.x = json["x"];
    city.y = json["y"];
    city.region = json["region"];
    city.description = json.value("description", "");

    if (json.contains("population"))
    {
        city.population = json["population"];
    }

    if (json.contains("availableGoods") && json["availableGoods"].is_array())
    {
        for (const auto &good : json["availableGoods"])
        {
            city.availableGoods.push_back(good);
        }
    }

    if (json.contains("availableShips") && json["availableShips"].is_array())
    {
        for (const auto &ship : json["availableShips"])
        {
            city.availableShips.push_back(ship);
        }
    }

    return city;
}

void City::updateFromJson(const nlohmann::json &json)
{
    // Ne mettre à jour que les champs présents dans le JSON
    if (json.contains("name") && json["name"].is_string())
    {
        name = json["name"];
    }

    if (json.contains("x") && json["x"].is_number())
    {
        x = json["x"];
    }

    if (json.contains("y") && json["y"].is_number())
    {
        y = json["y"];
    }

    if (json.contains("region") && json["region"].is_string())
    {
        region = json["region"];
    }

    if (json.contains("description") && json["description"].is_string())
    {
        description = json["description"];
    }

    if (json.contains("population") && json["population"].is_number())
    {
        population = json["population"];
    }

    if (json.contains("availableGoods") && json["availableGoods"].is_array())
    {
        availableGoods.clear();
        for (const auto &good : json["availableGoods"])
        {
            availableGoods.push_back(good);
        }
    }

    if (json.contains("availableShips") && json["availableShips"].is_array())
    {
        availableShips.clear();
        for (const auto &ship : json["availableShips"])
        {
            availableShips.push_back(ship);
        }
    }
}

// Implémentation de Region::toJson
nlohmann::json Region::toJson() const
{
    nlohmann::json j;
    j["id"] = id;
    j["name"] = name;
    j["description"] = description;

    nlohmann::json citiesArray = nlohmann::json::array();
    for (int cityId : cities)
    {
        citiesArray.push_back(cityId);
    }
    j["cities"] = citiesArray;

    return j;
}

// Implémentation de Region::fromJson
Region Region::fromJson(const nlohmann::json &json)
{
    Region region;
    region.id = json["id"];
    region.name = json["name"];

    if (json.contains("description"))
        region.description = json["description"];

    if (json.contains("cities") && json["cities"].is_array())
    {
        for (const auto &cityId : json["cities"])
        {
            region.cities.push_back(cityId);
        }
    }

    return region;
}

CoreWorld::CoreWorld()
{
    // L'initialisation complète se fait dans initialize()
}

bool CoreWorld::initialize()
{
    try
    {
        // Charger les données des villes et des régions
        loadCities();
        loadRegions();

        // Si aucune ville active n'est définie, choisir la première disponible
        if (m_currentCityId == -1 && !m_cities.empty())
        {
            m_currentCityId = m_cities.begin()->first;
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors de l'initialisation du monde: " << e.what() << std::endl;
        return false;
    }
}

bool CoreWorld::initialize(const nlohmann::json &data)
{
    try
    {
        // D'abord initialiser normalement
        if (!initialize())
        {
            return false;
        }

        // Ensuite charger les données spécifiques depuis le JSON
        if (data.contains("currentCityId") && data["currentCityId"].is_number())
        {
            m_currentCityId = data["currentCityId"];
        }

        // Mise à jour des villes si présentes dans les données
        if (data.contains("cities") && data["cities"].is_array())
        {
            for (const auto &cityData : data["cities"])
            {
                if (cityData.contains("id") && cityData["id"].is_number())
                {
                    int cityId = cityData["id"];
                    auto it = m_cities.find(cityId);
                    if (it != m_cities.end())
                    {
                        // Mettre à jour la ville existante
                        it->second.updateFromJson(cityData);
                    }
                }
            }
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors de l'initialisation du monde depuis JSON: " << e.what() << std::endl;
        return false;
    }
}

void CoreWorld::loadCities()
{
    try
    {
        // Charger les données des villes depuis le fichier JSON
        auto citiesJson = JsonLoader::loadJsonFile("data/cities.json");

        for (const auto &cityData : citiesJson)
        {
            City city = City::fromJson(cityData);
            m_cities[city.id] = city;
        }

        std::cout << "Chargement de " << m_cities.size() << " villes." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors du chargement des villes: " << e.what() << std::endl;
    }
}

void CoreWorld::loadRegions()
{
    try
    {
        // Créer d'abord un mapping des villes par région
        std::unordered_map<std::string, std::vector<int>> citiesByRegion;

        for (const auto &[cityId, city] : m_cities)
        {
            citiesByRegion[city.region].push_back(cityId);
        }

        // Créer les régions
        int regionId = 1;
        for (const auto &[regionName, cityIds] : citiesByRegion)
        {
            Region region;
            region.id = regionId++;
            region.name = regionName;
            region.description = "Région de " + regionName;
            region.cities = cityIds;

            m_regions[region.id] = region;
        }

        std::cout << "Chargement de " << m_regions.size() << " régions." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors du chargement des régions: " << e.what() << std::endl;
    }
}

nlohmann::json CoreWorld::toJson() const
{
    nlohmann::json j;

    j["currentCityId"] = m_currentCityId;

    return j;
}

const City *CoreWorld::getCityById(int cityId) const
{
    auto it = m_cities.find(cityId);
    return (it != m_cities.end()) ? &it->second : nullptr;
}

const Region *CoreWorld::getRegionById(int regionId) const
{
    auto it = m_regions.find(regionId);
    return (it != m_regions.end()) ? &it->second : nullptr;
}

const City *CoreWorld::getCurrentCity() const
{
    return getCityById(m_currentCityId);
}

bool CoreWorld::setCurrentCity(int cityId)
{
    // Vérifier que la ville existe
    if (m_cities.find(cityId) == m_cities.end())
    {
        return false;
    }

    m_currentCityId = cityId;
    return true;
}

float CoreWorld::calculateDistance(int fromCityId, int toCityId) const
{
    const City *fromCity = getCityById(fromCityId);
    const City *toCity = getCityById(toCityId);

    if (!fromCity || !toCity)
    {
        return -1.0f; // Erreur : ville(s) non trouvée(s)
    }

    // Calcul de la distance euclidienne entre les deux villes
    float deltaX = static_cast<float>(toCity->x - fromCity->x);
    float deltaY = static_cast<float>(toCity->y - fromCity->y);

    return std::sqrt(deltaX * deltaX + deltaY * deltaY);
}

const std::unordered_map<int, std::shared_ptr<TradeGood>> CoreWorld::getAllTradeGoods() const
{
    std::unordered_map<int, std::shared_ptr<TradeGood>> tradeGoods;
    const auto &gameData = GameData::getInstance();
    const auto &allTradeGoods = gameData.getTradeGoods();

    // Convertir de unordered_map<int, TradeGood> à unordered_map<int, shared_ptr<TradeGood>>
    for (const auto &[id, good] : allTradeGoods)
    {
        tradeGoods[id] = std::make_shared<TradeGood>(good);
    }

    return tradeGoods;
}