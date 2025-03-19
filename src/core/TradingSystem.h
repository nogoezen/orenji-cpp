#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>

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
    std::unordered_map<int, std::unordered_map<int, float>> m_regionFactors;

    // Méthodes privées
    void initializePrices();
    void updatePrices();
    float calculatePriceFactor(int cityId, int goodId) const;

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
};