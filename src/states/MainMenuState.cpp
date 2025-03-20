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
        // Note: We expect the Game class to handle window clearing
        // and display, so we just draw the menu elements
        sf::RenderWindow *window = nullptr; // TODO: Get window from game singleton

        if (window)
        {
            window->draw(m_titleText);
            window->draw(m_startGameText);
            window->draw(m_exitText);
        }
        return true;
    }

    bool MainMenuState::handleInput()
    {
        // Handle key presses for menu navigation
        // Check for key presses, move selection up or down
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            m_selectedOption = (m_selectedOption + 1) % 2;
            updateMenuDisplay();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            m_selectedOption = (m_selectedOption + 1) % 2;
            updateMenuDisplay();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            if (m_selectedOption == 0)
            {
                // Start Game option selected
                // Request state change
                if (getParent())
                {
                    getParent()->changeState(std::make_shared<GameState>());
                }
            }
            else
            {
                // Exit option selected
                // Exit the game
                // TODO: Signal game to exit
            }
        }
        return true;
    }

    void MainMenuState::updateMenuDisplay()
    {
        // Update text colors based on which option is selected
        if (m_selectedOption == 0)
        {
            m_startGameText.setFillColor(sf::Color::Yellow);
            m_exitText.setFillColor(sf::Color::White);
        }
        else
        {
            m_startGameText.setFillColor(sf::Color::White);
            m_exitText.setFillColor(sf::Color::Yellow);
        }
    }

} // namespace Orenji