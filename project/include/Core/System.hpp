#pragma once

#include <string>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Core/Entity.hpp"

namespace Orenji
{
    class EntityManager;
    class System;

    // Forward declaration de l'alias EntityPtr s'il n'est pas reconnu
    using EntityPtr = std::shared_ptr<Entity>;

    /**
     * @brief Classe de base pour tous les systèmes
     *
     * Un système dans l'architecture ECS traite les entités
     * possédant certains composants pour implémenter des fonctionnalités.
     */
    class System
    {
    public:
        /**
         * @brief Constructeur
         *
         * @param name Nom du système
         */
        System(const std::string &name);

        /**
         * @brief Destructeur virtuel
         */
        virtual ~System() = default;

        /**
         * @brief Initialise le système
         *
         * @param entityManager Gestionnaire d'entités
         */
        virtual void initialize(EntityManager *entityManager);

        /**
         * @brief Met à jour le système
         *
         * @param dt Temps écoulé depuis la dernière mise à jour (en secondes)
         */
        virtual void update(float dt) = 0;

        /**
         * @brief Dessine les éléments du système
         *
         * @param target Cible de rendu
         */
        virtual void draw(sf::RenderTarget &target) {}

        /**
         * @brief Récupère le nom du système
         *
         * @return Nom du système
         */
        const std::string &getName() const { return m_name; }

        /**
         * @brief Vérifie si une entité est compatible avec ce système
         *
         * @param entity Entité à vérifier
         * @return true si l'entité est compatible, false sinon
         */
        virtual bool isCompatibleWithEntity(const EntityPtr &entity) const = 0;

        /**
         * @brief Récupère toutes les entités compatibles avec ce système
         *
         * @return Vecteur d'entités compatibles
         */
        std::vector<EntityPtr> getCompatibleEntities() const;

        /**
         * @brief Active ou désactive le système
         *
         * @param active Nouvel état d'activation
         */
        void setActive(bool active) { m_active = active; }

        /**
         * @brief Vérifie si le système est actif
         *
         * @return true si le système est actif, false sinon
         */
        bool isActive() const { return m_active; }

    protected:
        std::string m_name;             ///< Nom du système
        bool m_active;                  ///< État d'activation du système
        EntityManager *m_entityManager; ///< Gestionnaire d'entités
    };

    /**
     * @brief Alias pour un pointeur intelligent vers un System
     */
    using SystemPtr = std::shared_ptr<System>;

} // namespace Orenji