#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace Orenji
{
    /**
     * @brief Énumération des types de marchandises
     */
    enum class GoodType
    {
        RAW_MATERIAL, // Matière première (minerai, bois, etc.)
        FOOD,         // Nourriture (blé, viande, poisson, etc.)
        MANUFACTURED, // Produit manufacturé (vêtements, outils, etc.)
        LUXURY,       // Produit de luxe (bijoux, parfums, etc.)
        MILITARY,     // Équipement militaire (armes, armures, etc.)
        NAVAL,        // Équipement naval (voiles, cordes, etc.)
        OTHER         // Autre type
    };

    /**
     * @brief Classe représentant une marchandise commerciale
     *
     * Cette classe représente une marchandise qui peut être achetée,
     * vendue et transportée dans le jeu.
     */
    class TradeGood
    {
    private:
        int m_id;                             // Identifiant unique de la marchandise
        std::string m_name;                   // Nom de la marchandise
        std::string m_description;            // Description détaillée
        GoodType m_type;                      // Type de marchandise
        int m_basePrice;                      // Prix de base
        float m_weight;                       // Poids par unité
        int m_rarity;                         // Rareté (1-10, 10 étant le plus rare)
        bool m_isContraband;                  // Indique si c'est une contrebande
        std::vector<std::string> m_producers; // Liste des types de villes qui produisent cette marchandise
        std::vector<std::string> m_consumers; // Liste des types de villes qui consomment cette marchandise

        // Attributs pour la gestion des prix et de la disponibilité
        float m_volatility; // Volatilité du prix (0.0-1.0)
        int m_minPrice;     // Prix minimum
        int m_maxPrice;     // Prix maximum

    public:
        /**
         * @brief Constructeur par défaut
         */
        TradeGood();

        /**
         * @brief Constructeur avec paramètres
         *
         * @param id Identifiant unique
         * @param name Nom de la marchandise
         * @param description Description détaillée
         * @param type Type de marchandise
         * @param basePrice Prix de base
         * @param weight Poids par unité
         * @param rarity Rareté (1-10)
         * @param isContraband Indique si c'est une contrebande
         */
        TradeGood(int id, const std::string &name, const std::string &description,
                  GoodType type, int basePrice, float weight, int rarity = 5,
                  bool isContraband = false);

        /**
         * @brief Constructeur à partir d'un objet JSON
         *
         * @param json Objet JSON contenant les données de la marchandise
         */
        TradeGood(const nlohmann::json &json);

        /**
         * @brief Destructeur
         */
        ~TradeGood() = default;

        /**
         * @brief Convertit l'objet en JSON
         *
         * @return Objet JSON représentant la marchandise
         */
        nlohmann::json toJson() const;

        // Getters
        int getId() const { return m_id; }
        const std::string &getName() const { return m_name; }
        const std::string &getDescription() const { return m_description; }
        GoodType getType() const { return m_type; }
        int getBasePrice() const { return m_basePrice; }
        float getWeight() const { return m_weight; }
        int getRarity() const { return m_rarity; }
        bool isContraband() const { return m_isContraband; }
        float getVolatility() const { return m_volatility; }
        int getMinPrice() const { return m_minPrice; }
        int getMaxPrice() const { return m_maxPrice; }

        // Obtient la liste des producteurs
        const std::vector<std::string> &getProducers() const { return m_producers; }

        // Obtient la liste des consommateurs
        const std::vector<std::string> &getConsumers() const { return m_consumers; }

        // Setters
        void setId(int id) { m_id = id; }
        void setName(const std::string &name) { m_name = name; }
        void setDescription(const std::string &description) { m_description = description; }
        void setType(GoodType type) { m_type = type; }
        void setBasePrice(int price) { m_basePrice = price; }
        void setWeight(float weight) { m_weight = weight; }
        void setRarity(int rarity) { m_rarity = rarity; }
        void setContraband(bool isContraband) { m_isContraband = isContraband; }
        void setVolatility(float volatility) { m_volatility = volatility; }
        void setMinPrice(int minPrice) { m_minPrice = minPrice; }
        void setMaxPrice(int maxPrice) { m_maxPrice = maxPrice; }

        /**
         * @brief Ajoute un type de ville productrice
         *
         * @param producer Type de ville à ajouter comme producteur
         */
        void addProducer(const std::string &producer);

        /**
         * @brief Ajoute un type de ville consommatrice
         *
         * @param consumer Type de ville à ajouter comme consommateur
         */
        void addConsumer(const std::string &consumer);

        /**
         * @brief Vérifie si un type de ville est producteur de cette marchandise
         *
         * @param cityType Type de ville à vérifier
         * @return true si le type de ville est producteur, false sinon
         */
        bool isProducedBy(const std::string &cityType) const;

        /**
         * @brief Vérifie si un type de ville est consommateur de cette marchandise
         *
         * @param cityType Type de ville à vérifier
         * @return true si le type de ville est consommateur, false sinon
         */
        bool isConsumedBy(const std::string &cityType) const;

        /**
         * @brief Calcule un prix aléatoire dans les limites définies
         *
         * @return Prix aléatoire
         */
        int generateRandomPrice() const;

        /**
         * @brief Convertit une chaîne de caractères en GoodType
         *
         * @param typeStr Chaîne représentant le type
         * @return GoodType correspondant
         */
        static GoodType stringToGoodType(const std::string &typeStr);

        /**
         * @brief Convertit un GoodType en chaîne de caractères
         *
         * @param type Type à convertir
         * @return Chaîne représentant le type
         */
        static std::string goodTypeToString(GoodType type);
    };

} // namespace Orenji