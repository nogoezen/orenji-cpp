#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct ShipTrait {
    int id;
    std::string name;
    std::string effect;
};

struct ShipUpgrades {
    int hull = 0;
    int rigging = 0;
    int cannons = 0;
};

class Ship {
private:
    int m_id;
    std::string m_name;
    std::string m_description;
    std::string m_shipClass;
    std::string m_nationality;
    int m_maxCrew;
    int m_cargoCapacity;
    int m_maxPassengers;
    int m_speed;
    int m_maneuverability;
    int m_durability;
    int m_combatPower;
    std::vector<ShipTrait> m_traits;
    int m_weaponSlots;
    int m_cannonCapacity;
    std::vector<std::string> m_specializations;
    int m_maintenanceCost;
    int m_requiredCrew;
    ShipUpgrades m_upgrades;

public:
    // Constructeur
    Ship(const nlohmann::json& shipData) {
        m_id = shipData["id"].get<int>();
        m_name = shipData["name"].get<std::string>();
        m_description = shipData["description"].get<std::string>();
        m_shipClass = shipData["shipClass"].get<std::string>();
        m_nationality = shipData["nationality"].get<std::string>();
        m_maxCrew = shipData["maxCrew"].get<int>();
        m_cargoCapacity = shipData["cargoCapacity"].get<int>();
        m_maxPassengers = shipData["maxPassengers"].get<int>();
        m_speed = shipData["speed"].get<int>();
        m_maneuverability = shipData["maneuverability"].get<int>();
        m_durability = shipData["durability"].get<int>();
        m_combatPower = shipData["combatPower"].get<int>();
        
        // Charger les traits
        for (const auto& trait : shipData["traits"]) {
            m_traits.push_back({
                trait["id"].get<int>(),
                trait["name"].get<std::string>(),
                trait["effect"].get<std::string>()
            });
        }
        
        m_weaponSlots = shipData["weaponSlots"].get<int>();
        m_cannonCapacity = shipData["cannonCapacity"].get<int>();
        
        // Charger les spécialisations
        for (const auto& spec : shipData["specializations"]) {
            m_specializations.push_back(spec.get<std::string>());
        }
        
        m_maintenanceCost = shipData["maintenanceCost"].get<int>();
        m_requiredCrew = shipData["requiredCrew"].get<int>();
        
        // Charger les upgrades
        m_upgrades.hull = shipData["upgrades"]["hull"].get<int>();
        m_upgrades.rigging = shipData["upgrades"]["rigging"].get<int>();
        m_upgrades.cannons = shipData["upgrades"]["cannons"].get<int>();
    }
    
    // Getters
    int getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    const std::string& getDescription() const { return m_description; }
    const std::string& getShipClass() const { return m_shipClass; }
    const std::string& getNationality() const { return m_nationality; }
    int getMaxCrew() const { return m_maxCrew; }
    int getCargoCapacity() const { return m_cargoCapacity; }
    int getMaxPassengers() const { return m_maxPassengers; }
    int getSpeed() const { return m_speed; }
    int getManeuverability() const { return m_maneuverability; }
    int getDurability() const { return m_durability; }
    int getCombatPower() const { return m_combatPower; }
    const std::vector<ShipTrait>& getTraits() const { return m_traits; }
    int getWeaponSlots() const { return m_weaponSlots; }
    int getCannonCapacity() const { return m_cannonCapacity; }
    const std::vector<std::string>& getSpecializations() const { return m_specializations; }
    int getMaintenanceCost() const { return m_maintenanceCost; }
    int getRequiredCrew() const { return m_requiredCrew; }
    const ShipUpgrades& getUpgrades() const { return m_upgrades; }
}; 