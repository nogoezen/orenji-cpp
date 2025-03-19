#pragma once

#include "../core/State.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <memory>

class TitleScreenState : public State
{
public:
    TitleScreenState();
    ~TitleScreenState() override = default;

    void enter() override;
    void exit() override;
    void pause() override;
    void resume() override;

    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;

private:
    // Ressources graphiques
    std::shared_ptr<sf::Font> m_font;
    std::shared_ptr<sf::Sprite> m_background;
    std::shared_ptr<sf::Text> m_titleText;
    std::shared_ptr<sf::Text> m_pressStartText;

    // Animation
    float m_blinkTimer;
    bool m_showPressStart;

    // Méthodes utilitaires
    void initializeGraphics();
    void updateAnimation(float deltaTime);
};