#include "../../include/Scenes/MainMenuScene.hpp"
#include "../../include/Engine.hpp"
#include "../../include/Scenes/GameScene.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace Scenes
{
    MainMenuScene::MainMenuScene(Core::Engine &engine)
        : Scene("MainMenu"), m_engine(engine), m_background(), m_showDemosList(false),
          m_selectedDemo(0), m_selectedItem(0), m_music(nullptr)
    {
        std::cout << "MainMenuScene created" << std::endl;
    }

    MainMenuScene::~MainMenuScene()
    {
        std::cout << "MainMenuScene destroyed" << std::endl;
    }

    void MainMenuScene::init()
    {
        // Load font
        try
        {
            m_resourceManager.loadFont("main", "resources/fonts/VeniceClassic.ttf");
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to load main font: " << e.what() << std::endl;
            return;
        }

        // Load background image
        try
        {
            m_resourceManager.loadTexture("menu_bg", "resources/textures/Titles/title-bg.png");
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to load background image: " << e.what() << std::endl;
            return;
        }

        // Create title text
        try
        {
            sf::Font &font = m_resourceManager.getFont("main");
            m_titleText = std::make_unique<sf::Text>("Orenji Engine", font, 72);
            m_titleText->setFillColor(sf::Color(255, 128, 0));
            m_titleText->setOutlineColor(sf::Color(128, 64, 0));
            m_titleText->setOutlineThickness(2.0f);

            // SFML 3 way to center text
            sf::FloatRect bounds = m_titleText->getLocalBounds();
            m_titleText->setOrigin(sf::Vector2f(bounds.width / 2.0f, bounds.height / 2.0f));
            m_titleText->setPosition(sf::Vector2f(m_engine.getWindow().getSize().x / 2.0f, 100.0f));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to create title text: " << e.what() << std::endl;
        }

        // Create menu items
        createMenuItem("New Game", 250.0f);
        createMenuItem("Examples", 350.0f);
        createMenuItem("Options", 450.0f);
        createMenuItem("Exit", 550.0f);

        // Create background
        try
        {
            sf::Texture &bgTexture = m_resourceManager.getTexture("menu_bg");
            m_background.setTexture(bgTexture);

            sf::Vector2u windowSize = m_engine.getWindow().getSize();
            sf::Vector2u textureSize = bgTexture.getSize();

            float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
            float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);

            m_background.setScale(sf::Vector2f(scaleX, scaleY));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to create background: " << e.what() << std::endl;
        }

        // Load available demos
        loadDemosList();

        updateMenuSelection();
    }

    void MainMenuScene::update(float deltaTime)
    {
        // Update animations or other dynamic elements
    }

    void MainMenuScene::render(sf::RenderWindow &window)
    {
        // Draw background
        window.draw(m_background);

        // Draw title
        if (m_titleText)
        {
            window.draw(*m_titleText);
        }

        // Draw menu items
        for (auto &item : m_menuItems)
        {
            window.draw(item);
        }

        // Draw demos list if examples menu is active
        if (m_showDemosList)
        {
            // Draw semi-transparent background
            sf::Vector2u windowSize = window.getSize();
            sf::RectangleShape overlay(sf::Vector2f(windowSize.x, windowSize.y));
            overlay.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(overlay);

            // Draw demos title
            if (m_demosTitle)
            {
                window.draw(*m_demosTitle);
            }

            // Draw demos list
            for (auto &item : m_demoItems)
            {
                window.draw(item);
            }

            // Draw back instruction
            if (m_backText)
            {
                window.draw(*m_backText);
            }

            // Draw coming soon message
            if (m_comingSoonText)
            {
                window.draw(*m_comingSoonText);
            }
        }
    }

    void MainMenuScene::handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (m_showDemosList)
            {
                // Handle demo list navigation
                if (event.key.code == sf::Keyboard::Up)
                {
                    if (m_selectedDemo > 0)
                    {
                        m_selectedDemo--;
                        updateDemoSelection();
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (m_selectedDemo < m_demoItems.size() - 1)
                    {
                        m_selectedDemo++;
                        updateDemoSelection();
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    // Demos are not launchable yet
                    if (m_comingSoonText)
                    {
                        m_comingSoonText->setFillColor(sf::Color(255, 0, 0));
                    }
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    // Return to main menu
                    m_showDemosList = false;
                }
            }
            else
            {
                // Handle main menu navigation
                if (event.key.code == sf::Keyboard::Up)
                {
                    if (m_selectedItem > 0)
                    {
                        m_selectedItem--;
                        updateMenuSelection();
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (m_selectedItem < m_menuItems.size() - 1)
                    {
                        m_selectedItem++;
                        updateMenuSelection();
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    // Execute the selected menu item
                    switch (m_selectedItem)
                    {
                    case 0: // New Game
                        // TODO: Create and set GameScene when implemented
                        break;
                    case 1: // Examples
                        showDemosList();
                        break;
                    case 2: // Options
                        // TODO: Show options screen
                        break;
                    case 3: // Exit
                        m_engine.shutdown();
                        break;
                    }
                }
            }
        }
    }

    void MainMenuScene::createMenuItem(const std::string &text, float yPos)
    {
        try
        {
            sf::Font &font = m_resourceManager.getFont("main");
            sf::Text menuItem(text, font, 48);
            menuItem.setFillColor(sf::Color::White);

            // SFML 3 way to center text
            sf::FloatRect bounds = menuItem.getLocalBounds();
            menuItem.setOrigin(sf::Vector2f(bounds.width / 2.0f, bounds.height / 2.0f));
            menuItem.setPosition(sf::Vector2f(m_engine.getWindow().getSize().x / 2.0f, yPos));

            m_menuItems.push_back(menuItem);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to create menu item: " << e.what() << std::endl;
        }
    }

    void MainMenuScene::updateMenuSelection()
    {
        for (size_t i = 0; i < m_menuItems.size(); ++i)
        {
            if (i == m_selectedItem)
            {
                m_menuItems[i].setFillColor(sf::Color(255, 200, 0));
                m_menuItems[i].setOutlineColor(sf::Color(255, 100, 0));
                m_menuItems[i].setOutlineThickness(2.0f);
                m_menuItems[i].setStyle(sf::Text::Bold);
            }
            else
            {
                m_menuItems[i].setFillColor(sf::Color::White);
                m_menuItems[i].setOutlineThickness(0.0f);
                m_menuItems[i].setStyle(sf::Text::Regular);
            }
        }
    }

    void MainMenuScene::updateDemoSelection()
    {
        for (size_t i = 0; i < m_demoItems.size(); ++i)
        {
            if (i == m_selectedDemo)
            {
                m_demoItems[i].setFillColor(sf::Color(255, 200, 0));
                m_demoItems[i].setOutlineColor(sf::Color(255, 100, 0));
                m_demoItems[i].setOutlineThickness(2.0f);
                m_demoItems[i].setStyle(sf::Text::Bold);
            }
            else
            {
                m_demoItems[i].setFillColor(sf::Color::White);
                m_demoItems[i].setOutlineThickness(0.0f);
                m_demoItems[i].setStyle(sf::Text::Regular);
            }
        }
    }

    void MainMenuScene::centerText(sf::Text &text, const sf::Vector2f &position)
    {
        // SFML 3 way to center text
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(bounds.width / 2.0f, bounds.height / 2.0f));
        text.setPosition(position);
    }

    void MainMenuScene::loadDemosList()
    {
        m_demoCommands.clear();
        m_demoDescriptions.clear();

        // Try to load the demos list from file
        std::ifstream file("examples/demos.txt");
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                // Format changed - we now just have descriptions
                m_demoDescriptions.push_back(line);
            }
            file.close();
        }

        try
        {
            sf::Font &font = m_resourceManager.getFont("main");
            sf::Vector2u windowSize = m_engine.getWindow().getSize();

            // Create title for demos screen
            m_demosTitle = std::make_unique<sf::Text>("Available Examples", font, 60);
            m_demosTitle->setFillColor(sf::Color(255, 128, 0));
            m_demosTitle->setOutlineColor(sf::Color(128, 64, 0));
            m_demosTitle->setOutlineThickness(2.0f);
            centerText(*m_demosTitle, sf::Vector2f(windowSize.x / 2.0f, 100.0f));

            // Create back instruction text
            m_backText = std::make_unique<sf::Text>("Press ESC to return to main menu", font, 24);
            m_backText->setFillColor(sf::Color(200, 200, 200));
            centerText(*m_backText, sf::Vector2f(windowSize.x / 2.0f, windowSize.y - 50.0f));

            // Create coming soon message
            m_comingSoonText = std::make_unique<sf::Text>("These demos are coming soon!", font, 36);
            m_comingSoonText->setFillColor(sf::Color(255, 165, 0));
            centerText(*m_comingSoonText, sf::Vector2f(windowSize.x / 2.0f, windowSize.y - 120.0f));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to create demos texts: " << e.what() << std::endl;
        }
    }

    void MainMenuScene::showDemosList()
    {
        m_showDemosList = true;
        m_selectedDemo = 0;
        m_demoItems.clear();

        try
        {
            sf::Font &font = m_resourceManager.getFont("main");
            sf::Vector2u windowSize = m_engine.getWindow().getSize();
            float startY = 200.0f;
            float spacing = 60.0f;

            for (size_t i = 0; i < m_demoDescriptions.size(); ++i)
            {
                sf::Text demoItem(m_demoDescriptions[i], font, 36);
                demoItem.setFillColor(sf::Color::White);
                centerText(demoItem, sf::Vector2f(windowSize.x / 2.0f, startY + i * spacing));
                m_demoItems.push_back(demoItem);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to create demo items: " << e.what() << std::endl;
        }

        updateDemoSelection();

        // Reset coming soon message color
        if (m_comingSoonText)
        {
            m_comingSoonText->setFillColor(sf::Color(255, 165, 0));
        }
    }

} // namespace Scenes