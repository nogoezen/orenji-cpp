#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

namespace Orenji
{
    /**
     * @brief Classe template pour gérer un cache de ressources.
     *
     * Cette classe permet de stocker et gérer des ressources en mémoire
     * pour éviter de les recharger à chaque fois qu'elles sont demandées.
     *
     * @tparam T Type de ressource à mettre en cache
     */
    template <typename T>
    class ResourceCache
    {
    public:
        /**
         * @brief Charge une ressource à partir d'un fichier, ou la récupère du cache si déjà chargée.
         *
         * @param filepath Chemin du fichier de la ressource
         * @param forceReload Force le rechargement même si la ressource est déjà en cache
         * @return Pointeur partagé vers la ressource, ou nullptr en cas d'erreur
         */
        std::shared_ptr<T> load(const std::string &filepath, bool forceReload = false)
        {
            // Si on demande un rechargement forcé, on supprime d'abord la ressource existante
            if (forceReload)
            {
                unload(filepath);
            }

            // Vérifier si la ressource est déjà en cache
            auto it = m_resources.find(filepath);
            if (it != m_resources.end())
            {
                return it->second;
            }

            // Créer une nouvelle ressource
            auto resource = std::make_shared<T>();

            // Tenter de charger le fichier
            if (resource->loadFromFile(filepath))
            {
                // Ajouter la ressource au cache et la retourner
                m_resources[filepath] = resource;
                return resource;
            }

            // Échec du chargement
            return nullptr;
        }

        /**
         * @brief Vérifie si une ressource est déjà chargée.
         *
         * @param filepath Chemin du fichier de la ressource
         * @return true si la ressource est déjà chargée, false sinon
         */
        bool isLoaded(const std::string &filepath) const
        {
            return m_resources.find(filepath) != m_resources.end();
        }

        /**
         * @brief Décharge une ressource spécifique du cache.
         *
         * @param filepath Chemin du fichier de la ressource
         */
        void unload(const std::string &filepath)
        {
            auto it = m_resources.find(filepath);
            if (it != m_resources.end())
            {
                m_resources.erase(it);
            }
        }

        /**
         * @brief Décharge toutes les ressources du cache.
         */
        void clear()
        {
            m_resources.clear();
        }

        /**
         * @brief Obtient le nombre de ressources actuellement en cache.
         *
         * @return Nombre de ressources en cache
         */
        size_t getResourceCount() const
        {
            return m_resources.size();
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<T>> m_resources; ///< Dictionnaire des ressources en cache
    };

} // namespace Orenji