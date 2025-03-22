#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

namespace Orenji
{

    // Déclaration avancée
    class Entity;

    /**
     * @class Component
     * @brief Classe de base pour tous les composants
     */
    class Component
    {
    public:
        /**
         * @brief Constructeur
         * @param name Nom du composant
         */
        explicit Component(const std::string &name = "Component");

        /**
         * @brief Destructeur virtuel
         */
        virtual ~Component();

        /**
         * @brief Initialise le composant
         * @return true si l'initialisation a réussi, false sinon
         */
        virtual bool initialize();

        /**
         * @brief Met à jour le composant
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        virtual void update(float deltaTime);

        /**
         * @brief Dessine le composant
         * @param target La cible de rendu SFML
         * @param states Les états de rendu SFML
         */
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        /**
         * @brief Obtient le nom du composant
         * @return Nom du composant
         */
        const std::string &getName() const { return m_name; }

        /**
         * @brief Définit le nom du composant
         * @param name Nouveau nom du composant
         */
        void setName(const std::string &name) { m_name = name; }

        /**
         * @brief Obtient l'état d'activité du composant
         * @return true si le composant est actif, false sinon
         */
        bool isActive() const { return m_active; }

        /**
         * @brief Définit l'état d'activité du composant
         * @param active Nouvel état d'activité
         */
        void setActive(bool active) { m_active = active; }

        /**
         * @brief Obtient l'entité propriétaire du composant
         * @return Pointeur vers l'entité propriétaire
         */
        Entity *getOwner() const { return m_owner; }

        /**
         * @brief Définit l'entité propriétaire du composant
         * @param owner Nouvelle entité propriétaire
         */
        void setOwner(Entity *owner) { m_owner = owner; }

    private:
        std::string m_name; ///< Nom du composant
        bool m_active;      ///< État d'activité du composant
        Entity *m_owner;    ///< Entité propriétaire du composant
    };

    /**
     * @typedef ComponentPtr
     * @brief Pointeur vers un composant
     */
    using ComponentPtr = std::shared_ptr<Component>;

} // namespace Orenji