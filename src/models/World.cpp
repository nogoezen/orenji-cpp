#include "World.h"
#include <fstream>
#include <iostream>
#include <algorithm>

World::World() : m_initialized(false)
{
}

bool World::initialize()
{
    try
    {
        // Charger les données des villes
        std::ifstream cityFile("bin/data/cities.json");
        if (!cityFile.is_open())
        {
            std::cerr << "Impossible d'ouvrir le fichier cities.json" << std::endl;
            return false;
        }

        nlohmann::json cityJson;
        cityFile >> cityJson;

        // Parcourir les villes dans le JSON et les ajouter à notre liste
        for (const auto &city : cityJson)
        {
            City newCity;
            newCity.id = city["id"].get<int>();
            newCity.name = city["name"].get<std::string>();
            newCity.kingdom = city["kingdom"].get<std::string>();
            newCity.posX = city["position"]["x"].get<int>();
            newCity.posY = city["position"]["y"].get<int>();

            // Charger les connexions si elles existent
            if (city.contains("connections") && city["connections"].is_array())
            {
                for (const auto &connection : city["connections"])
                {
                    newCity.connections.push_back(connection.get<int>());
                }
            }

            m_cities.push_back(newCity);
        }

        m_initialized = true;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors de l'initialisation du monde: " << e.what() << std::endl;
        return false;
    }
}

nlohmann::json World::toJson() const
{
    nlohmann::json data;

    // Sauvegarder les villes
    nlohmann::json citiesArray = nlohmann::json::array();
    for (const auto &city : m_cities)
    {
        nlohmann::json cityData;
        cityData["id"] = city.id;
        cityData["name"] = city.name;
        cityData["kingdom"] = city.kingdom;
        cityData["position"]["x"] = city.posX;
        cityData["position"]["y"] = city.posY;

        // Sauvegarder les connexions
        nlohmann::json connectionsArray = nlohmann::json::array();
        for (int connectionId : city.connections)
        {
            connectionsArray.push_back(connectionId);
        }
        cityData["connections"] = connectionsArray;

        citiesArray.push_back(cityData);
    }

    data["cities"] = citiesArray;
    return data;
}

bool World::fromJson(const nlohmann::json &data)
{
    try
    {
        m_cities.clear();

        // Charger les villes si elles existent
        if (data.contains("cities") && data["cities"].is_array())
        {
            for (const auto &city : data["cities"])
            {
                City newCity;
                newCity.id = city["id"].get<int>();
                newCity.name = city["name"].get<std::string>();
                newCity.kingdom = city["kingdom"].get<std::string>();
                newCity.posX = city["position"]["x"].get<int>();
                newCity.posY = city["position"]["y"].get<int>();

                // Charger les connexions si elles existent
                if (city.contains("connections") && city["connections"].is_array())
                {
                    for (const auto &connection : city["connections"])
                    {
                        newCity.connections.push_back(connection.get<int>());
                    }
                }

                m_cities.push_back(newCity);
            }
        }

        m_initialized = true;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors du chargement du monde depuis JSON: " << e.what() << std::endl;
        return false;
    }
}

const World::City *World::getCity(int cityId) const
{
    auto it = std::find_if(m_cities.begin(), m_cities.end(),
                           [cityId](const City &city)
                           { return city.id == cityId; });

    if (it != m_cities.end())
    {
        return &(*it);
    }

    return nullptr;
}

const World::City *World::getCityAtPosition(int x, int y) const
{
    auto it = std::find_if(m_cities.begin(), m_cities.end(),
                           [x, y](const City &city)
                           {
                               return city.posX == x && city.posY == y;
                           });

    if (it != m_cities.end())
    {
        return &(*it);
    }

    return nullptr;
}

std::vector<const World::City *> World::getCitiesByKingdom(const std::string &kingdom) const
{
    std::vector<const City *> result;

    for (const auto &city : m_cities)
    {
        if (city.kingdom == kingdom)
        {
            result.push_back(&city);
        }
    }

    return result;
}