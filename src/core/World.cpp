#include "World.h"
#include "../data/GameData.h"
#include <iostream>
#include <cmath>

// Implémentation des méthodes de City
nlohmann::json City::toJson() const
{
    nlohmann::json j;
    j["id"] = id;
    j["name"] = name;
    j["x"] = x;
    j["y"] = y;
    j["region"] = region;
    j["description"] = description;
    j["availableGoods"] = availableGoods;
    j["availableShips"] = availableShips;
    return j;
}

City City::fromJson(const nlohmann::json &json)
{
    City city;
    city.id = json["id"];
    city.name = json["name"];
    city.x = json["x"];
    city.y = json["y"];
    city.region = json["region"];
    city.description = json.value("description", "");

    if (json.contains("availableGoods") && json["availableGoods"].is_array())
    {
        city.availableGoods = json["availableGoods"].get<std::vector<int>>();
    }

    if (json.contains("availableShips") && json["availableShips"].is_array())
    {
        city.availableShips = json["availableShips"].get<std::vector<int>>();
    }

    return city;
}

// Implémentation des méthodes de Region
nlohmann::json Region::toJson() const
{
    nlohmann::json j;
    j["id"] = id;
    j["name"] = name;
    j["description"] = description;
    j["cities"] = cities;
    return j;
}

Region Region::fromJson(const nlohmann::json &json)
{
    Region region;
    region.id = json["id"];
    region.name = json["name"];
    region.description = json.value("description", "");

    if (json.contains("cities") && json["cities"].is_array())
    {
        region.cities = json["cities"].get<std::vector<int>>();
    }

    return region;
}

// Implémentation de World
World::World()
{
    // Le chargement complet se fait dans initialize()
}

bool World::initialize(const nlohmann::json &saveData)
{
    try
    {
        // Si des données de sauvegarde sont fournies, les utiliser
        if (!saveData.is_null())
        {
            if (saveData.contains("currentCityId"))
            {
                m_currentCityId = saveData["currentCityId"];
            }

            // D'autres données de sauvegarde pourraient être chargées ici
        }

        // Charger les données du monde
        loadCities();
        loadRegions();

        // Si aucune ville actuelle n'est définie, en définir une par défaut
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

void World::loadCities()
{
    try
    {
        // Charger les données des villes depuis le fichier JSON
        const auto &citiesData = GameData::getInstance().getJsonData("cities");

        for (const auto &cityData : citiesData)
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

void World::loadRegions()
{
    try
    {
        // Pour cet exemple, nous supposons que les données des régions sont dans un fichier séparé
        // Dans un vrai jeu, cela pourrait être dans le même fichier que les villes ou dans une autre source

        // Exemple simplifié: Créer des régions basées sur les villes existantes
        std::unordered_map<std::string, Region> regionsByName;

        // Regrouper les villes par région
        for (const auto &[cityId, city] : m_cities)
        {
            if (!city.region.empty())
            {
                if (regionsByName.find(city.region) == regionsByName.end())
                {
                    // Créer une nouvelle région
                    Region newRegion;
                    newRegion.id = regionsByName.size() + 1;
                    newRegion.name = city.region;
                    newRegion.description = "Région de " + city.region;
                    regionsByName[city.region] = newRegion;
                }

                regionsByName[city.region].cities.push_back(cityId);
            }
        }

        // Stocker les régions dans la map par ID
        for (const auto &[name, region] : regionsByName)
        {
            m_regions[region.id] = region;
        }

        std::cout << "Chargement de " << m_regions.size() << " régions." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors du chargement des régions: " << e.what() << std::endl;
    }
}

nlohmann::json World::toJson() const
{
    nlohmann::json j;
    j["currentCityId"] = m_currentCityId;

    // On n'a pas besoin de sauvegarder toutes les villes et régions puisqu'elles
    // sont chargées à partir des fichiers de données. On ne sauvegarde que l'état.

    return j;
}

const City *World::getCityById(int cityId) const
{
    auto it = m_cities.find(cityId);
    return (it != m_cities.end()) ? &(it->second) : nullptr;
}

const Region *World::getRegionById(int regionId) const
{
    auto it = m_regions.find(regionId);
    return (it != m_regions.end()) ? &(it->second) : nullptr;
}

const City *World::getCurrentCity() const
{
    return getCityById(m_currentCityId);
}

bool World::setCurrentCity(int cityId)
{
    // Vérifier si la ville existe
    if (m_cities.find(cityId) == m_cities.end())
    {
        return false;
    }

    m_currentCityId = cityId;
    return true;
}

float World::calculateDistance(int fromCityId, int toCityId) const
{
    const City *fromCity = getCityById(fromCityId);
    const City *toCity = getCityById(toCityId);

    if (!fromCity || !toCity)
    {
        return -1.0f; // Erreur: ville non trouvée
    }

    // Calculer la distance euclidienne entre les deux villes
    int dx = toCity->x - fromCity->x;
    int dy = toCity->y - fromCity->y;

    return std::sqrt(dx * dx + dy * dy);
}