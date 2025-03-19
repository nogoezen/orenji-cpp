#include "MainMenuState.h"
#include "../components/TransformComponent.h"
#include "../components/SpriteComponent.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

// Assuming this is available or will be created
extern sf::RenderWindow *g_window;

MainMenuState::MainMenuState()
    : State("MainMenu"), m_currentItemIndex(0), m_menuChanged(true)
{
    m_font = std::make_shared<sf::Font>();
    if (!m_font->loadFromFile("assets/fonts/default.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }
}

void MainMenuState::enter()
{
    setActive(true);

    // Initialize the entity manager and create menu entities
    auto &entityManager = getEntityManager();

    // Create a background entity
    auto backgroundEntity = entityManager.createEntity("Background");
    auto transform = backgroundEntity->addComponent<TransformComponent>(400.0f, 300.0f);
    auto sprite = backgroundEntity->addComponent<SpriteComponent>("assets/images/menu_background.png");

    // Initialize the menu items
    initMenu();

    std::cout << "Main Menu State Entered" << std::endl;
}

void MainMenuState::exit()
{
    setActive(false);
    getEntityManager().destroyAllEntities();
    std::cout << "Main Menu State Exited" << std::endl;
}

void MainMenuState::pause()
{
    State::pause();
    std::cout << "Main Menu State Paused" << std::endl;
}

void MainMenuState::resume()
{
    State::resume();
    m_menuChanged = true;
    std::cout << "Main Menu State Resumed" << std::endl;
}

void MainMenuState::handleInput()
{
    sf::Event event;
    while (g_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            g_window->close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                g_window->close();
            }
            else if (event.key.code == sf::Keyboard::Up)
            {
                if (m_currentItemIndex > 0)
                {
                    m_currentItemIndex--;
                    m_menuChanged = true;
                }
            }
            else if (event.key.code == sf::Keyboard::Down)
            {
                if (m_currentItemIndex < m_menuItems.size() - 1)
                {
                    m_currentItemIndex++;
                    m_menuChanged = true;
                }
            }
            else if (event.key.code == sf::Keyboard::Return)
            {
                if (m_currentItemIndex >= 0 && m_currentItemIndex < m_menuItems.size())
                {
                    m_menuItems[m_currentItemIndex].action();
                }
            }
        }
    }
}

void MainMenuState::update(float deltaTime)
{
    // Update all entities in the state
    getEntityManager().update(deltaTime);

    if (m_menuChanged)
    {
        updateMenuDisplay();
        m_menuChanged = false;
    }
}

void MainMenuState::render()
{
    // Render all entities in the state
    getEntityManager().render();

    // Render the menu items
    for (const auto &item : m_menuItems)
    {
        g_window->draw(item.text);
    }
}

void MainMenuState::initMenu()
{
    // Clear existing menu items
    m_menuItems.clear();

    // Define menu items and their actions
    MenuItem newGameItem;
    newGameItem.text.setFont(*m_font);
    newGameItem.text.setString("New Game");
    newGameItem.text.setCharacterSize(30);
    newGameItem.text.setFillColor(sf::Color::White);
    newGameItem.action = [this]()
    {
        std::cout << "Starting new game..." << std::endl;
        // Here you would transition to the game state
        // Example: stateManager->pushState("GameState");
    };

    MenuItem loadGameItem;
    loadGameItem.text.setFont(*m_font);
    loadGameItem.text.setString("Load Game");
    loadGameItem.text.setCharacterSize(30);
    loadGameItem.text.setFillColor(sf::Color::White);
    loadGameItem.action = [this]()
    {
        std::cout << "Loading game..." << std::endl;
        // Here you would open a load game dialog or state
    };

    MenuItem optionsItem;
    optionsItem.text.setFont(*m_font);
    optionsItem.text.setString("Options");
    optionsItem.text.setCharacterSize(30);
    optionsItem.text.setFillColor(sf::Color::White);
    optionsItem.action = [this]()
    {
        std::cout << "Opening options..." << std::endl;
        // Here you would transition to the options state
    };

    MenuItem exitItem;
    exitItem.text.setFont(*m_font);
    exitItem.text.setString("Exit");
    exitItem.text.setCharacterSize(30);
    exitItem.text.setFillColor(sf::Color::White);
    exitItem.action = [this]()
    {
        std::cout << "Exiting game..." << std::endl;
        g_window->close();
    };

    // Add items to the menu
    m_menuItems.push_back(newGameItem);
    m_menuItems.push_back(loadGameItem);
    m_menuItems.push_back(optionsItem);
    m_menuItems.push_back(exitItem);

    // Initial positioning of menu items
    updateMenuDisplay();
}

void MainMenuState::handleMenuNavigation()
{
    // This functionality is now in handleInput
}

void MainMenuState::updateMenuDisplay()
{
    // Position and style the menu items
    const float yStart = 200.0f;
    const float ySpacing = 50.0f;

    for (size_t i = 0; i < m_menuItems.size(); ++i)
    {
        // Set position
        m_menuItems[i].text.setPosition(400.0f, yStart + i * ySpacing);

        // Center the text
        sf::FloatRect textBounds = m_menuItems[i].text.getLocalBounds();
        m_menuItems[i].text.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);

        // Set color based on selection
        if (i == m_currentItemIndex)
        {
            m_menuItems[i].text.setFillColor(sf::Color::Yellow);
        }
        else
        {
            m_menuItems[i].text.setFillColor(sf::Color::White);
        }
    }
}