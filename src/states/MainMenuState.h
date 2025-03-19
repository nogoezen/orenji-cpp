#pragma once

#include "../core/State.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <vector>
#include <functional>

class MainMenuState : public State
{
public:
    struct MenuItem
    {
        sf::Text text;
        std::function<void()> action;
    };

    MainMenuState();
    ~MainMenuState() override = default;

    void enter() override;
    void exit() override;
    void pause() override;
    void resume() override;

    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;

private:
    void initMenu();
    void handleMenuNavigation();
    void updateMenuDisplay();

    std::shared_ptr<sf::Font> m_font;
    std::vector<MenuItem> m_menuItems;
    int m_currentItemIndex;
    bool m_menuChanged;
};