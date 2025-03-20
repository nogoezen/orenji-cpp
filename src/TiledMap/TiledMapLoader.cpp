#include "../../include/TiledMap/TiledMapLoader.hpp"
#include "../../include/TiledMap/TiledMap.hpp"
#include <filesystem>
#include <iostream>

namespace Orenji
{

    // Initialiser les membres statiques
    ResourceCache<TiledMap> TiledMapLoader::s_mapCache;
    std::string TiledMapLoader::s_rootDirectory = "";

    std::shared_ptr<TiledMap> TiledMapLoader::load(const std::string &filepath, bool forceReload)
    {
        std::string fullPath = resolvePath(filepath);

        // Vérifier si la carte est déjà chargée et si on ne force pas le rechargement
        if (!forceReload && s_mapCache.isLoaded(fullPath))
        {
            return s_mapCache.get(fullPath);
        }

        // Vérifier si le fichier existe
        if (!std::filesystem::exists(fullPath))
        {
            std::cerr << "Erreur: Impossible de trouver le fichier de carte: " << fullPath << std::endl;
            return nullptr;
        }

        // Créer une nouvelle carte
        auto map = std::make_shared<TiledMap>();

        // Charger la carte depuis le fichier
        if (!map->loadFromFile(fullPath))
        {
            std::cerr << "Erreur: Impossible de charger la carte depuis: " << fullPath << std::endl;
            return nullptr;
        }

        // Ajouter la carte au cache
        s_mapCache.add(fullPath, map);

        return map;
    }

    bool TiledMapLoader::isLoaded(const std::string &filepath)
    {
        std::string fullPath = resolvePath(filepath);
        return s_mapCache.isLoaded(fullPath);
    }

    void TiledMapLoader::unload(const std::string &filepath)
    {
        std::string fullPath = resolvePath(filepath);
        s_mapCache.remove(fullPath);
    }

    void TiledMapLoader::unloadAll()
    {
        s_mapCache.clear();
    }

    void TiledMapLoader::setRootDirectory(const std::string &rootDir)
    {
        s_rootDirectory = rootDir;

        // S'assurer que le chemin se termine par un séparateur
        if (!s_rootDirectory.empty() && s_rootDirectory.back() != '/' && s_rootDirectory.back() != '\\')
        {
            s_rootDirectory += '/';
        }
    }

    std::string TiledMapLoader::getRootDirectory()
    {
        return s_rootDirectory;
    }

    std::string TiledMapLoader::resolvePath(const std::string &filepath)
    {
        // Si le chemin est déjà absolu, le retourner tel quel
        if (filepath.size() > 1 && (filepath[0] == '/' || filepath[0] == '\\' || filepath[1] == ':'))
        {
            return filepath;
        }

        // Sinon, combiner avec le dossier racine
        return s_rootDirectory + filepath;
    }

} // namespace Orenji