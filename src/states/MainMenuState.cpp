#include "../../include/States/MainMenuState.hpp"
#include "../../include/States/GameState.hpp"
#include "../../include/States/StateMachine.hpp"
#include <iostream>

namespace Orenji
{

    MainMenuState::MainMenuState() : m_selectedOption(0)
    {
    }

    MainMenuState::~MainMenuState()
    {
    }

    bool MainMenuState::onEnter()
    {
        // Load resources
        if (!m_font.loadFromFile("resources/fonts/VeniceClassic.ttf"))
        {
            std::cerr << "Error: Unable to load font" << std::endl;
            return false;
        }

        // Set up title text
        m_titleText.setFont(m_font);
        m_titleText.setString("Orenji - Uncharted Waters");
        m_titleText.setCharacterSize(48);
        m_titleText.setFillColor(sf::Color::White);
        m_titleText.setStyle(sf::Text::Bold);
        m_titleText.setPosition(400, 100);
        m_titleText.setOrigin(m_titleText.getLocalBounds().width / 2,
                              m_titleText.getLocalBounds().height / 2);

        // Set up menu options
        m_startGameText.setFont(m_font);
        m_startGameText.setString("Start Game");
        m_startGameText.setCharacterSize(32);
        m_startGameText.setPosition(400, 300);
        m_startGameText.setOrigin(m_startGameText.getLocalBounds().width / 2,
                                  m_startGameText.getLocalBounds().height / 2);

        m_exitText.setFont(m_font);
        m_exitText.setString("Exit");
        m_exitText.setCharacterSize(32);
        m_exitText.setPosition(400, 350);
        m_exitText.setOrigin(m_exitText.getLocalBounds().width / 2,
                             m_exitText.getLocalBounds().height / 2);

        updateMenuDisplay();
        return true;
    }

    bool MainMenuState::onExit()
    {
        // Clean up resources if needed
        return true;
    }

    bool MainMenuState::update(float deltaTime)
    {
        // Nothing to do here for a simple menu
        return true;
    }

    bool MainMenuState::render()
    {
        if (getStateMachine())
        {
            sf::RenderWindow &window = getStateMachine()->getWindow();
            window.clear(sf::Color(0, 0, 68)); // Navy blue background
            window.draw(m_titleText);
            window.draw(m_startGameText);
            window.draw(m_exitText);
        }
        return true;
    }

    bool MainMenuState::handleInput()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            if (m_selectedOption < 1) // 1 is the max index (Exit option)
            {
                m_selectedOption++;
                updateMenuDisplay();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            if (m_selectedOption > 0)
            {
                m_selectedOption--;
                updateMenuDisplay();
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            if (m_selectedOption == 0) // Start Game
            {
                if (getStateMachine())
                {
                    getStateMachine()->changeState(std::make_unique<GameState>());
                }
            }
            else if (m_selectedOption == 1) // Exit
            {
                if (getStateMachine())
                {
                    getStateMachine()->getWindow().close();
                }
            }
        }
        return true;
    }

    void MainMenuState::updateMenuDisplay()
    {
        // Reset all text to white
        m_startGameText.setFillColor(sf::Color::White);
        m_exitText.setFillColor(sf::Color::White);

        // Highlight selected option
        switch (m_selectedOption)
        {
        case 0:
            m_startGameText.setFillColor(sf::Color::Yellow);
            break;
        case 1:
            m_exitText.setFillColor(sf::Color::Yellow);
            break;
        }
    }

} // namespace Orenji