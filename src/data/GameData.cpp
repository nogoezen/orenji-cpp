#include "GameData.hpp"
#include "../models/Ship.hpp"
#include "../models/TradeGood.hpp"
#include "../utils/JsonLoader.hpp"
#include <iostream>
#include <filesystem>

// Initialisation de l'instance unique
std::unique_ptr<GameData> GameData::s_instance = nullptr;

// Obtenir l'instance unique
GameData &GameData::getInstance()
{
    if (!s_instance)
    {
        s_instance = std::unique_ptr<GameData>(new GameData());
    }
    return *s_instance;
}

void GameData::loadAllData()
{
    try
    {
        // Charger les fichiers JSON principaux
        static const std::vector<std::string> mainJsonFiles = {
            "Ships.json", "TradeGoods.json", "Classes.json", "Enemies.json",
            "Equipage.json", "Items.json", "Npc.json", "Skills.json",
            "System.json", "Actors.json"};

        for (const auto &filename : mainJsonFiles)
        {
            loadJsonFile(filename);
        }

        // Traiter les données spécifiques
        loadShips();
        loadTradeGoods();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors du chargement des données: " << e.what() << std::endl;
    }
}

void GameData::reload()
{
    // Vider les données actuelles
    m_ships.clear();
    m_tradeGoods.clear();
    m_jsonData.clear();

    // Recharger depuis zéro
    loadAllData();
}

void GameData::loadJsonFile(const std::string &filename)
{
    try
    {
        // Extraire le nom du fichier sans extension pour l'utiliser comme clé
        std::string dataType = filename;
        size_t dotPos = dataType.find_last_of('.');
        if (dotPos != std::string::npos)
        {
            dataType = dataType.substr(0, dotPos);
        }

        // Charger le fichier et stocker les données
        m_jsonData[dataType] = JsonLoader::loadJsonFile(filename);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors du chargement du fichier " << filename << ": " << e.what() << std::endl;
    }
}

void GameData::loadShips()
{
    try
    {
        const auto &shipsData = m_jsonData["Ships"];
        for (const auto &shipData : shipsData)
        {
            Ship ship(shipData);
            m_ships[ship.getId()] = ship;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors du traitement des navires: " << e.what() << std::endl;
    }
}

void GameData::loadTradeGoods()
{
    try
    {
        const auto &goodsData = m_jsonData["TradeGoods"];
        for (const auto &goodData : goodsData)
        {
            TradeGood good(goodData);
            m_tradeGoods[good.getId()] = good;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors du traitement des biens commerciaux: " << e.what() << std::endl;
    }
}

const Ship *GameData::findShipById(int id) const
{
    auto it = m_ships.find(id);
    return (it != m_ships.end()) ? &(it->second) : nullptr;
}

const TradeGood *GameData::findTradeGoodById(int id) const
{
    auto it = m_tradeGoods.find(id);
    return (it != m_tradeGoods.end()) ? &(it->second) : nullptr;
}

const nlohmann::json &GameData::getJsonData(const std::string &dataType) const
{
    static const nlohmann::json emptyJson = nlohmann::json::object();

    auto it = m_jsonData.find(dataType);
    return (it != m_jsonData.end()) ? it->second : emptyJson;
}

nlohmann::json GameData::loadJsonFile(const std::string &filePath, bool useCache) const
{
    return JsonLoader::loadJsonFile(filePath, useCache);
}
