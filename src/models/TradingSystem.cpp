#include "TradingSystem.hpp"
#include "../utils/MathUtils.h"
#include <algorithm>
#include <cmath>
#include <limits>

namespace Orenji
{
    TradingSystem::TradingSystem()
        : m_maxTransactionHistory(100), m_maxPriceModifiers(20), m_currentGameTime(0)
    {
    }

    void TradingSystem::initialize(const std::unordered_map<int, TradeGood> &goods)
    {
        m_goods = goods;
    }

    bool TradingSystem::addTradeGood(const TradeGood &good)
    {
        // Vérifier si la marchandise existe déjà
        if (m_goods.find(good.getId()) != m_goods.end())
        {
            return false;
        }

        // Ajouter la marchandise au catalogue
        m_goods[good.getId()] = good;
        return true;
    }

    const TradeGood &TradingSystem::getTradeGood(int goodId) const
    {
        // Rechercher la marchandise dans le catalogue
        auto it = m_goods.find(goodId);
        if (it != m_goods.end())
        {
            return it->second;
        }

        // Si la marchandise n'existe pas, renvoyer une marchandise "invalide"
        static TradeGood invalidGood;
        return invalidGood;
    }

    bool TradingSystem::buyGoods(Player &player, City &city, int goodId, int quantity, int shipId)
    {
        // Vérifier que la quantité est positive
        if (quantity <= 0)
        {
            return false;
        }

        // Vérifier que la marchandise existe dans la ville
        if (!city.hasGood(goodId))
        {
            return false;
        }

        // Vérifier que la quantité demandée est disponible
        if (city.getGoodQuantity(goodId) < quantity)
        {
            return false;
        }

        // Calculer le prix total
        int unitPrice = city.getGoodPrice(goodId);
        int totalPrice = unitPrice * quantity;

        // Appliquer les modificateurs de prix (facteurs de réputation, faction, etc.)
        int finalPrice = calculateFinalPrice(totalPrice, player.getFaction(), city.getFaction());

        // Vérifier que le joueur a assez d'argent
        if (player.getMoney() < finalPrice)
        {
            return false;
        }

        // Vérifier que le navire peut transporter les marchandises (si un navire est spécifié)
        if (shipId >= 0)
        {
            const Ship *ship = player.getShip(shipId);
            if (!ship || !canShipCarry(*ship, goodId, quantity))
            {
                return false;
            }
        }

        // Effectuer la transaction
        player.spendMoney(finalPrice);
        city.removeGood(goodId, quantity);

        // Ajouter les marchandises au navire ou à l'inventaire du joueur
        if (shipId >= 0)
        {
            Ship *ship = player.getShip(shipId);
            ship->addCargo(goodId, quantity);
        }
        else
        {
            player.addItem(goodId, quantity);
        }

        // Créer et enregistrer la transaction
        TradeTransaction transaction(
            goodId,
            getTradeGood(goodId).getName(),
            quantity,
            unitPrice,
            city.getId(),
            city.getName(),
            true, // C'est un achat
            m_currentGameTime);

        // Ajouter la transaction à l'historique
        m_transactionHistory.push_back(transaction);

        // Limiter la taille de l'historique
        if (m_transactionHistory.size() > m_maxTransactionHistory)
        {
            m_transactionHistory.erase(m_transactionHistory.begin());
        }

        // Appeler le callback si défini
        if (m_transactionCallback)
        {
            m_transactionCallback(transaction);
        }

        return true;
    }

    bool TradingSystem::sellGoods(Player &player, City &city, int goodId, int quantity, int shipId)
    {
        // Vérifier que la quantité est positive
        if (quantity <= 0)
        {
            return false;
        }

        // Vérifier que la ville accepte cette marchandise
        if (!city.hasGood(goodId))
        {
            return false;
        }

        // Vérifier que le joueur possède la quantité spécifiée
        bool hasQuantity = false;

        if (shipId >= 0)
        {
            // Vérifier dans le navire spécifié
            Ship *ship = player.getShip(shipId);
            if (ship && ship->getCargoQuantity(goodId) >= quantity)
            {
                hasQuantity = true;
            }
        }
        else
        {
            // Vérifier dans l'inventaire du joueur
            if (player.getItemQuantity(goodId) >= quantity)
            {
                hasQuantity = true;
            }
        }

        if (!hasQuantity)
        {
            return false;
        }

        // Calculer le prix de vente
        int unitPrice = city.getGoodPrice(goodId);
        int totalPrice = unitPrice * quantity;

        // Appliquer les modificateurs de prix (facteurs de réputation, faction, etc.)
        int finalPrice = calculateFinalPrice(totalPrice, city.getFaction(), player.getFaction());

        // Effectuer la transaction
        player.addMoney(finalPrice);

        // Retirer les marchandises de l'inventaire du joueur ou du navire
        if (shipId >= 0)
        {
            Ship *ship = player.getShip(shipId);
            ship->removeCargo(goodId, quantity);
        }
        else
        {
            player.removeItem(goodId, quantity);
        }

        // Ajouter les marchandises à la ville
        city.addGood(goodId, quantity);

        // Créer et enregistrer la transaction
        TradeTransaction transaction(
            goodId,
            getTradeGood(goodId).getName(),
            quantity,
            unitPrice,
            city.getId(),
            city.getName(),
            false, // C'est une vente
            m_currentGameTime);

        // Ajouter la transaction à l'historique
        m_transactionHistory.push_back(transaction);

        // Limiter la taille de l'historique
        if (m_transactionHistory.size() > m_maxTransactionHistory)
        {
            m_transactionHistory.erase(m_transactionHistory.begin());
        }

        // Appeler le callback si défini
        if (m_transactionCallback)
        {
            m_transactionCallback(transaction);
        }

        return true;
    }

    int TradingSystem::calculateFinalPrice(int basePrice, const std::string &buyer, const std::string &seller) const
    {
        float priceMultiplier = 1.0f;

        // Appliquer tous les modificateurs actifs
        for (const auto &modifier : m_priceModifiers)
        {
            priceMultiplier *= modifier.value;
        }

        // Appliquer les facteurs spécifiques aux factions
        if (buyer == seller)
        {
            // Bonus pour les transactions au sein de la même faction
            priceMultiplier *= 0.95f;
        }

        // Calculer le prix final et arrondir à l'entier le plus proche
        int finalPrice = static_cast<int>(std::round(basePrice * priceMultiplier));

        // Assurer que le prix ne tombe pas en dessous d'un minimum
        return std::max(1, finalPrice);
    }

    void TradingSystem::addPriceModifier(const PriceModifier &modifier)
    {
        // Ajouter le modificateur à la liste
        m_priceModifiers.push_back(modifier);

        // Limiter le nombre de modificateurs
        if (m_priceModifiers.size() > m_maxPriceModifiers)
        {
            // Supprimer le modificateur le plus ancien
            m_priceModifiers.erase(m_priceModifiers.begin());
        }
    }

    bool TradingSystem::removePriceModifier(PriceModifierType type, const std::string &description)
    {
        // Rechercher le modificateur à supprimer
        auto it = std::find_if(m_priceModifiers.begin(), m_priceModifiers.end(),
                               [&](const PriceModifier &mod)
                               {
                                   return mod.type == type && mod.description == description;
                               });

        // Si trouvé, le supprimer
        if (it != m_priceModifiers.end())
        {
            m_priceModifiers.erase(it);
            return true;
        }

        return false;
    }

    void TradingSystem::updatePriceModifiers(int currentTime)
    {
        m_currentGameTime = currentTime;

        // Supprimer les modificateurs expirés
        m_priceModifiers.erase(
            std::remove_if(m_priceModifiers.begin(), m_priceModifiers.end(),
                           [currentTime](const PriceModifier &mod)
                           {
                               return mod.expiryTime > 0 && mod.expiryTime <= currentTime;
                           }),
            m_priceModifiers.end());
    }

    void TradingSystem::setTransactionCallback(std::function<void(const TradeTransaction &)> callback)
    {
        m_transactionCallback = callback;
    }

    int TradingSystem::calculateTransportCost(const City &fromCity, const City &toCity, int quantity) const
    {
        // Calculer la distance entre les deux villes
        float distance = std::sqrt(
            std::pow(toCity.getX() - fromCity.getX(), 2) +
            std::pow(toCity.getY() - fromCity.getY(), 2));

        // Coût de base par unité de distance et par unité de marchandise
        const float baseCostPerUnit = 0.5f;

        // Calculer le coût total
        int totalCost = static_cast<int>(std::ceil(distance * baseCostPerUnit * quantity));

        // Assurer un coût minimum
        return std::max(1, totalCost);
    }

    void TradingSystem::generatePriceFluctuations(City &city, int magnitude)
    {
        // Parcourir toutes les marchandises de la ville
        for (const auto &goodEntry : city.getAllGoods())
        {
            int goodId = goodEntry.first;

            // Obtenir le prix de base de la marchandise
            int basePrice = getTradeGood(goodId).getBasePrice();

            // Calculer un facteur de fluctuation (entre 0.8 et 1.2 par défaut)
            float fluctuation = 1.0f + (MathUtils::randFloat(-0.2f, 0.2f) * (magnitude / 10.0f));

            // Appliquer la fluctuation
            int newPrice = static_cast<int>(std::round(basePrice * fluctuation));

            // Mettre à jour le prix de la marchandise
            city.setGoodPrice(goodId, newPrice);
        }
    }

    void TradingSystem::updateCityGoods(City &city)
    {
        // Liste des marchandises à ajouter/supprimer
        std::vector<int> goodsToAdd;
        std::vector<int> goodsToRemove;

        // Parcourir le catalogue complet des marchandises
        for (const auto &goodEntry : m_goods)
        {
            int goodId = goodEntry.first;
            const TradeGood &good = goodEntry.second;

            // Vérifier si la ville produit cette marchandise
            bool produces = city.isProducerOf(goodId);

            // Vérifier si la ville a besoin de cette marchandise
            bool needs = city.isConsumerOf(goodId);

            // Déterminer si cette marchandise devrait être disponible dans la ville
            bool shouldBeAvailable = produces || needs || MathUtils::randFloat(0, 1) < 0.3f;

            // Vérifier si la marchandise est déjà disponible
            bool isAvailable = city.hasGood(goodId);

            if (shouldBeAvailable && !isAvailable)
            {
                goodsToAdd.push_back(goodId);
            }
            else if (!shouldBeAvailable && isAvailable)
            {
                goodsToRemove.push_back(goodId);
            }
        }

        // Ajouter les nouvelles marchandises
        for (int goodId : goodsToAdd)
        {
            // Quantité initiale aléatoire (10-50 pour les marchandises produites, 5-20 pour les autres)
            int initialQuantity = city.isProducerOf(goodId) ? MathUtils::randInt(10, 50) : MathUtils::randInt(5, 20);

            // Prix initial (80%-120% du prix de base)
            const TradeGood &good = getTradeGood(goodId);
            int initialPrice = static_cast<int>(good.getBasePrice() * MathUtils::randFloat(0.8f, 1.2f));

            // Ajouter la marchandise à la ville
            city.addGood(goodId, initialQuantity);
            city.setGoodPrice(goodId, initialPrice);
        }

        // Supprimer les marchandises qui ne devraient plus être disponibles
        for (int goodId : goodsToRemove)
        {
            city.removeGood(goodId, city.getGoodQuantity(goodId));
        }
    }

    bool TradingSystem::canShipCarry(const Ship &ship, int goodId, int quantity) const
    {
        // Obtenir le poids unitaire de la marchandise
        const TradeGood &good = getTradeGood(goodId);
        float unitWeight = good.getWeight();

        // Calculer le poids total de la marchandise
        float totalWeight = unitWeight * quantity;

        // Obtenir la capacité de fret restante du navire
        float remainingCapacity = ship.getCargoCapacity() - ship.getCurrentCargoWeight();

        // Vérifier si le navire peut transporter la marchandise
        return totalWeight <= remainingCapacity;
    }

} // namespace Orenji