#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include "TiledMap.hpp"
#include "../Core/ResourceCache.hpp"

namespace Orenji
{
    /**
     * @brief Classe utilitaire pour charger et gérer les cartes Tiled.
     *
     * Cette classe fournit des méthodes statiques pour charger, décharger et gérer
     * les cartes Tiled. Elle utilise un cache interne pour éviter de charger plusieurs
     * fois la même carte et optimiser les performances.
     */
    class TiledMapLoader
    {
    public:
        /**
         * @brief Charge une carte Tiled à partir d'un fichier.
         *
         * @param filepath Chemin du fichier TMX
         * @param forceReload Force le rechargement même si la carte est déjà en cache
         * @return Pointeur partagé vers la carte chargée, ou nullptr en cas d'erreur
         */
        static std::shared_ptr<TiledMap> load(const std::string &filepath, bool forceReload = false);

        /**
         * @brief Vérifie si une carte est déjà chargée.
         *
         * @param filepath Chemin du fichier TMX
         * @return true si la carte est déjà chargée, false sinon
         */
        static bool isLoaded(const std::string &filepath);

        /**
         * @brief Décharge une carte spécifique du cache.
         *
         * @param filepath Chemin du fichier TMX
         */
        static void unload(const std::string &filepath);

        /**
         * @brief Décharge toutes les cartes du cache.
         */
        static void unloadAll();

        /**
         * @brief Définit le répertoire racine pour les chemins relatifs.
         *
         * @param rootDir Chemin du répertoire racine
         */
        static void setRootDirectory(const std::string &rootDir);

        /**
         * @brief Obtient le répertoire racine actuel.
         *
         * @return Le chemin du répertoire racine
         */
        static const std::string &getRootDirectory();

    private:
        /**
         * @brief Résout un chemin relatif par rapport au répertoire racine.
         *
         * @param path Chemin à résoudre
         * @return Chemin résolu
         */
        static std::string resolvePath(const std::string &path);

    private:
        static ResourceCache<TiledMap> s_mapCache; ///< Cache pour les cartes
        static std::string s_rootDirectory;        ///< Répertoire racine pour les chemins relatifs
    };

} // namespace Orenji