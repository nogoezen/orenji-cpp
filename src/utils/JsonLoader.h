#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

class JsonLoader {
private:
    // Cache pour les fichiers JSON déjà chargés
    static std::unordered_map<std::string, nlohmann::json> s_jsonCache;
    
    // Chemin de base pour les données
    static std::string s_basePath;

public:
    /**
     * Définir le chemin de base pour les données
     * @param basePath Chemin de base
     */
    static void setBasePath(const std::string& basePath) {
        s_basePath = basePath;
    }
    
    /**
     * Obtenir le chemin de base pour les données
     * @return Chemin de base
     */
    static const std::string& getBasePath() {
        return s_basePath;
    }
    
    /**
     * Charge un fichier JSON
     * @param filePath Chemin du fichier à charger
     * @param useCache Utiliser le cache si disponible
     * @return Objet JSON contenant les données
     */
    static nlohmann::json loadJsonFile(const std::string& filePath, bool useCache = true) {
        // Vérifier si le fichier est dans le cache
        if (useCache && s_jsonCache.find(filePath) != s_jsonCache.end()) {
            return s_jsonCache[filePath];
        }
        
        // Construire le chemin complet
        std::string fullPath = filePath;
        if (!s_basePath.empty() && filePath.find(s_basePath) != 0) {
            fullPath = s_basePath + "/" + filePath;
        }
        
        // Ouvrir et charger le fichier
        std::ifstream file(fullPath);
        if (!file.is_open()) {
            throw std::runtime_error("Impossible d'ouvrir le fichier: " + fullPath);
        }
        
        try {
            nlohmann::json data = nlohmann::json::parse(file);
            
            // Mettre en cache si demandé
            if (useCache) {
                s_jsonCache[filePath] = data;
            }
            
            return data;
        } catch (const nlohmann::json::parse_error& e) {
            throw std::runtime_error("Erreur de parsing JSON dans le fichier: " + fullPath + "\n" + e.what());
        }
    }
    
    /**
     * Charge tous les fichiers JSON dans un répertoire
     * @param dirPath Chemin du répertoire
     * @param recursive Charger les sous-répertoires de manière récursive
     * @param useCache Utiliser le cache si disponible
     * @return Map des objets JSON indexés par leur chemin relatif
     */
    static std::map<std::string, nlohmann::json> loadJsonDirectory(
        const std::string& dirPath, bool recursive = true, bool useCache = true) {
        
        std::map<std::string, nlohmann::json> result;
        std::string fullDirPath = dirPath;
        
        // Construire le chemin complet
        if (!s_basePath.empty() && dirPath.find(s_basePath) != 0) {
            fullDirPath = s_basePath + "/" + dirPath;
        }
        
        try {
            // Parcourir le répertoire
            for (const auto& entry : std::filesystem::directory_iterator(fullDirPath)) {
                std::string path = entry.path().string();
                std::string relativePath = path;
                
                // Calculer le chemin relatif
                if (path.find(s_basePath) == 0 && s_basePath.length() < path.length()) {
                    relativePath = path.substr(s_basePath.length() + 1);
                }
                
                if (entry.is_directory()) {
                    // Charger les sous-répertoires si demandé
                    if (recursive) {
                        auto subDirData = loadJsonDirectory(path, recursive, useCache);
                        result.insert(subDirData.begin(), subDirData.end());
                    }
                } else if (entry.is_regular_file() && entry.path().extension() == ".json") {
                    // Charger les fichiers JSON
                    try {
                        result[relativePath] = loadJsonFile(path, useCache);
                    } catch (const std::exception& e) {
                        std::cerr << "Erreur lors du chargement du fichier " << path << ": " << e.what() << std::endl;
                    }
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            throw std::runtime_error("Erreur lors du parcours du répertoire " + fullDirPath + ": " + e.what());
        }
        
        return result;
    }
    
    /**
     * Vider le cache de fichiers JSON
     */
    static void clearCache() {
        s_jsonCache.clear();
    }
    
    /**
     * Recharger un fichier JSON spécifique (ignore le cache)
     * @param filePath Chemin du fichier à recharger
     * @return Objet JSON contenant les données
     */
    static nlohmann::json reloadJsonFile(const std::string& filePath) {
        // Supprimer du cache si présent
        if (s_jsonCache.find(filePath) != s_jsonCache.end()) {
            s_jsonCache.erase(filePath);
        }
        
        // Charger à nouveau
}; 