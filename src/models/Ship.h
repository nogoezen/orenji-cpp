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
        m_id = shipData["id"];
        m_name = shipData["name"];
        m_description = shipData["description"];
        m_shipClass = shipData["shipClass"];
        m_nationality = shipData["nationality"];
        m_maxCrew = shipData["maxCrew"];
        m_cargoCapacity = shipData["cargoCapacity"];
        m_maxPassengers = shipData["maxPassengers"];
        m_speed = shipData["speed"];
        m_maneuverability = shipData["maneuverability"];
        m_durability = shipData["durability"];
        m_combatPower = shipData["combatPower"];
        
        // Charger les traits
        for (const auto& trait : shipData["traits"]) {
            m_traits.push_back({
                trait["id"],
                trait["name"],
                trait["effect"]
            });
        }
        
        m_weaponSlots = shipData["weaponSlots"];
        m_cannonCapacity = shipData["cannonCapacity"];
        
        // Charger les spécialisations
        for (const auto& spec : shipData["specializations"]) {
            m_specializations.push_back(spec);
        }
        
        m_maintenanceCost = shipData["maintenanceCost"];
        m_requiredCrew = shipData["requiredCrew"];
        
        // Charger les upgrades
        m_upgrades.hull = shipData["upgrades"]["hull"];
        m_upgrades.rigging = shipData["upgrades"]["rigging"];
        m_upgrades.cannons = shipData["upgrades"]["cannons"];
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