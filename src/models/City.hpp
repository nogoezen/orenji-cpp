#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>

namespace Orenji
{
    /**
     * @brief Classe représentant une cité ou ville
     *
     * Cette classe contient toutes les propriétés et méthodes liées à une ville,
     * comme son type, sa population, ses ressources, son commerce, etc.
     */
    class City
    {
    private:
        // Identifiant unique
        int m_id;

        // Nom de la ville
        std::string m_name;

        // Type de ville (port, capitale, avant-poste, etc.)
        std::string m_type;

        // Localisation
        int m_x;              // Position X sur la carte
        int m_y;              // Position Y sur la carte
        std::string m_region; // Région ou continent

        // Caractéristiques
        int m_population;   // Population totale
        int m_defenseLevel; // Niveau de défense
        int m_wealth;       // Richesse/Économie
        int m_influence;    // Influence politique
        int m_stability;    // Stabilité sociale

        // Commerce
        int m_portCapacity;  // Capacité du port (nombre de navires)
        int m_tradingVolume; // Volume de commerce
        int m_taxRate;       // Taux de taxe (%)

        // Contrôle et allégeance
        std::string m_faction; // Faction qui contrôle la ville
        int m_loyaltyLevel;    // Niveau de loyauté envers la faction

        // Ressources principales produites
        std::map<int, std::pair<std::string, int>> m_resources; // ID -> (nom, quantité)

        // Prix des marchandises (ID marchandise -> prix unitaire)
        std::map<int, int> m_prices;

        // Disponibilité des marchandises (ID marchandise -> quantité disponible)
        std::map<int, int> m_availableGoods;

        // Navires disponibles à l'achat
        std::vector<int> m_availableShips; // IDs des modèles de navires

        // Description
        std::string m_description;

        // État du marché (modifieurs temporaires)
        float m_marketModifier;

        // Bâtiments spéciaux
        bool m_hasShipyard;
        bool m_hasBank;
        bool m_hasGuild;
        bool m_hasMarketplace;
        bool m_hasTavern;
        bool m_hasBarracks;

    public:
        /**
         * Constructeur par défaut
         */
        City()
            : m_id(-1), m_name("Unknown"), m_type("Village"),
              m_x(0), m_y(0), m_region("Unknown"),
              m_population(0), m_defenseLevel(0), m_wealth(0), m_influence(0), m_stability(50),
              m_portCapacity(0), m_tradingVolume(0), m_taxRate(10),
              m_faction("Independent"), m_loyaltyLevel(50),
              m_description(""),
              m_marketModifier(1.0f),
              m_hasShipyard(false), m_hasBank(false), m_hasGuild(false),
              m_hasMarketplace(false), m_hasTavern(false), m_hasBarracks(false) {}

        /**
         * Constructeur avec paramètres de base
         */
        City(int id, const std::string &name, const std::string &type,
             int x, int y, const std::string &region,
             int population, int defenseLevel, int wealth)
            : m_id(id), m_name(name), m_type(type),
              m_x(x), m_y(y), m_region(region),
              m_population(population), m_defenseLevel(defenseLevel), m_wealth(wealth),
              m_influence(wealth / 10), m_stability(50),
              m_portCapacity(population / 1000), m_tradingVolume(wealth / 5), m_taxRate(10),
              m_faction("Independent"), m_loyaltyLevel(50),
              m_description("A " + type + " in the region of " + region),
              m_marketModifier(1.0f),
              m_hasShipyard(false), m_hasBank(false), m_hasGuild(false),
              m_hasMarketplace(true), m_hasTavern(true), m_hasBarracks(false) {}

        /**
         * Constructeur à partir de données JSON
         */
        City(const nlohmann::json &data);

        /**
         * Destructeur
         */
        ~City() = default;

        /**
         * Conversion en JSON pour sauvegarde
         */
        nlohmann::json toJson() const;

        // Accesseurs
        int getId() const { return m_id; }
        const std::string &getName() const { return m_name; }
        const std::string &getType() const { return m_type; }

        int getX() const { return m_x; }
        int getY() const { return m_y; }
        const std::string &getRegion() const { return m_region; }

        int getPopulation() const { return m_population; }
        int getDefenseLevel() const { return m_defenseLevel; }
        int getWealth() const { return m_wealth; }
        int getInfluence() const { return m_influence; }
        int getStability() const { return m_stability; }

        int getPortCapacity() const { return m_portCapacity; }
        int getTradingVolume() const { return m_tradingVolume; }
        int getTaxRate() const { return m_taxRate; }

        const std::string &getFaction() const { return m_faction; }
        int getLoyaltyLevel() const { return m_loyaltyLevel; }

        float getMarketModifier() const { return m_marketModifier; }

        bool hasShipyard() const { return m_hasShipyard; }
        bool hasBank() const { return m_hasBank; }
        bool hasGuild() const { return m_hasGuild; }
        bool hasMarketplace() const { return m_hasMarketplace; }
        bool hasTavern() const { return m_hasTavern; }
        bool hasBarracks() const { return m_hasBarracks; }

        const std::string &getDescription() const { return m_description; }

        // Obtenir le prix d'une marchandise
        int getGoodPrice(int goodId) const;

        // Obtenir la quantité disponible d'une marchandise
        int getGoodAvailability(int goodId) const;

        // Vérifier si un modèle de navire est disponible
        bool isShipAvailable(int shipModelId) const;

        // Liste des ressources principales produites
        std::map<int, std::pair<std::string, int>> getResources() const { return m_resources; }

        // Obtenir la quantité d'une ressource spécifique
        int getResourceAmount(int resourceId) const;

        // Méthodes pour compatibilité avec le commerce
        bool hasGood(int goodId) const { return m_availableGoods.find(goodId) != m_availableGoods.end(); }
        int getGoodQuantity(int goodId) const { return getGoodAvailability(goodId); }
        bool removeGood(int goodId, int quantity) { return sellGood(goodId, quantity); }

        // Mutateurs
        void setName(const std::string &name) { m_name = name; }
        void setFaction(const std::string &faction) { m_faction = faction; }
        void setTaxRate(int rate) { m_taxRate = std::max(0, std::min(rate, 30)); } // Limité à 30%
        void setMarketModifier(float modifier) { m_marketModifier = std::max(0.5f, std::min(modifier, 2.0f)); }
        void setDefenseLevel(int level) { m_defenseLevel = std::max(0, level); }
        void setWealth(int wealth) { m_wealth = std::max(0, wealth); }
        void setPopulation(int population) { m_population = std::max(0, population); }
        void setLoyaltyLevel(int level) { m_loyaltyLevel = std::max(0, std::min(level, 100)); }

        // Fonctions d'achat/vente
        bool buyGood(int goodId, int quantity);
        bool sellGood(int goodId, int quantity);

        // Ajouter une ressource
        void addResource(int resourceId, const std::string &name, int amount);

        // Ajouter une marchandise disponible
        void addAvailableGood(int goodId, int quantity, int price);

        // Ajouter un navire disponible
        void addAvailableShip(int shipModelId);

        // Fonctions de mise à jour
        void updateMarket();
        void updatePopulation();
        void updateWealth(int delta);

        // Fonctions de construction
        bool buildFacility(const std::string &facilityType);

        // Événements
        void applyEvent(const std::string &eventType, int magnitude);

        // Fonction de combat/siège
        bool attackCity(int attackStrength);

        // Fonction de calcul de défense effective
        int getEffectiveDefense() const;

        // Fonctions d'influence
        void increaseInfluence(int amount);
        void decreaseInfluence(int amount);
    };
} // namespace Orenji