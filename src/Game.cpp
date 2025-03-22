#include "Game.hpp"
#include <iostream>

namespace Orenji
{

    Game::Game()
        : m_deltaTime(0.0f), m_running(false)
    {
    }

    Game::~Game()
    {
    }

    Game &Game::getInstance()
    {
        static Game instance;
        return instance;
    }

    bool Game::initialize(const Settings &settings)
    {
        // Création de la fenêtre
        sf::Uint32 style = settings.fullscreen ? sf::Style::Fullscreen : sf::Style::Default;
        m_window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(settings.windowWidth, settings.windowHeight),
            settings.windowTitle,
            style);

        // Configuration de la fenêtre
        m_window->setFramerateLimit(settings.frameRateLimit);
        m_window->setVerticalSyncEnabled(settings.verticalSync);

        // Initialisation des systèmes
        m_running = true;
        m_deltaTime = 0.0f;
        m_clock.restart();

        std::cout << "Orenji Engine initialized successfully" << std::endl;
        std::cout << "Version: " << ORENJIC_VERSION << std::endl;

        return true;
    }

    void Game::run()
    {
        if (!m_window)
        {
            std::cerr << "Game::run: Window not initialized" << std::endl;
            return;
        }

        while (m_running)
        {
            processEvents();
            update();
            render();
        }
    }

    void Game::quit()
    {
        m_running = false;
    }

    sf::RenderWindow &Game::getWindow()
    {
        return *m_window;
    }

    StateManager &Game::getStateManager()
    {
        return m_stateManager;
    }

    SceneManager &Game::getSceneManager()
    {
        return m_sceneManager;
    }

    float Game::getDeltaTime() const
    {
        return m_deltaTime;
    }

    void Game::processEvents()
    {
        sf::Event event;
        while (m_window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                quit();
                return;
            }

            // Propagation de l'événement au gestionnaire d'états
            m_stateManager.handleEvent(event);
        }
    }

    void Game::update()
    {
        // Calcul du temps écoulé
        m_deltaTime = m_clock.restart().asSeconds();

        // Mise à jour des systèmes
        m_stateManager.update(m_deltaTime);
        m_sceneManager.update(m_deltaTime);

        // Si tous les états ont été retirés, on quitte le jeu
        if (m_stateManager.isEmpty())
        {
            quit();
        }
    }

    void Game::render()
    {
        m_window->clear(sf::Color(16, 16, 24));

        // Rendu des systèmes
        m_stateManager.render(*m_window);
        m_sceneManager.render(*m_window);

        m_window->display();
    }

} // namespace Orenji