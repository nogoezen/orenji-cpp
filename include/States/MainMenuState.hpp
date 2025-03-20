#pragma once
#include <SFML/Graphics.hpp>
#include "State.hpp"

namespace Orenji {

class MainMenuState : public State {
public:
    MainMenuState();
    virtual ~MainMenuState();

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void update(float deltaTime) override;
    virtual void render() override;
    virtual void handleInput() override;

private:
    sf::Font m_font;
    sf::Text m_titleText;
    sf::Text m_startGameText;
    sf::Text m_exitText;
    
    int m_selectedOption;
    
    void updateMenuDisplay();
};

} // namespace Orenji 