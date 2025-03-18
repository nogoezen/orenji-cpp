#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class TradeGood {
private:
    int m_id;
    std::string m_name;
    std::string m_description;
    int m_basePrice;
    int m_weight;
    std::string m_category;
    int m_rarity;
    std::vector<std::string> m_origin;

public:
    // Constructeur
    TradeGood(const nlohmann::json& goodData) {
        m_id = goodData["id"];
        m_name = goodData["name"];
        m_description = goodData["description"];
        m_basePrice = goodData["basePrice"];
        m_weight = goodData["weight"];
        m_category = goodData["category"];
        m_rarity = goodData["rarity"];
        
        // Charger les origines
        for (const auto& origin : goodData["origin"]) {
            m_origin.push_back(origin);
        }
    }
    
    // Getters
    int getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    const std::string& getDescription() const { return m_description; }
    int getBasePrice() const { return m_basePrice; }
    int getWeight() const { return m_weight; }
    const std::string& getCategory() const { return m_category; }
    int getRarity() const { return m_rarity; }
    const std::vector<std::string>& getOrigin() const { return m_origin; }
    
    // Calcul du prix en fonction du marché (facteur de multiplicateur)
    int calculatePrice(float marketFactor) const {
        return static_cast<int>(m_basePrice * marketFactor);
    }
}; 