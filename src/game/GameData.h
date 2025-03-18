#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <nlohmann/json.hpp>

#include "../models/Ship.h"
#include "../models/TradeGood.h"
#include "../utils/JsonLoader.h"
#include "../utils/DataManager.h"

class GameData {
private:
    std::vector<Ship> m_ships;
    std::vector<TradeGood> m_tradeGoods;
    
    // Les autres données du jeu
    nlohmann::json m_classes;
    nlohmann::json m_enemies;
    nlohmann::json m_equipage;
    nlohmann::json m_items;
    nlohmann::json m_npcs;
    nlohmann::json m_skills;
    nlohmann::json m_system;
    nlohmann::json m_actors;
    
    // Instance singleton
    static std::unique_ptr<GameData> s_instance;
    
    // Constructeur privé (singleton)
    GameData() {
        // Enregistrer les processeurs de données
        registerDataProcessors();
        
        // Charger toutes les données
        loadAllData();
    }
    
    // Enregistrement des processeurs de données
    void registerDataProcessors() {
        // Processeur pour les navires
        DataManager::getInstance().registerProcessor("ships", [this](const std::string& path, const nlohmann::json& data) {
            processShips(data);
        });
        
        // Processeur pour les biens commerciaux
        DataManager::getInstance().registerProcessor("tradegoods", [this](const std::string& path, const nlohmann::json& data) {
            processTradeGoods(data);
        });
        
        // Processeur pour les autres types de données
        DataManager::getInstance().registerProcessor("main", [this](const std::string& path, const nlohmann::json& data) {
            // Déterminer le type de données en fonction du nom de fichier
            std::string filename = path.substr(path.find_last_of('/') + 1);
            
            if (filename == "Classes.json") {
                m_classes = data;
            } else if (filename == "Enemies.json") {
                m_enemies = data;
            } else if (filename == "Equipage.json") {
                m_equipage = data;
            } else if (filename == "Items.json") {
                m_items = data;
            } else if (filename == "Npc.json") {
                m_npcs = data;
            } else if (filename == "Skills.json") {
                m_skills = data;
            } else if (filename == "System.json") {
                m_system = data;
            } else if (filename == "Actors.json") {
                m_actors = data;
            } else if (filename == "Ships.json") {
                processShips(data);
            } else if (filename == "TradeGoods.json") {
                processTradeGoods(data);
            }
        });
    }
    
    // Chargement de toutes les données
    void loadAllData() {
        // Utiliser le DataManager pour charger toutes les données
        DataManager::getInstance().loadAllData();
        
        // Si aucune donnée n'a été chargée pour les navires ou les biens commerciaux, essayer de les charger directement
        if (m_ships.empty()) {
            tryLoadFile("Ships.json", "ships");
        }
        
        if (m_tradeGoods.empty()) {
            tryLoadFile("TradeGoods.json", "tradegoods");
        }
    }
    
    // Essayer de charger un fichier spécifique
    void tryLoadFile(const std::string& filename, const std::string& category) {
        try {
            DataManager::getInstance().loadFile(filename, category);
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors du chargement de " << filename << ": " << e.what() << std::endl;
        }
    }
    
    // Traiter les données des navires
    void processShips(const nlohmann::json& data) {
        try {
            for (const auto& shipData : data) {
                m_ships.emplace_back(shipData);
            }
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors du traitement des navires: " << e.what() << std::endl;
        }
    }
    
    // Traitement des données des biens commerciaux
    void processTradeGoods(const nlohmann::json& data) {
        try {
            for (const auto& goodData : data) {
                m_tradeGoods.emplace_back(goodData);
            }
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors du traitement des biens commerciaux: " << e.what() << std::endl;
        }
    }

public:
    // Obtenir l'instance unique (singleton)
    static GameData& getInstance() {
        if (!s_instance) {
            s_instance = std::unique_ptr<GameData>(new GameData());
        }
        return *s_instance;
    }
    
    // Recharger toutes les données
    void reload() {
        // Vider les données actuelles
        m_ships.clear();
        m_tradeGoods.clear();
        
        // Recharger depuis zéro
        loadAllData();
    }
    
    // Getters
    const std::vector<Ship>& getShips() const { return m_ships; }
    const std::vector<TradeGood>& getTradeGoods() const { return m_tradeGoods; }
    
    // Trouver un navire par ID
    const Ship* findShipById(int id) const {
        for (const auto& ship : m_ships) {
            if (ship.getId() == id) {
                return &ship;
            }
        }
        return nullptr;
    }
    
    // Trouver un bien commercial par ID
    const TradeGood* findTradeGoodById(int id) const {
        for (const auto& good : m_tradeGoods) {
            if (good.getId() == id) {
                return &good;
            }
        }
        return nullptr;
    }
    
    // Accès aux autres données
    const nlohmann::json& getClasses() const { return m_classes; }
    const nlohmann::json& getEnemies() const { return m_enemies; }
    const nlohmann::json& getEquipage() const { return m_equipage; }
    const nlohmann::json& getItems() const { return m_items; }
    const nlohmann::json& getNpcs() const { return m_npcs; }
    const nlohmann::json& getSkills() const { return m_skills; }
    const nlohmann::json& getSystem() const { return m_system; }
    const nlohmann::json& getActors() const { return m_actors; }
}; 