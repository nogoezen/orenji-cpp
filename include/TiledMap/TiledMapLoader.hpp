#pragma once

#include "TiledMap.hpp"
#include "../Core/ResourceCache.hpp"
#include <string>
#include <memory>

namespace Orenji
{

    /**
     * @brief Classe utilitaire pour charger et gérer les cartes Tiled.
     *
     * Cette classe facilite le chargement des cartes Tiled et gère un cache
     * pour éviter de charger plusieurs fois la même carte.
     */
    class TiledMapLoader
    {
    public:
        /**
         * @brief Charge une carte Tiled depuis un fichier.
         *
         * Si la carte est déjà chargée, retourne la version en cache.
         * @param filename Chemin vers le fichier de carte (.tmx).
         * @param forceReload Si vrai, force le rechargement même si la carte est en cache.
         * @return Pointeur partagé vers la carte ou nullptr en cas d'échec.
         */
        static std::shared_ptr<TiledMap> load(const std::string &filename, bool forceReload = false);

        /**
         * @brief Vérifie si une carte est chargée en mémoire.
         * @param filename Chemin vers le fichier de carte.
         * @return true si la carte est en cache, false sinon.
         */
        static bool isLoaded(const std::string &filename);

        /**
         * @brief Libère une carte de la mémoire.
         * @param filename Chemin vers le fichier de carte.
         * @return true si la carte a été libérée, false si elle n'était pas chargée.
         */
        static bool unload(const std::string &filename);

        /**
         * @brief Libère toutes les cartes de la mémoire.
         */
        static void unloadAll();

        /**
         * @brief Configure le répertoire racine pour les cartes.
         *
         * Les chemins relatifs seront résolus par rapport à ce répertoire.
         * @param path Chemin du répertoire racine.
         */
        static void setRootDirectory(const std::string &path);

        /**
         * @brief Obtient le répertoire racine pour les cartes.
         * @return Chemin du répertoire racine.
         */
        static std::string getRootDirectory();

    private:
        /**
         * @brief Cache pour les cartes chargées.
         */
        static ResourceCache<TiledMap> s_mapCache;

        /**
         * @brief Répertoire racine pour les cartes.
         */
        static std::string s_rootDirectory;

        /**
         * @brief Résout un chemin relatif par rapport au répertoire racine.
         * @param filename Chemin relatif ou absolu.
         * @return Chemin résolu.
         */
        static std::string resolvePath(const std::string &filename);
    };

} // namespace Orenji