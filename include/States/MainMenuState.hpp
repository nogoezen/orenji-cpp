#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"

namespace Orenji
{

    /**
     * @class MainMenuState
     * @brief State for the main menu
     */
    class MainMenuState : public State
    {
    public:
        /**
         * @brief Constructor
         * @param stateMachine Reference to the state machine
         */
        explicit MainMenuState(StateMachine &stateMachine);

        /**
         * @brief Called when the state is entered
         */
        virtual void onEnter() override;

        /**
         * @brief Called when the state is exited
         */
        virtual void onExit() override;

        /**
         * @brief Handle input events
         * @param event SFML event to handle
         */
        virtual void handleEvent(const sf::Event &event) override;

        /**
         * @brief Update the state
         * @param deltaTime Time elapsed since last update
         */
        virtual void update(float deltaTime) override;

        /**
         * @brief Render the state
         * @param target Render target
         */
        virtual void render(sf::RenderTarget &target) override;

    private:
        // Menu options
        enum class MenuOptions
        {
            Play,
            Options,
            Quit,
            Count // Used to count the number of options
        };

        int m_currentOption = 0;
        sf::Texture m_backgroundTexture;
        sf::Sprite m_backgroundSprite;
        sf::Font m_font;
        std::vector<sf::Text> m_menuTexts;

        void updateMenuSelection();
    };

} // namespace Orenji