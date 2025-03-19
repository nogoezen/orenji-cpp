#include "TradingSystem.h"
#include "../models/Player.h"
#include "../models/TradeGood.h"
#include "../data/GameData.h"
#include "World.h"
#include <iostream>
#include <random>
#include <ctime>

TradingSystem::TradingSystem() : m_rng(std::random_device{}())
{
    // L'initialisation complète se fait dans initialize()
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

            // Charger les réputations des royaumes
            if (saveData.contains("kingdomReputations") && saveData["kingdomReputations"].is_object())
            {
                for (auto it = saveData["kingdomReputations"].begin(); it != saveData["kingdomReputations"].end(); ++it)
                {
                    m_kingdomReputations[it.key()] = it.value();
                }
            }

            // Charger les événements actifs
            if (saveData.contains("events") && saveData["events"].is_array())
            {
                for (const auto &eventData : saveData["events"])
                {
                    TradeEvent event;
                    event.type = static_cast<TradeEvent::Type>(eventData["type"].get<int>());
                    event.description = eventData["description"];
                    event.priceModifier = eventData["priceModifier"];
                    event.duration = eventData["duration"];

                    if (eventData.contains("affectedRegions") && eventData["affectedRegions"].is_array())
                    {
                        for (const auto &region : eventData["affectedRegions"])
                        {
                            event.affectedRegions.push_back(region);
                        }
                    }

                    if (eventData.contains("affectedGoods") && eventData["affectedGoods"].is_array())
                    {
                        for (const auto &goodId : eventData["affectedGoods"])
                        {
                            event.affectedGoods.push_back(goodId);
                        }
                    }

                    m_activeEvents.push_back(event);
                }
            }
        }
        else
        {
            // Initialiser les prix avec les valeurs par défaut
            initializePrices();

            // Initialiser les réputations des royaumes
            m_kingdomReputations["Spain"] = 0.0f;
            m_kingdomReputations["England"] = 0.0f;
            m_kingdomReputations["Portugal"] = 0.0f;
            m_kingdomReputations["France"] = 0.0f;
            m_kingdomReputations["Netherlands"] = 0.0f;
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
    CoreWorld world;

    // Pour chaque ville
    for (const auto &[cityId, city] : world.getAllCities())
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
    float baseFactor = 1.0f;

    // Facteurs appliqués hiérarchiquement
    // 1. Facteur de rareté/abondance de base pour ce bien
    const auto &gameData = GameData::getInstance();
    const TradeGood *good = gameData.findTradeGoodById(goodId);
    if (good)
    {
        baseFactor *= good->getDemandFactor();
    }

    // 2. Facteur régional si la ville existe
    CoreWorld world;
    const auto &cities = world.getAllCities();
    auto cityIt = cities.find(cityId);
    if (cityIt != cities.end())
    {
        const auto &regionName = cityIt->second.region;

        // Facteur spécifique à la région pour ce bien
        auto regionFactorIt = m_regionFactors.find(regionName);
        if (regionFactorIt != m_regionFactors.end())
        {
            auto goodFactorIt = regionFactorIt->second.find(goodId);
            if (goodFactorIt != regionFactorIt->second.end())
            {
                baseFactor *= goodFactorIt->second;
            }
        }

        // Facteur lié à l'économie locale
        baseFactor *= calculateLocalEconomyFactor(cityId);
    }

    // Ajouter une composante aléatoire (±10%)
    // Utiliser un générateur non-membre pour éviter les problèmes avec la méthode const
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> distribution(0.9f, 1.1f);
    float randomFactor = distribution(rng);

    return baseFactor * randomFactor;
}

float TradingSystem::calculateLocalEconomyFactor(int cityId) const
{
    const CoreWorld world;
    const auto cities = world.getAllCities();
    auto cityIt = cities.find(cityId);

    if (cityIt == cities.end())
        return 1.0f;

    const auto &city = cityIt->second;

    // Facteur basé sur la population
    float populationFactor = 1.0f + (city.population - 5000) / 20000.0f;

    // Limiter entre 0.8 et 1.5
    populationFactor = std::max(0.8f, std::min(populationFactor, 1.5f));

    return populationFactor;
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

    // Sauvegarder les réputations des royaumes
    nlohmann::json kingdomReps;
    for (const auto &[kingdom, reputation] : m_kingdomReputations)
    {
        kingdomReps[kingdom] = reputation;
    }
    j["kingdomReputations"] = kingdomReps;

    // Sauvegarder les événements actifs
    j["events"] = serializeEvents();

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
    // Vérifier que la marchandise est disponible dans cette ville
    if (!isGoodAvailable(cityId, goodId) || quantity <= 0)
    {
        return false;
    }

    // Obtenir le prix actuel
    int basePrice = getGoodPrice(cityId, goodId);
    if (basePrice <= 0)
    {
        return false;
    }

    // Obtenir les informations sur la ville
    const CoreWorld world;
    const auto cities = world.getAllCities();
    auto cityIt = cities.find(cityId);
    if (cityIt == cities.end())
    {
        return false;
    }

    // Appliquer les bonus et malus de négociation
    float negotiationBonus = 1.0f - (player.getTradeSkills().negotiation * 0.005f); // -0.5% par niveau
    float kingdomBonus = getKingdomTradeBonus(cityIt->second.region);
    float reputationBonus = player.getCityReputation(cityId) > 50.0f ? 0.05f : 0.0f;
    float finalPrice = basePrice * (1.0f - kingdomBonus - reputationBonus) * negotiationBonus;

    // Ajouter un petit facteur aléatoire (±2%)
    std::uniform_real_distribution<float> distribution(0.98f, 1.02f);
    finalPrice *= distribution(m_rng);

    // Calculer le coût total
    int totalCost = static_cast<int>(finalPrice * quantity);

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
    // 1. Retirer l'argent au joueur
    if (!player.removeGold(totalCost))
    {
        return false;
    }

    // 2. Ajouter la marchandise à l'inventaire du joueur
    const TradeGood *good = GameData::getInstance().findTradeGoodById(goodId);
    if (!good)
    {
        // Rembourser le joueur si la marchandise n'existe pas
        player.addGold(totalCost);
        return false;
    }

    if (!player.addCargo(goodId, good->getName(), quantity, good->getWeight()))
    {
        // Rembourser le joueur s'il n'y a pas assez de place
        player.addGold(totalCost);
        return false;
    }

    // 3. Mettre à jour le stock de la ville
    auto &cityPrices = m_prices[cityId];
    auto priceIt = cityPrices.find(goodId);
    if (priceIt != cityPrices.end())
    {
        // Diminuer le stock
        priceIt->second.currentStock = std::max(0, priceIt->second.currentStock - quantity);

        // Augmenter légèrement le prix (rareté)
        float stockFactor = 1.0f + (quantity / 100.0f); // +1% par 100 unités achetées
        priceIt->second.priceFactor *= stockFactor;
    }

    // 4. Mettre à jour l'historique des prix
    if (m_priceHistory[cityId].find(goodId) == m_priceHistory[cityId].end())
    {
        m_priceHistory[cityId][goodId] = PriceHistory();
    }
    m_priceHistory[cityId][goodId].addPrice(static_cast<int>(finalPrice));

    // 5. Améliorer légèrement la réputation avec la ville et le royaume
    player.updateCityReputation(cityId, 0.01f);
    player.updateKingdomReputation(cityIt->second.region, 0.05f);

    return true;
}

bool TradingSystem::sellGood(Player &player, int cityId, int goodId, int quantity)
{
    // Vérifier que le joueur possède la marchandise
    if (!player.hasCargo(goodId, quantity) || quantity <= 0)
    {
        return false;
    }

    // Obtenir le prix actuel
    int basePrice = getGoodPrice(cityId, goodId);
    if (basePrice <= 0)
    {
        return false;
    }

    // Obtenir les informations sur la ville
    const CoreWorld world;
    const auto cities = world.getAllCities();
    auto cityIt = cities.find(cityId);
    if (cityIt == cities.end())
    {
        return false;
    }

    // Appliquer les bonus et malus de négociation
    float negotiationBonus = 1.0f + (player.getTradeSkills().negotiation * 0.005f); // +0.5% par niveau
    float kingdomBonus = getKingdomTradeBonus(cityIt->second.region);
    float reputationBonus = player.getCityReputation(cityId) > 50.0f ? 0.05f : 0.0f;
    float finalPrice = basePrice * (1.0f + kingdomBonus + reputationBonus) * negotiationBonus;

    // Ajouter un petit facteur aléatoire (±2%)
    std::uniform_real_distribution<float> distribution(0.98f, 1.02f);
    finalPrice *= distribution(m_rng);

    // Calculer le montant total
    int totalAmount = static_cast<int>(finalPrice * quantity);

    // Effectuer la transaction
    if (!player.removeCargo(goodId, quantity))
    {
        return false;
    }

    player.addGold(totalAmount);

    // Mettre à jour le stock de la ville
    auto &cityPrices = m_prices[cityId];
    auto priceIt = cityPrices.find(goodId);
    if (priceIt != cityPrices.end())
    {
        // Augmenter le stock
        priceIt->second.currentStock += quantity;

        // Diminuer légèrement le prix (abondance)
        float stockFactor = 1.0f - (quantity / 200.0f); // -0.5% par 100 unités vendues
        stockFactor = std::max(0.9f, stockFactor);      // Limite de -10%
        priceIt->second.priceFactor *= stockFactor;
    }

    // Mettre à jour l'historique des prix
    if (m_priceHistory[cityId].find(goodId) == m_priceHistory[cityId].end())
    {
        m_priceHistory[cityId][goodId] = PriceHistory();
    }
    m_priceHistory[cityId][goodId].addPrice(static_cast<int>(finalPrice));

    // Améliorer légèrement la réputation avec la ville et le royaume
    player.updateCityReputation(cityId, 0.01f);
    player.updateKingdomReputation(cityIt->second.region, 0.05f);

    return true;
}

void TradingSystem::advanceTime(int days)
{
    // Mettre à jour les prix et les stocks en fonction du temps écoulé
    for (int i = 0; i < days; ++i)
    {
        // Mettre à jour les prix
        updatePrices();

        // Appliquer les effets des événements
        applyEventEffects();
    }
}

void TradingSystem::updatePrices()
{
    const CoreWorld world;

    // Mettre à jour les prix en fonction des événements, des tendances économiques, etc.
    for (auto &[cityId, cityPrices] : m_prices)
    {
        for (auto &[goodId, price] : cityPrices)
        {
            // Recalculer le facteur de prix
            price.priceFactor = calculatePriceFactor(cityId, goodId);
        }
    }
}

void TradingSystem::generateRandomEvents()
{
    // Limiter le nombre d'événements actifs
    if (m_activeEvents.size() >= 10)
    {
        return;
    }

    // 10% de chance de générer un nouvel événement
    std::uniform_real_distribution<float> chanceDistribution(0.0f, 1.0f);
    if (chanceDistribution(m_rng) > 0.1f)
    {
        return;
    }

    // Récupérer toutes les régions pour les événements
    const CoreWorld world;
    std::vector<std::string> allRegions;

    for (const auto &[cityId, city] : world.getAllCities())
    {
        // Vérifier si la région est déjà dans la liste
        if (std::find(allRegions.begin(), allRegions.end(), city.region) == allRegions.end())
        {
            allRegions.push_back(city.region);
        }
    }

    if (allRegions.empty())
    {
        return;
    }

    // Récupérer tous les IDs de marchandises
    const auto &gameData = GameData::getInstance();
    std::vector<int> allGoodIds;

    // Récupérer les IDs des marchandises directement à partir des TradeGoods disponibles
    const auto &tradeGoods = gameData.getTradeGoods();
    for (const auto &good : tradeGoods)
    {
        allGoodIds.push_back(good.first); // first est l'ID dans la paire <ID, TradeGood>
    }

    if (allGoodIds.empty())
    {
        return;
    }

    // Choisir aléatoirement un type d'événement
    std::uniform_int_distribution<int> eventTypeDistribution(0, 4); // 5 types d'événements
    int eventTypeInt = eventTypeDistribution(m_rng);
    TradeEvent::Type eventType = static_cast<TradeEvent::Type>(eventTypeInt);

    // Créer l'événement
    TradeEvent event;
    event.type = eventType;

    // Choisir des régions affectées (1-3)
    std::uniform_int_distribution<int> regionCountDistribution(1, 3);
    int regionCount = regionCountDistribution(m_rng);
    regionCount = std::min(regionCount, static_cast<int>(allRegions.size()));

    std::shuffle(allRegions.begin(), allRegions.end(), m_rng);
    for (int i = 0; i < regionCount; ++i)
    {
        event.affectedRegions.push_back(allRegions[i]);
    }

    // Choisir des marchandises affectées (1-5)
    std::uniform_int_distribution<int> goodCountDistribution(1, 5);
    int goodCount = goodCountDistribution(m_rng);
    goodCount = std::min(goodCount, static_cast<int>(allGoodIds.size()));

    std::shuffle(allGoodIds.begin(), allGoodIds.end(), m_rng);
    for (int i = 0; i < goodCount; ++i)
    {
        event.affectedGoods.push_back(allGoodIds[i]);
    }

    // Définir les détails selon le type d'événement
    switch (event.type)
    {
    case TradeEvent::Type::STORM:
        event.description = "Une tempête fait rage sur les mers";
        // Les tempêtes augmentent les prix (rareté)
        event.priceModifier = 1.2f;
        event.duration = 7; // 7 jours
        break;
    case TradeEvent::Type::CONFLICT:
        event.description = "Un conflit perturbe le commerce";
        // Les conflits augmentent fortement les prix
        event.priceModifier = 1.5f;
        event.duration = 14; // 14 jours
        break;
    case TradeEvent::Type::HARVEST:
    {
        // 50/50 chance de bonne ou mauvaise récolte
        bool goodHarvest = (chanceDistribution(m_rng) > 0.5f);
        if (goodHarvest)
        {
            event.description = "Une récolte exceptionnelle fait baisser les prix";
            event.priceModifier = 0.8f; // -20% (prix plus bas)
        }
        else
        {
            event.description = "Une mauvaise récolte fait grimper les prix";
            event.priceModifier = 1.3f; // +30% (prix plus élevés)
        }
        event.duration = 30; // 30 jours
    }
    break;
    case TradeEvent::Type::FESTIVAL:
        event.description = "Un festival augmente la demande";
        // Les festivals augmentent les prix (plus de demande)
        event.priceModifier = 1.1f;
        event.duration = 10; // 10 jours
        break;
    case TradeEvent::Type::DISCOVERY:
    {
        // 50/50 chance de faire baisser ou augmenter les prix
        bool lowerPrices = (chanceDistribution(m_rng) > 0.5f);
        if (lowerPrices)
        {
            event.description = "Une découverte permet de produire davantage";
            event.priceModifier = 0.9f; // -10% (prix plus bas)
        }
        else
        {
            event.description = "Une découverte révèle une nouvelle utilité";
            event.priceModifier = 1.15f; // +15% (prix plus élevés)
        }
        event.duration = 60; // 60 jours (effet durable)
    }
    break;
    }

    // Défnir la date d'expiration
    auto now = std::chrono::system_clock::now();
    event.expiryTime = now + std::chrono::hours(24 * event.duration);

    // Ajouter l'événement à la liste
    m_activeEvents.push_back(event);
}

void TradingSystem::applyEventEffects()
{
    // Cette méthode est appelée pour appliquer les effets des événements actifs
    // aux prix et stocks des marchandises dans les villes concernées

    const CoreWorld world;

    // Pour chaque événement actif
    for (const auto &event : m_activeEvents)
    {
        // Pour chaque ville qui pourrait être affectée
        for (const auto &[cityId, city] : world.getAllCities())
        {
            // Vérifier si cette ville est dans une région affectée par l'événement
            bool isAffected = event.affectedRegions.empty() || // Si aucune région n'est spécifiée, toutes sont affectées
                              std::find(event.affectedRegions.begin(), event.affectedRegions.end(),
                                        city.region) != event.affectedRegions.end();

            if (!isAffected)
                continue;

            // Vérifier que la ville existe dans m_prices
            auto cityPricesIt = m_prices.find(cityId);
            if (cityPricesIt == m_prices.end())
                continue;

            // Pour chaque marchandise dans cette ville
            for (auto &[goodId, price] : cityPricesIt->second)
            {
                // Vérifier si cette marchandise est affectée par l'événement
                bool goodAffected = event.affectedGoods.empty() || // Si aucune marchandise n'est spécifiée, toutes sont affectées
                                    std::find(event.affectedGoods.begin(), event.affectedGoods.end(),
                                              goodId) != event.affectedGoods.end();

                if (!goodAffected)
                    continue;

                // Appliquer le modificateur de prix de l'événement
                switch (event.type)
                {
                case TradeEvent::Type::STORM:
                    // Tempêtes : diminuent le stock disponible
                    price.currentStock = std::max(5, static_cast<int>(price.currentStock * 0.7f));
                    break;

                case TradeEvent::Type::CONFLICT:
                    // Conflits : augmentent les prix et diminuent le stock
                    price.priceFactor *= 1.2f;
                    price.currentStock = std::max(10, static_cast<int>(price.currentStock * 0.8f));
                    break;

                case TradeEvent::Type::HARVEST:
                    // Récoltes : peuvent augmenter ou diminuer le stock selon le modificateur
                    if (event.priceModifier > 1.0f) // Mauvaise récolte
                    {
                        price.currentStock = std::max(5, static_cast<int>(price.currentStock * 0.6f));
                    }
                    else // Bonne récolte
                    {
                        price.currentStock = std::min(100, static_cast<int>(price.currentStock * 1.5f));
                    }
                    break;

                case TradeEvent::Type::FESTIVAL:
                    // Festivals : augmentent légèrement les prix en raison de la demande
                    price.priceFactor *= 1.1f;
                    break;

                case TradeEvent::Type::DISCOVERY:
                    // Découvertes : peuvent avoir des effets variés selon le modificateur
                    price.priceFactor *= event.priceModifier;
                    break;

                default:
                    break;
                }

                // S'assurer que le facteur de prix reste dans des limites raisonnables
                price.priceFactor = std::max(0.5f, std::min(price.priceFactor, 2.0f));
            }
        }
    }
}

const PriceHistory *TradingSystem::getPriceHistory(int cityId, int goodId) const
{
    auto cityIt = m_priceHistory.find(cityId);
    if (cityIt == m_priceHistory.end())
        return nullptr;

    auto goodIt = cityIt->second.find(goodId);
    if (goodIt == cityIt->second.end())
        return nullptr;

    return &goodIt->second;
}

float TradingSystem::getPriceTrend(int cityId, int goodId) const
{
    const PriceHistory *history = getPriceHistory(cityId, goodId);
    if (!history)
        return 0.0f;

    return history->getTrend();
}

const std::vector<TradeEvent> &TradingSystem::getActiveEvents() const
{
    return m_activeEvents;
}

bool TradingSystem::hasBlackMarket(int cityId, const Player &player) const
{
    // Un marché noir est disponible si la réputation du joueur est assez basse
    // et qu'il possède un niveau minimum en capacité "smuggling"
    float cityReputation = player.getCityReputation(cityId);
    int smugglingSkill = player.getTradeSkills().smuggling;

    // Pour accéder au marché noir, il faut soit:
    // 1. Une réputation très basse (< 20)
    // 2. Une réputation moyenne (< 50) et des compétences de contrebande (> 3)
    return (cityReputation < 20.0f) || (cityReputation < 50.0f && smugglingSkill > 3);
}

float TradingSystem::getKingdomReputation(const std::string &kingdom) const
{
    auto it = m_kingdomReputations.find(kingdom);
    if (it == m_kingdomReputations.end())
    {
        return 0.0f;
    }
    return it->second;
}

float TradingSystem::getKingdomTradeBonus(const std::string &kingdom) const
{
    float reputation = getKingdomReputation(kingdom);

    // Bonus maximal de 20% pour une réputation de 100
    return reputation / 500.0f; // 0.0 - 0.2 (0% - 20%)
}

int TradingSystem::createTradeRoute(const TradeRoute &route)
{
    // Cette fonctionnalité sera implémentée ultérieurement
    return -1;
}

void TradingSystem::updateTradeRoutes(float deltaTime, Player &player)
{
    // Cette fonctionnalité sera implémentée ultérieurement
}

std::vector<std::pair<int, float>> TradingSystem::findProfitableRoutes(int cityId, int maxResults) const
{
    std::vector<std::pair<int, float>> profitableRoutes;

    // Vérifier que la ville source existe
    const CoreWorld world;
    const auto cities = world.getAllCities();
    auto cityIt = cities.find(cityId);
    if (cityIt == cities.end())
    {
        return profitableRoutes;
    }

    const auto &sourceCity = cityIt->second;

    // Pour chaque ville de destination (sauf la source)
    for (const auto &[destCityId, destCity] : world.getAllCities())
    {
        if (destCityId == cityId)
        {
            continue; // Ignorer la ville source
        }

        // Calculer le profit estimé pour cette route
        float totalProfit = 0.0f;

        // Obtenir les prix dans les deux villes
        const auto &sourcePrices = m_prices.find(cityId);
        const auto &destPrices = m_prices.find(destCityId);

        if (sourcePrices == m_prices.end() || destPrices == m_prices.end())
        {
            continue; // Pas d'information de prix pour l'une des villes
        }

        // Pour chaque type de marchandise disponible dans la ville source
        for (int goodId : sourceCity.availableGoods)
        {
            // Vérifier s'il y a des prix pour ce bien dans les deux villes
            auto srcGoodIt = sourcePrices->second.find(goodId);
            auto destGoodIt = destPrices->second.find(goodId);

            if (srcGoodIt == sourcePrices->second.end() || destGoodIt == destPrices->second.end())
            {
                continue; // Ce bien n'est pas négocié dans l'une des villes
            }

            // Calculer la différence de prix
            int buyPrice = srcGoodIt->second.getCurrentPrice();
            int sellPrice = destGoodIt->second.getCurrentPrice();

            // Si le profit est positif, l'ajouter au profit total
            int profitPerUnit = sellPrice - buyPrice;
            if (profitPerUnit > 0)
            {
                // Estimer le profit pour une quantité raisonnable
                int estimatedQuantity = std::min(10, srcGoodIt->second.currentStock);
                totalProfit += static_cast<float>(profitPerUnit * estimatedQuantity);
            }
        }

        // Si le profit est suffisant, ajouter cette route à la liste
        if (totalProfit > 0.0f)
        {
            profitableRoutes.push_back(std::make_pair(destCityId, totalProfit));
        }
    }

    // Trier les routes par profit décroissant
    std::sort(profitableRoutes.begin(), profitableRoutes.end(),
              [](const auto &a, const auto &b)
              { return a.second > b.second; });

    // Limiter le nombre de résultats
    if (profitableRoutes.size() > maxResults)
    {
        profitableRoutes.resize(maxResults);
    }

    return profitableRoutes;
}

void TradingSystem::updatePriceHistory()
{
    const CoreWorld world;
    const auto &cities = world.getAllCities();
    const auto &tradeGoods = world.getAllTradeGoods();

    for (const auto &[cityId, city] : cities)
    {
        // Vérifier si cette ville existe dans m_prices
        auto cityPricesIt = m_prices.find(cityId);
        if (cityPricesIt == m_prices.end())
        {
            continue; // Passer à la ville suivante si elle n'existe pas dans m_prices
        }

        auto &cityPrices = cityPricesIt->second;

        // Pour chaque bien commercial disponible dans cette ville
        for (int goodId : city.availableGoods)
        {
            auto goodPriceIt = cityPrices.find(goodId);
            if (goodPriceIt == cityPrices.end())
            {
                continue; // Passer au bien suivant s'il n'existe pas dans cityPrices
            }

            auto &goodPrice = goodPriceIt->second;

            // Calculer le delta de prix pour ce bien dans cette ville
            int priceDelta = generatePriceDelta(cityId, goodId);

            // Appliquer le delta au prix actuel
            int newPrice = goodPrice.getCurrentPrice() + priceDelta;

            // S'assurer que le prix reste dans les limites raisonnables
            auto tradeGoodIt = tradeGoods.find(goodId);
            if (tradeGoodIt != tradeGoods.end())
            {
                const auto &tradeGood = tradeGoodIt->second;
                int basePrice = tradeGood->getBasePrice();
                int minPrice = static_cast<int>(basePrice * 0.5f); // 50% du prix de base minimum
                int maxPrice = static_cast<int>(basePrice * 2.0f); // 200% du prix de base maximum

                newPrice = std::max(minPrice, std::min(newPrice, maxPrice));
            }

            // Mettre à jour le prix
            if (m_priceHistory[cityId].find(goodId) == m_priceHistory[cityId].end())
            {
                m_priceHistory[cityId][goodId] = PriceHistory();
            }
            m_priceHistory[cityId][goodId].addPrice(newPrice);
        }
    }
}

int TradingSystem::generatePriceDelta(int cityId, int goodId) const
{
    // Créer une instance temporaire du monde
    CoreWorld world;

    auto cityIt = world.getAllCities().find(cityId);
    if (cityIt == world.getAllCities().end())
        return 0;

    auto tradeGoodIt = world.getAllTradeGoods().find(goodId);
    if (tradeGoodIt == world.getAllTradeGoods().end())
        return 0;

    // Récupérer la structure de prix pour cette ville et ce bien
    auto cityPricesIt = m_prices.find(cityId);
    if (cityPricesIt == m_prices.end())
    {
        return 0; // Ville non trouvée dans les prix
    }

    auto goodPriceIt = cityPricesIt->second.find(goodId);
    if (goodPriceIt == cityPricesIt->second.end())
    {
        return 0; // Bien non trouvé dans les prix
    }

    const auto &priceInfo = goodPriceIt->second;
    int currentPrice = priceInfo.getCurrentPrice();
    int basePrice = tradeGoodIt->second->getBasePrice();

    // Facteurs qui influencent les changements de prix

    // 1. Stock actuel
    int stock = priceInfo.currentStock;
    float stockFactor = 0.0f;

    if (stock < 10)
    {
        stockFactor = 0.05f; // Augmentation de prix si le stock est bas
    }
    else if (stock > 100)
    {
        stockFactor = -0.05f; // Diminution de prix si le stock est élevé
    }

    // 2. Tendances du marché (légère variation aléatoire)
    float trendFactor = (rand() % 5 - 2) / 100.0f; // Entre -0.02 et 0.02

    // 3. Événements actifs
    float eventFactor = 0.0f;
    for (const auto &event : m_activeEvents)
    {
        // Vérifier si l'événement affecte cette ville
        auto city = world.getCityById(cityId);
        if (!city)
            continue;

        bool affectsCity = false;
        if (event.affectedRegions.empty())
        {
            affectsCity = true; // Événement global
        }
        else
        {
            for (const auto &regionId : event.affectedRegions)
            {
                if (city->region == regionId)
                {
                    affectsCity = true;
                    break;
                }
            }
        }

        if (!affectsCity)
            continue;

        // Vérifier si l'événement affecte ce bien
        bool affectsGood = false;
        if (event.affectedGoods.empty())
        {
            affectsGood = true; // Affecte tous les biens
        }
        else
        {
            for (const auto &affectedGoodId : event.affectedGoods)
            {
                if (affectedGoodId == goodId)
                {
                    affectsGood = true;
                    break;
                }
            }
        }

        if (affectsGood)
        {
            eventFactor += event.priceModifier;
        }
    }

    // Calculer le delta final
    float totalModifier = stockFactor + trendFactor + eventFactor;
    int priceDelta = static_cast<int>(currentPrice * totalModifier);

    return priceDelta;
}

void TradingSystem::updateEvents()
{
    // Supprimer les événements expirés
    auto now = std::chrono::system_clock::now();
    m_activeEvents.erase(
        std::remove_if(m_activeEvents.begin(), m_activeEvents.end(),
                       [&now](const TradeEvent &event)
                       {
                           return now > event.expiryTime;
                       }),
        m_activeEvents.end());

    // Générer de nouveaux événements aléatoires
    generateRandomEvents();
}

nlohmann::json TradingSystem::serializeEvents() const
{
    nlohmann::json j = nlohmann::json::array();

    for (const auto &event : m_activeEvents)
    {
        nlohmann::json eventData;
        eventData["type"] = static_cast<int>(event.type);
        eventData["description"] = event.description;
        eventData["priceModifier"] = event.priceModifier;
        eventData["duration"] = event.duration;

        // Ajouter l'heure d'expiration
        auto expiryTimeT = std::chrono::system_clock::to_time_t(event.expiryTime);
        eventData["expiryTime"] = expiryTimeT;

        // Ajouter les régions affectées
        nlohmann::json regions = nlohmann::json::array();
        for (const auto &region : event.affectedRegions)
        {
            regions.push_back(region);
        }
        eventData["affectedRegions"] = regions;

        // Ajouter les marchandises affectées
        nlohmann::json goods = nlohmann::json::array();
        for (const auto &goodId : event.affectedGoods)
        {
            goods.push_back(goodId);
        }
        eventData["affectedGoods"] = goods;

        j.push_back(eventData);
    }

    return j;
}