#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <nlohmann/json.hpp>

// Déclarations anticipées
class Ship;
class TradeGood;

/**
 * @brief Classe GameData optimisée pour gérer toutes les données du jeu
 *
 * Cette classe utilise le pattern singleton pour fournir un accès centralisé
 * à toutes les données du jeu, avec un système de cache efficace et des
 * structures de données optimisées pour les recherches.
 */
class GameData
{
private:
    // Collections optimisées avec unordered_map pour une recherche O(1) par ID
    std::unordered_map<int, Ship> m_ships;
    std::unordered_map<int, TradeGood> m_tradeGoods;

    // Données JSON brutes par type
    std::unordered_map<std::string, nlohmann::json> m_jsonData;

    // Instance singleton
    static std::unique_ptr<GameData> s_instance;

    // Constructeur privé (singleton)
    GameData() = default;

    // Chargement des données par type
    void loadShips();
    void loadTradeGoods();
    void loadJsonFile(const std::string &filename);

public:
    // Obtenir l'instance unique (singleton)
    static GameData &getInstance();

    // Chargement des données
    void loadAllData();
    void reload();

    // Accesseurs avec const correctness
    const Ship *findShipById(int id) const;
    const TradeGood *findTradeGoodById(int id) const;

    // Accesseurs pour les collections complètes
    const std::unordered_map<int, Ship> &getShips() const { return m_ships; }
    const std::unordered_map<int, TradeGood> &getTradeGoods() const { return m_tradeGoods; }

    // Accesseur générique pour les données JSON
    const nlohmann::json &getJsonData(const std::string &dataType) const;

    // Méthodes utilitaires
    nlohmann::json loadJsonFile(const std::string &filePath, bool useCache = true) const;
};