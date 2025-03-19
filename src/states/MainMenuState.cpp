#include "MainMenuState.h"
#include "../components/TransformComponent.h"
#include "../components/SpriteComponent.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

// Variable globale pour la fenêtre
extern sf::RenderWindow *g_window;

MainMenuState::MainMenuState()
    : State("MainMenu"), m_selectedItem(0), m_hasSaveGame(false)
{
    initializeGraphics();
    loadSaveGameStatus();
}

void MainMenuState::enter()
{
    setActive(true);
    std::cout << "Main Menu State Entered" << std::endl;
}

void MainMenuState::exit()
{
    setActive(false);
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
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                if (m_selectedItem > 0)
                {
                    m_selectedItem--;
                    updateMenuSelection();
                }
                break;
            case sf::Keyboard::Down:
                if (m_selectedItem < m_menuItems.size() - 1)
                {
                    m_selectedItem++;
                    updateMenuSelection();
                }
                break;
            case sf::Keyboard::Return:
                handleMenuSelection();
                break;
            case sf::Keyboard::Escape:
                if (getManager())
                {
                    getManager()->pushState("TitleScreen");
                }
                break;
            }
        }
    }
}

void MainMenuState::update(float deltaTime)
{
    if (!isActive())
        return;
}

void MainMenuState::render()
{
    if (!isActive())
        return;

    // Rendu du fond
    if (m_background)
    {
        g_window->draw(*m_background);
    }

    // Rendu des éléments du menu
    for (const auto &item : m_menuItems)
    {
        if (item)
        {
            g_window->draw(*item);
        }
    }
}

void MainMenuState::initializeGraphics()
{
    // Chargement de la police
    m_font = std::make_shared<sf::Font>();
    if (!m_font->loadFromFile("assets/fonts/VeniceClassic.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }

    // Création des éléments du menu
    std::vector<std::string> menuTexts = {
        "Nouvelle Partie",
        "Charger Partie",
        "Options",
        "Quitter"};

    float startY = g_window->getSize().y / 2;
    float spacing = 50.0f;

    for (size_t i = 0; i < menuTexts.size(); ++i)
    {
        auto text = std::make_shared<sf::Text>();
        text->setFont(*m_font);
        text->setString(menuTexts[i]);
        text->setCharacterSize(32);
        text->setFillColor(sf::Color::White);

        // Centrer le texte
        sf::FloatRect textRect = text->getLocalBounds();
        text->setOrigin(textRect.width / 2, textRect.height / 2);
        text->setPosition(g_window->getSize().x / 2, startY + (i * spacing));

        m_menuItems.push_back(text);
    }

    // Mise à jour de la sélection initiale
    updateMenuSelection();
}

void MainMenuState::updateMenuSelection()
{
    for (size_t i = 0; i < m_menuItems.size(); ++i)
    {
        if (m_menuItems[i])
        {
            if (i == m_selectedItem)
            {
                m_menuItems[i]->setFillColor(sf::Color::Yellow);
                m_menuItems[i]->setStyle(sf::Text::Bold);
            }
            else
            {
                m_menuItems[i]->setFillColor(sf::Color::White);
                m_menuItems[i]->setStyle(sf::Text::Regular);
            }
        }
    }
}

void MainMenuState::handleMenuSelection()
{
    switch (m_selectedItem)
    {
    case 0: // Nouvelle Partie
        if (getManager())
        {
            getManager()->pushState("CharacterCreation");
        }
        break;
    case 1: // Charger Partie
        if (m_hasSaveGame && getManager())
        {
            // TODO: Implémenter le chargement de partie
            getManager()->pushState("Game");
        }
        break;
    case 2: // Options
        if (getManager())
        {
            // TODO: Implémenter l'état des options
            // getManager()->pushState("Options");
        }
        break;
    case 3: // Quitter
        g_window->close();
        break;
    }
}

void MainMenuState::loadSaveGameStatus()
{
    // TODO: Vérifier si une partie sauvegardée existe
    m_hasSaveGame = false;
}