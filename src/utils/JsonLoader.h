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

class JsonLoader
{
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
    static void setBasePath(const std::string &basePath);

    /**
     * Obtenir le chemin de base pour les données
     * @return Chemin de base
     */
    static const std::string &getBasePath();

    /**
     * Charge un fichier JSON
     * @param filePath Chemin du fichier à charger
     * @param useCache Utiliser le cache si disponible
     * @return Objet JSON contenant les données
     */
    static nlohmann::json loadJsonFile(const std::string &filePath, bool useCache = true);

    /**
     * Charge tous les fichiers JSON dans un répertoire
     * @param dirPath Chemin du répertoire
     * @param recursive Charger les sous-répertoires de manière récursive
     * @param useCache Utiliser le cache si disponible
     * @return Map des objets JSON indexés par leur chemin relatif
     */
    static std::map<std::string, nlohmann::json> loadJsonDirectory(
        const std::string &dirPath, bool recursive = true, bool useCache = true);

    /**
     * Vider le cache de fichiers JSON
     */
    static void clearCache();

    /**
     * Recharger un fichier JSON spécifique (ignore le cache)
     * @param filePath Chemin du fichier à recharger
     * @return Objet JSON contenant les données
     */
    static nlohmann::json reloadJsonFile(const std::string &filePath);
};