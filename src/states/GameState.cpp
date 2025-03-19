#include "GameState.h"
#include "../components/PlayerComponent.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

// Variable globale pour la fenêtre
extern sf::RenderWindow *g_window;

GameState::GameState()
    : State("Game")
{
    m_font = std::make_shared<sf::Font>();
    if (!m_font->loadFromFile("assets/fonts/VeniceClassic.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }

    m_statusText = std::make_shared<sf::Text>();
    m_statusText->setFont(*m_font);
    m_statusText->setCharacterSize(20);
    m_statusText->setFillColor(sf::Color::White);
    m_statusText->setPosition(10.0f, 10.0f);
}

void GameState::enter()
{
    setActive(true);
    initializeSystems();
    std::cout << "Game State Entered" << std::endl;
}

void GameState::exit()
{
    setActive(false);
    getEntityManager().destroyAllEntities();
    std::cout << "Game State Exited" << std::endl;
}

void GameState::pause()
{
    State::pause();
    std::cout << "Game State Paused" << std::endl;
}

void GameState::resume()
{
    State::resume();
    std::cout << "Game State Resumed" << std::endl;
}

void GameState::handleInput()
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
                // Retourner au menu principal
                if (m_manager)
                {
                    m_manager->pushState("MainMenu");
                }
            }
        }
    }

    handleGameInput();
}

void GameState::update(float deltaTime)
{
    if (!isActive())
        return;

    // Mise à jour des entités
    getEntityManager().update(deltaTime);

    // Mise à jour des systèmes
    if (m_world)
        m_world->update(deltaTime);
    if (m_tradingSystem)
        m_tradingSystem->update(deltaTime);

    // Mise à jour de l'interface
    updateStatusText();
}

void GameState::render()
{
    if (!isActive())
        return;

    // Rendu des entités
    getEntityManager().render();

    // Rendu de l'interface
    if (m_statusText)
    {
        g_window->draw(*m_statusText);
    }
}

void GameState::initializeSystems()
{
    // Initialisation du joueur
    m_player = std::make_shared<Player>();

    // Initialisation du monde
    m_world = std::make_shared<World>();
    if (!m_world->initialize())
    {
        std::cerr << "Failed to initialize world" << std::endl;
        return;
    }

    // Initialisation du système de commerce
    m_tradingSystem = std::make_shared<TradingSystem>();
    m_tradingSystem->initialize();

    // Création de l'entité joueur avec ses composants
    auto playerEntity = getEntityManager().createEntity("Player");
    auto playerComponent = playerEntity->addComponent<PlayerComponent>(m_player);
}

void GameState::updateStatusText()
{
    if (!m_player)
        return;

    std::string status = "Position: " + m_player->getCurrentPosition().toString() + "\n";
    status += "Or: " + std::to_string(m_player->getGold()) + "\n";
    status += "Dette: " + std::to_string(m_player->getDebt());

    m_statusText->setString(status);
}

void GameState::handleGameInput()
{
    // La gestion des entrées de jeu est maintenant gérée par le PlayerComponent
    // Cette méthode peut être utilisée pour d'autres entrées globales si nécessaire
}