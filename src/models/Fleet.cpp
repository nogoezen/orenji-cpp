#include "Fleet.hpp"
#include <algorithm>
#include <cmath>
#include <nlohmann/json.hpp>

namespace Orenji
{
    Fleet::Fleet(const std::string &name) : m_name(name)
    {
        // Initialisation avec un vecteur vide
    }

    bool Fleet::addShip(std::shared_ptr<Ship> ship)
    {
        if (m_ships.size() >= MAX_FLEET_SIZE)
        {
            return false; // La flotte est déjà pleine
        }

        m_ships.push_back(ship);

        // Si c'est le premier navire, il devient automatiquement le navire amiral
        if (m_ships.size() == 1)
        {
            m_flagship = 0;
        }

        // Initialiser la position personnalisée pour ce navire
        m_customPositions.push_back(std::make_pair(0, 0));

        // L'ajout d'un nouveau navire réduit temporairement la cohésion
        adjustCohesion(-5.0f);

        return true;
    }

    bool Fleet::removeShip(size_t index)
    {
        if (index >= m_ships.size())
        {
            return false; // Index invalide
        }

        // Si le navire à supprimer est le navire amiral
        if (index == m_flagship)
        {
            // S'il y a d'autres navires, définir le premier comme amiral
            if (m_ships.size() > 1)
            {
                m_flagship = (index == 0) ? 1 : 0;
            }
        }
        else if (index < m_flagship)
        {
            // Si le navire supprimé est avant le navire amiral, ajuster l'index
            m_flagship--;
        }

        m_ships.erase(m_ships.begin() + index);

        // Supprimer également la position personnalisée
        if (index < m_customPositions.size())
        {
            m_customPositions.erase(m_customPositions.begin() + index);
        }

        // Retirer un navire peut affecter négativement le moral
        adjustMorale(-3.0f);

        // Le retrait d'un navire réduit temporairement la cohésion
        adjustCohesion(-5.0f);

        return true;
    }

    std::shared_ptr<Ship> Fleet::getShip(size_t index) const
    {
        if (index >= m_ships.size())
        {
            return nullptr; // Index invalide
        }

        return m_ships[index];
    }

    std::shared_ptr<Ship> Fleet::getFlagship() const
    {
        if (m_ships.empty())
        {
            return nullptr;
        }

        return m_ships[m_flagship];
    }

    bool Fleet::setFlagship(size_t index)
    {
        if (index >= m_ships.size())
        {
            return false; // Index invalide
        }

        m_flagship = index;
        return true;
    }

    int Fleet::getTotalCargo() const
    {
        int totalCargo = 0;
        for (const auto &ship : m_ships)
        {
            totalCargo += ship->getCargoCapacity();
        }
        return totalCargo;
    }

    int Fleet::getUsedCargo() const
    {
        int usedCargo = 0;
        for (const auto &ship : m_ships)
        {
            usedCargo += ship->getUsedCargo();
        }
        return usedCargo;
    }

    int Fleet::getTotalCrew() const
    {
        int totalCrew = 0;
        for (const auto &ship : m_ships)
        {
            totalCrew += ship->getCrewCapacity();
        }
        return totalCrew;
    }

    int Fleet::getCurrentCrew() const
    {
        int currentCrew = 0;
        for (const auto &ship : m_ships)
        {
            currentCrew += ship->getCurrentCrew();
        }
        return currentCrew;
    }

    int Fleet::getSpeed() const
    {
        // La vitesse de la flotte est déterminée par le navire le plus lent
        // mais peut être modifiée par la formation, la morale et la fatigue
        if (m_ships.empty())
        {
            return 0;
        }

        int minSpeed = m_ships[0]->getSpeed();
        for (size_t i = 1; i < m_ships.size(); ++i)
        {
            int shipSpeed = m_ships[i]->getSpeed();
            if (shipSpeed < minSpeed)
            {
                minSpeed = shipSpeed;
            }
        }

        // Appliquer les modificateurs
        float moraleModifier = 0.5f + (m_morale / 200.0f);       // 0.5 à 1.0
        float fatigueModifier = 1.0f - (m_crewFatigue / 200.0f); // 0.5 à 1.0
        float formationModifier = 1.0f + getFormationBonus("speed");
        float cohesionModifier = 0.8f + (m_cohesion / 500.0f); // 0.8 à 1.0

        return static_cast<int>(minSpeed * moraleModifier * fatigueModifier * formationModifier * cohesionModifier);
    }

    int Fleet::getVisibilityRange() const
    {
        // La visibilité de base est déterminée par le navire amiral
        auto flagship = getFlagship();
        if (!flagship)
        {
            return 5; // Valeur par défaut
        }

        int baseRange = flagship->getVisibilityRange();

        // Bonus de formation pour la visibilité
        float formationModifier = 1.0f + getFormationBonus("visibility");

        // Bonus d'expérience
        float expModifier = 1.0f + (getExperienceLevel() * 0.05f); // +5% par niveau d'expérience

        return static_cast<int>(baseRange * formationModifier * expModifier);
    }

    int Fleet::getCombatPower() const
    {
        if (m_ships.empty())
        {
            return 0;
        }

        // Calculer la puissance de base (somme des puissances des navires)
        int basePower = 0;
        for (const auto &ship : m_ships)
        {
            basePower += ship->getCombatPower();
        }

        // Appliquer les modificateurs
        float moraleModifier = 0.5f + (m_morale / 200.0f);       // 0.5 à 1.0
        float fatigueModifier = 1.0f - (m_crewFatigue / 200.0f); // 0.5 à 1.0
        float formationAttackModifier = 1.0f + getFormationBonus("attack");
        float formationDefenseModifier = 1.0f + getFormationBonus("defense");
        float cohesionModifier = 0.7f + (m_cohesion / 333.0f);           // 0.7 à 1.0
        float experienceModifier = 1.0f + (getExperienceLevel() * 0.1f); // +10% par niveau

        float combatModifier = moraleModifier * fatigueModifier *
                               formationAttackModifier * formationDefenseModifier *
                               cohesionModifier * experienceModifier;

        return static_cast<int>(basePower * combatModifier);
    }

    bool Fleet::canNavigate() const
    {
        if (m_ships.empty())
        {
            return false;
        }

        // Vérifier que tous les navires ont suffisamment d'équipage
        for (const auto &ship : m_ships)
        {
            if (!ship->hasMinimumCrew())
            {
                return false;
            }
        }

        // Vérifier les provisions
        if (m_provisions <= 0)
        {
            return false; // Pas de provisions, pas de navigation
        }

        // Si la fatigue est trop élevée, la navigation devient dangereuse
        if (m_crewFatigue > 90.0f)
        {
            return false;
        }

        return true;
    }

    float Fleet::getFormationBonus(const std::string &bonusType) const
    {
        // Valeurs par défaut si la flotte est trop petite pour former efficacement
        if (m_ships.size() < 2)
        {
            return 0.0f;
        }

        // Bonus selon la formation et le type demandé
        switch (m_formation)
        {
        case FleetFormation::LINE:
            if (bonusType == "range")
                return 0.15f; // +15% de portée d'attaque
            if (bonusType == "attack")
                return 0.05f; // +5% d'attaque
            break;

        case FleetFormation::WEDGE:
            if (bonusType == "attack")
                return 0.2f; // +20% d'attaque
            if (bonusType == "speed")
                return -0.05f; // -5% de vitesse
            break;

        case FleetFormation::CIRCLE:
            if (bonusType == "defense")
                return 0.2f; // +20% de défense
            if (bonusType == "speed")
                return -0.1f; // -10% de vitesse
            break;

        case FleetFormation::SCATTERED:
            if (bonusType == "evasion")
                return 0.2f; // +20% d'évasion
            if (bonusType == "defense")
                return -0.1f; // -10% de défense
            if (bonusType == "cohesion")
                return -0.1f; // -10% de cohésion
            break;

        case FleetFormation::COLUMN:
            if (bonusType == "speed")
                return 0.15f; // +15% de vitesse
            if (bonusType == "defense")
                return -0.05f; // -5% de défense
            break;

        case FleetFormation::CUSTOM:
            // Les bonus de la formation personnalisée dépendent de l'agencement des navires
            // et peuvent varier. Pour simplifier, on suppose des bonus moyens.
            if (bonusType == "attack")
                return 0.05f;
            if (bonusType == "defense")
                return 0.05f;
            if (bonusType == "speed")
                return 0.05f;
            break;
        }

        return 0.0f; // Aucun bonus pour ce type
    }

    std::string Fleet::getFormationDescription(FleetFormation formation)
    {
        switch (formation)
        {
        case FleetFormation::LINE:
            return "Ligne - Les navires sont alignés côte à côte pour maximiser la puissance de feu latérale. +15% de portée d'attaque, +5% d'attaque.";

        case FleetFormation::WEDGE:
            return "Coin - Formation en V permettant de concentrer le feu vers l'avant. +20% d'attaque, -5% de vitesse.";

        case FleetFormation::CIRCLE:
            return "Cercle - Formation défensive où les navires se protègent mutuellement. +20% de défense, -10% de vitesse.";

        case FleetFormation::SCATTERED:
            return "Dispersé - Les navires sont espacés pour être des cibles plus difficiles. +20% d'évasion, -10% de défense, -10% de cohésion.";

        case FleetFormation::COLUMN:
            return "Colonne - Les navires se suivent en file indienne pour une meilleure navigation. +15% de vitesse, -5% de défense.";

        case FleetFormation::CUSTOM:
            return "Personnalisé - Formation définie manuellement selon vos préférences tactiques.";

        default:
            return "Formation inconnue";
        }
    }

    void Fleet::setCustomPosition(size_t shipIndex, int relativeX, int relativeY)
    {
        if (shipIndex >= m_ships.size() || shipIndex >= m_customPositions.size())
        {
            return;
        }

        m_customPositions[shipIndex] = std::make_pair(relativeX, relativeY);
    }

    std::pair<int, int> Fleet::getCustomPosition(size_t shipIndex) const
    {
        if (shipIndex >= m_customPositions.size())
        {
            return std::make_pair(0, 0);
        }

        return m_customPositions[shipIndex];
    }

    void Fleet::simulateDay(bool inCombat)
    {
        // Consommer des provisions
        int crewSize = getCurrentCrew();
        float consumptionRate = inCombat ? 1.5f : 1.0f; // Combat augmente la consommation

        // Consommation minimale de 1, dépend du nombre d'équipage
        int consumption = std::max(1, static_cast<int>(crewSize / 50 * consumptionRate));
        consumeProvisions(consumption);

        // Augmenter la fatigue
        float fatigueIncrease = inCombat ? 10.0f : 2.0f;
        increaseCrewFatigue(fatigueIncrease);

        // Ajuster le moral selon les conditions
        if (m_provisions <= 5)
        {
            // Peu de provisions affecte le moral
            adjustMorale(-5.0f);
        }

        if (m_crewFatigue > 75.0f)
        {
            // Fatigue élevée réduit le moral
            adjustMorale(-3.0f);
        }

        // En combat, l'équipage peut gagner ou perdre du moral selon les circonstances
        if (inCombat)
        {
            // Pour simplifier, supposons une chance égale de gain ou perte
            float combatMoraleChange = ((std::rand() % 100) < 50) ? -5.0f : 5.0f;
            adjustMorale(combatMoraleChange);

            // Gain d'expérience en combat
            addExperience(20 + (std::rand() % 30)); // 20-50 points d'XP par jour de combat
        }
        else
        {
            // Gain d'expérience normal
            addExperience(5 + (std::rand() % 10)); // 5-15 points d'XP par jour
        }

        // Augmenter lentement la cohésion lorsque les navires travaillent ensemble
        adjustCohesion(0.5f);
    }

    void Fleet::resupply()
    {
        // Réapprovisionner à 30 jours de provisions
        m_provisions = 30;

        // Repos de l'équipage
        m_crewFatigue = 0.0f;

        // Boost de moral
        adjustMorale(10.0f);

        // Enregistrer la date du ravitaillement
        m_lastResupplyDay = 0; // À mettre à jour avec le système de date du jeu
    }

    nlohmann::json Fleet::toJson() const
    {
        nlohmann::json j;

        j["name"] = m_name;
        j["posX"] = m_posX;
        j["posY"] = m_posY;
        j["flagship"] = m_flagship;
        j["formation"] = static_cast<int>(m_formation);
        j["fleetExperience"] = m_fleetExperience;
        j["morale"] = m_morale;
        j["crewFatigue"] = m_crewFatigue;
        j["lastResupplyDay"] = m_lastResupplyDay;
        j["provisions"] = m_provisions;
        j["cohesion"] = m_cohesion;

        // Sauvegarder les navires
        nlohmann::json ships = nlohmann::json::array();
        for (const auto &ship : m_ships)
        {
            ships.push_back(ship->toJson());
        }
        j["ships"] = ships;

        // Sauvegarder les positions personnalisées
        nlohmann::json positions = nlohmann::json::array();
        for (const auto &pos : m_customPositions)
        {
            nlohmann::json position;
            position["x"] = pos.first;
            position["y"] = pos.second;
            positions.push_back(position);
        }
        j["customPositions"] = positions;

        return j;
    }

    bool Fleet::fromJson(const nlohmann::json &data)
    {
        try
        {
            if (data.contains("name"))
                m_name = data["name"];
            if (data.contains("posX"))
                m_posX = data["posX"];
            if (data.contains("posY"))
                m_posY = data["posY"];
            if (data.contains("flagship"))
                m_flagship = data["flagship"];
            if (data.contains("formation"))
                m_formation = static_cast<FleetFormation>(data["formation"].get<int>());
            if (data.contains("fleetExperience"))
                m_fleetExperience = data["fleetExperience"];
            if (data.contains("morale"))
                m_morale = data["morale"];
            if (data.contains("crewFatigue"))
                m_crewFatigue = data["crewFatigue"];
            if (data.contains("lastResupplyDay"))
                m_lastResupplyDay = data["lastResupplyDay"];
            if (data.contains("provisions"))
                m_provisions = data["provisions"];
            if (data.contains("cohesion"))
                m_cohesion = data["cohesion"];

            // Charger les positions personnalisées
            m_customPositions.clear();
            if (data.contains("customPositions") && data["customPositions"].is_array())
            {
                for (const auto &posData : data["customPositions"])
                {
                    int x = posData["x"];
                    int y = posData["y"];
                    m_customPositions.push_back(std::make_pair(x, y));
                }
            }

            return true;
        }
        catch (const std::exception &e)
        {
            // Gestion des erreurs
            return false;
        }
    }
} // namespace Orenji
