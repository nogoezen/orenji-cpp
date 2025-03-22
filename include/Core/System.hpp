#pragma once

namespace Core
{

    class EntityManager;

    /**
     * @brief Base interface for all systems
     */
    class System
    {
    public:
        /**
         * @brief Constructor
         * @param entityManager Reference to entity manager
         */
        System(EntityManager &entityManager);

        /**
         * @brief Virtual destructor
         */
        virtual ~System();

        /**
         * @brief Initialize the system
         */
        virtual void init() = 0;

        /**
         * @brief Update the system
         * @param deltaTime Time since last frame in seconds
         */
        virtual void update(float deltaTime) = 0;

    protected:
        EntityManager &m_entityManager;
    };

} // namespace Core