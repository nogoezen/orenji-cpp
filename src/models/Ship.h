#pragma once

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include <map>

/**
 * @brief Classe représentant un navire
 *
 * Cette classe contient toutes les propriétés et méthodes liées à un navire,
 * comme son type, ses caractéristiques, sa capacité de cargaison, etc.
 */
class Ship
{
private:
    // Identifiant unique
    int m_id;

    // Nom du navire
    std::string m_name;

    // Type de navire (galion, caravelle, etc.)
    std::string m_type;

    // Caractéristiques
    int m_speed;        // Vitesse
    int m_maneuver;     // Maniabilité
    int m_armor;        // Armure/résistance
    int m_cargoSpace;   // Espace de cargaison
    int m_crewCapacity; // Capacité d'équipage

    // Équipement
    int m_cannons; // Nombre de canons
    int m_sails;   // Qualité des voiles
    int m_hull;    // État de la coque

    // État actuel
    int m_currentHull;  // État actuel de la coque
    int m_currentSails; // État actuel des voiles
    int m_currentCrew;  // Nombre actuel de membres d'équipage
    int m_usedCargo;    // Cargaison utilisée

    // Coût d'achat et de maintenance
    int m_purchasePrice;
    int m_maintenanceCost;

    // Description
    std::string m_description;

    // Gestion de la cargaison spécifique (pour TradeGoods)
    std::map<int, std::pair<std::string, int>> m_cargo; // Map d'ID de marchandise -> (nom, quantité)

public:
    /**
     * Constructeur par défaut
     */
    Ship()
        : m_id(-1), m_name("Unknown"), m_type("Unknown"),
          m_speed(0), m_maneuver(0), m_armor(0), m_cargoSpace(0), m_crewCapacity(0),
          m_cannons(0), m_sails(0), m_hull(0),
          m_currentHull(0), m_currentSails(0), m_currentCrew(0), m_usedCargo(0),
          m_purchasePrice(0), m_maintenanceCost(0),
          m_description("") {}

    /**
     * Constructeur avec paramètres de base
     */
    Ship(int id, const std::string &name, const std::string &type,
         int speed, int maneuver, int armor, int cargoSpace, int crewCapacity)
        : m_id(id), m_name(name), m_type(type),
          m_speed(speed), m_maneuver(maneuver), m_armor(armor),
          m_cargoSpace(cargoSpace), m_crewCapacity(crewCapacity),
          m_cannons(0), m_sails(speed), m_hull(armor),
          m_currentHull(armor), m_currentSails(speed), m_currentCrew(0), m_usedCargo(0),
          m_purchasePrice(1000), m_maintenanceCost(100),
          m_description("A " + type + " ship") {}

    /**
     * Constructeur à partir de données JSON
     */
    Ship(const nlohmann::json &data);

    /**
     * Opérateur de copie pour éviter les problèmes avec unordered_map
     */
    Ship(const Ship &other) = default;
    Ship &operator=(const Ship &other) = default;

    /**
     * Destructeur
     */
    ~Ship() = default;

    /**
     * Conversion en JSON pour sauvegarde
     */
    nlohmann::json toJson() const;

    // Accesseurs
    int getId() const { return m_id; }
    const std::string &getName() const { return m_name; }
    const std::string &getType() const { return m_type; }
    std::string getShipClass() const { return m_type; } // Alias pour getType

    int getSpeed() const { return m_speed; }
    int getManeuver() const { return m_maneuver; }
    int getManeuverability() const { return m_maneuver; } // Alias pour getManeuver
    int getArmor() const { return m_armor; }
    int getCargoSpace() const { return m_cargoSpace; }
    int getCrewCapacity() const { return m_crewCapacity; }
    int getMaxCrew() const { return m_crewCapacity; } // Alias pour getCrewCapacity

    int getCannons() const { return m_cannons; }
    int getSails() const { return m_sails; }
    int getHull() const { return m_hull; }
    int getDurability() const { return m_currentHull; } // Alias pour getCurrentHull

    int getCurrentHull() const { return m_currentHull; }
    int getCurrentSails() const { return m_currentSails; }
    int getCurrentCrew() const { return m_currentCrew; }
    int getUsedCargo() const { return m_usedCargo; }
    int getRemainingCargo() const { return m_cargoSpace - m_usedCargo; }
    int getCargoCapacity() const { return m_cargoSpace; } // Alias pour getCargoSpace

    int getPurchasePrice() const { return m_purchasePrice; }
    int getMaintenanceCost() const { return m_maintenanceCost; }

    const std::string &getDescription() const { return m_description; }

    // Obtenir le pourcentage de durabilité de la coque
    float getDurabilityPercent() const { return static_cast<float>(m_currentHull) / m_hull * 100.0f; }

    // Obtenir le minimum d'équipage nécessaire (moitié de l'équipage total)
    int getMinimumCrew() const { return m_crewCapacity / 2; }

    // Vérifier si le navire a le minimum d'équipage requis
    bool hasMinimumCrew() const { return m_currentCrew >= getMinimumCrew(); }

    // Calculer le rayon de visibilité (basé sur le type et les attributs)
    int getVisibilityRange() const { return 5 + (m_speed / 2); }

    // Calculer la puissance de combat (basée sur les canons, la coque et l'équipage)
    int getCombatPower() const
    {
        if (!hasMinimumCrew())
            return 0;

        float crewFactor = static_cast<float>(m_currentCrew) / m_crewCapacity;
        return static_cast<int>((m_cannons * 2 + m_armor) * crewFactor);
    }

    // Mutateurs
    void setName(const std::string &name) { m_name = name; }
    void setCurrentHull(int hull) { m_currentHull = std::max(0, std::min(hull, m_hull)); }
    void setCurrentSails(int sails) { m_currentSails = std::max(0, std::min(sails, m_sails)); }
    void setCurrentCrew(int crew) { m_currentCrew = std::max(0, std::min(crew, m_crewCapacity)); }
    void setUsedCargo(int cargo) { m_usedCargo = std::max(0, std::min(cargo, m_cargoSpace)); }

    // Ajouter une marchandise spécifique
    bool addCargo(int goodId, const std::string &name, int quantity, int unitWeight = 1)
    {
        // Vérifier l'espace disponible
        int requiredSpace = quantity * unitWeight;
        if (m_usedCargo + requiredSpace > m_cargoSpace)
        {
            return false;
        }

        // Ajouter ou mettre à jour la marchandise
        if (m_cargo.find(goodId) != m_cargo.end())
        {
            m_cargo[goodId].second += quantity;
        }
        else
        {
            m_cargo[goodId] = std::make_pair(name, quantity);
        }

        // Mettre à jour l'espace utilisé
        m_usedCargo += requiredSpace;
        return true;
    }

    // Retirer une marchandise spécifique
    bool removeCargo(int goodId, int quantity, int unitWeight = 1)
    {
        auto it = m_cargo.find(goodId);
        if (it == m_cargo.end() || it->second.second < quantity)
        {
            return false;
        }

        // Mettre à jour la quantité
        it->second.second -= quantity;

        // Mettre à jour l'espace utilisé
        m_usedCargo -= quantity * unitWeight;

        // Supprimer l'entrée si la quantité est nulle
        if (it->second.second <= 0)
        {
            m_cargo.erase(it);
        }

        return true;
    }

    // Obtenir la quantité d'une marchandise spécifique
    int getCargoQuantity(int goodId) const
    {
        auto it = m_cargo.find(goodId);
        return (it != m_cargo.end()) ? it->second.second : 0;
    }

    // Méthodes fonctionnelles pour la cargaison générique
    bool addCargo(int amount)
    {
        if (m_usedCargo + amount > m_cargoSpace)
        {
            return false;
        }
        m_usedCargo += amount;
        return true;
    }

    bool removeCargo(int amount)
    {
        if (amount > m_usedCargo)
        {
            return false;
        }
        m_usedCargo -= amount;
        return true;
    }

    bool addCrew(int amount)
    {
        if (m_currentCrew + amount > m_crewCapacity)
        {
            return false;
        }
        m_currentCrew += amount;
        return true;
    }

    bool removeCrew(int amount)
    {
        if (amount > m_currentCrew)
        {
            return false;
        }
        m_currentCrew -= amount;
        return true;
    }

    // Méthodes d'état
    bool isSeaworthy() const
    {
        return m_currentHull > 0 && m_currentSails > 0 && m_currentCrew >= m_crewCapacity / 2;
    }

    int getEffectiveSpeed() const
    {
        if (!isSeaworthy())
        {
            return 0;
        }

        // La vitesse effective dépend de l'état des voiles et de l'équipage
        float sailFactor = static_cast<float>(m_currentSails) / m_sails;
        float crewFactor = static_cast<float>(m_currentCrew) / m_crewCapacity;

        return static_cast<int>(m_speed * sailFactor * crewFactor);
    }

    int getEffectiveManeuver() const
    {
        if (!isSeaworthy())
        {
            return 0;
        }

        // La maniabilité effective dépend de l'état de l'équipage
        float crewFactor = static_cast<float>(m_currentCrew) / m_crewCapacity;

        return static_cast<int>(m_maneuver * crewFactor);
    }

    int getEffectiveArmor() const
    {
        if (!isSeaworthy())
        {
            return 0;
        }

        // L'armure effective dépend de l'état de la coque
        float hullFactor = static_cast<float>(m_currentHull) / m_hull;

        return static_cast<int>(m_armor * hullFactor);
    }
};