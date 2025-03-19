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
    float baseFactor = 1.0f;
    const World world;
    const auto cities = world.getAllCities();
    auto cityIt = cities.find(cityId);

    if (cityIt != cities.end())
    {
        // Facteur basé sur la région
        const std::string &region = cityIt->second.region;
        auto regionFactorIt = m_regionFactors.find(region);
        if (regionFactorIt != m_regionFactors.end())
        {
            auto goodFactorIt = regionFactorIt->second.find(goodId);
            if (goodFactorIt != regionFactorIt->second.end())
            {
                baseFactor *= goodFactorIt->second;
            }
        }

        // Appliquer les effets des événements actifs
        for (const auto &event : m_activeEvents)
        {
            // Vérifier si cet événement affecte cette région
            bool affectsRegion = std::find(event.affectedRegions.begin(), event.affectedRegions.end(),
                                           region) != event.affectedRegions.end();

            // Vérifier si cet événement affecte ce bien
            bool affectsGood = std::find(event.affectedGoods.begin(), event.affectedGoods.end(),
                                         goodId) != event.affectedGoods.end();

            if (affectsRegion || affectsGood)
            {
                baseFactor *= event.priceModifier;
            }
        }

        // Facteur lié à l'économie locale
        baseFactor *= calculateLocalEconomyFactor(&cityIt->second);
    }

    // Ajouter une composante aléatoire (±10%)
    std::uniform_real_distribution<float> distribution(0.9f, 1.1f);
    float randomFactor = distribution(m_rng);

    return baseFactor * randomFactor;
}

float TradingSystem::calculateLocalEconomyFactor(const World::City *city) const
{
    if (!city)
        return 1.0f;

    // Facteur basé sur la population
    float populationFactor = 1.0f + (city->population - 5000) / 20000.0f;

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
    nlohmann::json events = nlohmann::json::array();
    for (const auto &event : m_activeEvents)
    {
        nlohmann::json eventData;
        eventData["type"] = static_cast<int>(event.type);
        eventData["description"] = event.description;
        eventData["priceModifier"] = event.priceModifier;
        eventData["duration"] = event.duration;

        nlohmann::json regions = nlohmann::json::array();
        for (const auto &region : event.affectedRegions)
        {
            regions.push_back(region);
        }
        eventData["affectedRegions"] = regions;

        nlohmann::json goods = nlohmann::json::array();
        for (const auto &goodId : event.affectedGoods)
        {
            goods.push_back(goodId);
        }
        eventData["affectedGoods"] = goods;

        events.push_back(eventData);
    }
    j["events"] = events;

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

    // Calculer le coût total avec bonus de compétence de négociation
    int unitPrice = price.getCurrentPrice();

    // Appliquer le bonus de négociation (-5% par niveau)
    float negotiationBonus = 1.0f - (player.getTradeSkills().negotiation * 0.05f);
    unitPrice = static_cast<int>(unitPrice * negotiationBonus);

    // Appliquer le bonus de réputation du royaume
    const World world;
    const auto cities = world.getAllCities();
    auto cityIt = cities.find(cityId);
    if (cityIt != cities.end())
    {
        float kingdomBonus = getKingdomTradeBonus(cityIt->second.region);
        unitPrice = static_cast<int>(unitPrice * (1.0f - kingdomBonus));
    }

    int totalCost = unitPrice * quantity;

    // Vérifier si le joueur a assez d'argent
    if (player.getGold() < totalCost)
    {
        return false;
    }

    // Vérifier si le joueur a assez de place dans son inventaire
    // Tener compte du bonus de logistique (+10% capacité par niveau)
    float logisticsBonus = 1.0f + (player.getTradeSkills().logistics * 0.1f);
    if (!player.canAddCargo(goodId, quantity))
    {
        return false;
    }

    // Effectuer la transaction
    player.removeGold(totalCost);
    player.addCargo(goodId, quantity);
    price.currentStock -= quantity;

    // Enregistrer le prix dans l'historique
    if (m_priceHistory[cityId].find(goodId) == m_priceHistory[cityId].end())
    {
        m_priceHistory[cityId][goodId] = PriceHistory();
    }
    m_priceHistory[cityId][goodId].addPrice(unitPrice);

    // Ajuster légèrement le prix à la hausse due à la demande
    price.priceFactor *= (1.0f + 0.01f * quantity / (price.currentStock + quantity));

    // Augmenter légèrement la réputation de la ville et du royaume
    player.updateCityReputation(cityId, 0.1f);

    if (cityIt != cities.end())
    {
        player.updateKingdomReputation(cityIt->second.region, 0.05f);
    }

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

    // Appliquer le bonus de négociation (+5% par niveau)
    float negotiationBonus = 1.0f + (player.getTradeSkills().negotiation * 0.05f);
    unitPrice = static_cast<int>(unitPrice * negotiationBonus);

    // Appliquer le bonus de réputation du royaume
    const World world;
    const auto cities = world.getAllCities();
    auto cityIt = cities.find(cityId);
    if (cityIt != cities.end())
    {
        float kingdomBonus = getKingdomTradeBonus(cityIt->second.region);
        unitPrice = static_cast<int>(unitPrice * (1.0f + kingdomBonus));
    }

    int totalValue = unitPrice * quantity;

    // Effectuer la transaction
    player.addGold(totalValue);
    player.removeCargo(goodId, quantity);
    price.currentStock += quantity;

    // Enregistrer le prix dans l'historique
    if (m_priceHistory[cityId].find(goodId) == m_priceHistory[cityId].end())
    {
        m_priceHistory[cityId][goodId] = PriceHistory();
    }
    m_priceHistory[cityId][goodId].addPrice(unitPrice);

    // Ajuster légèrement le prix à la baisse due à l'offre
    price.priceFactor *= (1.0f - 0.01f * quantity / (price.currentStock));

    // Augmenter légèrement la réputation de la ville et du royaume
    player.updateCityReputation(cityId, 0.1f);

    if (cityIt != cities.end())
    {
        player.updateKingdomReputation(cityIt->second.region, 0.05f);
    }

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
            std::uniform_real_distribution<float> distribution(-0.05f, 0.05f);
            float randomChange = distribution(m_rng);
            price.priceFactor += randomChange;

            // Limiter le facteur de prix
            if (price.priceFactor < 0.5f)
                price.priceFactor = 0.5f;
            if (price.priceFactor > 2.0f)
                price.priceFactor = 2.0f;

            // Augmenter légèrement le stock
            std::uniform_int_distribution<int> stockDist(1, 3);
            price.currentStock += days * stockDist(m_rng);

            // Enregistrer le prix dans l'historique
            if (m_priceHistory[cityId].find(goodId) == m_priceHistory[cityId].end())
            {
                m_priceHistory[cityId][goodId] = PriceHistory();
            }
            m_priceHistory[cityId][goodId].addPrice(price.getCurrentPrice());
        }
    }

    // Mettre à jour les événements actifs
    for (auto it = m_activeEvents.begin(); it != m_activeEvents.end();)
    {
        it->duration -= days;
        if (it->duration <= 0)
        {
            it = m_activeEvents.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Chance de générer de nouveaux événements
    std::uniform_real_distribution<float> eventChance(0.0f, 1.0f);
    if (eventChance(m_rng) < 0.2f * days) // 20% de chance par jour
    {
        generateRandomEvents();
    }

    // Appliquer les effets des événements actifs
    applyEventEffects();

    // Mettre à jour les prix
    updatePrices();
}

void TradingSystem::updatePrices()
{
    const World world;

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
    const World world;
    const auto &gameData = GameData::getInstance();

    // Types d'événements possibles
    std::vector<TradeEvent::Type> eventTypes = {
        TradeEvent::Type::STORM,
        TradeEvent::Type::CONFLICT,
        TradeEvent::Type::HARVEST,
        TradeEvent::Type::FESTIVAL,
        TradeEvent::Type::DISCOVERY};

    // Sélectionner aléatoirement un type d'événement
    std::uniform_int_distribution<int> typeDist(0, eventTypes.size() - 1);
    TradeEvent::Type eventType = eventTypes[typeDist(m_rng)];

    // Créer l'événement
    TradeEvent newEvent;
    newEvent.type = eventType;

    // Déterminer la durée (entre 5 et 15 jours)
    std::uniform_int_distribution<int> durationDist(5, 15);
    newEvent.duration = durationDist(m_rng);

    // Sélectionner une région affectée
    std::vector<std::string> allRegions;
    for (const auto &[cityId, city] : world.getAllCities())
    {
        if (std::find(allRegions.begin(), allRegions.end(), city.region) == allRegions.end())
        {
            allRegions.push_back(city.region);
        }
    }

    std::uniform_int_distribution<int> regionDist(0, allRegions.size() - 1);
    std::string affectedRegion = allRegions[regionDist(m_rng)];
    newEvent.affectedRegions.push_back(affectedRegion);

    // Sélectionner des biens affectés (1 à 3)
    std::uniform_int_distribution<int> numGoodsDist(1, 3);
    int numAffectedGoods = numGoodsDist(m_rng);

    std::vector<int> allGoodIds = gameData.getAllTradeGoodIds();
    std::uniform_int_distribution<int> goodDist(0, allGoodIds.size() - 1);

    for (int i = 0; i < numAffectedGoods; ++i)
    {
        int goodId = allGoodIds[goodDist(m_rng)];
        // Éviter les doublons
        if (std::find(newEvent.affectedGoods.begin(), newEvent.affectedGoods.end(), goodId) == newEvent.affectedGoods.end())
        {
            newEvent.affectedGoods.push_back(goodId);
        }
    }

    // Définir le modificateur de prix selon le type d'événement
    switch (eventType)
    {
    case TradeEvent::Type::STORM:
        newEvent.description = "Une violente tempête frappe " + affectedRegion;
        newEvent.priceModifier = 1.3f; // Augmentation des prix
        break;
    case TradeEvent::Type::CONFLICT:
        newEvent.description = "Un conflit éclate dans " + affectedRegion;
        newEvent.priceModifier = 1.5f; // Forte augmentation
        break;
    case TradeEvent::Type::HARVEST:
    {
        std::uniform_real_distribution<float> harvestQuality(0.0f, 1.0f);
        float quality = harvestQuality(m_rng);
        if (quality > 0.5f)
        {
            newEvent.description = "Récolte exceptionnelle dans " + affectedRegion;
            newEvent.priceModifier = 0.7f; // Baisse des prix
        }
        else
        {
            newEvent.description = "Mauvaise récolte dans " + affectedRegion;
            newEvent.priceModifier = 1.4f; // Hausse des prix
        }
        break;
    }
    case TradeEvent::Type::FESTIVAL:
        newEvent.description = "Festival important dans " + affectedRegion;
        newEvent.priceModifier = 1.2f; // Légère hausse (demande accrue)
        break;
    case TradeEvent::Type::DISCOVERY:
    {
        std::uniform_real_distribution<float> discoveryImpact(0.0f, 1.0f);
        float impact = discoveryImpact(m_rng);
        if (impact > 0.5f)
        {
            newEvent.description = "Nouvelle source découverte dans " + affectedRegion;
            newEvent.priceModifier = 0.8f; // Baisse des prix
        }
        else
        {
            newEvent.description = "Pénurie inattendue dans " + affectedRegion;
            newEvent.priceModifier = 1.3f; // Hausse des prix
        }
        break;
    }
    }

    // Ajouter l'événement à la liste
    m_activeEvents.push_back(newEvent);
}

void TradingSystem::applyEventEffects()
{
    // Les effets des événements sont déjà appliqués dans calculatePriceFactor
    // Cette méthode pourrait être utilisée pour d'autres effets des événements
    // qui ne sont pas directement liés aux prix
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
    // La compétence de contrebande augmente les chances d'accéder au marché noir
    int smugglingSkill = player.getTradeSkills().smuggling;

    // Chance de base de 10% + 10% par niveau de contrebande
    float baseChance = 0.1f + (smugglingSkill * 0.1f);

    // Une mauvaise réputation dans la ville réduit les chances
    float cityReputation = player.getCityReputation(cityId);
    float reputationModifier = 1.0f;

    if (cityReputation < 0)
    {
        reputationModifier = 1.0f + (cityReputation / 100.0f); // Réduit les chances si réputation négative
    }

    float finalChance = baseChance * reputationModifier;

    // Générer un nombre aléatoire pour déterminer l'accès
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    return distribution(m_rng) < finalChance;
}

float TradingSystem::getKingdomReputation(const std::string &kingdom) const
{
    auto it = m_kingdomReputations.find(kingdom);
    return (it != m_kingdomReputations.end()) ? it->second : 0.0f;
}

float TradingSystem::getKingdomTradeBonus(const std::string &kingdom) const
{
    float reputation = getKingdomReputation(kingdom);

    // Réputation de 0 à 100 donne un bonus de 0% à 20%
    return std::max(0.0f, reputation / 500.0f);
}

int TradingSystem::createTradeRoute(const TradeRoute &route)
{
    // Pas encore implémenté - retourne un ID fictif
    return 1;
}

void TradingSystem::updateTradeRoutes(float deltaTime, Player &player)
{
    auto &routes = player.getTradeRoutes();

    // Cette fonction serait implémentée pour mettre à jour les routes commerciales
    // automatiques du joueur, vérifier si elles sont arrivées à destination, etc.
}

std::vector<std::pair<int, float>> TradingSystem::findProfitableRoutes(int cityId, int maxResults) const
{
    std::vector<std::pair<int, float>> profitableRoutes;

    // Pour chaque ville
    const World world;
    auto cityIt = world.getAllCities().find(cityId);
    if (cityIt == world.getAllCities().end())
        return profitableRoutes;

    const auto &sourceCity = cityIt->second;

    for (const auto &[destCityId, destCity] : world.getAllCities())
    {
        if (destCityId == cityId)
            continue; // Ignorer la ville de départ

        float maxProfit = 0.0f;

        // Pour chaque marchandise disponible dans la ville de départ
        for (int goodId : sourceCity.availableGoods)
        {
            // Vérifier si cette marchandise est plus chère dans la ville de destination
            if (m_prices.find(cityId) != m_prices.end() &&
                m_prices.at(cityId).find(goodId) != m_prices.at(cityId).end() &&
                m_prices.find(destCityId) != m_prices.end() &&
                m_prices.at(destCityId).find(goodId) != m_prices.at(destCityId).end())
            {
                int buyPrice = m_prices.at(cityId).at(goodId).getCurrentPrice();
                int sellPrice = m_prices.at(destCityId).at(goodId).getCurrentPrice();

                // Si profitable
                if (sellPrice > buyPrice)
                {
                    float profitMargin = (sellPrice - buyPrice) / static_cast<float>(buyPrice);
                    maxProfit = std::max(maxProfit, profitMargin);
                }
            }
        }

        if (maxProfit > 0.0f)
        {
            profitableRoutes.push_back(std::make_pair(destCityId, maxProfit));
        }
    }

    // Trier par profit décroissant
    std::sort(profitableRoutes.begin(), profitableRoutes.end(),
              [](const auto &a, const auto &b)
              { return a.second > b.second; });

    // Limiter au nombre maximum demandé
    if (profitableRoutes.size() > maxResults)
    {
        profitableRoutes.resize(maxResults);
    }

    return profitableRoutes;
}

// Implémentation des méthodes de TradeRoute
bool TradeRoute::calculateRisks() const
{
    // La sécurité va de 0 à 10, où 10 est très sécurisé
    // Retourne true s'il y a un risque, false sinon

    // Chance de base de 50% qui diminue avec le niveau de sécurité
    float riskChance = 0.5f - (securityLevel * 0.05f);

    // Génération d'un nombre aléatoire pour déterminer le risque
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    return dist(gen) < riskChance;
}

int TradeRoute::estimateProfit(TradingSystem &system) const
{
    int totalBuyCost = 0;
    int totalSellValue = 0;

    // Calculer le coût total à l'achat dans la ville source
    for (const auto &[goodId, quantity] : tradedGoods)
    {
        int buyPrice = system.getGoodPrice(sourceCity, goodId);
        if (buyPrice > 0)
        {
            totalBuyCost += buyPrice * quantity;
        }
    }

    // Calculer la valeur totale à la vente dans la ville de destination
    for (const auto &[goodId, quantity] : tradedGoods)
    {
        int sellPrice = system.getGoodPrice(destinationCity, goodId);
        if (sellPrice > 0)
        {
            // On vend généralement à 90% du prix affiché
            totalSellValue += static_cast<int>(sellPrice * 0.9f) * quantity;
        }
    }

    // Profit estimé
    return totalSellValue - totalBuyCost;
}

nlohmann::json TradeRoute::toJson() const
{
    nlohmann::json j;
    j["sourceCity"] = sourceCity;
    j["destinationCity"] = destinationCity;
    j["tripDuration"] = tripDuration;
    j["securityLevel"] = securityLevel;
    j["profitMargin"] = profitMargin;
    j["currentTime"] = currentTime;

    nlohmann::json goods = nlohmann::json::array();
    for (const auto &[goodId, quantity] : tradedGoods)
    {
        nlohmann::json good;
        good["id"] = goodId;
        good["quantity"] = quantity;
        goods.push_back(good);
    }
    j["tradedGoods"] = goods;

    return j;
}

TradeRoute TradeRoute::fromJson(const nlohmann::json &data)
{
    TradeRoute route;

    route.sourceCity = data["sourceCity"];
    route.destinationCity = data["destinationCity"];
    route.tripDuration = data["tripDuration"];
    route.securityLevel = data["securityLevel"];
    route.profitMargin = data["profitMargin"];
    route.currentTime = data["currentTime"];

    if (data.contains("tradedGoods") && data["tradedGoods"].is_array())
    {
        for (const auto &goodData : data["tradedGoods"])
        {
            int goodId = goodData["id"];
            int quantity = goodData["quantity"];
            route.tradedGoods.push_back(std::make_pair(goodId, quantity));
        }
    }

    return route;
}