#include "TradeGood.hpp"
#include "../../include/utils/MathUtils.hpp"
#include <algorithm>

namespace Orenji
{
    TradeGood::TradeGood()
        : m_id(-1), m_name(""), m_description(""), m_type(GoodType::OTHER), m_basePrice(0), m_weight(0.0f), m_rarity(5), m_isContraband(false), m_volatility(0.1f), m_minPrice(0), m_maxPrice(0)
    {
    }

    TradeGood::TradeGood(int id, const std::string &name, const std::string &description,
                         GoodType type, int basePrice, float weight, int rarity,
                         bool isContraband)
        : m_id(id), m_name(name), m_description(description), m_type(type), m_basePrice(basePrice), m_weight(weight), m_rarity(rarity), m_isContraband(isContraband), m_volatility(0.1f + (rarity * 0.02f)) // Plus la marchandise est rare, plus le prix est volatile
          ,
          m_minPrice(static_cast<int>(basePrice * 0.5f)), m_maxPrice(static_cast<int>(basePrice * 2.0f))
    {
    }

    TradeGood::TradeGood(const nlohmann::json &json)
        : m_id(-1), m_name(""), m_description(""), m_type(GoodType::OTHER), m_basePrice(0), m_weight(0.0f), m_rarity(5), m_isContraband(false), m_volatility(0.1f), m_minPrice(0), m_maxPrice(0)
    {
        // Charger les propriétés de base à partir du JSON
        if (json.contains("id") && json["id"].is_number())
            m_id = json["id"];

        if (json.contains("name") && json["name"].is_string())
            m_name = json["name"];

        if (json.contains("description") && json["description"].is_string())
            m_description = json["description"];

        if (json.contains("type") && json["type"].is_string())
            m_type = stringToGoodType(json["type"]);

        if (json.contains("basePrice") && json["basePrice"].is_number())
            m_basePrice = json["basePrice"];

        if (json.contains("weight") && json["weight"].is_number())
            m_weight = json["weight"];

        if (json.contains("rarity") && json["rarity"].is_number())
            m_rarity = json["rarity"];

        if (json.contains("isContraband") && json["isContraband"].is_boolean())
            m_isContraband = json["isContraband"];

        if (json.contains("volatility") && json["volatility"].is_number())
            m_volatility = json["volatility"];
        else
            m_volatility = 0.1f + (m_rarity * 0.02f); // Par défaut basé sur la rareté

        if (json.contains("minPrice") && json["minPrice"].is_number())
            m_minPrice = json["minPrice"];
        else
            m_minPrice = static_cast<int>(m_basePrice * 0.5f); // Par défaut 50% du prix de base

        if (json.contains("maxPrice") && json["maxPrice"].is_number())
            m_maxPrice = json["maxPrice"];
        else
            m_maxPrice = static_cast<int>(m_basePrice * 2.0f); // Par défaut 200% du prix de base

        // Charger les producteurs
        if (json.contains("producers") && json["producers"].is_array())
        {
            for (const auto &producer : json["producers"])
            {
                if (producer.is_string())
                    m_producers.push_back(producer);
            }
        }

        // Charger les consommateurs
        if (json.contains("consumers") && json["consumers"].is_array())
        {
            for (const auto &consumer : json["consumers"])
            {
                if (consumer.is_string())
                    m_consumers.push_back(consumer);
            }
        }
    }

    nlohmann::json TradeGood::toJson() const
    {
        nlohmann::json json;

        // Propriétés de base
        json["id"] = m_id;
        json["name"] = m_name;
        json["description"] = m_description;
        json["type"] = goodTypeToString(m_type);
        json["basePrice"] = m_basePrice;
        json["weight"] = m_weight;
        json["rarity"] = m_rarity;
        json["isContraband"] = m_isContraband;
        json["volatility"] = m_volatility;
        json["minPrice"] = m_minPrice;
        json["maxPrice"] = m_maxPrice;

        // Producteurs
        nlohmann::json producers = nlohmann::json::array();
        for (const auto &producer : m_producers)
        {
            producers.push_back(producer);
        }
        json["producers"] = producers;

        // Consommateurs
        nlohmann::json consumers = nlohmann::json::array();
        for (const auto &consumer : m_consumers)
        {
            consumers.push_back(consumer);
        }
        json["consumers"] = consumers;

        return json;
    }

    void TradeGood::addProducer(const std::string &producer)
    {
        // Vérifier si le producteur existe déjà
        if (std::find(m_producers.begin(), m_producers.end(), producer) == m_producers.end())
        {
            m_producers.push_back(producer);
        }
    }

    void TradeGood::addConsumer(const std::string &consumer)
    {
        // Vérifier si le consommateur existe déjà
        if (std::find(m_consumers.begin(), m_consumers.end(), consumer) == m_consumers.end())
        {
            m_consumers.push_back(consumer);
        }
    }

    bool TradeGood::isProducedBy(const std::string &cityType) const
    {
        return std::find(m_producers.begin(), m_producers.end(), cityType) != m_producers.end();
    }

    bool TradeGood::isConsumedBy(const std::string &cityType) const
    {
        return std::find(m_consumers.begin(), m_consumers.end(), cityType) != m_consumers.end();
    }

    int TradeGood::generateRandomPrice() const
    {
        // Générer un prix aléatoire dans les limites définies
        return MathUtils::randInt(m_minPrice, m_maxPrice);
    }

    GoodType TradeGood::stringToGoodType(const std::string &typeStr)
    {
        if (typeStr == "RAW_MATERIAL")
            return GoodType::RAW_MATERIAL;
        else if (typeStr == "FOOD")
            return GoodType::FOOD;
        else if (typeStr == "MANUFACTURED")
            return GoodType::MANUFACTURED;
        else if (typeStr == "LUXURY")
            return GoodType::LUXURY;
        else if (typeStr == "MILITARY")
            return GoodType::MILITARY;
        else if (typeStr == "NAVAL")
            return GoodType::NAVAL;
        else
            return GoodType::OTHER;
    }

    std::string TradeGood::goodTypeToString(GoodType type)
    {
        switch (type)
        {
        case GoodType::RAW_MATERIAL:
            return "RAW_MATERIAL";
        case GoodType::FOOD:
            return "FOOD";
        case GoodType::MANUFACTURED:
            return "MANUFACTURED";
        case GoodType::LUXURY:
            return "LUXURY";
        case GoodType::MILITARY:
            return "MILITARY";
        case GoodType::NAVAL:
            return "NAVAL";
        case GoodType::OTHER:
        default:
            return "OTHER";
        }
    }

} // namespace Orenji
