#pragma once
#include <SFML/Graphics.hpp>
#include "State.hpp"

namespace Orenji
{

    class MainMenuState : public State
    {
    public:
        MainMenuState();
        virtual ~MainMenuState();

        virtual bool onEnter() override;
        virtual bool onExit() override;
        virtual bool update(float deltaTime) override;
        virtual bool render() override;
        virtual bool handleInput() override;

    private:
        sf::Font m_font;
        sf::Text m_titleText;
        sf::Text m_startGameText;
        sf::Text m_exitText;

        int m_selectedOption;

        void updateMenuDisplay();
    };

} // namespace Orenji