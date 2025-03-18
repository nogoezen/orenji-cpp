#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <nlohmann/json.hpp>
#include "JsonLoader.h"

/**
 * Classe singleton pour gérer toutes les données du jeu
 */
class DataManager {
private:
    // Instance singleton
    static std::unique_ptr<DataManager> s_instance;
    
    // Constructeur privé
    DataManager() {
        // Initialiser le chemin de base
        JsonLoader::setBasePath("data");
    }
    
    // Données chargées par catégorie
    std::unordered_map<std::string, std::map<std::string, nlohmann::json>> m_dataByCategory;
    
    // Callbacks de traitement pour chaque type de données
    std::unordered_map<std::string, std::function<void(const std::string&, const nlohmann::json&)>> m_processors;

public:
    /**
     * Obtenir l'instance unique
     */
    static DataManager& getInstance() {
        if (!s_instance) {
            s_instance = std::unique_ptr<DataManager>(new DataManager());
        }
        return *s_instance;
    }
    
    /**
     * Définir le chemin de base pour les données
     */
    void setBasePath(const std::string& path) {
        JsonLoader::setBasePath(path);
    }
    
    /**
     * Enregistrer un processeur pour un type de données spécifique
     * @param category Catégorie des données (dossier/préfixe)
     * @param processor Fonction de traitement
     */
    void registerProcessor(const std::string& category, 
                          std::function<void(const std::string&, const nlohmann::json&)> processor) {
        m_processors[category] = processor;
    }
    
    /**
     * Charger toutes les données
     * @param forceReload Forcer le rechargement même si déjà en cache
     */
    void loadAllData(bool forceReload = false) {
        if (forceReload) {
            JsonLoader::clearCache();
            m_dataByCategory.clear();
        }
        
        // Charger les données du répertoire principal
        try {
            auto mainData = JsonLoader::loadJsonDirectory(".", true);
            processLoadedData("main", mainData);
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors du chargement des données principales: " << e.what() << std::endl;
        }
        
        // Charger les sous-répertoires spécifiques
        std::vector<std::string> subDirs = {
            "world", "items", "skills", "equipment"
        };
        
        for (const auto& dir : subDirs) {
            try {
                auto dirData = JsonLoader::loadJsonDirectory(dir, true);
                processLoadedData(dir, dirData);
            } catch (const std::exception& e) {
                std::cerr << "Erreur lors du chargement du répertoire " << dir << ": " << e.what() << std::endl;
            }
        }
    }
    
    /**
     * Charger un répertoire spécifique
     * @param dirPath Chemin du répertoire
     * @param category Catégorie des données (pour le traitement)
     * @param recursive Charger récursivement
     */
    void loadDirectory(const std::string& dirPath, const std::string& category, bool recursive = true) {
        try {
            auto dirData = JsonLoader::loadJsonDirectory(dirPath, recursive);
            processLoadedData(category, dirData);
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors du chargement du répertoire " << dirPath << ": " << e.what() << std::endl;
        }
    }
    
    /**
     * Charger un fichier JSON spécifique
     * @param filePath Chemin du fichier
     * @param category Catégorie des données (pour le traitement)
     */
    void loadFile(const std::string& filePath, const std::string& category) {
        try {
            auto data = JsonLoader::loadJsonFile(filePath);
            
            // Stocker les données
            std::string relativePath = filePath;
            if (relativePath.find(JsonLoader::getBasePath()) == 0) {
                relativePath = relativePath.substr(JsonLoader::getBasePath().length() + 1);
            }
            
            m_dataByCategory[category][relativePath] = data;
            
            // Traiter les données si un processeur est enregistré
            if (m_processors.find(category) != m_processors.end()) {
                m_processors[category](relativePath, data);
            }
            
        } catch (const std::exception& e) {
            std::cerr << "Erreur lors du chargement du fichier " << filePath << ": " << e.what() << std::endl;
        }
    }
    
    /**
     * Obtenir toutes les données d'une catégorie
     */
    const std::map<std::string, nlohmann::json>& getDataByCategory(const std::string& category) const {
        static const std::map<std::string, nlohmann::json> empty;
        
        auto it = m_dataByCategory.find(category);
        if (it != m_dataByCategory.end()) {
            return it->second;
        }
        
        return empty;
    }
    
    /**
     * Obtenir un fichier JSON spécifique par son chemin relatif et sa catégorie
     */
    const nlohmann::json* getJsonData(const std::string& category, const std::string& relativePath) const {
        auto catIt = m_dataByCategory.find(category);
        if (catIt != m_dataByCategory.end()) {
            auto fileIt = catIt->second.find(relativePath);
            if (fileIt != catIt->second.end()) {
                return &fileIt->second;
            }
        }
        
        return nullptr;
    }
    
private:
    /**
     * Traiter les données chargées
     */
    void processLoadedData(const std::string& category, const std::map<std::string, nlohmann::json>& data) {
        // Stocker les données
        for (const auto& [path, json] : data) {
            m_dataByCategory[category][path] = json;
        }
        
        // Appliquer le processeur si disponible
        if (m_processors.find(category) != m_processors.end()) {
            for (const auto& [path, json] : data) {
                m_processors[category](path, json);
            }
        }
    }
}; 