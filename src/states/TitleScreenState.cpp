#include "TitleScreenState.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

// Variable globale pour la fenêtre
extern sf::RenderWindow *g_window;

TitleScreenState::TitleScreenState()
    : State("TitleScreen"), m_blinkTimer(0.0f), m_showPressStart(true)
{
    initializeGraphics();
}

void TitleScreenState::enter()
{
    setActive(true);
    std::cout << "Title Screen State Entered" << std::endl;
}

void TitleScreenState::exit()
{
    setActive(false);
    std::cout << "Title Screen State Exited" << std::endl;
}

void TitleScreenState::pause()
{
    State::pause();
    std::cout << "Title Screen State Paused" << std::endl;
}

void TitleScreenState::resume()
{
    State::resume();
    std::cout << "Title Screen State Resumed" << std::endl;
}

void TitleScreenState::handleInput()
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
            if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
            {
                // Passer au menu principal
                if (m_manager)
                {
                    m_manager->pushState("MainMenu");
                }
            }
        }
    }
}

void TitleScreenState::update(float deltaTime)
{
    if (!isActive())
        return;
    updateAnimation(deltaTime);
}

void TitleScreenState::render()
{
    if (!isActive())
        return;

    // Rendu du fond
    if (m_background)
    {
        g_window->draw(*m_background);
    }

    // Rendu du titre
    if (m_titleText)
    {
        g_window->draw(*m_titleText);
    }

    // Rendu du texte "Press Start" si visible
    if (m_showPressStart && m_pressStartText)
    {
        g_window->draw(*m_pressStartText);
    }
}

void TitleScreenState::initializeGraphics()
{
    // Chargement de la police
    m_font = std::make_shared<sf::Font>();
    if (!m_font->loadFromFile("assets/fonts/VeniceClassic.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }

    // Création du titre
    m_titleText = std::make_shared<sf::Text>();
    m_titleText->setFont(*m_font);
    m_titleText->setString("Uncharted Waters");
    m_titleText->setCharacterSize(64);
    m_titleText->setFillColor(sf::Color::White);

    // Centrer le titre
    sf::FloatRect textRect = m_titleText->getLocalBounds();
    m_titleText->setOrigin(textRect.width / 2, textRect.height / 2);
    m_titleText->setPosition(g_window->getSize().x / 2, g_window->getSize().y / 3);

    // Création du texte "Press Start"
    m_pressStartText = std::make_shared<sf::Text>();
    m_pressStartText->setFont(*m_font);
    m_pressStartText->setString("Press Enter to Start");
    m_pressStartText->setCharacterSize(32);
    m_pressStartText->setFillColor(sf::Color::White);

    // Centrer le texte "Press Start"
    textRect = m_pressStartText->getLocalBounds();
    m_pressStartText->setOrigin(textRect.width / 2, textRect.height / 2);
    m_pressStartText->setPosition(g_window->getSize().x / 2, g_window->getSize().y * 2 / 3);

    // Chargement du fond
    // TODO: Ajouter le chargement du fond une fois que l'image sera disponible
}

void TitleScreenState::updateAnimation(float deltaTime)
{
    m_blinkTimer += deltaTime;
    if (m_blinkTimer >= 1.0f)
    {
        m_blinkTimer = 0.0f;
        m_showPressStart = !m_showPressStart;
    }
}