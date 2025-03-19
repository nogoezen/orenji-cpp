#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm> // Pour std::min, std::max, std::clamp
#include <nlohmann/json.hpp>
#include "Ship.h"
#include "Fleet.h"
#include "TradeGood.h"
#include "Character.h"

// Fonction clamp (au cas où std::clamp n'est pas disponible dans la version de C++ utilisée)
template <typename T>
T customClamp(const T &value, const T &min, const T &max)
{
    return std::min(std::max(value, min), max);
}

enum class PlayerMode
{
    LAND, // Mode terrestre (utilise Character)
    SEA   // Mode maritime (utilise Fleet/Ship)
};

/**
 * Structure représentant un élément dans l'inventaire du joueur
 */
struct CargoItem
{
    int id;
    std::string name;
    int quantity;
    int unitWeight;

    CargoItem(int _id, const std::string &_name, int _quantity, int _unitWeight)
        : id(_id), name(_name), quantity(_quantity), unitWeight(_unitWeight) {}
};

// Déclaration anticipée
class TradingSystem;
class TradeRoute;

/**
 * Structure pour les compétences commerciales
 */
struct TradeSkills
{
    int negotiation = 0; // Améliore les prix (±5% par niveau)
    int logistics = 0;   // Augmente la capacité (±10% par niveau)
    int smuggling = 0;   // Chance d'accéder aux marchés noirs
    int influence = 0;   // Débloque des opportunités spéciales

    nlohmann::json toJson() const
    {
        nlohmann::json j;
        j["negotiation"] = negotiation;
        j["logistics"] = logistics;
        j["smuggling"] = smuggling;
        j["influence"] = influence;
        return j;
    }

    void fromJson(const nlohmann::json &j)
    {
        if (j.contains("negotiation"))
            negotiation = j["negotiation"];
        if (j.contains("logistics"))
            logistics = j["logistics"];
        if (j.contains("smuggling"))
            smuggling = j["smuggling"];
        if (j.contains("influence"))
            influence = j["influence"];
    }
};

/**
 * Structure pour les compétences d'amirauté
 */
struct AdmiraltySkills
{
    int navigation = 0;      // Navigation (vitesse, précision)
    int tactics = 0;         // Tactiques (formations, combat)
    int leadership = 0;      // Leadership (moral, cohésion)
    int resourcefulness = 0; // Débrouillardise (provisions, réparations)
    int crewManagement = 0;  // Gestion d'équipage
    int shipMaintenance = 0; // Entretien des navires

    nlohmann::json toJson() const
    {
        nlohmann::json j;
        j["navigation"] = navigation;
        j["tactics"] = tactics;
        j["leadership"] = leadership;
        j["resourcefulness"] = resourcefulness;
        j["crewManagement"] = crewManagement;
        j["shipMaintenance"] = shipMaintenance;
        return j;
    }

    void fromJson(const nlohmann::json &data)
    {
        if (data.contains("navigation"))
            navigation = data["navigation"];
        if (data.contains("tactics"))
            tactics = data["tactics"];
        if (data.contains("leadership"))
            leadership = data["leadership"];
        if (data.contains("resourcefulness"))
            resourcefulness = data["resourcefulness"];
        if (data.contains("crewManagement"))
            crewManagement = data["crewManagement"];
        if (data.contains("shipMaintenance"))
            shipMaintenance = data["shipMaintenance"];
    }

    // Calculer les différents bonus basés sur les compétences
    float getNavigationBonus() const { return navigation * 0.05f; }           // 5% par niveau
    float getTacticsBonus() const { return tactics * 0.05f; }                 // 5% par niveau
    float getLeadershipBonus() const { return leadership * 0.05f; }           // 5% par niveau
    float getResourcefulnessBonus() const { return resourcefulness * 0.05f; } // 5% par niveau
    float getCrewManagementBonus() const { return crewManagement * 0.05f; }   // 5% par niveau
    float getShipMaintenanceBonus() const { return shipMaintenance * 0.05f; } // 5% par niveau
};

/**
 * Structure pour les routes commerciales
 */
struct TradeRoute
{
    int sourceCity;
    int destinationCity;
    std::vector<std::pair<int, int>> tradedGoods; // Paire (goodId, quantity)
    int tripDuration;                             // En jours
    int securityLevel;                            // Niveau de sécurité (investissement)
    float profitMargin;                           // Marge bénéficiaire estimée
    float currentTime;                            // Temps écoulé depuis le départ

    nlohmann::json toJson() const;
    static TradeRoute fromJson(const nlohmann::json &data);
};

/**
 * @brief Classe représentant le joueur
 *
 * Cette classe gère les données et les actions du joueur, comme l'inventaire,
 * l'équipement, les statistiques, etc.
 */
class Player
{
private:
    // Données de base
    int m_id;
    std::string m_name;
    int m_gold;
    int m_debt;
    int m_level;
    int m_experience;

    // Inventaire
    std::map<int, CargoItem> m_cargo;
    std::map<int, int> m_inventory;
    int m_cargoCapacity;
    int m_currentCargoWeight;

    // Flotte
    std::vector<int> m_ships;

    // Équipage
    std::vector<int> m_crew;

    // Personnage (attributs RPG)
    std::shared_ptr<Character> m_character;

    // Flotte du joueur (remplace le navire unique)
    std::shared_ptr<Fleet> m_fleet;

    // Position du joueur (x, y)
    int m_posX;
    int m_posY;

    // Mode actuel du joueur (terre ou mer)
    PlayerMode m_currentMode;

    // Système de réputation envers les nations
    std::map<std::string, int> m_reputation;

    // Compétences commerciales
    TradeSkills m_tradeSkills;

    // Compétences d'amirauté
    AdmiraltySkills m_admiraltySkills;

    // Réputation par ville/royaume
    std::unordered_map<int, float> m_cityReputations;
    std::unordered_map<std::string, float> m_kingdomReputations;

    // Routes commerciales
    std::vector<TradeRoute> m_tradeRoutes;

    // Date du dernier paiement pour la dette
    int m_lastDebtPaymentDay = 0;

    // Taux d'intérêt de la dette (en pourcentage)
    float m_debtInterestRate = 5.0f;

public:
    /**
     * Constructeur par défaut
     */
    Player()
        : m_id(0), m_name(""), m_gold(1000), m_debt(0), m_level(1), m_experience(0),
          m_cargoCapacity(100), m_currentCargoWeight(0),
          m_fleet(std::make_shared<Fleet>("Fleet")),
          m_posX(0), m_posY(0), m_currentMode(PlayerMode::LAND) {}

    /**
     * Constructeur avec données de base
     */
    Player(const std::string &name, int gold = 1000, int level = 1, int experience = 0)
        : m_id(0), m_name(name), m_gold(gold), m_debt(0), m_level(level), m_experience(experience),
          m_cargoCapacity(100), m_currentCargoWeight(0), m_fleet(std::make_shared<Fleet>(name + "'s Fleet")),
          m_posX(0), m_posY(0), m_currentMode(PlayerMode::LAND) {}

    /**
     * Constructeur avec personnage
     */
    Player(const std::string &name, std::shared_ptr<Character> character, int gold = 1000)
        : m_id(0), m_name(name), m_gold(gold), m_debt(0), m_level(1), m_experience(0),
          m_cargoCapacity(100), m_currentCargoWeight(0), m_fleet(std::make_shared<Fleet>(name + "'s Fleet")),
          m_character(character), m_posX(0), m_posY(0), m_currentMode(PlayerMode::LAND)
    {
        if (character)
        {
            m_level = character->getLevel();
            m_experience = character->getExperience();
        }
    }

    /**
     * Constructeur à partir de données JSON
     */
    Player(const nlohmann::json &data);

    /**
     * Convertit le joueur en JSON pour la sauvegarde
     */
    nlohmann::json toJson() const;

    // Accesseurs
    int getId() const { return m_id; }
    const std::string &getName() const { return m_name; }
    int getGold() const { return m_gold; }
    int getDebt() const { return m_debt; }
    int getLevel() const { return m_level; }
    int getExperience() const { return m_experience; }

    // Gestion de l'inventaire
    const std::map<int, CargoItem> &getCargo() const { return m_cargo; }
    int getCargoCapacity() const { return m_cargoCapacity; }
    int getCurrentCargoWeight() const { return m_currentCargoWeight; }

    /**
     * Vérifie si le joueur peut ajouter une quantité de marchandise à son inventaire
     * @param goodId ID de la marchandise
     * @param quantity Quantité à ajouter
     * @return true si le joueur a assez de place
     */
    bool canAddCargo(int goodId, int quantity) const;

    /**
     * Ajoute une marchandise à l'inventaire
     * @param goodId ID de la marchandise
     * @param name Nom de la marchandise
     * @param quantity Quantité à ajouter
     * @param unitWeight Poids unitaire
     * @return true si l'ajout a réussi
     */
    bool addCargo(int goodId, const std::string &name, int quantity, int unitWeight);

    /**
     * Version simplifiée pour l'ajout de marchandise depuis TradingSystem
     */
    bool addCargo(int goodId, int quantity);

    /**
     * Vérifie si le joueur possède une quantité de marchandise
     * @param goodId ID de la marchandise
     * @param quantity Quantité à vérifier
     * @return true si le joueur possède au moins cette quantité
     */
    bool hasCargo(int goodId, int quantity) const;

    /**
     * Retire une quantité de marchandise de l'inventaire
     * @param goodId ID de la marchandise
     * @param quantity Quantité à retirer
     * @return true si le retrait a réussi
     */
    bool removeCargo(int goodId, int quantity);

    // Getters
    const std::shared_ptr<Fleet> &getFleet() const { return m_fleet; }
    std::shared_ptr<Ship> getFlagship() const { return m_fleet ? m_fleet->getFlagship() : nullptr; }
    int getPosX() const { return m_posX; }
    int getPosY() const { return m_posY; }
    PlayerMode getCurrentMode() const { return m_currentMode; }
    const std::map<std::string, int> &getReputation() const { return m_reputation; }

    // Getter pour le personnage
    const std::shared_ptr<Character> &getCharacter() const { return m_character; }

    // Setters et méthodes d'action
    void setPosition(int x, int y)
    {
        m_posX = x;
        m_posY = y;
        // Mettre à jour la position de la flotte également
        if (m_fleet)
        {
            m_fleet->setPosition(x, y);
        }
    }
    void move(int deltaX, int deltaY)
    {
        m_posX += deltaX;
        m_posY += deltaY;
        // Mettre à jour la position de la flotte également
        if (m_fleet)
        {
            m_fleet->setPosition(m_posX, m_posY);
        }
    }

    // Définir le personnage
    void setCharacter(std::shared_ptr<Character> character)
    {
        m_character = character;

        // Mettre à jour les réputations
        if (character)
        {
            const auto &morality = character->getMorality();
            for (const auto &[nation, rep] : morality.reputation)
            {
                m_reputation[nation] = rep;
            }
        }
    }

    // Méthodes liées à la flotte
    void setFleet(std::shared_ptr<Fleet> fleet)
    {
        m_fleet = fleet;
        if (fleet)
        {
            // Synchroniser la position de la flotte avec celle du joueur
            fleet->setPosition(m_posX, m_posY);
        }
    }

    // Ajouter/supprimer des navires à la flotte
    bool addShipToFleet(std::shared_ptr<Ship> ship)
    {
        if (!m_fleet)
        {
            m_fleet = std::make_shared<Fleet>(m_name + "'s Fleet");
        }
        return m_fleet->addShip(ship);
    }

    bool removeShipFromFleet(size_t index)
    {
        return m_fleet ? m_fleet->removeShip(index) : false;
    }

    std::shared_ptr<Ship> getShipFromFleet(size_t index) const
    {
        return m_fleet ? m_fleet->getShip(index) : nullptr;
    }

    bool setFlagship(size_t index)
    {
        return m_fleet ? m_fleet->setFlagship(index) : false;
    }

    size_t getFleetSize() const
    {
        return m_fleet ? m_fleet->getSize() : 0;
    }

    // Création d'une nouvelle flotte
    void createNewFleet(const std::string &name)
    {
        m_fleet = std::make_shared<Fleet>(name);
        m_fleet->setPosition(m_posX, m_posY);
    }

    // Gestion du mode (terre/mer)
    void setMode(PlayerMode mode)
    {
        m_currentMode = mode;
    }

    void toggleMode()
    {
        // Basculer entre mode terrestre et maritime
        // Seulement si les deux modes sont disponibles
        if (m_character && m_fleet && m_fleet->getSize() > 0)
        {
            m_currentMode = (m_currentMode == PlayerMode::LAND) ? PlayerMode::SEA : PlayerMode::LAND;
        }
    }

    // Gestion de l'équipage
    void addCrew(int amount)
    {
        auto flagship = getFlagship();
        if (flagship)
        {
            flagship->addCrew(amount);
        }
    }

    void removeCrew(int amount)
    {
        auto flagship = getFlagship();
        if (flagship)
        {
            flagship->removeCrew(amount);
        }
    }

    int getCurrentCrew() const
    {
        return m_fleet ? m_fleet->getCurrentCrew() : 0;
    }

    int getTotalCrew() const
    {
        return m_fleet ? m_fleet->getTotalCrew() : 0;
    }

    bool hasMinimumCrew() const
    {
        return m_fleet && m_fleet->canNavigate();
    }

    // Gestion de la réputation
    void changeReputation(const std::string &nation, int amount)
    {
        m_reputation[nation] = customClamp(m_reputation[nation] + amount, -100, 100);
    }

    int getReputationWith(const std::string &nation) const
    {
        auto it = m_reputation.find(nation);
        return (it != m_reputation.end()) ? it->second : 0;
    }

    // Méthodes pour obtenir les attributs selon le mode actuel
    std::string getCurrentEntityName() const
    {
        if (m_currentMode == PlayerMode::LAND)
        {
            return m_character ? m_character->getName() : m_name;
        }
        else
        {
            auto flagship = getFlagship();
            return flagship ? flagship->getName() : "Sans navire";
        }
    }

    int getSpeed() const
    {
        return (m_currentMode == PlayerMode::LAND) ? 5 : // Vitesse de base sur terre
                   (m_fleet ? m_fleet->getSpeed() : 0);
    }

    int getManeuverability() const
    {
        if (m_currentMode == PlayerMode::SEA)
        {
            auto flagship = getFlagship();
            return flagship ? flagship->getManeuverability() : 0;
        }
        return 0;
    }

    int getDurability() const
    {
        if (m_currentMode == PlayerMode::SEA)
        {
            auto flagship = getFlagship();
            return flagship ? flagship->getDurability() : 0;
        }
        return 0;
    }

    int getCombatPower() const
    {
        if (m_currentMode == PlayerMode::SEA)
        {
            auto flagship = getFlagship();
            return flagship ? flagship->getCombatPower() : 0;
        }
        return 0;
    }

    // Méthodes de cargaison (maintenant déléguées à la flotte)
    int getTotalCargoCapacity() const { return m_fleet ? m_fleet->getTotalCargo() : 0; }
    int getUsedCargo() const { return m_fleet ? m_fleet->getUsedCargo() : 0; }
    int getRemainingCargoCapacity() const { return m_fleet ? m_fleet->getRemainingCargo() : 0; }

    // Déclarations des méthodes manquantes
    void addToInventory(int itemId, int quantity);
    void removeFromInventory(int itemId, int quantity);
    int getItemQuantity(int itemId) const;
    bool addToCargo(int goodId, const std::string &name, int quantity, int unitWeight);
    bool removeFromCargo(int goodId, int quantity);
    int getCargoQuantity(int goodId) const;
    int getTotalCargoWeight() const;

    // Getters pour les compétences commerciales
    const TradeSkills &getTradeSkills() const { return m_tradeSkills; }
    const AdmiraltySkills &getAdmiraltySkills() const { return m_admiraltySkills; }
    float getCityReputation(int cityId) const;
    float getKingdomReputation(const std::string &kingdom) const;
    const std::vector<TradeRoute> &getTradeRoutes() const { return m_tradeRoutes; }

    // Setters pour les compétences commerciales
    void improveTradeSkill(const std::string &skillName, int amount);
    void improveAdmiraltySkill(const std::string &skillName, int amount);
    void setCityReputation(int cityId, float reputation);
    void setKingdomReputation(const std::string &kingdom, float reputation);

    // Setters
    void setName(const std::string &name) { m_name = name; }
    void setGold(int gold) { m_gold = gold; }
    void addGold(int amount) { m_gold += amount; }
    bool removeGold(int amount)
    {
        m_gold = std::max(0, m_gold - amount);
        return m_gold > 0;
    }
    void setLevel(int level) { m_level = level; }
    void addExperience(int experience) { m_experience += experience; }
    void addDebt(int amount) { m_debt += amount; }
    bool payDebt(int amount)
    {
        m_gold = std::max(0, m_gold - amount);
        return m_gold > 0;
    }

    // Méthodes pour les routes commerciales
    int addTradeRoute(const TradeRoute &route);
    bool removeTradeRoute(size_t index);
    void updateCityReputation(int cityId, float amount);
    void updateKingdomReputation(const std::string &kingdom, float amount);

    // Gestion de la dette
    float getDebtInterestRate() const { return m_debtInterestRate; }
    int getLastDebtPaymentDay() const { return m_lastDebtPaymentDay; }

    // Méthodes pour la gestion de la flotte
    int getFleetSpeed() const { return m_fleet ? m_fleet->getSpeed() * (1.0f + m_admiraltySkills.getNavigationBonus()) : 0; }
    int getFleetVisibility() const { return m_fleet ? m_fleet->getVisibilityRange() * (1.0f + m_admiraltySkills.getNavigationBonus()) : 0; }
    float getFleetMorale() const { return m_fleet ? m_fleet->getMorale() * (1.0f + m_admiraltySkills.getLeadershipBonus()) : 0.0f; }
    float getFormationBonus(const std::string &bonusType) const
    {
        if (!m_fleet)
            return 0.0f;

        float baseBonus = m_fleet->getFormationBonus(bonusType);
        float tacticsBonus = m_admiraltySkills.getTacticsBonus();

        return baseBonus * (1.0f + tacticsBonus);
    }

    // Simulation d'une journée
    void simulateDay(bool inCombat = false)
    {
        if (m_fleet)
        {
            // Simuler une journée pour la flotte
            m_fleet->simulateDay(inCombat);

            // Bonus de leadership pour le moral
            if (m_admiraltySkills.leadership > 0)
            {
                m_fleet->adjustMorale(m_admiraltySkills.leadership * 0.2f);
            }

            // Bonus de gestion d'équipage pour la fatigue
            if (m_admiraltySkills.crewManagement > 0)
            {
                m_fleet->restCrew(m_admiraltySkills.crewManagement * 0.5f);
            }
        }
    }
};