#include "../../include/Scenes/MainMenuScene.hpp"
#include "../../include/Engine.hpp"
#include "../../include/Scenes/GameScene.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace Scenes
{
    MainMenuScene::MainMenuScene(Core::Engine &engine)
        : Scene("MainMenu"), m_engine(engine), m_showDemosList(false),
          m_selectedDemo(0), m_selectedItem(0), m_music(nullptr),
          m_transitionAlpha(0.0f), m_isTransitioning(false)
    {
        std::cout << "MainMenuScene created" << std::endl;
    }

    MainMenuScene::~MainMenuScene()
    {
        std::cout << "MainMenuScene destroyed" << std::endl;

        // Properly clean up music
        if (m_music != nullptr)
        {
            m_music->stop();
            delete m_music;
            m_music = nullptr;
        }
    }

    void MainMenuScene::init()
    {
        // Load font
        try
        {
            m_resourceManager.loadFont("main", "resources/fonts/VeniceClassic.ttf");
            m_resourceManager.loadFont("secondary", "resources/fonts/arial.ttf");
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to load fonts: " << e.what() << std::endl;
            return;
        }

        // Load background images
        try
        {
            m_resourceManager.loadTexture("menu_bg", "resources/textures/Titles/title-bg.png");
            m_resourceManager.loadTexture("title_overlay", "resources/textures/Titles/001-Title01.jpg");
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to load background images: " << e.what() << std::endl;
            return;
        }

        // Create title text
        try
        {
            sf::Font &font = m_resourceManager.getFont("main");
            m_titleText = std::make_unique<sf::Text>(font, "Orenji Engine", 80);
            m_titleText->setFillColor(sf::Color(255, 128, 0, 255));
            m_titleText->setOutlineColor(sf::Color(128, 64, 0, 255));
            m_titleText->setOutlineThickness(3.0f);
            m_titleText->setStyle(sf::Text::Bold);

            // SFML 3 way to center text
            sf::FloatRect bounds = m_titleText->getLocalBounds();
            m_titleText->setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));
            m_titleText->setPosition(sf::Vector2f(m_engine.getWindow().getSize().x / 2.0f, 120.0f));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to create title text: " << e.what() << std::endl;
        }

        // Create background
        try
        {
            sf::Texture &bgTexture = m_resourceManager.getTexture("menu_bg");
            m_backgroundTexture = &bgTexture;
            m_background = std::make_unique<sf::Sprite>(bgTexture);

            sf::Vector2u windowSize = m_engine.getWindow().getSize();
            sf::Vector2u textureSize = bgTexture.getSize();

            float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x);
            float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y);

            m_background->setScale(sf::Vector2f(scaleX, scaleY));

            // Create title overlay
            sf::Texture &overlayTexture = m_resourceManager.getTexture("title_overlay");
            m_backgroundSprite = std::make_unique<sf::Sprite>(overlayTexture);

            // Scale the overlay to fit the screen width but preserve aspect ratio
            sf::Vector2u overlaySize = overlayTexture.getSize();
            float overlayScale = static_cast<float>(windowSize.x) / static_cast<float>(overlaySize.x);
            m_backgroundSprite->setScale(sf::Vector2f(overlayScale, overlayScale));

            // Position at bottom of screen
            float overlayHeight = overlaySize.y * overlayScale;
            m_backgroundSprite->setPosition(sf::Vector2f(0, windowSize.y - overlayHeight));

            // Set initial alpha (transparent)
            m_backgroundSprite->setColor(sf::Color(255, 255, 255, 0));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to create background: " << e.what() << std::endl;
        }

        // Create menu items with animation delay
        createMenuItem("New Game", 280.0f);
        createMenuItem("Examples", 360.0f);
        createMenuItem("Options", 440.0f);
        createMenuItem("Exit", 520.0f);

        // Load available demos
        loadDemosList();

        // Update menu selection to highlight the first item
        updateMenuSelection();

        // Load and play background music
        try
        {
            m_music = new sf::Music();
            if (m_music->openFromFile("resources/sounds/BGM/012-Theme01.mp3"))
            {
                m_music->setLooping(true);
                m_music->setVolume(70.0f);
                m_music->play();
            }
            else
            {
                std::cerr << "Failed to open music file" << std::endl;
                delete m_music;
                m_music = nullptr;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to create music: " << e.what() << std::endl;
            if (m_music != nullptr)
            {
                delete m_music;
                m_music = nullptr;
            }
        }

        // Start fade-in transition
        m_isTransitioning = true;
        m_transitionAlpha = 0.0f;
    }

    void MainMenuScene::update(float deltaTime)
    {
        // Update animations
        static float elapsedTime = 0.0f;
        elapsedTime += deltaTime;

        // Subtle floating animation for title
        if (m_titleText)
        {
            float offsetY = std::sin(elapsedTime * 1.5f) * 5.0f;
            m_titleText->setPosition(
                sf::Vector2f(m_engine.getWindow().getSize().x / 2.0f,
                             120.0f + offsetY));
        }

        // Handle fade-in transition
        if (m_isTransitioning)
        {
            m_transitionAlpha += deltaTime * 255.0f * 0.7f; // 0.7 seconds for full fade

            if (m_transitionAlpha >= 255.0f)
            {
                m_transitionAlpha = 255.0f;
                m_isTransitioning = false;
            }

            // Apply alpha to overlay and menu items
            if (m_backgroundSprite)
            {
                m_backgroundSprite->setColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(m_transitionAlpha)));
            }

            float titleAlpha = std::min(255.0f, m_transitionAlpha * 1.5f);
            if (m_titleText)
            {
                sf::Color titleColor = m_titleText->getFillColor();
                m_titleText->setFillColor(sf::Color(titleColor.r, titleColor.g, titleColor.b,
                                                    static_cast<std::uint8_t>(titleAlpha)));

                sf::Color outlineColor = m_titleText->getOutlineColor();
                m_titleText->setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b,
                                                       static_cast<std::uint8_t>(titleAlpha)));
            }

            // Fade in menu items with cascading effect
            for (size_t i = 0; i < m_menuItems.size(); ++i)
            {
                float delay = 0.1f * static_cast<float>(i); // 0.1 seconds delay between items
                float itemAlpha = std::max(0.0f, std::min(255.0f, (m_transitionAlpha - (delay * 255.0f)) * 1.2f));

                sf::Color itemColor = m_menuItems[i].getFillColor();
                m_menuItems[i].setFillColor(sf::Color(itemColor.r, itemColor.g, itemColor.b,
                                                      static_cast<std::uint8_t>(itemAlpha)));

                if (i == m_selectedItem)
                {
                    sf::Color outlineColor = m_menuItems[i].getOutlineColor();
                    m_menuItems[i].setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b,
                                                             static_cast<std::uint8_t>(itemAlpha)));
                }
            }
        }

        // Breathing effect for selected menu item
        if (!m_isTransitioning && !m_showDemosList)
        {
            float breathingFactor = (std::sin(elapsedTime * 3.0f) + 1.0f) / 2.0f; // 0 to 1
            float scaleFactor = 1.0f + breathingFactor * 0.05f;                   // Scale between 1.0 and 1.05

            for (size_t i = 0; i < m_menuItems.size(); ++i)
            {
                if (i == m_selectedItem)
                {
                    m_menuItems[i].setScale(sf::Vector2f(scaleFactor, scaleFactor));

                    // Pulse color effect
                    sf::Color pulseColor = sf::Color(
                        255,
                        200 + static_cast<std::uint8_t>(breathingFactor * 55.0f), // 200-255
                        0 + static_cast<std::uint8_t>(breathingFactor * 100.0f)   // 0-100
                    );
                    m_menuItems[i].setFillColor(pulseColor);
                }
                else
                {
                    m_menuItems[i].setScale(sf::Vector2f(1.0f, 1.0f));
                }
            }
        }
    }

    void MainMenuScene::render(sf::RenderWindow &window)
    {
        // Draw background
        if (m_background)
        {
            window.draw(*m_background);
        }

        // Draw title overlay background
        if (m_backgroundSprite)
        {
            window.draw(*m_backgroundSprite);
        }

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
        // Skip input during initial transition
        if (m_isTransitioning)
        {
            return;
        }

        if (const auto *keyEvent = event.getIf<sf::Event::KeyPressed>())
        {
            if (m_showDemosList)
            {
                // Handle demo list navigation
                if (keyEvent->code == sf::Keyboard::Key::Up)
                {
                    if (m_selectedDemo > 0)
                    {
                        m_selectedDemo--;
                        updateDemoSelection();
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::Down)
                {
                    if (m_selectedDemo < m_demoItems.size() - 1)
                    {
                        m_selectedDemo++;
                        updateDemoSelection();
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::Enter)
                {
                    // Demos are not launchable yet
                    if (m_comingSoonText)
                    {
                        m_comingSoonText->setFillColor(sf::Color(255, 0, 0));
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    // Return to main menu
                    m_showDemosList = false;
                }
            }
            else
            {
                // Handle main menu navigation
                if (keyEvent->code == sf::Keyboard::Key::Up)
                {
                    if (m_selectedItem > 0)
                    {
                        m_selectedItem--;
                        updateMenuSelection();
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::Down)
                {
                    if (m_selectedItem < m_menuItems.size() - 1)
                    {
                        m_selectedItem++;
                        updateMenuSelection();
                    }
                }
                else if (keyEvent->code == sf::Keyboard::Key::Enter)
                {
                    // Execute the selected menu item
                    switch (m_selectedItem)
                    {
                    case 0: // New Game
                        // TODO: Create and set GameScene when implemented
                        // For now, let's just show a placeholder effect
                        if (m_titleText)
                        {
                            m_titleText->setFillColor(sf::Color(0, 255, 128));
                            m_titleText->setOutlineColor(sf::Color(0, 128, 64));
                        }
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
            sf::Text menuItem(font, text, 48);
            menuItem.setFillColor(sf::Color(255, 255, 255, 0)); // Start invisible
            menuItem.setStyle(sf::Text::Regular);

            // SFML 3 way to center text
            sf::FloatRect bounds = menuItem.getLocalBounds();
            menuItem.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));
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
            // Get current alpha for transition
            std::uint8_t currentAlpha = m_menuItems[i].getFillColor().a;

            if (i == m_selectedItem)
            {
                m_menuItems[i].setFillColor(sf::Color(255, 200, 0, currentAlpha));
                m_menuItems[i].setOutlineColor(sf::Color(255, 100, 0, currentAlpha));
                m_menuItems[i].setOutlineThickness(2.0f);
                m_menuItems[i].setStyle(sf::Text::Bold);
            }
            else
            {
                m_menuItems[i].setFillColor(sf::Color(255, 255, 255, currentAlpha));
                m_menuItems[i].setOutlineThickness(0.0f);
                m_menuItems[i].setStyle(sf::Text::Regular);
                m_menuItems[i].setScale(sf::Vector2f(1.0f, 1.0f)); // Reset scale
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

                // Add a subtle scale effect
                m_demoItems[i].setScale(sf::Vector2f(1.05f, 1.05f));
            }
            else
            {
                m_demoItems[i].setFillColor(sf::Color::White);
                m_demoItems[i].setOutlineThickness(0.0f);
                m_demoItems[i].setStyle(sf::Text::Regular);
                m_demoItems[i].setScale(sf::Vector2f(1.0f, 1.0f));
            }
        }
    }

    void MainMenuScene::centerText(sf::Text &text, const sf::Vector2f &position)
    {
        // SFML 3 way to center text
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));
        text.setPosition(position);
    }

    void MainMenuScene::loadDemosList()
    {
        m_demoCommands.clear();
        m_demoDescriptions.clear();

        // Fallback demo descriptions if file not found
        m_demoDescriptions = {
            "Particle Effects Demo",
            "Camera Controls Demo",
            "Physics Simulation Demo",
            "Map Editor Demo",
            "Animation System Demo"};

        // Try to load the demos list from file
        std::ifstream file("examples/demos.txt");
        if (file.is_open())
        {
            m_demoDescriptions.clear();
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
            m_demosTitle = std::make_unique<sf::Text>(font, "Available Examples", 60);
            m_demosTitle->setFillColor(sf::Color(255, 128, 0));
            m_demosTitle->setOutlineColor(sf::Color(128, 64, 0));
            m_demosTitle->setOutlineThickness(2.0f);
            centerText(*m_demosTitle, sf::Vector2f(windowSize.x / 2.0f, 100.0f));

            // Create back instruction text
            m_backText = std::make_unique<sf::Text>(font, "Press ESC to return to main menu", 24);
            m_backText->setFillColor(sf::Color(200, 200, 200));
            centerText(*m_backText, sf::Vector2f(windowSize.x / 2.0f, windowSize.y - 50.0f));

            // Create coming soon message
            m_comingSoonText = std::make_unique<sf::Text>(font, "These demos are coming soon!", 36);
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
                sf::Text demoItem(font, m_demoDescriptions[i], 36);
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