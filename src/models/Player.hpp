#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <algorithm> // Pour std::min, std::max, std::clamp
#include <nlohmann/json.hpp>
#include "Ship.hpp"
#include "Fleet.hpp"
#include "TradeGood.hpp"
#include "Character.hpp"

namespace Orenji
{

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

        // Ajout d'un constructeur par défaut pour résoudre les problèmes de compilation
        CargoItem() : id(0), name(""), quantity(0), unitWeight(0) {}

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
        int bargaining = 0;      // Négociation (meilleurs prix)
        int marketKnowledge = 0; // Connaissance des marchés (meilleures opportunités)
        int networking = 0;      // Réseau (accès à des marchés spéciaux)
        int logistics = 0;       // Logistique (plus de marchandises transportables)
        int contraband = 0;      // Contrebande (commerce de biens illégaux)

        // Calcule le bonus de prix en fonction des compétences
        float getPriceBonus() const
        {
            return 0.01f * bargaining;
        }

        // Calcule le bonus de quantité disponible
        float getQuantityBonus() const
        {
            return 0.02f * marketKnowledge;
        }

        // Calcule le bonus de capacité de transport
        float getCargoBonus() const
        {
            return 0.05f * logistics;
        }

        // Calcule la probabilité de trouver des marchandises rares
        float getRareGoodChance() const
        {
            return 0.02f * networking;
        }

        // Calcule le bonus pour la contrebande
        float getContrabandBonus() const
        {
            return 0.03f * contraband;
        }
    };

    /**
     * Structure pour les compétences d'amirauté
     */
    struct AdmiraltySkills
    {
        int navigation = 0;     // Navigation (vitesse et manœuvrabilité)
        int gunnery = 0;        // Artillerie (précision et puissance)
        int tactics = 0;        // Tactiques (avantages en combat)
        int leadership = 0;     // Commandement (moral et cohésion)
        int engineering = 0;    // Ingénierie (réparations et améliorations)
        int crewManagement = 0; // Gestion d'équipage (fatigue et efficacité)

        // Bonus de navigation
        float getNavigationBonus() const
        {
            return 0.02f * navigation;
        }

        // Bonus d'artillerie
        float getGunneryBonus() const
        {
            return 0.02f * gunnery;
        }

        // Bonus tactique
        float getTacticalBonus() const
        {
            return 0.01f * tactics;
        }

        // Bonus de commandement
        float getLeadershipBonus() const
        {
            return 0.01f * leadership;
        }

        // Bonus d'ingénierie
        float getEngineeringBonus() const
        {
            return 0.02f * engineering;
        }

        // Bonus de gestion d'équipage
        float getCrewManagementBonus() const
        {
            return 0.02f * crewManagement;
        }
    };

    /**
     * Statistiques du joueur
     */
    struct PlayerStats
    {
        int citiesVisited = 0;    // Nombre de villes visitées
        int transactionsMade = 0; // Nombre de transactions commerciales
        int shipsDestroyed = 0;   // Nombre de navires ennemis détruits
        int distanceTraveled = 0; // Distance parcourue (en miles)
        int goodsTraded = 0;      // Quantité de marchandises échangées
        int battlesWon = 0;       // Nombre de batailles gagnées
        int battlesLost = 0;      // Nombre de batailles perdues
    };

    /**
     * Classe représentant un joueur
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
        std::shared_ptr<Orenji::Character> m_character;

        // Flotte du joueur (remplace le navire unique)
        std::shared_ptr<Orenji::Fleet> m_fleet;

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
              m_fleet(std::make_shared<Orenji::Fleet>("Fleet")),
              m_posX(0), m_posY(0), m_currentMode(PlayerMode::LAND) {}

        /**
         * Constructeur avec données de base
         */
        Player(const std::string &name, int gold = 1000, int level = 1, int experience = 0)
            : m_id(0), m_name(name), m_gold(gold), m_debt(0), m_level(level), m_experience(experience),
              m_cargoCapacity(100), m_currentCargoWeight(0), m_fleet(std::make_shared<Orenji::Fleet>(name + "'s Fleet")),
              m_posX(0), m_posY(0), m_currentMode(PlayerMode::LAND) {}

        /**
         * Constructeur avec personnage
         */
        Player(const std::string &name, std::shared_ptr<Orenji::Character> character, int gold = 1000)
            : m_id(0), m_name(name), m_gold(gold), m_debt(0), m_level(1), m_experience(0),
              m_cargoCapacity(100), m_currentCargoWeight(0), m_fleet(std::make_shared<Orenji::Fleet>(name + "'s Fleet")),
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
        bool canAddCargo(int goodId, int quantity, int unitWeight) const;

        /**
         * Ajoute une marchandise à l'inventaire du joueur
         * @param goodId ID de la marchandise
         * @param name Nom de la marchandise
         * @param quantity Quantité à ajouter
         * @param unitWeight Poids unitaire
         * @return true si l'ajout a réussi
         */
        bool addCargo(int goodId, const std::string &name, int quantity, int unitWeight);

        /**
         * Retire une marchandise de l'inventaire du joueur
         * @param goodId ID de la marchandise
         * @param quantity Quantité à retirer
         * @return true si le retrait a réussi
         */
        bool removeCargo(int goodId, int quantity);

        // Accesseurs pour la position
        int getX() const { return m_posX; }
        int getY() const { return m_posY; }
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

        // Déplacement
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
        void setCharacter(std::shared_ptr<Orenji::Character> character)
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
        void setFleet(std::shared_ptr<Orenji::Fleet> fleet)
        {
            m_fleet = fleet;
            if (fleet)
            {
                // Synchroniser la position de la flotte avec celle du joueur
                fleet->setPosition(m_posX, m_posY);
            }
        }

        // Ajouter/supprimer des navires à la flotte
        bool addShipToFleet(std::shared_ptr<Orenji::Ship> ship)
        {
            if (!m_fleet)
            {
                m_fleet = std::make_shared<Orenji::Fleet>(m_name + "'s Fleet");
            }
            return m_fleet->addShip(ship);
        }

        bool removeShipFromFleet(size_t index)
        {
            return m_fleet ? m_fleet->removeShip(index) : false;
        }

        std::shared_ptr<Orenji::Ship> getShipFromFleet(size_t index) const
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
            m_fleet = std::make_shared<Orenji::Fleet>(name);
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
            // Vitesse de base en fonction du mode + bonus de compétence
            return (m_currentMode == PlayerMode::LAND) ? (5 + (m_character ? m_character->getNavigation() / 5 : 0)) : (m_fleet ? m_fleet->getSpeed() : 0);
        }

        int getManeuverability() const
        {
            if (m_currentMode == PlayerMode::LAND)
            {
                // En mode terrestre, la manœuvrabilité dépend des compétences du personnage
                return 10 + (m_character ? m_character->getParam(2) / 10 : 0);
            }
            else
            {
                // En mode maritime, elle dépend du navire actif
                auto flagship = getFlagship();
                return flagship ? flagship->getManeuverability() : 0;
            }
        }

        int getDurability() const
        {
            if (m_currentMode == PlayerMode::LAND)
            {
                // En mode terrestre, la durabilité est liée à la santé du personnage
                return m_character ? m_character->getHealth() : 100;
            }
            else
            {
                // En mode maritime, elle dépend de l'intégrité du navire
                auto flagship = getFlagship();
                return flagship ? flagship->getDurability() : 0;
            }
        }

        int getCombatPower() const
        {
            if (m_currentMode == PlayerMode::LAND)
            {
                // En mode terrestre, la puissance de combat dépend des compétences du personnage
                return m_character ? m_character->getCombat() * 2 : 10;
            }
            else
            {
                // En mode maritime, elle dépend de la puissance du navire
                auto flagship = getFlagship();
                return flagship ? flagship->getCombatPower() : 0;
            }
        }

        int getTotalCargoCapacity() const { return m_fleet ? m_fleet->getTotalCargo() : 0; }
        int getUsedCargo() const { return m_fleet ? m_fleet->getUsedCargo() : 0; }
        int getRemainingCargoCapacity() const { return m_fleet ? m_fleet->getRemainingCargo() : 0; }

        // Méthodes pour les compétences
        const TradeSkills &getTradeSkills() const { return m_tradeSkills; }
        const AdmiraltySkills &getAdmiraltySkills() const { return m_admiraltySkills; }

        // Méthodes spéciales pour les attributs selon le mode
        std::shared_ptr<Orenji::Ship> getFlagship() const
        {
            return m_fleet ? m_fleet->getFlagship() : nullptr;
        }

        const std::shared_ptr<Orenji::Character> &getCharacter() const { return m_character; }
        const std::shared_ptr<Orenji::Fleet> &getFleet() const { return m_fleet; }

        // Statistiques sur les villes et royaumes
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
            if (m_gold < amount)
                return false;
            m_gold -= amount;
            return true;
        }
        void setLevel(int level) { m_level = level; }
        void addExperience(int experience);
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
            // Appliquer les bonus de compétence appropriés
            if (bonusType == "combat")
                return baseBonus * (1.0f + m_admiraltySkills.getTacticalBonus());
            if (bonusType == "speed")
                return baseBonus * (1.0f + m_admiraltySkills.getNavigationBonus());
            return baseBonus;
        }

        // Simulation d'une journée
        void simulateDay(bool inCombat = false)
        {
            // Si en mode maritime, simuler la flotte
            if (m_currentMode == PlayerMode::SEA && m_fleet)
            {
                m_fleet->simulateDay(inCombat);

                // Bonus du joueur
                if (m_admiraltySkills.leadership > 0)
                {
                    m_fleet->adjustMorale(m_admiraltySkills.leadership * 0.2f);
                }

                // Gestion d'équipage améliore la récupération de fatigue
                if (m_admiraltySkills.crewManagement > 0)
                {
                    m_fleet->restCrew(m_admiraltySkills.crewManagement * 0.5f);
                }
            }

            // TODO: Simuler d'autres aspects (consommation de ressources, etc.)
        }
    };

} // namespace Orenji
