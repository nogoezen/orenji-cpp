#include "Ship.h"
#include "../utils/MathUtils.h"
#include <cmath>
#include <algorithm>

namespace Orenji
{
    Ship::Ship()
        : m_id(-1), m_name(""), m_type(ShipType::SMALL_MERCHANT), m_description(""), m_level(1), m_maxHealth(100), m_currentHealth(100), m_x(0.0f), m_y(0.0f), m_rotation(0.0f), m_speed(0.0f), m_maxSpeed(10.0f), m_acceleration(2.0f), m_turnRate(90.0f), m_attackPower(10), m_defense(5), m_range(100), m_accuracy(70), m_maxCrewSize(10), m_currentCrewSize(5), m_cargoCapacity(100.0f), m_currentCargoWeight(0.0f), m_ownerId(-1), m_faction(""), m_cannonSlots(2), m_installedCannons(0), m_fuelCapacity(1000.0f), m_currentFuel(1000.0f), m_fuelConsumptionRate(0.5f), m_isDestroyed(false), m_isAnchored(false)
    {
    }

    Ship::Ship(int id, const std::string &name, ShipType type)
        : m_id(id), m_name(name), m_type(type), m_description(""), m_level(1), m_x(0.0f), m_y(0.0f), m_rotation(0.0f), m_speed(0.0f), m_ownerId(-1), m_faction(""), m_isDestroyed(false), m_isAnchored(false)
    {
        // Définir les attributs en fonction du type de navire
        switch (type)
        {
        case ShipType::SMALL_MERCHANT:
            m_maxHealth = 100;
            m_currentHealth = 100;
            m_maxSpeed = 8.0f;
            m_acceleration = 1.5f;
            m_turnRate = 70.0f;
            m_attackPower = 5;
            m_defense = 3;
            m_range = 80;
            m_accuracy = 60;
            m_maxCrewSize = 15;
            m_currentCrewSize = 10;
            m_cargoCapacity = 200.0f;
            m_currentCargoWeight = 0.0f;
            m_cannonSlots = 2;
            m_installedCannons = 1;
            m_fuelCapacity = 800.0f;
            m_currentFuel = 800.0f;
            m_fuelConsumptionRate = 0.3f;
            break;

        case ShipType::LARGE_MERCHANT:
            m_maxHealth = 200;
            m_currentHealth = 200;
            m_maxSpeed = 6.0f;
            m_acceleration = 1.0f;
            m_turnRate = 50.0f;
            m_attackPower = 10;
            m_defense = 8;
            m_range = 90;
            m_accuracy = 55;
            m_maxCrewSize = 30;
            m_currentCrewSize = 20;
            m_cargoCapacity = 500.0f;
            m_currentCargoWeight = 0.0f;
            m_cannonSlots = 4;
            m_installedCannons = 2;
            m_fuelCapacity = 1500.0f;
            m_currentFuel = 1500.0f;
            m_fuelConsumptionRate = 0.6f;
            break;

        case ShipType::WARSHIP:
            m_maxHealth = 300;
            m_currentHealth = 300;
            m_maxSpeed = 7.5f;
            m_acceleration = 1.8f;
            m_turnRate = 65.0f;
            m_attackPower = 30;
            m_defense = 20;
            m_range = 150;
            m_accuracy = 80;
            m_maxCrewSize = 40;
            m_currentCrewSize = 30;
            m_cargoCapacity = 150.0f;
            m_currentCargoWeight = 0.0f;
            m_cannonSlots = 10;
            m_installedCannons = 8;
            m_fuelCapacity = 1200.0f;
            m_currentFuel = 1200.0f;
            m_fuelConsumptionRate = 0.8f;
            break;

        case ShipType::PATROL_BOAT:
            m_maxHealth = 80;
            m_currentHealth = 80;
            m_maxSpeed = 12.0f;
            m_acceleration = 3.0f;
            m_turnRate = 120.0f;
            m_attackPower = 15;
            m_defense = 5;
            m_range = 100;
            m_accuracy = 75;
            m_maxCrewSize = 10;
            m_currentCrewSize = 8;
            m_cargoCapacity = 50.0f;
            m_currentCargoWeight = 0.0f;
            m_cannonSlots = 3;
            m_installedCannons = 2;
            m_fuelCapacity = 500.0f;
            m_currentFuel = 500.0f;
            m_fuelConsumptionRate = 0.4f;
            break;

        case ShipType::PIRATE_SHIP:
            m_maxHealth = 150;
            m_currentHealth = 150;
            m_maxSpeed = 10.0f;
            m_acceleration = 2.2f;
            m_turnRate = 80.0f;
            m_attackPower = 25;
            m_defense = 12;
            m_range = 120;
            m_accuracy = 70;
            m_maxCrewSize = 25;
            m_currentCrewSize = 20;
            m_cargoCapacity = 180.0f;
            m_currentCargoWeight = 0.0f;
            m_cannonSlots = 6;
            m_installedCannons = 5;
            m_fuelCapacity = 900.0f;
            m_currentFuel = 900.0f;
            m_fuelConsumptionRate = 0.7f;
            break;

        case ShipType::ROYAL_SHIP:
            m_maxHealth = 400;
            m_currentHealth = 400;
            m_maxSpeed = 8.0f;
            m_acceleration = 1.5f;
            m_turnRate = 60.0f;
            m_attackPower = 35;
            m_defense = 25;
            m_range = 180;
            m_accuracy = 85;
            m_maxCrewSize = 50;
            m_currentCrewSize = 40;
            m_cargoCapacity = 300.0f;
            m_currentCargoWeight = 0.0f;
            m_cannonSlots = 12;
            m_installedCannons = 10;
            m_fuelCapacity = 2000.0f;
            m_currentFuel = 2000.0f;
            m_fuelConsumptionRate = 1.0f;
            break;

        case ShipType::CUSTOM:
        default:
            m_maxHealth = 150;
            m_currentHealth = 150;
            m_maxSpeed = 9.0f;
            m_acceleration = 2.0f;
            m_turnRate = 75.0f;
            m_attackPower = 15;
            m_defense = 10;
            m_range = 100;
            m_accuracy = 70;
            m_maxCrewSize = 20;
            m_currentCrewSize = 15;
            m_cargoCapacity = 150.0f;
            m_currentCargoWeight = 0.0f;
            m_cannonSlots = 5;
            m_installedCannons = 3;
            m_fuelCapacity = 1000.0f;
            m_currentFuel = 1000.0f;
            m_fuelConsumptionRate = 0.5f;
            break;
        }
    }

    Ship::Ship(const nlohmann::json &json)
        : m_id(-1), m_name(""), m_type(ShipType::SMALL_MERCHANT), m_description(""), m_level(1), m_maxHealth(100), m_currentHealth(100), m_x(0.0f), m_y(0.0f), m_rotation(0.0f), m_speed(0.0f), m_maxSpeed(10.0f), m_acceleration(2.0f), m_turnRate(90.0f), m_attackPower(10), m_defense(5), m_range(100), m_accuracy(70), m_maxCrewSize(10), m_currentCrewSize(5), m_cargoCapacity(100.0f), m_currentCargoWeight(0.0f), m_ownerId(-1), m_faction(""), m_cannonSlots(2), m_installedCannons(0), m_fuelCapacity(1000.0f), m_currentFuel(1000.0f), m_fuelConsumptionRate(0.5f), m_isDestroyed(false), m_isAnchored(false)
    {
        // Charger les propriétés de base à partir du JSON
        if (json.contains("id") && json["id"].is_number())
            m_id = json["id"];

        if (json.contains("name") && json["name"].is_string())
            m_name = json["name"];

        if (json.contains("type") && json["type"].is_string())
            m_type = stringToShipType(json["type"]);

        if (json.contains("description") && json["description"].is_string())
            m_description = json["description"];

        if (json.contains("level") && json["level"].is_number())
            m_level = json["level"];

        if (json.contains("maxHealth") && json["maxHealth"].is_number())
            m_maxHealth = json["maxHealth"];

        if (json.contains("currentHealth") && json["currentHealth"].is_number())
            m_currentHealth = json["currentHealth"];

        if (json.contains("x") && json["x"].is_number())
            m_x = json["x"];

        if (json.contains("y") && json["y"].is_number())
            m_y = json["y"];

        if (json.contains("rotation") && json["rotation"].is_number())
            m_rotation = json["rotation"];

        if (json.contains("speed") && json["speed"].is_number())
            m_speed = json["speed"];

        if (json.contains("maxSpeed") && json["maxSpeed"].is_number())
            m_maxSpeed = json["maxSpeed"];

        if (json.contains("acceleration") && json["acceleration"].is_number())
            m_acceleration = json["acceleration"];

        if (json.contains("turnRate") && json["turnRate"].is_number())
            m_turnRate = json["turnRate"];

        if (json.contains("attackPower") && json["attackPower"].is_number())
            m_attackPower = json["attackPower"];

        if (json.contains("defense") && json["defense"].is_number())
            m_defense = json["defense"];

        if (json.contains("range") && json["range"].is_number())
            m_range = json["range"];

        if (json.contains("accuracy") && json["accuracy"].is_number())
            m_accuracy = json["accuracy"];

        if (json.contains("maxCrewSize") && json["maxCrewSize"].is_number())
            m_maxCrewSize = json["maxCrewSize"];

        if (json.contains("currentCrewSize") && json["currentCrewSize"].is_number())
            m_currentCrewSize = json["currentCrewSize"];

        if (json.contains("cargoCapacity") && json["cargoCapacity"].is_number())
            m_cargoCapacity = json["cargoCapacity"];

        if (json.contains("ownerId") && json["ownerId"].is_number())
            m_ownerId = json["ownerId"];

        if (json.contains("faction") && json["faction"].is_string())
            m_faction = json["faction"];

        if (json.contains("cannonSlots") && json["cannonSlots"].is_number())
            m_cannonSlots = json["cannonSlots"];

        if (json.contains("installedCannons") && json["installedCannons"].is_number())
            m_installedCannons = json["installedCannons"];

        if (json.contains("fuelCapacity") && json["fuelCapacity"].is_number())
            m_fuelCapacity = json["fuelCapacity"];

        if (json.contains("currentFuel") && json["currentFuel"].is_number())
            m_currentFuel = json["currentFuel"];

        if (json.contains("fuelConsumptionRate") && json["fuelConsumptionRate"].is_number())
            m_fuelConsumptionRate = json["fuelConsumptionRate"];

        if (json.contains("isDestroyed") && json["isDestroyed"].is_boolean())
            m_isDestroyed = json["isDestroyed"];

        if (json.contains("isAnchored") && json["isAnchored"].is_boolean())
            m_isAnchored = json["isAnchored"];

        // Charger les effets actifs
        if (json.contains("activeEffects") && json["activeEffects"].is_array())
        {
            for (const auto &effect : json["activeEffects"])
            {
                if (effect.is_string())
                    m_activeEffects.push_back(effect);
            }
        }

        // Charger le cargo
        if (json.contains("cargo") && json["cargo"].is_object())
        {
            for (auto it = json["cargo"].begin(); it != json["cargo"].end(); ++it)
            {
                int goodId = std::stoi(it.key());
                int quantity = it.value();
                m_cargo[goodId] = quantity;
            }
        }

        // Calculer le poids actuel du cargo
        if (json.contains("currentCargoWeight") && json["currentCargoWeight"].is_number())
        {
            m_currentCargoWeight = json["currentCargoWeight"];
        }
    }

    nlohmann::json Ship::toJson() const
    {
        nlohmann::json json;

        // Propriétés de base
        json["id"] = m_id;
        json["name"] = m_name;
        json["type"] = shipTypeToString(m_type);
        json["description"] = m_description;
        json["level"] = m_level;
        json["maxHealth"] = m_maxHealth;
        json["currentHealth"] = m_currentHealth;
        json["x"] = m_x;
        json["y"] = m_y;
        json["rotation"] = m_rotation;
        json["speed"] = m_speed;
        json["maxSpeed"] = m_maxSpeed;
        json["acceleration"] = m_acceleration;
        json["turnRate"] = m_turnRate;
        json["attackPower"] = m_attackPower;
        json["defense"] = m_defense;
        json["range"] = m_range;
        json["accuracy"] = m_accuracy;
        json["maxCrewSize"] = m_maxCrewSize;
        json["currentCrewSize"] = m_currentCrewSize;
        json["cargoCapacity"] = m_cargoCapacity;
        json["currentCargoWeight"] = m_currentCargoWeight;
        json["ownerId"] = m_ownerId;
        json["faction"] = m_faction;
        json["cannonSlots"] = m_cannonSlots;
        json["installedCannons"] = m_installedCannons;
        json["fuelCapacity"] = m_fuelCapacity;
        json["currentFuel"] = m_currentFuel;
        json["fuelConsumptionRate"] = m_fuelConsumptionRate;
        json["isDestroyed"] = m_isDestroyed;
        json["isAnchored"] = m_isAnchored;

        // Effets actifs
        nlohmann::json effects = nlohmann::json::array();
        for (const auto &effect : m_activeEffects)
        {
            effects.push_back(effect);
        }
        json["activeEffects"] = effects;

        // Cargo
        nlohmann::json cargo = nlohmann::json::object();
        for (const auto &item : m_cargo)
        {
            cargo[std::to_string(item.first)] = item.second;
        }
        json["cargo"] = cargo;

        return json;
    }

    void Ship::addEffect(const std::string &effect)
    {
        // Vérifier si l'effet existe déjà
        if (std::find(m_activeEffects.begin(), m_activeEffects.end(), effect) == m_activeEffects.end())
        {
            m_activeEffects.push_back(effect);
        }
    }

    bool Ship::removeEffect(const std::string &effect)
    {
        auto it = std::find(m_activeEffects.begin(), m_activeEffects.end(), effect);
        if (it != m_activeEffects.end())
        {
            m_activeEffects.erase(it);
            return true;
        }
        return false;
    }

    bool Ship::hasEffect(const std::string &effect) const
    {
        return std::find(m_activeEffects.begin(), m_activeEffects.end(), effect) != m_activeEffects.end();
    }

    void Ship::clearEffects()
    {
        m_activeEffects.clear();
    }

    bool Ship::addCargo(int goodId, int quantity, float goodWeight)
    {
        // Vérifier que la quantité est positive
        if (quantity <= 0)
            return false;

        // Calculer le poids total à ajouter
        float totalWeight = goodWeight * quantity;

        // Vérifier si l'ajout dépasse la capacité
        if (m_currentCargoWeight + totalWeight > m_cargoCapacity)
            return false;

        // Ajouter la marchandise
        m_cargo[goodId] += quantity;
        m_currentCargoWeight += totalWeight;

        return true;
    }

    bool Ship::removeCargo(int goodId, int quantity, float goodWeight)
    {
        // Vérifier que la quantité est positive
        if (quantity <= 0)
            return false;

        // Vérifier si la marchandise existe et en quantité suffisante
        auto it = m_cargo.find(goodId);
        if (it == m_cargo.end() || it->second < quantity)
            return false;

        // Retirer la marchandise
        it->second -= quantity;
        m_currentCargoWeight -= goodWeight * quantity;

        // Si la quantité devient nulle, supprimer l'entrée
        if (it->second == 0)
            m_cargo.erase(it);

        return true;
    }

    bool Ship::hasCargo(int goodId) const
    {
        return m_cargo.find(goodId) != m_cargo.end();
    }

    int Ship::getCargoQuantity(int goodId) const
    {
        auto it = m_cargo.find(goodId);
        if (it != m_cargo.end())
            return it->second;
        return 0;
    }

    void Ship::clearCargo()
    {
        m_cargo.clear();
        m_currentCargoWeight = 0.0f;
    }

    bool Ship::takeDamage(int damage)
    {
        // Appliquer la défense pour réduire les dégâts
        int actualDamage = std::max(1, damage - m_defense / 2);

        // Réduire les points de vie
        m_currentHealth -= actualDamage;

        // Vérifier si le navire est détruit
        if (m_currentHealth <= 0)
        {
            m_currentHealth = 0;
            m_isDestroyed = true;
        }

        return m_isDestroyed;
    }

    int Ship::repair(int amount)
    {
        // Vérifier si le navire est détruit
        if (m_isDestroyed)
            return 0;

        // Calculer les points de vie à restaurer
        int oldHealth = m_currentHealth;
        m_currentHealth = std::min(m_maxHealth, m_currentHealth + amount);

        // Retourner le nombre de points effectivement restaurés
        return m_currentHealth - oldHealth;
    }

    void Ship::updatePosition(float deltaTime)
    {
        // Ne pas mettre à jour si le navire est ancré ou détruit
        if (m_isAnchored || m_isDestroyed)
            return;

        // Convertir la rotation en radians pour le calcul
        float radians = m_rotation * (3.14159f / 180.0f);

        // Calculer les composantes de la vitesse
        float vx = m_speed * std::cos(radians);
        float vy = m_speed * std::sin(radians);

        // Mettre à jour la position
        m_x += vx * deltaTime;
        m_y += vy * deltaTime;

        // Consommer du carburant
        consumeFuel(deltaTime);
    }

    void Ship::accelerate(float deltaTime, float acceleration)
    {
        // Ne pas accélérer si le navire est ancré ou détruit
        if (m_isAnchored || m_isDestroyed)
            return;

        // Appliquer l'accélération
        float accelerationValue = m_acceleration * acceleration;
        m_speed += accelerationValue * deltaTime;

        // Limiter la vitesse
        m_speed = std::max(0.0f, std::min(m_speed, m_maxSpeed));
    }

    void Ship::turn(float deltaTime, float direction)
    {
        // Ne pas tourner si le navire est ancré ou détruit
        if (m_isAnchored || m_isDestroyed)
            return;

        // Appliquer la rotation
        m_rotation += m_turnRate * direction * deltaTime;

        // Normaliser la rotation entre 0 et 360 degrés
        while (m_rotation < 0.0f)
            m_rotation += 360.0f;
        while (m_rotation >= 360.0f)
            m_rotation -= 360.0f;
    }

    float Ship::consumeFuel(float deltaTime)
    {
        // Calculer la consommation en fonction de la vitesse
        float consumption = m_fuelConsumptionRate * (m_speed / m_maxSpeed) * deltaTime;

        // Assurer que la consommation n'est pas négative
        consumption = std::max(0.0f, consumption);

        // Limiter la consommation au carburant disponible
        consumption = std::min(consumption, m_currentFuel);

        // Réduire le carburant
        m_currentFuel -= consumption;

        // Si le carburant est épuisé, arrêter le navire
        if (m_currentFuel <= 0.0f)
        {
            m_currentFuel = 0.0f;
            m_speed = 0.0f;
        }

        return consumption;
    }

    float Ship::refuel(float amount)
    {
        // Calculer la quantité effective à ajouter
        float added = std::min(amount, m_fuelCapacity - m_currentFuel);

        // Ajouter le carburant
        m_currentFuel += added;

        return added;
    }

    float Ship::distanceTo(float x, float y) const
    {
        // Calculer la distance euclidienne
        float dx = x - m_x;
        float dy = y - m_y;
        return std::sqrt(dx * dx + dy * dy);
    }

    float Ship::distanceTo(const Ship &other) const
    {
        return distanceTo(other.getX(), other.getY());
    }

    bool Ship::isInRange(float x, float y) const
    {
        return distanceTo(x, y) <= m_range;
    }

    bool Ship::isInRange(const Ship &other) const
    {
        return distanceTo(other) <= m_range;
    }

    ShipType Ship::stringToShipType(const std::string &typeStr)
    {
        if (typeStr == "SMALL_MERCHANT")
            return ShipType::SMALL_MERCHANT;
        else if (typeStr == "LARGE_MERCHANT")
            return ShipType::LARGE_MERCHANT;
        else if (typeStr == "WARSHIP")
            return ShipType::WARSHIP;
        else if (typeStr == "PATROL_BOAT")
            return ShipType::PATROL_BOAT;
        else if (typeStr == "PIRATE_SHIP")
            return ShipType::PIRATE_SHIP;
        else if (typeStr == "ROYAL_SHIP")
            return ShipType::ROYAL_SHIP;
        else if (typeStr == "CUSTOM")
            return ShipType::CUSTOM;
        else
            return ShipType::SMALL_MERCHANT;
    }

    std::string Ship::shipTypeToString(ShipType type)
    {
        switch (type)
        {
        case ShipType::SMALL_MERCHANT:
            return "SMALL_MERCHANT";
        case ShipType::LARGE_MERCHANT:
            return "LARGE_MERCHANT";
        case ShipType::WARSHIP:
            return "WARSHIP";
        case ShipType::PATROL_BOAT:
            return "PATROL_BOAT";
        case ShipType::PIRATE_SHIP:
            return "PIRATE_SHIP";
        case ShipType::ROYAL_SHIP:
            return "ROYAL_SHIP";
        case ShipType::CUSTOM:
            return "CUSTOM";
        default:
            return "SMALL_MERCHANT";
        }
    }

} // namespace Orenji