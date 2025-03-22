#include "../../include/States/MainMenuState.hpp"
#include "../../include/States/StateMachine.hpp"
#include "../../include/States/GameState.hpp"
#include <iostream>

namespace Orenji
{

    MainMenuState::MainMenuState(StateMachine &stateMachine) : State(stateMachine)
    {
        // Initialize menu state
    }

    void MainMenuState::onEnter()
    {
        // Load resources
        if (!m_backgroundTexture.loadFromFile("resources/textures/MainMenuBG.png"))
        {
            std::cerr << "Failed to load main menu background texture" << std::endl;
        }

        if (!m_font.loadFromFile("resources/fonts/arial.ttf"))
        {
            std::cerr << "Failed to load font" << std::endl;
        }

        // Setup background
        m_backgroundSprite.setTexture(m_backgroundTexture);

        // Setup menu options
        const int optionCount = static_cast<int>(MenuOptions::Count);
        m_menuTexts.resize(optionCount);

        const std::array<std::string, optionCount> optionLabels = {"Play", "Options", "Quit"};

        for (int i = 0; i < optionCount; ++i)
        {
            m_menuTexts[i].setFont(m_font);
            m_menuTexts[i].setString(optionLabels[i]);
            m_menuTexts[i].setCharacterSize(30);
            m_menuTexts[i].setPosition(400, 250 + i * 60);
            m_menuTexts[i].setOrigin(m_menuTexts[i].getGlobalBounds().width / 2.f,
                                     m_menuTexts[i].getGlobalBounds().height / 2.f);
        }

        // Initialize selection
        updateMenuSelection();
    }

    void MainMenuState::onExit()
    {
        // Cleanup if needed
    }

    void MainMenuState::handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                m_currentOption = (m_currentOption - 1 + static_cast<int>(MenuOptions::Count)) % static_cast<int>(MenuOptions::Count);
                updateMenuSelection();
                break;

            case sf::Keyboard::Down:
                m_currentOption = (m_currentOption + 1) % static_cast<int>(MenuOptions::Count);
                updateMenuSelection();
                break;

            case sf::Keyboard::Return:
                switch (static_cast<MenuOptions>(m_currentOption))
                {
                case MenuOptions::Play:
                    m_stateMachine.pushState<GameState>();
                    break;

                case MenuOptions::Options:
                    // TODO: Push options state
                    break;

                case MenuOptions::Quit:
                    // Exit the application
                    // This would be handled by the Game class
                    break;

                default:
                    break;
                }
                break;

            default:
                break;
            }
        }
    }

    void MainMenuState::update(float deltaTime)
    {
        // Update menu animations if needed
    }

    void MainMenuState::render(sf::RenderTarget &target)
    {
        // Draw background
        target.draw(m_backgroundSprite);

        // Draw menu options
        for (const auto &text : m_menuTexts)
        {
            target.draw(text);
        }
    }

    void MainMenuState::updateMenuSelection()
    {
        const int optionCount = static_cast<int>(MenuOptions::Count);

        for (int i = 0; i < optionCount; ++i)
        {
            if (i == m_currentOption)
            {
                m_menuTexts[i].setFillColor(sf::Color::Yellow);
                m_menuTexts[i].setStyle(sf::Text::Bold);
            }
            else
            {
                m_menuTexts[i].setFillColor(sf::Color::White);
                m_menuTexts[i].setStyle(sf::Text::Regular);
            }
        }
    }

} // namespace Orenji