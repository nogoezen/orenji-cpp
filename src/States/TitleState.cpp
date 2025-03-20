#include "../../include/States/TitleState.hpp"
#include "../../include/States/GameState.hpp"
#include "../../include/States/StateMachine.hpp"
#include <iostream>

namespace Orenji
{
    TitleState::TitleState()
        : m_selectedItemIndex(0), m_fadeAlpha(255.f), m_isExiting(false),
          m_titleY(100.f), m_titleYSpeed(15.f)
    {
    }

    TitleState::~TitleState()
    {
        // Arrêter la musique quand l'état est détruit
        m_music.stop();
    }

    void TitleState::initialize()
    {
        // Chargement de la texture d'arrière-plan
        if (!m_backgroundTexture.loadFromFile("resources/textures/Titles/001-Title01.jpg"))
        {
            std::cerr << "Erreur: Impossible de charger l'image d'arrière-plan du menu titre" << std::endl;
        }
        m_backgroundSprite.setTexture(m_backgroundTexture);

        // Ajuster la taille du sprite pour remplir l'écran
        sf::Vector2u windowSize = getStateMachine()->getWindow().getSize();
        sf::Vector2u textureSize = m_backgroundTexture.getSize();
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        m_backgroundSprite.setScale(scaleX, scaleY);

        // Chargement de la police
        if (!m_font.loadFromFile("resources/fonts/VeniceClassic.ttf"))
        {
            std::cerr << "Erreur: Impossible de charger la police pour le menu titre" << std::endl;
        }

        // Configuration du titre
        m_titleText.setFont(m_font);
        m_titleText.setString("Orenji RPG");
        m_titleText.setCharacterSize(72);
        m_titleText.setFillColor(sf::Color(255, 200, 50));  // Jaune orangé
        m_titleText.setOutlineColor(sf::Color(100, 50, 0)); // Marron
        m_titleText.setOutlineThickness(3.f);
        m_titleText.setStyle(sf::Text::Bold);

        // Centrer le titre
        sf::FloatRect titleBounds = m_titleText.getLocalBounds();
        m_titleText.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
        m_titleText.setPosition(windowSize.x / 2.f, m_titleY);

        // Configuration du rectangle de fondu
        m_fadeRect.setSize(sf::Vector2f(windowSize.x, windowSize.y));
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, m_fadeAlpha));

        // Création des éléments du menu
        const float menuItemY = 300.f;
        const float menuItemSpacing = 60.f;

        // Élément "Nouvelle Partie"
        MenuItem newGameItem;
        newGameItem.text.setFont(m_font);
        newGameItem.text.setString("Nouvelle Partie");
        newGameItem.text.setCharacterSize(36);
        newGameItem.isSelected = true;
        newGameItem.callback = [this]()
        { startGame(); };

        sf::FloatRect bounds = newGameItem.text.getLocalBounds();
        newGameItem.text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        newGameItem.text.setPosition(windowSize.x / 2.f, menuItemY);
        newGameItem.text.setFillColor(sf::Color::Yellow);
        m_menuItems.push_back(newGameItem);

        // Élément "Options"
        MenuItem optionsItem;
        optionsItem.text.setFont(m_font);
        optionsItem.text.setString("Options");
        optionsItem.text.setCharacterSize(36);
        optionsItem.isSelected = false;
        optionsItem.callback = [this]()
        { showOptions(); };

        bounds = optionsItem.text.getLocalBounds();
        optionsItem.text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        optionsItem.text.setPosition(windowSize.x / 2.f, menuItemY + menuItemSpacing);
        optionsItem.text.setFillColor(sf::Color::White);
        m_menuItems.push_back(optionsItem);

        // Élément "Quitter"
        MenuItem exitItem;
        exitItem.text.setFont(m_font);
        exitItem.text.setString("Quitter");
        exitItem.text.setCharacterSize(36);
        exitItem.isSelected = false;
        exitItem.callback = [this]()
        { exitGame(); };

        bounds = exitItem.text.getLocalBounds();
        exitItem.text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        exitItem.text.setPosition(windowSize.x / 2.f, menuItemY + menuItemSpacing * 2);
        exitItem.text.setFillColor(sf::Color::White);
        m_menuItems.push_back(exitItem);

        // Chargement et lecture de la musique
        if (!m_music.openFromFile("resources/sounds/BGM/012-Theme01.mid"))
        {
            std::cerr << "Erreur: Impossible de charger la musique du menu titre" << std::endl;
        }
        m_music.setLoop(true);
        m_music.setVolume(80.f);
        m_music.play();

        // Effet de fondu à l'entrée
        m_fadeAlpha = 255.f;
    }

    void TitleState::update(float deltaTime)
    {
        // Animation du titre (légère oscillation)
        m_titleY += m_titleYSpeed * deltaTime;
        if (m_titleY > 110.f)
        {
            m_titleY = 110.f;
            m_titleYSpeed = -m_titleYSpeed;
        }
        else if (m_titleY < 90.f)
        {
            m_titleY = 90.f;
            m_titleYSpeed = -m_titleYSpeed;
        }
        m_titleText.setPosition(m_titleText.getPosition().x, m_titleY);

        // Effet de fondu à l'entrée
        if (m_fadeAlpha > 0.f && !m_isExiting)
        {
            m_fadeAlpha -= 255.f * deltaTime; // Fondu en 1 seconde
            if (m_fadeAlpha < 0.f)
                m_fadeAlpha = 0.f;
            m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_fadeAlpha)));
        }

        // Effet de fondu à la sortie
        if (m_isExiting)
        {
            m_fadeAlpha += 255.f * deltaTime * 2.f; // Fondu en 0.5 seconde
            if (m_fadeAlpha >= 255.f)
            {
                // Transition terminée, passer à l'état suivant
                m_fadeAlpha = 255.f;
                m_music.stop();
                // Déclenchement de la transition définie dans les callbacks
            }
            m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_fadeAlpha)));
        }

        // Animation de pulsation pour l'élément sélectionné
        for (size_t i = 0; i < m_menuItems.size(); ++i)
        {
            if (i == m_selectedItemIndex)
            {
                // Effet de pulsation pour l'élément sélectionné
                float pulse = 1.f + 0.1f * std::sin(getStateMachine()->getElapsedTime() * 5.f);
                m_menuItems[i].text.setScale(pulse, pulse);

                // Couleur jaune pour l'élément sélectionné
                m_menuItems[i].text.setFillColor(sf::Color::Yellow);
                m_menuItems[i].text.setOutlineColor(sf::Color(100, 50, 0));
                m_menuItems[i].text.setOutlineThickness(1.f);
            }
            else
            {
                // Paramètres normaux pour les éléments non sélectionnés
                m_menuItems[i].text.setScale(1.f, 1.f);
                m_menuItems[i].text.setFillColor(sf::Color::White);
                m_menuItems[i].text.setOutlineThickness(0.f);
            }
        }
    }

    void TitleState::handleEvent(sf::Event &event)
    {
        if (m_isExiting)
            return;

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                selectPreviousItem();
                break;
            case sf::Keyboard::Down:
                selectNextItem();
                break;
            case sf::Keyboard::Return:
                activateItem();
                break;
            case sf::Keyboard::Escape:
                exitGame();
                break;
            default:
                break;
            }
        }
    }

    void TitleState::render(sf::RenderWindow &window)
    {
        // Dessiner l'arrière-plan
        window.draw(m_backgroundSprite);

        // Dessiner le titre
        window.draw(m_titleText);

        // Dessiner les éléments du menu
        for (const auto &item : m_menuItems)
        {
            window.draw(item.text);
        }

        // Dessiner le rectangle de fondu
        window.draw(m_fadeRect);
    }

    void TitleState::selectNextItem()
    {
        // Changer la sélection vers l'élément suivant
        m_menuItems[m_selectedItemIndex].isSelected = false;
        m_selectedItemIndex = (m_selectedItemIndex + 1) % m_menuItems.size();
        m_menuItems[m_selectedItemIndex].isSelected = true;
    }

    void TitleState::selectPreviousItem()
    {
        // Changer la sélection vers l'élément précédent
        m_menuItems[m_selectedItemIndex].isSelected = false;
        m_selectedItemIndex = (m_selectedItemIndex - 1 + m_menuItems.size()) % m_menuItems.size();
        m_menuItems[m_selectedItemIndex].isSelected = true;
    }

    void TitleState::activateItem()
    {
        // Activer l'élément sélectionné
        if (!m_isExiting && m_selectedItemIndex >= 0 && m_selectedItemIndex < static_cast<int>(m_menuItems.size()))
        {
            m_menuItems[m_selectedItemIndex].callback();
        }
    }

    void TitleState::startGame()
    {
        m_isExiting = true;
        // Remplacer cette fonction par la transition vers l'état de jeu
        // à effectuer une fois l'animation de sortie terminée
        if (m_fadeAlpha >= 255.f)
        {
            getStateMachine()->changeState(std::make_unique<GameState>());
        }
    }

    void TitleState::showOptions()
    {
        // À implémenter: transition vers le menu des options
        // Pour le moment, on affiche juste un message dans la console
        std::cout << "Options menu (not implemented yet)" << std::endl;
    }

    void TitleState::exitGame()
    {
        m_isExiting = true;
        // Fermer la fenêtre une fois l'animation de sortie terminée
        if (m_fadeAlpha >= 255.f)
        {
            getStateMachine()->getWindow().close();
        }
    }

} // namespace Orenji