#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <ctime>
#include <random>
#include <nlohmann/json.hpp>
#include "../models/World.h"

// Déclarations anticipées
class Player;
class World;
class TradeGood;

/**
 * Structure pour représenter un prix de marchandise dans une ville
 */
struct GoodPrice
{
    int goodId;
    int basePrice;
    float priceFactor;
    int currentStock;

    int getCurrentPrice() const
    {
        return static_cast<int>(basePrice * priceFactor);
    }
};

/**
 * Structure pour stocker l'historique des prix
 */
struct PriceHistory
{
    std::vector<int> prices;
    int maxRecords = 30; // Conserve les 30 derniers prix

    void addPrice(int price)
    {
        prices.push_back(price);
        if (prices.size() > maxRecords)
        {
            prices.erase(prices.begin());
        }
    }

    float getTrend() const
    {
        if (prices.size() < 2)
            return 0.0f;

        // Calculer la tendance sur les 5 derniers points
        int count = std::min(5, static_cast<int>(prices.size() - 1));
        return (prices.back() - prices[prices.size() - 1 - count]) /
               static_cast<float>(count);
    }
};

/**
 * Structure pour les événements commerciaux
 */
struct TradeEvent
{
    enum class Type
    {
        STORM,    // Tempête
        CONFLICT, // Conflit
        HARVEST,  // Récolte exceptionnelle/mauvaise
        FESTIVAL, // Festival augmentant la demande de certains produits
        DISCOVERY // Découverte affectant certaines marchandises
    };

    Type type;
    std::string description;
    std::vector<std::string> affectedRegions;
    std::vector<int> affectedGoods;
    float priceModifier;
    int duration; // En jours

    static std::string getTypeName(Type type)
    {
        switch (type)
        {
        case Type::STORM:
            return "Tempête";
        case Type::CONFLICT:
            return "Conflit";
        case Type::HARVEST:
            return "Récolte";
        case Type::FESTIVAL:
            return "Festival";
        case Type::DISCOVERY:
            return "Découverte";
        default:
            return "Inconnu";
        }
    }
};

/**
 * Classe pour gérer les routes commerciales
 */
class TradeRoute
{
public:
    int sourceCity;
    int destinationCity;
    std::vector<std::pair<int, int>> tradedGoods; // Paire (goodId, quantity)
    int tripDuration;                             // En jours
    int securityLevel;                            // Niveau de sécurité (investissement)
    float profitMargin;                           // Marge bénéficiaire estimée
    float currentTime;                            // Temps écoulé depuis le départ

    TradeRoute() : sourceCity(0), destinationCity(0), tripDuration(0),
                   securityLevel(0), profitMargin(0.0f), currentTime(0.0f) {}

    TradeRoute(int source, int destination, const std::vector<std::pair<int, int>> &goods,
               int duration, int security)
        : sourceCity(source), destinationCity(destination), tradedGoods(goods),
          tripDuration(duration), securityLevel(security), profitMargin(0.0f), currentTime(0.0f) {}

    bool calculateRisks() const;
    int estimateProfit(class TradingSystem &system) const;

    nlohmann::json toJson() const;
    static TradeRoute fromJson(const nlohmann::json &data);
};

/**
 * @brief Système de commerce
 *
 * Cette classe gère toutes les opérations liées au commerce de marchandises
 * dans le jeu, y compris les prix, l'achat et la vente.
 */
class TradingSystem
{
private:
    // Prix des marchandises par ville et par ID de marchandise
    std::unordered_map<int, std::unordered_map<int, GoodPrice>> m_prices;

    // Facteurs de prix par région
    std::unordered_map<std::string, std::unordered_map<int, float>> m_regionFactors;

    // Historique des prix
    std::unordered_map<int, std::unordered_map<int, PriceHistory>> m_priceHistory;

    // Événements commerciaux actifs
    std::vector<TradeEvent> m_activeEvents;

    // Réputations des royaumes (partagées entre tous les joueurs)
    std::unordered_map<std::string, float> m_kingdomReputations;

    // Générateur de nombres aléatoires
    std::mt19937 m_rng;

    // Méthodes privées
    void initializePrices();
    void updatePrices();
    float calculatePriceFactor(int cityId, int goodId) const;
    void generateRandomEvents();
    void applyEventEffects();
    float calculateLocalEconomyFactor(const World::City *city) const;

public:
    /**
     * Constructeur
     */
    TradingSystem();

    /**
     * Initialise le système de commerce
     * @param saveData Données optionnelles pour charger un état sauvegardé
     */
    bool initialize(const nlohmann::json &saveData = nlohmann::json());

    /**
     * Convertit l'état du système de commerce en JSON pour la sauvegarde
     * @return Objet JSON contenant l'état du système
     */
    nlohmann::json toJson() const;

    /**
     * Obtient le prix actuel d'une marchandise dans une ville
     * @param cityId ID de la ville
     * @param goodId ID de la marchandise
     * @return Prix actuel ou -1 si la marchandise n'est pas disponible
     */
    int getGoodPrice(int cityId, int goodId) const;

    /**
     * Obtient tous les prix des marchandises dans une ville
     * @param cityId ID de la ville
     * @return Map des prix indexés par ID de marchandise
     */
    std::vector<GoodPrice> getCityPrices(int cityId) const;

    /**
     * Achète une quantité de marchandise dans une ville
     * @param player Joueur qui achète
     * @param cityId ID de la ville
     * @param goodId ID de la marchandise
     * @param quantity Quantité à acheter
     * @return true si l'achat a réussi
     */
    bool buyGood(Player &player, int cityId, int goodId, int quantity);

    /**
     * Vend une quantité de marchandise dans une ville
     * @param player Joueur qui vend
     * @param cityId ID de la ville
     * @param goodId ID de la marchandise
     * @param quantity Quantité à vendre
     * @return true si la vente a réussi
     */
    bool sellGood(Player &player, int cityId, int goodId, int quantity);

    /**
     * Vérifie si une marchandise est disponible dans une ville
     * @param cityId ID de la ville
     * @param goodId ID de la marchandise
     * @return true si la marchandise est disponible
     */
    bool isGoodAvailable(int cityId, int goodId) const;

    /**
     * Met à jour les prix et les stocks après un certain temps
     * @param days Nombre de jours écoulés
     */
    void advanceTime(int days);

    /**
     * Retourne l'historique des prix d'une marchandise dans une ville
     * @param cityId ID de la ville
     * @param goodId ID de la marchandise
     * @return Historique des prix
     */
    const PriceHistory *getPriceHistory(int cityId, int goodId) const;

    /**
     * Obtient la tendance actuelle du prix
     * @param cityId ID de la ville
     * @param goodId ID de la marchandise
     * @return Tendance (-1 à +1, où positif = hausse)
     */
    float getPriceTrend(int cityId, int goodId) const;

    /**
     * Retourne les événements commerciaux actifs
     * @return Liste des événements actifs
     */
    const std::vector<TradeEvent> &getActiveEvents() const;

    /**
     * Vérifie si un marché noir est disponible pour le joueur dans cette ville
     * @param cityId ID de la ville
     * @param player Joueur concerné
     * @return true si un marché noir est accessible
     */
    bool hasBlackMarket(int cityId, const Player &player) const;

    /**
     * Obtient la réputation d'un joueur dans un royaume
     * @param kingdom Nom du royaume
     * @return Valeur de réputation (0-100)
     */
    float getKingdomReputation(const std::string &kingdom) const;

    /**
     * Calcule le bonus commercial basé sur la réputation dans un royaume
     * @param kingdom Nom du royaume
     * @return Facteur bonus (0.0 - 0.2 soit 0%-20%)
     */
    float getKingdomTradeBonus(const std::string &kingdom) const;

    /**
     * Crée une nouvelle route commerciale
     * @param route Définition de la route
     * @return ID de la route créée
     */
    int createTradeRoute(const TradeRoute &route);

    /**
     * Met à jour toutes les routes commerciales
     * @param deltaTime Temps écoulé en secondes
     * @param player Joueur propriétaire des routes
     */
    void updateTradeRoutes(float deltaTime, Player &player);

    /**
     * Trouve les routes commerciales les plus rentables à partir d'une ville
     * @param cityId Ville de départ
     * @param maxResults Nombre maximum de suggestions à retourner
     * @return Liste des routes triées par profit potentiel
     */
    std::vector<std::pair<int, float>> findProfitableRoutes(int cityId, int maxResults = 5) const;
};