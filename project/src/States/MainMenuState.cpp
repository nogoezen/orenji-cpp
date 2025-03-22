#include "States/MainMenuState.hpp"
#include "States/StateMachine.hpp"
#include "Utilities/ResourceManager.hpp"
#include <iostream>

namespace Orenji
{
    MainMenuState::MainMenuState(StateMachine &machine)
        : State(machine), m_selectedItemIndex(0)
    {
    }

    void MainMenuState::onEnter()
    {
        // Charger la police
        if (!ResourceManager::getInstance().loadFont("main", "resources/fonts/arial.ttf"))
        {
            std::cerr << "Erreur lors du chargement de la police" << std::endl;
        }
        m_font = ResourceManager::getInstance().getFont("main");

        // Configuration du titre
        m_titleText.setFont(m_font);
        m_titleText.setString("ORENJI GAME");
        m_titleText.setCharacterSize(72);
        m_titleText.setFillColor(sf::Color(255, 140, 0)); // Orange
        m_titleText.setStyle(sf::Text::Bold);

        // Configuration des options du menu
        const std::vector<std::string> menuOptions = {
            "Start Game",
            "Options",
            "Credits",
            "Exit"};

        for (const auto &option : menuOptions)
        {
            sf::Text text;
            text.setFont(m_font);
            text.setString(option);
            text.setCharacterSize(36);
            text.setFillColor(sf::Color::White);
            m_menuItems.push_back(text);
        }

        // Mettre en évidence la première option
        m_menuItems[0].setFillColor(sf::Color(255, 140, 0)); // Orange

        // Charger le fond
        if (!ResourceManager::getInstance().loadTexture("menu_background", "resources/textures/background.jpg"))
        {
            std::cerr << "Erreur lors du chargement de l'image de fond" << std::endl;
        }
        m_backgroundTexture = ResourceManager::getInstance().getTexture("menu_background");
        m_backgroundSprite.setTexture(m_backgroundTexture);

        // Positionner les éléments (à ajuster dynamiquement dans draw pour s'adapter à la taille de la fenêtre)
    }

    void MainMenuState::onExit()
    {
        // Nettoyage si nécessaire
    }

    void MainMenuState::update(float dt)
    {
        // Animation si nécessaire
    }

    void MainMenuState::handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                updateMenuSelection(-1);
                break;

            case sf::Keyboard::Down:
                updateMenuSelection(1);
                break;

            case sf::Keyboard::Return:
                executeSelectedAction();
                break;

            case sf::Keyboard::Escape:
                // Quitter le jeu
                // window.close();  // Géré par la classe Game
                break;

            default:
                break;
            }
        }
    }

    void MainMenuState::draw(sf::RenderTarget &target)
    {
        // Récupérer la taille de la fenêtre
        sf::Vector2u windowSize = target.getSize();
        float windowWidth = static_cast<float>(windowSize.x);
        float windowHeight = static_cast<float>(windowSize.y);

        // Ajuster la taille et la position du fond
        float scaleX = windowWidth / m_backgroundTexture.getSize().x;
        float scaleY = windowHeight / m_backgroundTexture.getSize().y;
        float scale = std::max(scaleX, scaleY);
        m_backgroundSprite.setScale(scale, scale);

        // Centrer le fond
        m_backgroundSprite.setPosition(
            (windowWidth - m_backgroundTexture.getSize().x * scale) / 2,
            (windowHeight - m_backgroundTexture.getSize().y * scale) / 2);

        // Dessiner le fond
        target.draw(m_backgroundSprite);

        // Positionner et dessiner le titre
        centerText(m_titleText, windowHeight * 0.15f, windowWidth);
        target.draw(m_titleText);

        // Positionner et dessiner les options du menu
        float menuStartY = windowHeight * 0.4f;
        float menuItemSpacing = 60.0f;

        for (size_t i = 0; i < m_menuItems.size(); ++i)
        {
            centerText(m_menuItems[i], menuStartY + i * menuItemSpacing, windowWidth);
            target.draw(m_menuItems[i]);
        }
    }

    void MainMenuState::centerText(sf::Text &text, float y, float windowWidth)
    {
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        text.setPosition(windowWidth / 2.0f, y);
    }

    void MainMenuState::updateMenuSelection(int direction)
    {
        // Réinitialiser la couleur de l'item actuellement sélectionné
        m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::White);

        // Mettre à jour l'index de sélection
        m_selectedItemIndex += direction;

        // Assurer que l'index reste dans les limites
        if (m_selectedItemIndex < 0)
            m_selectedItemIndex = m_menuItems.size() - 1;
        else if (m_selectedItemIndex >= static_cast<int>(m_menuItems.size()))
            m_selectedItemIndex = 0;

        // Mettre en évidence le nouvel item sélectionné
        m_menuItems[m_selectedItemIndex].setFillColor(sf::Color(255, 140, 0)); // Orange
    }

    void MainMenuState::executeSelectedAction()
    {
        switch (m_selectedItemIndex)
        {
        case 0: // Start Game
            m_machine.changeState("Game");
            break;

        case 1: // Options
            // TODO: Implémenter les options
            break;

        case 2: // Credits
            // TODO: Implémenter les crédits
            break;

        case 3: // Exit
            // La fenêtre sera fermée par la classe Game
            break;

        default:
            break;
        }
    }

} // namespace Orenji