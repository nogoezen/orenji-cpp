#pragma once

namespace Core
{

    // Forward declaration to avoid circular dependency
    class Entity;

    /**
     * @brief Base class for all components
     */
    class Component
    {
    public:
        /**
         * @brief Constructor
         */
        Component();

        /**
         * @brief Virtual destructor
         */
        virtual ~Component();

        /**
         * @brief Initialize the component
         */
        virtual void init();

        /**
         * @brief Update the component
         * @param deltaTime Time since last frame in seconds
         */
        virtual void update(float deltaTime);

        /**
         * @brief Set the owner entity
         * @param owner Pointer to the owner entity
         */
        void setOwner(Entity *owner);

        /**
         * @brief Get the owner entity
         * @return Pointer to the owner entity
         */
        Entity *getOwner() const;

        /**
         * @brief Check if the component is active
         * @return true if the component is active
         */
        bool isActive() const;

        /**
         * @brief Set whether the component is active
         * @param active Active state
         */
        void setActive(bool active);

    private:
        Entity *m_owner;
        bool m_active;
    };

} // namespace Core