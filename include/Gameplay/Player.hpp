#pragma once

#include "../Core/Entity.hpp"
#include "../Physics/PhysicsComponent.hpp"
#include "../Graphics/Components/SpriteComponent.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

namespace Gameplay
{
    /**
     * @brief Player class that represents the player entity
     */
    class Player : public Core::Entity
    {
    public:
        /**
         * @brief Constructor
         * @param manager EntityManager that owns this entity
         * @param position Initial position for the player
         */
        Player(Core::EntityManager &manager, const sf::Vector2f &position = sf::Vector2f(0.0f, 0.0f));

        /**
         * @brief Destructor
         */
        virtual ~Player() = default;

        /**
         * @brief Initialize the player
         */
        void initialize();

        /**
         * @brief Update the player state
         * @param deltaTime Time elapsed since last update
         */
        void update(float deltaTime);

        /**
         * @brief Apply movement based on input
         * @param deltaTime Time elapsed since last update
         */
        void handleInput(float deltaTime);

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
         * @brief Set jump force
         * @param force Jump force value
         */
        void setJumpForce(float force);

        /**
         * @brief Get jump force
         * @return Current jump force
         */
        float getJumpForce() const;

        /**
         * @brief Check if player is grounded
         * @return True if player is on ground
         */
        bool isGrounded() const;

    private:
        float m_movementSpeed;
        float m_jumpForce;
        bool m_isGrounded;
        bool m_isFacingRight;
        Physics::PhysicsComponent *m_physicsComponent;
        Graphics::Components::SpriteComponent *m_spriteComponent;
    };
} // namespace Gameplay