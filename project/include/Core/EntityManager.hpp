#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <algorithm>
#include <functional>
#include <SFML/Graphics.hpp>
#include "Core/Entity.hpp"

namespace Orenji
{
    /**
     * @brief Gestionnaire d'entités
     *
     * Gère la création, mise à jour et destruction des entités dans le système
     */
    class EntityManager
    {
    public:
        /**
         * @brief Constructeur
         */
        EntityManager();

        /**
         * @brief Destructeur
         */
        ~EntityManager();

        /**
         * @brief Met à jour toutes les entités actives
         *
         * @param dt Temps écoulé depuis la dernière mise à jour (en secondes)
         */
        void update(float dt);

        /**
         * @brief Dessine toutes les entités actives
         *
         * @param target Cible de rendu
         */
        void draw(sf::RenderTarget &target);

        /**
         * @brief Crée une nouvelle entité
         *
         * @param id Identifiant unique de l'entité
         * @return Pointeur vers l'entité créée
         */
        EntityPtr createEntity(const std::string &id);

        /**
         * @brief Récupère une entité par son identifiant
         *
         * @param id Identifiant de l'entité
         * @return Pointeur vers l'entité, nullptr si non trouvée
         */
        EntityPtr getEntity(const std::string &id);

        /**
         * @brief Récupère toutes les entités
         *
         * @return Vecteur contenant toutes les entités
         */
        std::vector<EntityPtr> getAllEntities() const;

        /**
         * @brief Récupère toutes les entités actives
         *
         * @return Vecteur contenant toutes les entités actives
         */
        std::vector<EntityPtr> getActiveEntities() const;

        /**
         * @brief Supprime une entité
         *
         * @param id Identifiant de l'entité à supprimer
         * @return true si l'entité a été supprimée, false sinon
         */
        bool removeEntity(const std::string &id);

        /**
         * @brief Supprime toutes les entités
         */
        void clear();

        /**
         * @brief Trouve toutes les entités qui possèdent un composant d'un type spécifique
         *
         * @tparam T Type de composant à rechercher
         * @return Vecteur d'entités possédant le composant
         */
        template <typename T>
        std::vector<EntityPtr> findEntitiesWithComponent() const
        {
            std::vector<EntityPtr> result;
            for (const auto &pair : m_entities)
            {
                auto entity = pair.second;
                if (entity && entity->getComponentByType<T>())
                {
                    result.push_back(entity);
                }
            }
            return result;
        }

    private:
        std::unordered_map<std::string, EntityPtr> m_entities; ///< Map des entités par identifiant
    };

} // namespace Orenji