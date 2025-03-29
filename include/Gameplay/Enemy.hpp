#pragma once

#include "../Core/Entity.hpp"
#include "../Physics/PhysicsComponent.hpp"
#include "../Graphics/Components/SpriteComponent.hpp"
#include "../AI/Components/AIComponent.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

namespace Gameplay
{
    /**
     * @brief Base Enemy class for AI-controlled entities
     */
    class Enemy : public Core::Entity
    {
    public:
        /**
         * @brief Constructor
         * @param manager EntityManager that owns this entity
         * @param behaviorTreePath Path to the behavior tree XML file
         * @param position Initial position for the enemy
         */
        Enemy(Core::EntityManager &manager,
              const std::string &behaviorTreePath,
              const sf::Vector2f &position = sf::Vector2f(0.0f, 0.0f));

        /**
         * @brief Destructor
         */
        virtual ~Enemy() = default;

        /**
         * @brief Initialize the enemy components
         */
        virtual void initialize();

        /**
         * @brief Update the enemy state
         * @param deltaTime Time elapsed since last update
         */
        virtual void update(float deltaTime);

        /**
         * @brief Set the movement speed
         * @param speed New movement speed
         */
        void setMovementSpeed(float speed);

        /**
         * @brief Get the movement speed
         * @return Current movement speed
         */
        float getMovementSpeed() const;

        /**
         * @brief Set maximum health
         * @param maxHealth Maximum health value
         */
        void setMaxHealth(float maxHealth);

        /**
         * @brief Get maximum health
         * @return Maximum health value
         */
        float getMaxHealth() const;

        /**
         * @brief Set current health
         * @param health Current health value
         */
        void setHealth(float health);

        /**
         * @brief Get current health
         * @return Current health value
         */
        float getHealth() const;

        /**
         * @brief Damage the enemy
         * @param amount Amount of damage to apply
         * @return True if enemy is still alive
         */
        bool damage(float amount);

        /**
         * @brief Check if enemy is alive
         * @return True if health > 0
         */
        bool isAlive() const;

    protected:
        float m_movementSpeed;
        float m_maxHealth;
        float m_health;
        std::string m_behaviorTreePath;
        bool m_isFacingRight;

        Physics::PhysicsComponent *m_physicsComponent;
        Graphics::Components::SpriteComponent *m_spriteComponent;
        AI::Components::AIComponent *m_aiComponent;
    };
} // namespace Gameplay