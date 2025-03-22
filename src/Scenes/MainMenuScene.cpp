#include "../../include/Scenes/MainMenuScene.hpp"
#include "../../include/Engine.hpp"
#include "../../include/Scenes/GameScene.hpp"
#include <iostream>
#include <filesystem>

namespace Scenes
{

    MainMenuScene::MainMenuScene(Engine &engine)
        : Core::Scene("MainMenu"), m_engine(engine), m_selectedOption(0), m_transitionAlpha(0.0f), m_isTransitioning(false)
    {
        std::cout << "MainMenuScene created" << std::endl;
    }

    MainMenuScene::~MainMenuScene()
    {
        std::cout << "MainMenuScene destroyed" << std::endl;
    }

    void MainMenuScene::init()
    {
        bool resourcesLoaded = false;

        // Load resources if available
        try
        {
            auto &resourceManager = m_engine.getResourceManager();

            // Load font
            if (!resourceManager.hasFont("main"))
            {
                resourceManager.loadFont("main", "resources/fonts/VeniceClassic.ttf");
            }

            auto &font = resourceManager.getFont("main");
            // Create the title text with the loaded font
            m_titleText = std::make_unique<sf::Text>(font, "ORENJI", 60);

            // Center title text
            sf::FloatRect textRect = m_titleText->getLocalBounds();
            m_titleText->setOrigin(sf::Vector2f(textRect.size.x / 2.0f, textRect.size.y / 2.0f));
            m_titleText->setPosition(sf::Vector2f(
                m_engine.getWindow().getSize().x / 2.0f,
                150.0f));
            m_titleText->setFillColor(sf::Color(255, 165, 0)); // Orange color

            // Load background texture
            if (!resourceManager.hasTexture("menu_bg"))
            {
                resourceManager.loadTexture("menu_bg", "resources/textures/Titles/title-bg.png");
            }

            auto &bgTexture = resourceManager.getTexture("menu_bg");

            // Create sprite with the loaded texture
            m_backgroundSprite.emplace(bgTexture);

            // Scale to fit window
            sf::Vector2u windowSize = m_engine.getWindow().getSize();
            sf::Vector2u textureSize = bgTexture.getSize();
            m_backgroundSprite->setScale(sf::Vector2f(
                static_cast<float>(windowSize.x) / textureSize.x,
                static_cast<float>(windowSize.y) / textureSize.y));

            // Load and play background music
            if (!resourceManager.hasSoundBuffer("menu_music"))
            {
                resourceManager.loadSoundBuffer("menu_music", "resources/sounds/BGM/012-Theme01.mid");
            }

            resourcesLoaded = true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error loading resources: " << e.what() << std::endl;
        }

        // Create menu options
        createMenuOptions();

        std::cout << "MainMenuScene initialized" << (resourcesLoaded ? " with" : " without") << " resources" << std::endl;
    }

    void MainMenuScene::update(float deltaTime)
    {
        // Update transition effect
        if (m_isTransitioning)
        {
            m_transitionAlpha += deltaTime * 255.0f;
            if (m_transitionAlpha >= 255.0f)
            {
                m_transitionAlpha = 255.0f;
                m_isTransitioning = false;

                // Transition to the selected scene
                handleMenuSelection();
            }
        }
    }

    void MainMenuScene::render(sf::RenderWindow &window)
    {
        // Draw background if available
        if (m_backgroundSprite.has_value())
        {
            window.draw(*m_backgroundSprite);
        }
        else
        {
            // Fill with a default background color
            window.clear(sf::Color(30, 30, 30));
        }

        // Draw title if initialized
        if (m_titleText)
        {
            window.draw(*m_titleText);
        }

        // Draw menu options
        for (const auto &option : m_menuOptions)
        {
            window.draw(option);
        }

        // Draw transition fade overlay if transitioning
        if (m_isTransitioning && m_transitionAlpha > 0.0f)
        {
            sf::RectangleShape fadeRect;
            fadeRect.setSize(sf::Vector2f(
                static_cast<float>(window.getSize().x),
                static_cast<float>(window.getSize().y)));
            fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<unsigned char>(m_transitionAlpha * 255)));
            window.draw(fadeRect);
        }
    }

    void MainMenuScene::handleEvent(const sf::Event &event)
    {
        if (m_isTransitioning)
            return;

        if (event.is<sf::Event::KeyPressed>())
        {
            const auto *keyPressed = event.getIf<sf::Event::KeyPressed>();

            if (keyPressed)
            {
                if (keyPressed->code == sf::Keyboard::Key::Up)
                {
                    m_selectedOption = (m_selectedOption - 1 + m_menuOptions.size()) % m_menuOptions.size();
                    // Update option colors
                    for (size_t i = 0; i < m_menuOptions.size(); ++i)
                    {
                        m_menuOptions[i].setFillColor(i == m_selectedOption ? sf::Color::Yellow : sf::Color::White);
                    }
                }
                else if (keyPressed->code == sf::Keyboard::Key::Down)
                {
                    m_selectedOption = (m_selectedOption + 1) % m_menuOptions.size();
                    // Update option colors
                    for (size_t i = 0; i < m_menuOptions.size(); ++i)
                    {
                        m_menuOptions[i].setFillColor(i == m_selectedOption ? sf::Color::Yellow : sf::Color::White);
                    }
                }
                else if (keyPressed->code == sf::Keyboard::Key::Enter)
                {
                    m_isTransitioning = true;
                    m_transitionAlpha = 0.0f;
                }
            }
        }
    }

    void MainMenuScene::createMenuOptions()
    {
        // Clear existing options
        m_menuOptions.clear();

        // Get the font
        sf::Font *font = nullptr;
        try
        {
            auto &resourceManager = m_engine.getResourceManager();
            if (resourceManager.hasFont("main"))
            {
                font = &resourceManager.getFont("main");
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to get font: " << e.what() << std::endl;
        }

        // Define menu options
        std::vector<std::string> options = {
            "Start Game",
            "Options",
            "Credits",
            "Exit"};

        // Create menu option texts
        for (size_t i = 0; i < options.size(); ++i)
        {
            if (!font)
            {
                continue; // Skip if no font available
            }

            sf::Text text(*font, options[i], 40);
            text.setFillColor(i == m_selectedOption ? sf::Color::Yellow : sf::Color::White);

            // Center the text
            sf::FloatRect textBounds = text.getLocalBounds();
            text.setOrigin(sf::Vector2f(textBounds.size.x / 2.0f, textBounds.size.y / 2.0f));
            text.setPosition(sf::Vector2f(
                m_engine.getWindow().getSize().x / 2.0f,
                300.0f + i * 60.0f));

            m_menuOptions.push_back(text);
        }
    }

    void MainMenuScene::handleMenuSelection()
    {
        switch (m_selectedOption)
        {
        case 0: // Start Game
            std::cout << "Starting game..." << std::endl;
            // TODO: Create and switch to game scene
            // auto gameScene = std::make_shared<Scenes::GameScene>(m_engine);
            // m_engine.setScene(gameScene);
            break;

        case 1: // Options
            std::cout << "Options selected" << std::endl;
            // TODO: Create and switch to options scene
            break;

        case 2: // Credits
            std::cout << "Credits selected" << std::endl;
            // TODO: Create and switch to credits scene
            break;

        case 3: // Exit
            std::cout << "Exiting..." << std::endl;
            m_engine.getWindow().close();
            break;

        default:
            break;
        }
    }

} // namespace Scenes