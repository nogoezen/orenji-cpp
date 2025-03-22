#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <memory>
#include <type_traits>

namespace Orenji
{
    // Forward declarations
    class Entity;

    /**
     * @brief Classe de base pour tous les composants
     *
     * Cette classe fournit l'interface pour tous les composants
     * utilisés dans le système Entity-Component.
     */
    class Component
    {
    public:
        /**
         * @brief Constructeur
         *
         * @param id Identifiant unique du composant
         */
        Component(const std::string &id);

        /**
         * @brief Destructeur virtuel
         */
        virtual ~Component() = default;

        /**
         * @brief Initialise le composant
         *
         * Cette méthode est appelée lorsque le composant est ajouté à une entité
         */
        virtual void initialize() {}

        /**
         * @brief Met à jour le composant
         *
         * @param dt Temps écoulé depuis la dernière mise à jour (en secondes)
         */
        virtual void update(float dt) {}

        /**
         * @brief Dessine le composant
         *
         * @param target Cible de rendu
         */
        virtual void draw(sf::RenderTarget &target) {}

        /**
         * @brief Récupère l'identifiant du composant
         *
         * @return Identifiant du composant
         */
        const std::string &getId() const { return m_id; }

        /**
         * @brief Définit l'entité parente du composant
         *
         * @param entity Entité parente
         */
        void setParentEntity(Entity *entity) { m_parent = entity; }

        /**
         * @brief Récupère l'entité parente du composant
         *
         * @return Pointeur vers l'entité parente
         */
        Entity *getParentEntity() const { return m_parent; }

    private:
        std::string m_id; ///< Identifiant unique du composant
        Entity *m_parent; ///< Pointeur vers l'entité parente
    };

    /**
     * @brief Alias pour un pointeur intelligent vers un Component
     */
    using ComponentPtr = std::shared_ptr<Component>;

    /**
     * @brief Crée un nouveau composant du type spécifié
     *
     * @tparam T Type de composant à créer (doit hériter de Component)
     * @tparam Args Types des arguments du constructeur
     * @param id Identifiant du composant
     * @param args Arguments à transmettre au constructeur
     * @return Pointeur intelligent vers le composant créé
     */
    template <typename T, typename... Args>
    ComponentPtr createComponent(const std::string &id, Args &&...args)
    {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        return std::make_shared<T>(id, std::forward<Args>(args)...);
    }

} // namespace Orenji