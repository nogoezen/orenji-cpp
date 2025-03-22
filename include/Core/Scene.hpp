#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

namespace Core
{

    class EntityManager;

    /**
     * @brief Base class for all scenes
     */
    class Scene
    {
    public:
        /**
         * @brief Constructor
         * @param name Scene name
         */
        Scene(const std::string &name);

        /**
         * @brief Virtual destructor
         */
        virtual ~Scene();

        /**
         * @brief Initialize the scene
         */
        virtual void init() = 0;

        /**
         * @brief Update the scene
         * @param deltaTime Time since last frame in seconds
         */
        virtual void update(float deltaTime) = 0;

        /**
         * @brief Render the scene
         * @param window Reference to the render window
         */
        virtual void render(sf::RenderWindow &window) = 0;

        /**
         * @brief Handle an event
         * @param event SFML event
         */
        virtual void handleEvent(const sf::Event &event) = 0;

        /**
         * @brief Get the scene name
         * @return Scene name
         */
        const std::string &getName() const;

        /**
         * @brief Set the entity manager
         * @param entityManager Pointer to the entity manager
         */
        void setEntityManager(EntityManager *entityManager);

        /**
         * @brief Get the entity manager
         * @return Pointer to the entity manager
         */
        EntityManager *getEntityManager() const;

    protected:
        std::string m_name;
        EntityManager *m_entityManager;
    };

} // namespace Core