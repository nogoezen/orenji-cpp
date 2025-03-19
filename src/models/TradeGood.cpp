#include "TradeGood.h"

// Constructeur à partir de JSON
TradeGood::TradeGood(const nlohmann::json &data)
    : m_id(-1), m_name("Unknown"), m_basePrice(0), m_description(""), m_category("")
{
    if (data.contains("id") && data["id"].is_number())
    {
        m_id = data["id"];
    }

    if (data.contains("name") && data["name"].is_string())
    {
        m_name = data["name"];
    }

    if (data.contains("basePrice") && data["basePrice"].is_number())
    {
        m_basePrice = data["basePrice"];
    }

    if (data.contains("description") && data["description"].is_string())
    {
        m_description = data["description"];
    }

    if (data.contains("category") && data["category"].is_string())
    {
        m_category = data["category"];
    }
}

// Conversion vers JSON
nlohmann::json TradeGood::toJson() const
{
    nlohmann::json data;
    data["id"] = m_id;
    data["name"] = m_name;
    data["basePrice"] = m_basePrice;
    data["description"] = m_description;
    data["category"] = m_category;
    return data;
}