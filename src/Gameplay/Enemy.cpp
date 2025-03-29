#include "Gameplay/Enemy.hpp"
#include "Core/EntityManager.hpp"
#include "Physics/CollisionCategories.hpp"

namespace Gameplay
{
    Enemy::Enemy(Core::EntityManager &manager,
                 const std::string &behaviorTreePath,
                 const sf::Vector2f &position)
        : Entity(manager), m_movementSpeed(100.0f), m_maxHealth(100.0f), m_health(100.0f), m_behaviorTreePath(behaviorTreePath), m_isFacingRight(true), m_physicsComponent(nullptr), m_spriteComponent(nullptr), m_aiComponent(nullptr)
    {
        setPosition(position);
    }

    void Enemy::initialize()
    {
        // Create physics component
        m_physicsComponent = &addComponent<Physics::PhysicsComponent>();

        // Set up collision box
        m_physicsComponent->createBody(b2BodyType::b2_dynamicBody);
        m_physicsComponent->addBoxShape(sf::Vector2f(32.0f, 48.0f));

        // Set collision filtering
        m_physicsComponent->setFilterData(
            Physics::CollisionCategory::ENEMY,
            Physics::CollisionCategory::GROUND |
                Physics::CollisionCategory::PLAYER |
                Physics::CollisionCategory::PROJECTILE);

        // Set physical properties
        m_physicsComponent->setFixedRotation(true);
        m_physicsComponent->setDensity(1.0f);
        m_physicsComponent->setFriction(0.3f);

        // Initialize graphical representation
        m_spriteComponent = &addComponent<Graphics::Components::SpriteComponent>();
        m_spriteComponent->setSize(sf::Vector2f(64.0f, 64.0f));
        m_spriteComponent->setColor(sf::Color::Red); // Default color until texture is loaded

        // Initialize AI component with behavior tree
        m_aiComponent = &addComponent<AI::Components::AIComponent>();
        m_aiComponent->loadBehaviorTree(m_behaviorTreePath);

        // Set up blackboard with initial values
        auto &blackboard = m_aiComponent->getBlackboard();
        blackboard.setFloat("movementSpeed", m_movementSpeed);
        blackboard.setFloat("health", m_health);
        blackboard.setBool("isFacingRight", m_isFacingRight);
    }

    void Enemy::update(float deltaTime)
    {
        // Update the AI component first to make decisions
        if (m_aiComponent)
        {
            m_aiComponent->update(deltaTime);

            // Read updated values from the blackboard
            auto &blackboard = m_aiComponent->getBlackboard();
            m_isFacingRight = blackboard.getBool("isFacingRight", m_isFacingRight);
        }

        // Update the sprite based on facing direction
        if (m_spriteComponent)
        {
            if (m_isFacingRight)
            {
                m_spriteComponent->setScale(1.0f, 1.0f);
            }
            else
            {
                m_spriteComponent->setScale(-1.0f, 1.0f);
            }
        }

        // Base entity update will update all components
        Entity::update(deltaTime);
    }

    void Enemy::setMovementSpeed(float speed)
    {
        m_movementSpeed = speed;

        // Update AI blackboard
        if (m_aiComponent)
        {
            m_aiComponent->getBlackboard().setFloat("movementSpeed", m_movementSpeed);
        }
    }

    float Enemy::getMovementSpeed() const
    {
        return m_movementSpeed;
    }

    void Enemy::setMaxHealth(float maxHealth)
    {
        m_maxHealth = maxHealth;

        // Ensure current health doesn't exceed maximum
        if (m_health > m_maxHealth)
        {
            m_health = m_maxHealth;
        }

        // Update AI blackboard
        if (m_aiComponent)
        {
            m_aiComponent->getBlackboard().setFloat("maxHealth", m_maxHealth);
        }
    }

    float Enemy::getMaxHealth() const
    {
        return m_maxHealth;
    }

    void Enemy::setHealth(float health)
    {
        m_health = std::max(0.0f, std::min(health, m_maxHealth));

        // Update AI blackboard
        if (m_aiComponent)
        {
            m_aiComponent->getBlackboard().setFloat("health", m_health);
        }
    }

    float Enemy::getHealth() const
    {
        return m_health;
    }

    bool Enemy::damage(float amount)
    {
        setHealth(m_health - amount);

        // Notify AI that entity was damaged
        if (m_aiComponent)
        {
            m_aiComponent->getBlackboard().setBool("wasDamaged", true);
            m_aiComponent->getBlackboard().setFloat("lastDamageAmount", amount);
        }

        return isAlive();
    }

    bool Enemy::isAlive() const
    {
        return m_health > 0.0f;
    }
} // namespace Gameplay