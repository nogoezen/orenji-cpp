#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <functional>
#include "Ship.h"

// Nombre maximum de navires dans une flotte
constexpr int MAX_FLEET_SIZE = 5;

// Énumération des formations de flotte
enum class FleetFormation
{
    LINE,      // Ligne - Bonus de portée d'attaque
    WEDGE,     // Coin - Bonus d'attaque
    CIRCLE,    // Cercle - Bonus de défense
    SCATTERED, // Dispersé - Bonus d'évasion
    COLUMN,    // Colonne - Bonus de vitesse
    CUSTOM     // Formation personnalisée
};

class Fleet
{
private:
    std::vector<std::shared_ptr<Ship>> m_ships;
    std::string m_name;
    int m_posX = 0;
    int m_posY = 0;
    int m_flagship = 0; // Index du navire amiral dans le vecteur

    // Nouvelle variable pour les formations
    FleetFormation m_formation = FleetFormation::LINE;

    // Niveau d'expérience collectif de la flotte
    int m_fleetExperience = 0;

    // Morale de l'équipage (affecte les performances)
    float m_morale = 100.0f;

    // Fatigue de l'équipage (accumule pendant les voyages)
    float m_crewFatigue = 0.0f;

    // Date du dernier ravitaillement
    int m_lastResupplyDay = 0;

    // Provisions (en jours)
    int m_provisions = 30;

    // Relations entre les navires (un indice indiquant le fonctionnement d'ensemble)
    float m_cohesion = 50.0f;

    // Emplacement personnalisé des navires pour la formation personnalisée
    std::vector<std::pair<int, int>> m_customPositions;

public:
    Fleet(const std::string &name = "Ma Flotte");
    ~Fleet() = default;

    // Gestion des navires
    bool addShip(std::shared_ptr<Ship> ship);
    bool removeShip(size_t index);
    std::shared_ptr<Ship> getShip(size_t index) const;
    std::shared_ptr<Ship> getFlagship() const;
    bool setFlagship(size_t index);
    size_t getSize() const { return m_ships.size(); }
    bool isFull() const { return m_ships.size() >= MAX_FLEET_SIZE; }

    // Propriétés de la flotte
    std::string getName() const { return m_name; }
    void setName(const std::string &name) { m_name = name; }

    // Position de la flotte
    int getPosX() const { return m_posX; }
    int getPosY() const { return m_posY; }
    void setPosition(int x, int y)
    {
        m_posX = x;
        m_posY = y;
    }

    // Capacités de la flotte
    int getTotalCargo() const;
    int getUsedCargo() const;
    int getRemainingCargo() const { return getTotalCargo() - getUsedCargo(); }

    int getTotalCrew() const;
    int getCurrentCrew() const;

    // Vitesse et mobilité
    int getSpeed() const;
    int getVisibilityRange() const;

    // État de la flotte
    bool canNavigate() const;

    // Nouvelles méthodes pour les formations
    FleetFormation getFormation() const { return m_formation; }
    void setFormation(FleetFormation formation) { m_formation = formation; }

    // Obtenir un bonus basé sur la formation actuelle
    float getFormationBonus(const std::string &bonusType) const;

    // Description d'une formation pour l'interface utilisateur
    static std::string getFormationDescription(FleetFormation formation);

    // Nouvelle méthode pour obtenir la puissance de combat de la flotte
    int getCombatPower() const;

    // Méthodes pour la gestion de l'expérience de la flotte
    int getFleetExperience() const { return m_fleetExperience; }
    void addExperience(int exp) { m_fleetExperience += exp; }
    int getExperienceLevel() const { return m_fleetExperience / 1000; } // Chaque niveau = 1000 exp

    // Méthodes pour la gestion de la morale et de la fatigue
    float getMorale() const { return m_morale; }
    void adjustMorale(float amount) { m_morale = std::clamp(m_morale + amount, 0.0f, 100.0f); }

    float getCrewFatigue() const { return m_crewFatigue; }
    void increaseCrewFatigue(float amount) { m_crewFatigue = std::min(m_crewFatigue + amount, 100.0f); }
    void restCrew(float amount) { m_crewFatigue = std::max(m_crewFatigue - amount, 0.0f); }

    // Méthodes pour les provisions
    int getProvisions() const { return m_provisions; }
    void setProvisions(int days) { m_provisions = days; }
    void consumeProvisions(int days) { m_provisions = std::max(0, m_provisions - days); }
    void addProvisions(int days) { m_provisions += days; }

    // Gestion de la cohésion
    float getCohesion() const { return m_cohesion; }
    void adjustCohesion(float amount) { m_cohesion = std::clamp(m_cohesion + amount, 0.0f, 100.0f); }

    // Méthodes pour la formation personnalisée
    void setCustomPosition(size_t shipIndex, int relativeX, int relativeY);
    std::pair<int, int> getCustomPosition(size_t shipIndex) const;

    // Simulation d'un jour de voyage
    void simulateDay(bool inCombat = false);

    // Ravitaillement de la flotte
    void resupply();

    // Conversion vers JSON pour sauvegarde
    nlohmann::json toJson() const;
    bool fromJson(const nlohmann::json &data);
};