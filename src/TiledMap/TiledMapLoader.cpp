#include "../../include/TiledMap/TiledMapLoader.hpp"
#include <iostream>
#include <filesystem>

namespace Orenji
{
    // Initialisation des variables statiques
    ResourceCache<TiledMap> TiledMapLoader::s_mapCache;
    std::string TiledMapLoader::s_rootDirectory = "";

    std::shared_ptr<TiledMap> TiledMapLoader::load(const std::string &filepath, bool forceReload)
    {
        // Résoudre le chemin complet
        std::string fullPath = resolvePath(filepath);

        // Utiliser le cache pour charger la carte
        return s_mapCache.load(fullPath, forceReload);
    }

    bool TiledMapLoader::isLoaded(const std::string &filepath)
    {
        std::string fullPath = resolvePath(filepath);
        return s_mapCache.isLoaded(fullPath);
    }

    void TiledMapLoader::unload(const std::string &filepath)
    {
        std::string fullPath = resolvePath(filepath);
        s_mapCache.unload(fullPath);
    }

    void TiledMapLoader::unloadAll()
    {
        s_mapCache.clear();
    }

    void TiledMapLoader::setRootDirectory(const std::string &rootDir)
    {
        s_rootDirectory = rootDir;

        // Assurons-nous que le chemin se termine par un séparateur
        if (!s_rootDirectory.empty() && s_rootDirectory.back() != '/' && s_rootDirectory.back() != '\\')
        {
            s_rootDirectory += '/';
        }
    }

    const std::string &TiledMapLoader::getRootDirectory()
    {
        return s_rootDirectory;
    }

    std::string TiledMapLoader::resolvePath(const std::string &path)
    {
        // Si le chemin est absolu, le retourner tel quel
        if (std::filesystem::path(path).is_absolute())
        {
            return path;
        }

        // Sinon, combiner avec le répertoire racine
        return s_rootDirectory + path;
    }

} // namespace Orenji