#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class TradeGood
{
private:
    int m_id;
    std::string m_name;
    std::string m_description;
    int m_basePrice;
    int m_weight;
    std::string m_category;

    // Nouveaux champs
    float m_demandFactor;
    std::vector<std::string> m_specialtyRegions;

public:
    // Constructeur par défaut
    TradeGood()
        : m_id(0), m_basePrice(0), m_weight(0), m_demandFactor(1.0f) {}

    // Constructeur à partir de JSON
    TradeGood(const nlohmann::json &goodData);

    // Conversion vers JSON
    nlohmann::json toJson() const;

    // Getters
    int getId() const { return m_id; }
    const std::string &getName() const { return m_name; }
    const std::string &getDescription() const { return m_description; }
    int getBasePrice() const { return m_basePrice; }
    int getWeight() const { return m_weight; }
    const std::string &getCategory() const { return m_category; }
    float getDemandFactor() const { return m_demandFactor; }
    const std::vector<std::string> &getSpecialtyRegions() const { return m_specialtyRegions; }

    // Vérifier si une région est une spécialité de cette marchandise
    bool isSpecialtyRegion(const std::string &region) const
    {
        for (const auto &r : m_specialtyRegions)
        {
            if (r == region)
                return true;
        }
        return false;
    }

    // Calcul du prix en fonction du marché (facteur de multiplicateur)
    int calculatePrice(float marketFactor) const
    {
        return static_cast<int>(m_basePrice * marketFactor);
    }
};