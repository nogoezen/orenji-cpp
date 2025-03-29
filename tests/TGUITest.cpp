#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "UI/UIManager.hpp"
#include "UI/PauseMenu.hpp"
#include "UI/EditorForm.hpp"
#include <iostream>
#include <filesystem>
#include <memory>

class SimpleGame
{
public:
    SimpleGame() : m_window(sf::VideoMode(sf::Vector2u(800, 600)), "TGUI Integration Test"),
                   m_uiManager(m_window),
                   m_pauseMenu(&m_uiManager),
                   m_editorForm(&m_uiManager)
    {
        m_window.setFramerateLimit(60);

        // Initialize UI system
        m_uiManager.init();

        setupMainMenu();
        setupPauseMenu();
        setupEditorForm();

        // Set the initial state to main menu
        m_uiManager.showForm(UI::FormType::MAIN_MENU);
    }

    void run()
    {
        sf::Clock clock;
        while (m_window.isOpen())
        {
            sf::Event event;
            while (auto event = m_window.pollEvent())
            {
                // Check for ESC key to toggle pause menu during gameplay
                if (event->type == sf::Event::Closed)
                {
                    m_window.close();
                }
                else if (event->type == sf::Event::KeyPressed &&
                         event->key.code == sf::Keyboard::Key::Escape &&
                         m_gameIsActive)
                {
                    m_pauseMenu.toggle();
                }

                // Let the UI handle the event first
                m_uiManager.handleEvent(*event);
            }

            // Calculate delta time
            float deltaTime = clock.restart().asSeconds();

            // Update the UI
            m_uiManager.update(deltaTime);

            // Render
            m_window.clear(sf::Color(40, 40, 40));

            // Draw game elements if in gameplay state
            if (m_gameIsActive && !m_pauseMenu.isVisible())
            {
                drawGame();
            }

            // Draw UI
            m_uiManager.render();

            m_window.display();
        }
    }

private:
    sf::RenderWindow m_window;
    UI::UIManager m_uiManager;
    UI::PauseMenu m_pauseMenu;
    UI::EditorForm m_editorForm;

    bool m_gameIsActive = false;

    // Game objects for demonstration
    sf::RectangleShape m_player;
    sf::Color m_playerColor;
    sf::Vector2f m_playerSize;
    sf::Vector2f m_playerPosition;

    void setupMainMenu()
    {
        // Create main menu form
        auto mainMenu = m_uiManager.createForm(UI::FormType::MAIN_MENU, "MainMenu");

        // Create UI elements
        auto title = tgui::Label::create("Orenji Engine");
        title->setPosition("50%", "10%");
        title->setOrigin(0.5f, 0.5f);
        title->setTextSize(36);
        title->getRenderer()->setTextColor(tgui::Color(255, 165, 0));
        title->getRenderer()->setTextOutlineColor(tgui::Color::Black);
        title->getRenderer()->setTextOutlineThickness(2);
        mainMenu->add(title);

        auto startButton = tgui::Button::create("Start Game");
        startButton->setSize(200, 50);
        startButton->setPosition("50%", "40%");
        startButton->setOrigin(0.5f, 0.5f);
        mainMenu->add(startButton);

        auto editorButton = tgui::Button::create("Open Editor");
        editorButton->setSize(200, 50);
        editorButton->setPosition("50%", "55%");
        editorButton->setOrigin(0.5f, 0.5f);
        mainMenu->add(editorButton);

        auto exitButton = tgui::Button::create("Exit");
        exitButton->setSize(200, 50);
        exitButton->setPosition("50%", "70%");
        exitButton->setOrigin(0.5f, 0.5f);
        mainMenu->add(exitButton);

        // Set up callbacks
        startButton->onPress([this]()
                             {
            std::cout << "Starting game" << std::endl;
            m_uiManager.hideAllForms();
            m_gameIsActive = true;
            initGameObjects(); });

        editorButton->onPress([this]()
                              { m_editorForm.show(); });

        exitButton->onPress([this]()
                            { m_window.close(); });
    }

    void setupPauseMenu()
    {
        // Initialize pause menu
        m_pauseMenu.init();

        // Set custom callbacks
        m_pauseMenu.setResumeCallback([this]()
                                      { m_pauseMenu.hide(); });

        m_pauseMenu.setSettingsCallback([this]()
                                        {
            m_pauseMenu.hide();
            m_uiManager.showForm(UI::FormType::OPTIONS_MENU); });

        m_pauseMenu.setMainMenuCallback([this]()
                                        {
            m_pauseMenu.hide();
            m_gameIsActive = false;
            m_uiManager.showForm(UI::FormType::MAIN_MENU); });

        m_pauseMenu.setExitCallback([this]()
                                    { m_window.close(); });
    }

    void setupEditorForm()
    {
        // Initialize editor form
        m_editorForm.init();

        // Set custom callbacks
        m_editorForm.setSaveCallback([this]()
                                     {
            // Apply edited properties to the player object
            m_editorForm.hide();
            m_uiManager.showForm(UI::FormType::MAIN_MENU); });

        m_editorForm.setCloseCallback([this]()
                                      {
            m_editorForm.hide();
            m_uiManager.showForm(UI::FormType::MAIN_MENU); });

        // Add properties for the player object
        m_editorForm.addProperty({"Name",
                                  UI::PropertyType::STRING,
                                  std::string("Player"),
                                  [this](const UI::Property &prop)
                                  {
                                      std::cout << "Name changed to: " << std::get<std::string>(prop.value) << std::endl;
                                  }});

        m_editorForm.addProperty({"Position",
                                  UI::PropertyType::POSITION,
                                  sf::Vector2f(400.f, 300.f),
                                  [this](const UI::Property &prop)
                                  {
                                      m_playerPosition = std::get<sf::Vector2f>(prop.value);
                                      if (m_gameIsActive)
                                      {
                                          m_player.setPosition(m_playerPosition);
                                      }
                                  }});

        m_editorForm.addProperty({"Size",
                                  UI::PropertyType::SIZE,
                                  sf::Vector2f(50.f, 50.f),
                                  [this](const UI::Property &prop)
                                  {
                                      m_playerSize = std::get<sf::Vector2f>(prop.value);
                                      if (m_gameIsActive)
                                      {
                                          m_player.setSize(m_playerSize);
                                      }
                                  }});

        m_editorForm.addProperty({"Color",
                                  UI::PropertyType::COLOR,
                                  sf::Color(255, 165, 0),
                                  [this](const UI::Property &prop)
                                  {
                                      m_playerColor = std::get<sf::Color>(prop.value);
                                      if (m_gameIsActive)
                                      {
                                          m_player.setFillColor(m_playerColor);
                                      }
                                  }});

        m_editorForm.addProperty({"Speed",
                                  UI::PropertyType::FLOAT,
                                  200.0f,
                                  [this](const UI::Property &prop)
                                  {
                                      std::cout << "Speed changed to: " << std::get<float>(prop.value) << std::endl;
                                  }});

        m_editorForm.addProperty({"Visible",
                                  UI::PropertyType::BOOL,
                                  true,
                                  [this](const UI::Property &prop)
                                  {
                                      std::cout << "Visible changed to: " << std::get<bool>(prop.value) << std::endl;
                                  }});

        std::vector<std::string> types = {"Player", "Enemy", "NPC", "Item"};
        m_editorForm.addProperty({"Type",
                                  UI::PropertyType::SELECTION,
                                  types,
                                  [this](const UI::Property &prop)
                                  {
                                      auto options = std::get<std::vector<std::string>>(prop.value);
                                      if (!options.empty())
                                      {
                                          std::cout << "Type changed to: " << options[0] << std::endl;
                                      }
                                  }});
    }

    void initGameObjects()
    {
        // Initialize player object
        m_playerPosition = sf::Vector2f(400.f, 300.f);
        m_playerSize = sf::Vector2f(50.f, 50.f);
        m_playerColor = sf::Color(255, 165, 0);

        m_player.setSize(m_playerSize);
        m_player.setFillColor(m_playerColor);
        m_player.setPosition(m_playerPosition);
        m_player.setOrigin(m_playerSize.x / 2, m_playerSize.y / 2);
    }

    void drawGame()
    {
        // Draw game elements
        m_window.draw(m_player);

        // Draw instructions
        sf::Font font;
        if (font.loadFromFile("resources/fonts/VeniceClassic.ttf"))
        {
            sf::Text text("Press ESC for menu", font);
            text.setCharacterSize(16);
            text.setFillColor(sf::Color::White);
            text.setPosition(10, 10);
            m_window.draw(text);
        }
    }
};

// Simple test demonstrating TGUI integration
int main()
{
    SimpleGame game;
    game.run();

    return 0;
}