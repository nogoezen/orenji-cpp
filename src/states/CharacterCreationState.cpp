#include "CharacterCreationState.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

// Variable globale pour la fenêtre
extern sf::RenderWindow *g_window;

CharacterCreationState::CharacterCreationState()
    : State("CharacterCreation"), m_selectedItem(0), m_isEnteringName(false)
{
    initializeGraphics();
}

void CharacterCreationState::enter()
{
    setActive(true);
    std::cout << "Character Creation State Entered" << std::endl;
}

void CharacterCreationState::exit()
{
    setActive(false);
    std::cout << "Character Creation State Exited" << std::endl;
}

void CharacterCreationState::pause()
{
    State::pause();
    std::cout << "Character Creation State Paused" << std::endl;
}

void CharacterCreationState::resume()
{
    State::resume();
    std::cout << "Character Creation State Resumed" << std::endl;
}

void CharacterCreationState::handleInput()
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
            if (m_isEnteringName)
            {
                handleNameInput();
            }
            else
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
                    if (m_manager)
                    {
                        m_manager->popState();
                    }
                    break;
                }
            }
        }
        else if (event.type == sf::Event::TextEntered && m_isEnteringName)
        {
            if (event.text.unicode < 128) // ASCII characters only
            {
                if (event.text.unicode == '\b' && !m_currentInput.empty())
                {
                    m_currentInput.pop_back();
                }
                else if (event.text.unicode != '\b' && event.text.unicode != '\r' && event.text.unicode != '\n')
                {
                    m_currentInput += static_cast<char>(event.text.unicode);
                }
            }
        }
    }
}

void CharacterCreationState::update(float deltaTime)
{
    if (!isActive())
        return;
}

void CharacterCreationState::render()
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

    // Rendu du nom en cours de saisie
    if (m_isEnteringName)
    {
        auto nameText = std::make_shared<sf::Text>();
        nameText->setFont(*m_font);
        nameText->setString("Nom du personnage: " + m_currentInput + "|");
        nameText->setCharacterSize(32);
        nameText->setFillColor(sf::Color::White);

        // Centrer le texte
        sf::FloatRect textRect = nameText->getLocalBounds();
        nameText->setOrigin(textRect.width / 2, textRect.height / 2);
        nameText->setPosition(g_window->getSize().x / 2, g_window->getSize().y / 2);

        g_window->draw(*nameText);
    }
}

void CharacterCreationState::initializeGraphics()
{
    // Chargement de la police
    m_font = std::make_shared<sf::Font>();
    if (!m_font->loadFromFile("assets/fonts/VeniceClassic.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }

    // Création des éléments du menu
    std::vector<std::string> menuTexts = {
        "Nom du personnage",
        "Commencer l'aventure",
        "Retour"};

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

void CharacterCreationState::updateMenuSelection()
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

void CharacterCreationState::handleMenuSelection()
{
    switch (m_selectedItem)
    {
    case 0: // Nom du personnage
        m_isEnteringName = true;
        m_currentInput = m_characterName;
        break;
    case 1: // Commencer l'aventure
        if (!m_characterName.empty())
        {
            createCharacter();
        }
        break;
    case 2: // Retour
        if (m_manager)
        {
            m_manager->popState();
        }
        break;
    }
}

void CharacterCreationState::handleNameInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        m_isEnteringName = false;
        m_characterName = m_currentInput;
        m_currentInput.clear();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        m_isEnteringName = false;
        m_currentInput.clear();
    }
}

void CharacterCreationState::createCharacter()
{
    // Création du joueur
    auto player = std::make_shared<Player>();
    player->setName(m_characterName);

    // TODO: Initialiser les autres attributs du joueur

    // Passage à l'état de jeu
    if (m_manager)
    {
        m_manager->pushState("Game");
    }
}