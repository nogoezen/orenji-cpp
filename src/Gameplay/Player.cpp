#include "../../include/Gameplay/Player.hpp"

namespace Gameplay
{
    Player::Player(Core::EntityManager &manager, const sf::Vector2f &position)
        : Core::Entity(manager, "Player"), m_movementSpeed(200.0f), m_jumpForce(350.0f), m_isGrounded(false), m_isFacingRight(true), m_physicsComponent(nullptr), m_spriteComponent(nullptr)
    {
        initialize();

        // Set initial position
        if (m_physicsComponent)
        {
            m_physicsComponent->setPosition(position.x, position.y);
        }
    }

    void Player::initialize()
    {
        // Add physics component
        m_physicsComponent = addComponent<Physics::PhysicsComponent>();
        if (m_physicsComponent)
        {
            // Set up player's physical properties
            m_physicsComponent->initialize(Physics::BodyType::Dynamic, 1.0f, 0.1f, 0.0f);

            // Create a box shape for the player
            m_physicsComponent->addBoxShape(32.0f, 64.0f);

            // Set player collision filter
            m_physicsComponent->setCollisionFilter(
                Physics::CollisionCategory::PLAYER,
                Physics::CollisionCategory::GROUND |
                    Physics::CollisionCategory::ENEMY |
                    Physics::CollisionCategory::SENSOR);

            // Register collision callback to detect ground contact
            m_physicsComponent->setCollisionCallback([this](const Physics::CollisionResult &result)
                                                     {
                                                         // Check if collision is with ground
                                                         if (result.categoryB & Physics::CollisionCategory::GROUND)
                                                         {
                                                             // Check if contact point is below the player (ground contact)
                                                             if (result.normal.y < -0.5f)
                                                             {
                                                                 m_isGrounded = true;
                                                             }
                                                         }

                                                         return true; // Continue with physics resolution
                                                     });
        }

        // Add sprite component
        m_spriteComponent = addComponent<Graphics::Components::SpriteComponent>();
        if (m_spriteComponent)
        {
            // Set the origin to center of sprite for proper physics alignment
            m_spriteComponent->setOrigin(16.0f, 32.0f);

            // Set render layer (higher draws on top)
            m_spriteComponent->setLayer(10);
        }
    }

    void Player::update(float deltaTime)
    {
        handleInput(deltaTime);

        // Reset grounded state each frame (will be set by collision callback if touching ground)
        m_isGrounded = false;

        // Update sprite position to match physics
        if (m_physicsComponent && m_spriteComponent)
        {
            sf::Vector2f position = m_physicsComponent->getPosition();
            m_spriteComponent->getSprite().setPosition(position);

            // Flip sprite based on facing direction
            if (m_isFacingRight && m_spriteComponent->getSprite().getScale().x < 0)
            {
                m_spriteComponent->getSprite().setScale(1.0f, 1.0f);
            }
            else if (!m_isFacingRight && m_spriteComponent->getSprite().getScale().x > 0)
            {
                m_spriteComponent->getSprite().setScale(-1.0f, 1.0f);
            }
        }
    }

    void Player::handleInput(float deltaTime)
    {
        if (!m_physicsComponent)
            return;

        // Reset horizontal velocity
        sf::Vector2f velocity = m_physicsComponent->getLinearVelocity();
        velocity.x = 0.0f;

        // Handle horizontal movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            velocity.x = -m_movementSpeed;
            m_isFacingRight = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            velocity.x = m_movementSpeed;
            m_isFacingRight = true;
        }

        // Handle jumping
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) &&
            m_isGrounded)
        {
            velocity.y = -m_jumpForce;
            m_isGrounded = false;
        }

        // Apply the velocity
        m_physicsComponent->setLinearVelocity(velocity);
    }

    void Player::setMovementSpeed(float speed)
    {
        m_movementSpeed = speed;
    }

    float Player::getMovementSpeed() const
    {
        return m_movementSpeed;
    }

    void Player::setJumpForce(float force)
    {
        m_jumpForce = force;
    }

    float Player::getJumpForce() const
    {
        return m_jumpForce;
    }

    bool Player::isGrounded() const
    {
        return m_isGrounded;
    }
} // namespace Gameplay