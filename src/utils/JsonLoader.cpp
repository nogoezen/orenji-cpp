#include "JsonLoader.h"
#include <iostream>
#include <fstream>
#include <filesystem>

// Initialisation des variables statiques
std::unordered_map<std::string, nlohmann::json> JsonLoader::s_jsonCache;
std::string JsonLoader::s_basePath = "data";

void JsonLoader::setBasePath(const std::string &basePath)
{
    s_basePath = basePath;
}

const std::string &JsonLoader::getBasePath()
{
    return s_basePath;
}

nlohmann::json JsonLoader::loadJsonFile(const std::string &filePath, bool useCache)
{
    // Vérifier si le fichier est dans le cache
    if (useCache)
    {
        auto it = s_jsonCache.find(filePath);
        if (it != s_jsonCache.end())
        {
            return it->second;
        }
    }

    // Construire le chemin complet
    std::string fullPath = filePath;
    if (!s_basePath.empty() && filePath.find(s_basePath) != 0)
    {
        fullPath = s_basePath + "/" + filePath;
    }

    // Remplacer les barres obliques inversées par des barres obliques pour la compatibilité
    std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

    // Ouvrir et charger le fichier
    std::ifstream file(fullPath);
    if (!file.is_open())
    {
        throw std::runtime_error("Impossible d'ouvrir le fichier: " + fullPath);
    }

    try
    {
        nlohmann::json data = nlohmann::json::parse(file);

        // Mettre en cache si demandé
        if (useCache)
        {
            s_jsonCache[filePath] = data;
        }

        return data;
    }
    catch (const nlohmann::json::parse_error &e)
    {
        throw std::runtime_error("Erreur de parsing JSON dans le fichier: " + fullPath + "\n" + e.what());
    }
}

std::map<std::string, nlohmann::json> JsonLoader::loadJsonDirectory(
    const std::string &dirPath, bool recursive, bool useCache)
{

    std::map<std::string, nlohmann::json> result;
    std::string fullDirPath = dirPath;

    // Construire le chemin complet
    if (!s_basePath.empty() && dirPath.find(s_basePath) != 0)
    {
        fullDirPath = s_basePath + "/" + dirPath;
    }

    // Remplacer les barres obliques inversées par des barres obliques pour la compatibilité
    std::replace(fullDirPath.begin(), fullDirPath.end(), '\\', '/');

    try
    {
        // Vérifier si le répertoire existe
        if (!std::filesystem::exists(fullDirPath))
        {
            throw std::runtime_error("Le répertoire n'existe pas: " + fullDirPath);
        }

        // Parcourir le répertoire
        for (const auto &entry : std::filesystem::directory_iterator(fullDirPath))
        {
            std::string path = entry.path().string();
            std::string relativePath = path;

            // Calculer le chemin relatif
            if (path.find(s_basePath) == 0 && s_basePath.length() < path.length())
            {
                relativePath = path.substr(s_basePath.length() + 1);
            }

            if (entry.is_directory())
            {
                // Charger les sous-répertoires si demandé
                if (recursive)
                {
                    auto subDirData = loadJsonDirectory(path, recursive, useCache);
                    result.insert(subDirData.begin(), subDirData.end());
                }
            }
            else if (entry.is_regular_file() && entry.path().extension() == ".json")
            {
                // Charger les fichiers JSON
                try
                {
                    result[relativePath] = loadJsonFile(path, useCache);
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Erreur lors du chargement du fichier " << path << ": " << e.what() << std::endl;
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        throw std::runtime_error("Erreur lors du parcours du répertoire " + fullDirPath + ": " + e.what());
    }

    return result;
}

void JsonLoader::clearCache()
{
    s_jsonCache.clear();
}

nlohmann::json JsonLoader::reloadJsonFile(const std::string &filePath)
{
    // Supprimer du cache si présent
    auto it = s_jsonCache.find(filePath);
    if (it != s_jsonCache.end())
    {
        s_jsonCache.erase(it);
    }

    // Charger à nouveau
    return loadJsonFile(filePath, true);
}