#include "TradingSystem.h"
#include "../models/Player.h"
#include "../models/TradeGood.h"
#include "../data/GameData.h"
#include "World.h"
#include <iostream>
#include <random>
#include <ctime>

TradingSystem::TradingSystem()
{
    // L'initialisation complète se fait dans initialize()
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

bool TradingSystem::initialize(const nlohmann::json &saveData)
{
    try
    {
        // Si des données de sauvegarde sont fournies, les utiliser
        if (!saveData.is_null())
        {
            // Charger les prix depuis les données de sauvegarde
            if (saveData.contains("prices") && saveData["prices"].is_object())
            {
                for (auto cityIt = saveData["prices"].begin(); cityIt != saveData["prices"].end(); ++cityIt)
                {
                    int cityId = std::stoi(cityIt.key());

                    for (auto goodIt = cityIt.value().begin(); goodIt != cityIt.value().end(); ++goodIt)
                    {
                        int goodId = std::stoi(goodIt.key());

                        GoodPrice price;
                        price.goodId = goodId;
                        price.basePrice = goodIt.value()["basePrice"];
                        price.priceFactor = goodIt.value()["priceFactor"];
                        price.currentStock = goodIt.value()["currentStock"];

                        m_prices[cityId][goodId] = price;
                    }
                }
            }
        }
        else
        {
            // Initialiser les prix avec les valeurs par défaut
            initializePrices();
        }

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors de l'initialisation du système de commerce: " << e.what() << std::endl;
        return false;
    }
}

void TradingSystem::initializePrices()
{
    const auto &gameData = GameData::getInstance();

    // Pour chaque ville
    for (const auto &[cityId, city] : World().getAllCities())
    {
        // Pour chaque bien disponible dans la ville
        for (int goodId : city.availableGoods)
        {
            const TradeGood *good = gameData.findTradeGoodById(goodId);
            if (!good)
                continue;

            // Créer un prix pour ce bien dans cette ville
            GoodPrice price;
            price.goodId = goodId;
            price.basePrice = good->getBasePrice();
            price.priceFactor = calculatePriceFactor(cityId, goodId);
            price.currentStock = 50 + (std::rand() % 51); // Entre 50 et 100 unités

            m_prices[cityId][goodId] = price;
        }
    }
}

float TradingSystem::calculatePriceFactor(int cityId, int goodId) const
{
    // Cette méthode calculerait normalement le facteur de prix en fonction
    // de différents paramètres (région, rareté, événements, etc.)

    // Pour cet exemple, nous utilisons une valeur aléatoire entre 0.8 et 1.2
    float randomFactor = 0.8f + (static_cast<float>(std::rand()) / RAND_MAX) * 0.4f;

    return randomFactor;
}

nlohmann::json TradingSystem::toJson() const
{
    nlohmann::json j;

    // Sauvegarder les prix
    nlohmann::json prices;
    for (const auto &[cityId, cityPrices] : m_prices)
    {
        nlohmann::json cityData;

        for (const auto &[goodId, price] : cityPrices)
        {
            nlohmann::json priceData;
            priceData["basePrice"] = price.basePrice;
            priceData["priceFactor"] = price.priceFactor;
            priceData["currentStock"] = price.currentStock;

            cityData[std::to_string(goodId)] = priceData;
        }

        prices[std::to_string(cityId)] = cityData;
    }

    j["prices"] = prices;

    return j;
}

int TradingSystem::getGoodPrice(int cityId, int goodId) const
{
    auto cityIt = m_prices.find(cityId);
    if (cityIt == m_prices.end())
    {
        return -1; // Ville non trouvée
    }

    auto goodIt = cityIt->second.find(goodId);
    if (goodIt == cityIt->second.end())
    {
        return -1; // Bien non disponible dans cette ville
    }

    return goodIt->second.getCurrentPrice();
}

std::vector<GoodPrice> TradingSystem::getCityPrices(int cityId) const
{
    std::vector<GoodPrice> result;

    auto cityIt = m_prices.find(cityId);
    if (cityIt != m_prices.end())
    {
        for (const auto &[_, price] : cityIt->second)
        {
            result.push_back(price);
        }
    }

    return result;
}

bool TradingSystem::isGoodAvailable(int cityId, int goodId) const
{
    auto cityIt = m_prices.find(cityId);
    if (cityIt == m_prices.end())
    {
        return false;
    }

    auto goodIt = cityIt->second.find(goodId);
    return goodIt != cityIt->second.end() && goodIt->second.currentStock > 0;
}

bool TradingSystem::buyGood(Player &player, int cityId, int goodId, int quantity)
{
    // Vérifier si le bien est disponible
    if (!isGoodAvailable(cityId, goodId) || quantity <= 0)
    {
        return false;
    }

    // Vérifier le stock disponible
    auto &price = m_prices[cityId][goodId];
    if (price.currentStock < quantity)
    {
        return false;
    }

    // Calculer le coût total
    int unitPrice = price.getCurrentPrice();
    int totalCost = unitPrice * quantity;

    // Vérifier si le joueur a assez d'argent
    if (player.getGold() < totalCost)
    {
        return false;
    }

    // Vérifier si le joueur a assez de place dans son inventaire
    if (!player.canAddCargo(goodId, quantity))
    {
        return false;
    }

    // Effectuer la transaction
    player.removeGold(totalCost);
    player.addCargo(goodId, quantity);
    price.currentStock -= quantity;

    // Ajuster légèrement le prix à la hausse due à la demande
    price.priceFactor *= (1.0f + 0.01f * quantity / (price.currentStock + quantity));

    return true;
}

bool TradingSystem::sellGood(Player &player, int cityId, int goodId, int quantity)
{
    // Vérifier si le joueur possède le bien
    if (!player.hasCargo(goodId, quantity) || quantity <= 0)
    {
        return false;
    }

    // S'assurer que le prix est initialisé pour cette ville et ce bien
    if (m_prices.find(cityId) == m_prices.end() ||
        m_prices[cityId].find(goodId) == m_prices[cityId].end())
    {

        // Si le bien n'est pas vendu dans cette ville, créer un prix par défaut
        const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
        if (!good)
            return false;

        GoodPrice price;
        price.goodId = goodId;
        price.basePrice = good->getBasePrice();
        price.priceFactor = calculatePriceFactor(cityId, goodId);
        price.currentStock = 0;

        m_prices[cityId][goodId] = price;
    }

    // Calculer le prix de vente (généralement inférieur au prix d'achat)
    auto &price = m_prices[cityId][goodId];
    int unitPrice = static_cast<int>(price.getCurrentPrice() * 0.9f); // 90% du prix d'achat
    int totalValue = unitPrice * quantity;

    // Effectuer la transaction
    player.addGold(totalValue);
    player.removeCargo(goodId, quantity);
    price.currentStock += quantity;

    // Ajuster légèrement le prix à la baisse due à l'offre
    price.priceFactor *= (1.0f - 0.01f * quantity / (price.currentStock));

    return true;
}

void TradingSystem::advanceTime(int days)
{
    // Mettre à jour les prix et les stocks en fonction du temps écoulé
    for (auto &[cityId, cityPrices] : m_prices)
    {
        for (auto &[goodId, price] : cityPrices)
        {
            // Ajuster le facteur de prix aléatoirement
            float randomChange = (static_cast<float>(std::rand()) / RAND_MAX) * 0.1f - 0.05f; // Entre -0.05 et 0.05
            price.priceFactor += randomChange;

            // Limiter le facteur de prix
            if (price.priceFactor < 0.5f)
                price.priceFactor = 0.5f;
            if (price.priceFactor > 2.0f)
                price.priceFactor = 2.0f;

            // Augmenter légèrement le stock
            price.currentStock += days * (1 + std::rand() % 3); // 1-3 unités par jour
        }
    }

    // Mettre à jour les prix
    updatePrices();
}

void TradingSystem::updatePrices()
{
    // Cette méthode mettrait à jour les prix en fonction des événements mondiaux,
    // des relations commerciales, etc. Pour l'instant, rien de plus à faire.
}