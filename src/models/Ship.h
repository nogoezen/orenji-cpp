#pragma once

#include <string>
#include <vector>
#include <map>
#include "../../include/nlohmann/json.hpp"

struct ShipTrait
{
    int id;
    std::string name;
    std::string effect;
};

struct ShipUpgrades
{
    int hull = 0;
    int rigging = 0;
    int cannons = 0;
};

// Structure pour représenter la cargaison
struct Cargo
{
    int itemId;
    std::string name;
    int quantity;
    int unitWeight;
};

class Ship
{
private:
    int m_id;
    std::string m_name;
    std::string m_description;
    std::string m_shipClass;
    std::string m_nationality;
    int m_maxCrew;
    int m_currentCrew; // Équipage actuel
    int m_cargoCapacity;
    int m_usedCargo; // Espace utilisé
    int m_maxPassengers;
    int m_currentPassengers; // Passagers actuels
    int m_speed;
    int m_maneuverability;
    int m_durability;
    int m_currentDurability; // Durabilité actuelle (état du navire)
    int m_combatPower;
    std::vector<ShipTrait> m_traits;
    int m_weaponSlots;
    int m_cannonCapacity;
    std::vector<std::string> m_specializations;
    int m_maintenanceCost;
    int m_requiredCrew;
    ShipUpgrades m_upgrades;

    // Cargaison du navire
    std::vector<Cargo> m_cargo;

public:
    // Constructeur depuis JSON
    Ship(const nlohmann::json &shipData)
    {
        m_id = shipData["id"].get<int>();
        m_name = shipData["name"].get<std::string>();
        m_description = shipData["description"].get<std::string>();
        m_shipClass = shipData["shipClass"].get<std::string>();
        m_nationality = shipData["nationality"].get<std::string>();
        m_maxCrew = shipData["maxCrew"].get<int>();
        m_currentCrew = shipData["currentCrew"].get<int>();
        m_cargoCapacity = shipData["cargoCapacity"].get<int>();
        m_usedCargo = shipData["usedCargo"].get<int>();
        m_maxPassengers = shipData["maxPassengers"].get<int>();
        m_currentPassengers = shipData["currentPassengers"].get<int>();
        m_speed = shipData["speed"].get<int>();
        m_maneuverability = shipData["maneuverability"].get<int>();
        m_durability = shipData["durability"].get<int>();
        m_currentDurability = shipData["currentDurability"].get<int>();
        m_combatPower = shipData["combatPower"].get<int>();

        // Charger les traits
        for (const auto &trait : shipData["traits"])
        {
            m_traits.push_back({trait["id"].get<int>(),
                                trait["name"].get<std::string>(),
                                trait["effect"].get<std::string>()});
        }

        m_weaponSlots = shipData["weaponSlots"].get<int>();
        m_cannonCapacity = shipData["cannonCapacity"].get<int>();

        // Charger les spécialisations
        for (const auto &spec : shipData["specializations"])
        {
            m_specializations.push_back(spec.get<std::string>());
        }

        m_maintenanceCost = shipData["maintenanceCost"].get<int>();
        m_requiredCrew = shipData["requiredCrew"].get<int>();

        // Charger les upgrades
        m_upgrades.hull = shipData["upgrades"]["hull"].get<int>();
        m_upgrades.rigging = shipData["upgrades"]["rigging"].get<int>();
        m_upgrades.cannons = shipData["upgrades"]["cannons"].get<int>();

        // Charger la cargaison
        for (const auto &cargo : shipData["cargo"])
        {
            m_cargo.push_back({cargo["itemId"].get<int>(),
                               cargo["name"].get<std::string>(),
                               cargo["quantity"].get<int>(),
                               cargo["unitWeight"].get<int>()});
        }
    }

    // Constructeur de copie
    Ship(const Ship &other)
    {
        m_id = other.m_id;
        m_name = other.m_name;
        m_description = other.m_description;
        m_shipClass = other.m_shipClass;
        m_nationality = other.m_nationality;
        m_maxCrew = other.m_maxCrew;
        m_currentCrew = other.m_currentCrew;
        m_cargoCapacity = other.m_cargoCapacity;
        m_usedCargo = other.m_usedCargo;
        m_maxPassengers = other.m_maxPassengers;
        m_currentPassengers = other.m_currentPassengers;
        m_speed = other.m_speed;
        m_maneuverability = other.m_maneuverability;
        m_durability = other.m_durability;
        m_currentDurability = other.m_currentDurability;
        m_combatPower = other.m_combatPower;
        m_traits = other.m_traits;
        m_weaponSlots = other.m_weaponSlots;
        m_cannonCapacity = other.m_cannonCapacity;
        m_specializations = other.m_specializations;
        m_maintenanceCost = other.m_maintenanceCost;
        m_requiredCrew = other.m_requiredCrew;
        m_upgrades = other.m_upgrades;
        m_cargo = other.m_cargo;
    }

    // Constructeur par défaut (juste pour la compatibilité)
    Ship() : m_id(0), m_currentCrew(0), m_usedCargo(0), m_currentPassengers(0), m_currentDurability(0) {}

    // Getters
    int getId() const { return m_id; }
    const std::string &getName() const { return m_name; }
    const std::string &getDescription() const { return m_description; }
    const std::string &getShipClass() const { return m_shipClass; }
    const std::string &getNationality() const { return m_nationality; }
    int getMaxCrew() const { return m_maxCrew; }
    int getCurrentCrew() const { return m_currentCrew; }
    int getCargoCapacity() const { return m_cargoCapacity; }
    int getUsedCargo() const { return m_usedCargo; }
    int getRemainingCargo() const { return m_cargoCapacity - m_usedCargo; }
    int getMaxPassengers() const { return m_maxPassengers; }
    int getCurrentPassengers() const { return m_currentPassengers; }
    int getSpeed() const { return m_speed; }
    int getManeuverability() const { return m_maneuverability; }
    int getDurability() const { return m_durability; }
    int getCurrentDurability() const { return m_currentDurability; }
    float getDurabilityPercent() const { return (float)m_currentDurability / m_durability * 100.0f; }
    int getCombatPower() const { return m_combatPower; }
    const std::vector<ShipTrait> &getTraits() const { return m_traits; }
    int getWeaponSlots() const { return m_weaponSlots; }
    int getCannonCapacity() const { return m_cannonCapacity; }
    const std::vector<std::string> &getSpecializations() const { return m_specializations; }
    int getMaintenanceCost() const { return m_maintenanceCost; }
    int getRequiredCrew() const { return m_requiredCrew; }
    const ShipUpgrades &getUpgrades() const { return m_upgrades; }
    const std::vector<Cargo> &getCargo() const { return m_cargo; }

    // Setters
    void setName(const std::string &name) { m_name = name; }
    void setCurrentCrew(int crew) { m_currentCrew = (crew > m_maxCrew) ? m_maxCrew : crew; }
    void setCurrentDurability(int durability) { m_currentDurability = (durability > m_durability) ? m_durability : durability; }

    // Gestion de l'équipage
    bool addCrew(int amount)
    {
        if (m_currentCrew + amount > m_maxCrew)
            return false;
        m_currentCrew += amount;
        return true;
    }
    bool removeCrew(int amount)
    {
        if (m_currentCrew - amount < 0)
            return false;
        m_currentCrew -= amount;
        return true;
    }
    bool hasMinimumCrew() const { return m_currentCrew >= m_requiredCrew; }

    // Gestion des passagers
    bool addPassengers(int amount)
    {
        if (m_currentPassengers + amount > m_maxPassengers)
            return false;
        m_currentPassengers += amount;
        return true;
    }
    bool removePassengers(int amount)
    {
        if (m_currentPassengers - amount < 0)
            return false;
        m_currentPassengers -= amount;
        return true;
    }

    // Gestion de la cargaison
    bool addCargo(int itemId, const std::string &name, int quantity, int unitWeight)
    {
        if (m_usedCargo + quantity > m_cargoCapacity)
            return false;
        m_cargo.push_back({itemId, name, quantity, unitWeight});
        m_usedCargo += quantity;
        return true;
    }
    bool removeCargo(int itemId, int quantity)
    {
        for (auto &cargo : m_cargo)
        {
            if (cargo.itemId == itemId)
            {
                if (cargo.quantity - quantity < 0)
                    return false;
                cargo.quantity -= quantity;
                m_usedCargo -= quantity;
                return true;
            }
        }
        return false;
    }
    int getCargoQuantity(int itemId) const
    {
        for (const auto &cargo : m_cargo)
        {
            if (cargo.itemId == itemId)
                return cargo.quantity;
        }
        return 0;
    }

    // Gestion de la durabilité
    void repair(int amount)
    {
        if (m_currentDurability + amount > m_durability)
            m_currentDurability = m_durability;
        else
            m_currentDurability += amount;
    }
    void damage(int amount)
    {
        if (m_currentDurability - amount < 0)
            m_currentDurability = 0;
        else
            m_currentDurability -= amount;
    }
    bool isSeaworthy() const { return m_currentDurability > m_durability * 0.25; }

    // Visibilité et exploration
    int getVisibilityRange() const
    {
        // Implementation of getVisibilityRange method
        return 0; // Placeholder return, actual implementation needed
    }

    // Amélioration du navire
    bool upgradeHull(int level)
    {
        if (m_upgrades.hull + level > 10)
            return false;
        m_upgrades.hull += level;
        return true;
    }
    bool upgradeRigging(int level)
    {
        if (m_upgrades.rigging + level > 10)
            return false;
        m_upgrades.rigging += level;
        return true;
    }
    bool upgradeCannons(int level)
    {
        if (m_upgrades.cannons + level > 10)
            return false;
        m_upgrades.cannons += level;
        return true;
    }
};