#include "../../src/models/City.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>

// Constructeur à partir de données JSON
City::City(const nlohmann::json &data)
{
    try
    {
        m_id = data.at("id").get<int>();
        m_name = data.at("name").get<std::string>();
        m_type = data.at("type").get<std::string>();

        m_x = data.at("x").get<int>();
        m_y = data.at("y").get<int>();
        m_region = data.at("region").get<std::string>();

        m_population = data.at("population").get<int>();
        m_defenseLevel = data.at("defenseLevel").get<int>();
        m_wealth = data.at("wealth").get<int>();
        m_influence = data.at("influence").get<int>();
        m_stability = data.at("stability").get<int>();

        m_portCapacity = data.at("portCapacity").get<int>();
        m_tradingVolume = data.at("tradingVolume").get<int>();
        m_taxRate = data.at("taxRate").get<int>();

        m_faction = data.at("faction").get<std::string>();
        m_loyaltyLevel = data.at("loyaltyLevel").get<int>();

        m_marketModifier = data.at("marketModifier").get<float>();

        m_hasShipyard = data.at("hasShipyard").get<bool>();
        m_hasBank = data.at("hasBank").get<bool>();
        m_hasGuild = data.at("hasGuild").get<bool>();
        m_hasMarketplace = data.at("hasMarketplace").get<bool>();
        m_hasTavern = data.at("hasTavern").get<bool>();
        m_hasBarracks = data.at("hasBarracks").get<bool>();

        m_description = data.at("description").get<std::string>();

        // Charger les ressources
        if (data.contains("resources") && data["resources"].is_array())
        {
            for (const auto &resource : data["resources"])
            {
                int id = resource["id"].get<int>();
                std::string name = resource["name"].get<std::string>();
                int amount = resource["amount"].get<int>();
                m_resources[id] = std::make_pair(name, amount);
            }
        }

        // Charger les prix des marchandises
        if (data.contains("prices") && data["prices"].is_object())
        {
            for (auto it = data["prices"].begin(); it != data["prices"].end(); ++it)
            {
                int goodId = std::stoi(it.key());
                int price = it.value().get<int>();
                m_prices[goodId] = price;
            }
        }

        // Charger la disponibilité des marchandises
        if (data.contains("availableGoods") && data["availableGoods"].is_object())
        {
            for (auto it = data["availableGoods"].begin(); it != data["availableGoods"].end(); ++it)
            {
                int goodId = std::stoi(it.key());
                int quantity = it.value().get<int>();
                m_availableGoods[goodId] = quantity;
            }
        }

        // Charger les navires disponibles
        if (data.contains("availableShips") && data["availableShips"].is_array())
        {
            for (const auto &shipId : data["availableShips"])
            {
                m_availableShips.push_back(shipId.get<int>());
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error parsing City JSON: " << e.what() << std::endl;
        // Initialiser avec des valeurs par défaut en cas d'erreur
        *this = City();
    }
}

// Conversion en JSON pour sauvegarde
nlohmann::json City::toJson() const
{
    nlohmann::json data;

    data["id"] = m_id;
    data["name"] = m_name;
    data["type"] = m_type;

    data["x"] = m_x;
    data["y"] = m_y;
    data["region"] = m_region;

    data["population"] = m_population;
    data["defenseLevel"] = m_defenseLevel;
    data["wealth"] = m_wealth;
    data["influence"] = m_influence;
    data["stability"] = m_stability;

    data["portCapacity"] = m_portCapacity;
    data["tradingVolume"] = m_tradingVolume;
    data["taxRate"] = m_taxRate;

    data["faction"] = m_faction;
    data["loyaltyLevel"] = m_loyaltyLevel;

    data["marketModifier"] = m_marketModifier;

    data["hasShipyard"] = m_hasShipyard;
    data["hasBank"] = m_hasBank;
    data["hasGuild"] = m_hasGuild;
    data["hasMarketplace"] = m_hasMarketplace;
    data["hasTavern"] = m_hasTavern;
    data["hasBarracks"] = m_hasBarracks;

    data["description"] = m_description;

    // Sauvegarder les ressources
    nlohmann::json resources = nlohmann::json::array();
    for (const auto &resource : m_resources)
    {
        nlohmann::json res;
        res["id"] = resource.first;
        res["name"] = resource.second.first;
        res["amount"] = resource.second.second;
        resources.push_back(res);
    }
    data["resources"] = resources;

    // Sauvegarder les prix
    nlohmann::json prices = nlohmann::json::object();
    for (const auto &price : m_prices)
    {
        prices[std::to_string(price.first)] = price.second;
    }
    data["prices"] = prices;

    // Sauvegarder la disponibilité
    nlohmann::json availableGoods = nlohmann::json::object();
    for (const auto &good : m_availableGoods)
    {
        availableGoods[std::to_string(good.first)] = good.second;
    }
    data["availableGoods"] = availableGoods;

    // Sauvegarder les navires disponibles
    data["availableShips"] = m_availableShips;

    return data;
}

// Obtenir le prix d'une marchandise
int City::getGoodPrice(int goodId) const
{
    auto it = m_prices.find(goodId);
    if (it != m_prices.end())
    {
        // Appliquer le modificateur de marché
        return static_cast<int>(it->second * m_marketModifier);
    }
    return 0; // Marchandise non disponible
}

// Obtenir la quantité disponible d'une marchandise
int City::getGoodAvailability(int goodId) const
{
    auto it = m_availableGoods.find(goodId);
    return (it != m_availableGoods.end()) ? it->second : 0;
}

// Vérifier si un modèle de navire est disponible
bool City::isShipAvailable(int shipModelId) const
{
    return std::find(m_availableShips.begin(), m_availableShips.end(), shipModelId) != m_availableShips.end();
}

// Obtenir la quantité d'une ressource spécifique
int City::getResourceAmount(int resourceId) const
{
    auto it = m_resources.find(resourceId);
    return (it != m_resources.end()) ? it->second.second : 0;
}

// Achat d'une marchandise
bool City::buyGood(int goodId, int quantity)
{
    // Vérifier la disponibilité
    auto it = m_availableGoods.find(goodId);
    if (it == m_availableGoods.end() || it->second < quantity)
    {
        return false; // Quantité insuffisante
    }

    // Mettre à jour la disponibilité
    it->second -= quantity;

    // Si la quantité tombe à zéro, supprimer l'entrée
    if (it->second <= 0)
    {
        m_availableGoods.erase(it);
    }

    // Augmenter légèrement la richesse de la ville
    m_wealth += (getGoodPrice(goodId) * quantity) / 10;

    return true;
}

// Vente d'une marchandise
bool City::sellGood(int goodId, int quantity)
{
    // Obtenir le prix actuel ou définir un prix par défaut
    int price = getGoodPrice(goodId);
    if (price == 0)
    {
        // Prix par défaut si la marchandise n'est pas encore négociée dans cette ville
        price = 10; // Valeur arbitraire, à ajuster selon le système économique
    }

    // Mettre à jour la disponibilité
    auto it = m_availableGoods.find(goodId);
    if (it != m_availableGoods.end())
    {
        it->second += quantity;
    }
    else
    {
        m_availableGoods[goodId] = quantity;
    }

    // Mettre à jour le prix si nécessaire
    if (m_prices.find(goodId) == m_prices.end())
    {
        m_prices[goodId] = price;
    }

    return true;
}

// Ajouter une ressource
void City::addResource(int resourceId, const std::string &name, int amount)
{
    auto it = m_resources.find(resourceId);
    if (it != m_resources.end())
    {
        it->second.second += amount;
    }
    else
    {
        m_resources[resourceId] = std::make_pair(name, amount);
    }
}

// Ajouter une marchandise disponible
void City::addAvailableGood(int goodId, int quantity, int price)
{
    if (quantity <= 0 || price <= 0)
        return;

    // Mettre à jour la disponibilité
    auto it = m_availableGoods.find(goodId);
    if (it != m_availableGoods.end())
    {
        it->second += quantity;
    }
    else
    {
        m_availableGoods[goodId] = quantity;
    }

    // Mettre à jour le prix
    m_prices[goodId] = price;
}

// Ajouter un navire disponible
void City::addAvailableShip(int shipModelId)
{
    if (std::find(m_availableShips.begin(), m_availableShips.end(), shipModelId) == m_availableShips.end())
    {
        m_availableShips.push_back(shipModelId);
    }
}

// Mettre à jour le marché
void City::updateMarket()
{
    // Générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.9f, 1.1f);

    // Fluctuation aléatoire des prix
    for (auto &price : m_prices)
    {
        float fluctuation = dis(gen);
        price.second = static_cast<int>(price.second * fluctuation);

        // S'assurer que le prix reste dans une fourchette raisonnable
        price.second = std::max(1, std::min(price.second, 1000));
    }

    // Réapprovisionnement des marchandises
    for (auto &good : m_availableGoods)
    {
        // Calculer l'approvisionnement basé sur le commerce et la population
        int baseSupply = m_tradingVolume / 10;
        std::uniform_int_distribution<int> supplyDis(0, baseSupply);
        int newSupply = supplyDis(gen);

        good.second += newSupply;

        // Limiter la quantité maximale disponible
        int maxSupply = m_population / 50;
        good.second = std::min(good.second, maxSupply);
    }

    // Mise à jour du modificateur global du marché
    std::uniform_real_distribution<float> modDis(0.95f, 1.05f);
    m_marketModifier *= modDis(gen);

    // Limiter le modificateur pour éviter des fluctuations extrêmes
    m_marketModifier = std::max(0.5f, std::min(m_marketModifier, 2.0f));
}

// Mettre à jour la population
void City::updatePopulation()
{
    // Taux de croissance basé sur la stabilité et la richesse
    float growthRate = (m_stability / 100.0f) * 0.01f;
    growthRate += (m_wealth / 10000.0f) * 0.005f;

    // Ajuster en fonction de la faction (bonus pour les villes contrôlées par le joueur)
    if (m_faction == "Player")
    {
        growthRate *= 1.1f;
    }

    // Calculer le changement de population
    int populationChange = static_cast<int>(m_population * growthRate);

    // Appliquer le changement
    m_population += populationChange;

    // Mettre à jour les capacités qui dépendent de la population
    m_portCapacity = m_population / 1000;
    m_tradingVolume = m_wealth / 5 + m_population / 20;
}

// Mettre à jour la richesse
void City::updateWealth(int delta)
{
    m_wealth += delta;

    // S'assurer que la richesse ne devient pas négative
    m_wealth = std::max(0, m_wealth);

    // Mettre à jour les valeurs qui dépendent de la richesse
    m_influence = m_wealth / 10;
    m_tradingVolume = m_wealth / 5 + m_population / 20;
}

// Construire un bâtiment/installation
bool City::buildFacility(const std::string &facilityType)
{
    bool success = false;

    if (facilityType == "Shipyard" && !m_hasShipyard)
    {
        if (m_wealth >= 5000 && m_population >= 1000)
        {
            m_hasShipyard = true;
            m_wealth -= 5000;
            success = true;
        }
    }
    else if (facilityType == "Bank" && !m_hasBank)
    {
        if (m_wealth >= 3000 && m_population >= 2000)
        {
            m_hasBank = true;
            m_wealth -= 3000;
            success = true;
        }
    }
    else if (facilityType == "Guild" && !m_hasGuild)
    {
        if (m_wealth >= 2000 && m_population >= 1500)
        {
            m_hasGuild = true;
            m_wealth -= 2000;
            success = true;
        }
    }
    else if (facilityType == "Marketplace" && !m_hasMarketplace)
    {
        if (m_wealth >= 1000 && m_population >= 500)
        {
            m_hasMarketplace = true;
            m_wealth -= 1000;
            success = true;
        }
    }
    else if (facilityType == "Tavern" && !m_hasTavern)
    {
        if (m_wealth >= 500 && m_population >= 200)
        {
            m_hasTavern = true;
            m_wealth -= 500;
            success = true;
        }
    }
    else if (facilityType == "Barracks" && !m_hasBarracks)
    {
        if (m_wealth >= 2500 && m_population >= 1000)
        {
            m_hasBarracks = true;
            m_wealth -= 2500;
            m_defenseLevel += 10;
            success = true;
        }
    }

    // Si la construction a réussi, mise à jour des valeurs de la ville
    if (success)
    {
        m_influence += 5;
        m_stability += 2;
    }

    return success;
}

// Appliquer un événement à la ville
void City::applyEvent(const std::string &eventType, int magnitude)
{
    magnitude = std::max(1, std::min(magnitude, 10)); // Limiter entre 1 et 10

    if (eventType == "Plague")
    {
        // Réduire la population
        int populationLoss = (m_population * magnitude) / 100;
        m_population -= populationLoss;
        m_stability -= magnitude;
    }
    else if (eventType == "Fire")
    {
        // Réduire la richesse
        int wealthLoss = (m_wealth * magnitude) / 50;
        m_wealth -= wealthLoss;
        m_stability -= magnitude / 2;
    }
    else if (eventType == "Festival")
    {
        // Augmenter la stabilité et le commerce
        m_stability += magnitude * 2;
        m_tradingVolume += (m_tradingVolume * magnitude) / 100;
    }
    else if (eventType == "GoodHarvest")
    {
        // Augmenter les ressources et la richesse
        for (auto &resource : m_resources)
        {
            resource.second.second += (resource.second.second * magnitude) / 100;
        }
        m_wealth += (m_wealth * magnitude) / 100;
        m_stability += magnitude;
    }
    else if (eventType == "BadHarvest")
    {
        // Réduire les ressources et la stabilité
        for (auto &resource : m_resources)
        {
            resource.second.second -= (resource.second.second * magnitude) / 100;
            resource.second.second = std::max(0, resource.second.second);
        }
        m_stability -= magnitude * 2;
    }

    // S'assurer que la stabilité reste dans la plage [0, 100]
    m_stability = std::max(0, std::min(m_stability, 100));
}

// Fonction de combat/siège
bool City::attackCity(int attackStrength)
{
    int defenseStrength = getEffectiveDefense();

    // Calculer le résultat du combat
    bool success = attackStrength > defenseStrength;

    // Appliquer les dégâts à la ville
    if (success)
    {
        // Ville prise d'assaut
        m_defenseLevel = std::max(0, m_defenseLevel - attackStrength / 10);
        m_population = static_cast<int>(m_population * 0.9); // 10% de pertes
        m_wealth = static_cast<int>(m_wealth * 0.8);         // 20% de pertes
        m_stability = std::max(0, m_stability - 30);
        m_loyaltyLevel = 10; // Faible loyauté après la prise
    }
    else
    {
        // Attaque repoussée
        m_defenseLevel = std::max(0, m_defenseLevel - attackStrength / 20);
        m_population = static_cast<int>(m_population * 0.95); // 5% de pertes
        m_wealth = static_cast<int>(m_wealth * 0.9);          // 10% de pertes
        m_stability = std::max(0, m_stability - 15);
        m_loyaltyLevel = std::min(100, m_loyaltyLevel + 5); // Légère augmentation de loyauté
    }

    return success;
}

// Fonction de calcul de défense effective
int City::getEffectiveDefense() const
{
    int baseDefense = m_defenseLevel;

    // Ajouter des bonus basés sur les bâtiments
    if (m_hasBarracks)
    {
        baseDefense += 15;
    }

    // Ajouter un bonus basé sur la population (1 point par tranche de 1000 habitants)
    baseDefense += m_population / 1000;

    // Ajuster en fonction de la stabilité
    float stabilityFactor = m_stability / 100.0f;
    baseDefense = static_cast<int>(baseDefense * (0.5f + 0.5f * stabilityFactor));

    return baseDefense;
}

// Fonctions d'influence
void City::increaseInfluence(int amount)
{
    m_influence += amount;

    // Augmenter légèrement la loyauté
    m_loyaltyLevel = std::min(100, m_loyaltyLevel + amount / 5);

    // Augmenter légèrement la stabilité
    m_stability = std::min(100, m_stability + amount / 10);
}

void City::decreaseInfluence(int amount)
{
    m_influence = std::max(0, m_influence - amount);

    // Diminuer légèrement la loyauté
    m_loyaltyLevel = std::max(0, m_loyaltyLevel - amount / 5);

    // Diminuer légèrement la stabilité
    m_stability = std::max(0, m_stability - amount / 10);
}
