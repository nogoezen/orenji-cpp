#pragma once

#include "../core/Entity.h"
#include "../models/Player.h"
#include "TransformComponent.h"
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

class PlayerComponent : public Component
{
public:
    PlayerComponent(std::shared_ptr<Player> player);
    ~PlayerComponent() override = default;

    void initialize() override;
    void update(float deltaTime) override;
    void render() override;

    // Getters et setters
    std::shared_ptr<Player> getPlayer() const { return m_player; }
    void setPlayer(std::shared_ptr<Player> player) { m_player = player; }

    // Méthodes de mouvement
    void move(float x, float y);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

    // Méthodes de combat
    void attack();
    void defend();
    void takeDamage(int damage);

    // Méthodes de commerce
    void startTrading();
    void endTrading();

private:
    std::shared_ptr<Player> m_player;
    std::shared_ptr<TransformComponent> m_transform;
    std::shared_ptr<SpriteComponent> m_sprite;

    // État du composant
    bool m_isTrading;
    bool m_isInCombat;
    float m_moveSpeed;
};