#include "Ship.h"
#include <algorithm>

Ship::Ship(const nlohmann::json &data)
    : m_id(-1), m_name("Unknown"), m_type("Unknown"),
      m_speed(0), m_maneuver(0), m_armor(0), m_cargoSpace(0), m_crewCapacity(0),
      m_cannons(0), m_sails(0), m_hull(0),
      m_currentHull(0), m_currentSails(0), m_currentCrew(0), m_usedCargo(0),
      m_purchasePrice(0), m_maintenanceCost(0),
      m_description("")
{
    if (data.contains("id") && data["id"].is_number())
    {
        m_id = data["id"];
    }

    if (data.contains("name") && data["name"].is_string())
    {
        m_name = data["name"];
    }

    if (data.contains("type") && data["type"].is_string())
    {
        m_type = data["type"];
    }

    if (data.contains("speed") && data["speed"].is_number())
    {
        m_speed = data["speed"];
    }

    if (data.contains("maneuver") && data["maneuver"].is_number())
    {
        m_maneuver = data["maneuver"];
    }

    if (data.contains("armor") && data["armor"].is_number())
    {
        m_armor = data["armor"];
    }

    if (data.contains("cargoSpace") && data["cargoSpace"].is_number())
    {
        m_cargoSpace = data["cargoSpace"];
    }

    if (data.contains("crewCapacity") && data["crewCapacity"].is_number())
    {
        m_crewCapacity = data["crewCapacity"];
    }

    if (data.contains("cannons") && data["cannons"].is_number())
    {
        m_cannons = data["cannons"];
    }

    if (data.contains("hull") && data["hull"].is_number())
    {
        m_hull = data["hull"];
    }
    else
    {
        m_hull = m_armor; // Si hull n'est pas spécifié, utiliser armor
    }

    if (data.contains("sails") && data["sails"].is_number())
    {
        m_sails = data["sails"];
    }
    else
    {
        m_sails = m_speed; // Si sails n'est pas spécifié, utiliser speed
    }

    // État actuel
    if (data.contains("currentHull") && data["currentHull"].is_number())
    {
        m_currentHull = data["currentHull"];
    }
    else
    {
        m_currentHull = m_hull; // Plein par défaut
    }

    if (data.contains("currentSails") && data["currentSails"].is_number())
    {
        m_currentSails = data["currentSails"];
    }
    else
    {
        m_currentSails = m_sails; // Plein par défaut
    }

    if (data.contains("currentCrew") && data["currentCrew"].is_number())
    {
        m_currentCrew = data["currentCrew"];
    }

    if (data.contains("usedCargo") && data["usedCargo"].is_number())
    {
        m_usedCargo = data["usedCargo"];
    }

    if (data.contains("purchasePrice") && data["purchasePrice"].is_number())
    {
        m_purchasePrice = data["purchasePrice"];
    }

    if (data.contains("maintenanceCost") && data["maintenanceCost"].is_number())
    {
        m_maintenanceCost = data["maintenanceCost"];
    }

    if (data.contains("description") && data["description"].is_string())
    {
        m_description = data["description"];
    }
    else
    {
        m_description = "A " + m_type + " ship";
    }

    // Charger les données de cargaison
    if (data.contains("cargo") && data["cargo"].is_array())
    {
        for (const auto &item : data["cargo"])
        {
            if (item.contains("id") && item.contains("name") && item.contains("quantity"))
            {
                int id = item["id"];
                std::string name = item["name"];
                int quantity = item["quantity"];

                m_cargo[id] = std::make_pair(name, quantity);
            }
        }
    }
}

nlohmann::json Ship::toJson() const
{
    nlohmann::json data;

    data["id"] = m_id;
    data["name"] = m_name;
    data["type"] = m_type;
    data["speed"] = m_speed;
    data["maneuver"] = m_maneuver;
    data["armor"] = m_armor;
    data["cargoSpace"] = m_cargoSpace;
    data["crewCapacity"] = m_crewCapacity;
    data["cannons"] = m_cannons;
    data["sails"] = m_sails;
    data["hull"] = m_hull;
    data["currentHull"] = m_currentHull;
    data["currentSails"] = m_currentSails;
    data["currentCrew"] = m_currentCrew;
    data["usedCargo"] = m_usedCargo;
    data["purchasePrice"] = m_purchasePrice;
    data["maintenanceCost"] = m_maintenanceCost;
    data["description"] = m_description;

    // Ajouter les données de cargaison
    nlohmann::json cargo = nlohmann::json::array();
    for (const auto &[id, item] : m_cargo)
    {
        nlohmann::json cargoItem;
        cargoItem["id"] = id;
        cargoItem["name"] = item.first;
        cargoItem["quantity"] = item.second;
        cargo.push_back(cargoItem);
    }
    data["cargo"] = cargo;

    return data;
}