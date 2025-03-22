#include "PlayerComponent.hpp"
#include <SFML/Window/Keyboard.hpp>

PlayerComponent::PlayerComponent(std::shared_ptr<Player> player)
    : m_player(player), m_isTrading(false), m_isInCombat(false), m_moveSpeed(200.0f)
{
}

void PlayerComponent::initialize()
{
    if (auto entity = getEntity())
    {
        // Ajouter les composants nécessaires
        m_transform = entity->addComponent<TransformComponent>();
        m_sprite = entity->addComponent<SpriteComponent>("resources/textures/player.png");

        // Initialiser la position
        if (m_player)
        {
            auto pos = m_player->getCurrentPosition();
            setPosition(pos.x, pos.y);
        }
    }
}

void PlayerComponent::update(float deltaTime)
{
    if (!m_player || !m_transform)
        return;

    // Mise à jour de la position du joueur dans le modèle
    auto pos = m_transform->getPosition();
    m_player->setCurrentPosition(pos.x, pos.y);

    // Gestion du mouvement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        move(0, -m_moveSpeed * deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        move(0, m_moveSpeed * deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        move(-m_moveSpeed * deltaTime, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        move(m_moveSpeed * deltaTime, 0);
    }
}

void PlayerComponent::render()
{
    // Le rendu est géré par le SpriteComponent
}

void PlayerComponent::move(float x, float y)
{
    if (m_transform)
    {
        m_transform->move(x, y);
    }
}

void PlayerComponent::setPosition(float x, float y)
{
    if (m_transform)
    {
        m_transform->setPosition(x, y);
    }
}

sf::Vector2f PlayerComponent::getPosition() const
{
    if (m_transform)
    {
        return m_transform->getPosition();
    }
    return sf::Vector2f(0, 0);
}

void PlayerComponent::attack()
{
    if (m_player)
    {
        // Implémenter la logique d'attaque
    }
}

void PlayerComponent::defend()
{
    if (m_player)
    {
        // Implémenter la logique de défense
    }
}

void PlayerComponent::takeDamage(int damage)
{
    if (m_player)
    {
        // Implémenter la logique de dégâts
    }
}

void PlayerComponent::startTrading()
{
    m_isTrading = true;
    if (m_player)
    {
        // Implémenter la logique de début de commerce
    }
}

void PlayerComponent::endTrading()
{
    m_isTrading = false;
    if (m_player)
    {
        // Implémenter la logique de fin de commerce
    }
}
