#include "../../include/States/TitleState.hpp"
#include "../../include/States/GameState.hpp"
#include "../../include/States/StateMachine.hpp"
#include "../../include/Utilities/InputHandler.hpp"
#include "../../include/Utilities/ResourceManager.hpp"
#include <iostream>
#include <cmath>

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

    bool TitleState::initialize()
    {
        // Chargement de la texture d'arrière-plan avec ResourceManager
        ResourceManager &resourceManager = ResourceManager::getInstance();
        sf::Texture *backgroundTexture = resourceManager.loadTexture("titleBackground", "resources/textures/Titles/001-Title01.jpg");

        if (!backgroundTexture)
        {
            std::cerr << "Erreur: Impossible de charger l'image d'arrière-plan du menu titre" << std::endl;
            return false;
        }
        m_backgroundTexture = *backgroundTexture;
        m_backgroundSprite.setTexture(m_backgroundTexture);

        // Ajuster la taille du sprite pour remplir l'écran
        sf::Vector2u windowSize = getStateMachine()->getWindow().getSize();
        sf::Vector2u textureSize = m_backgroundTexture.getSize();
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        m_backgroundSprite.setScale(scaleX, scaleY);

        // Chargement de la police avec ResourceManager
        sf::Font *titleFont = resourceManager.loadFont("titleFont", "resources/fonts/VeniceClassic.ttf");
        if (!titleFont)
        {
            std::cerr << "Erreur: Impossible de charger la police pour le menu titre" << std::endl;
            return false;
        }
        m_font = *titleFont;

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

        // Création des éléments du menu
        MenuItem newGameItem;
        newGameItem.text.setFont(m_font);
        newGameItem.text.setString("Nouvelle Partie");
        newGameItem.text.setCharacterSize(36);
        newGameItem.text.setFillColor(sf::Color::White);
        newGameItem.isSelected = true;
        newGameItem.callback = [this]()
        {
            startGame();
        };
        sf::FloatRect bounds = newGameItem.text.getLocalBounds();
        newGameItem.text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        newGameItem.text.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);

        MenuItem optionsItem;
        optionsItem.text.setFont(m_font);
        optionsItem.text.setString("Options");
        optionsItem.text.setCharacterSize(36);
        optionsItem.text.setFillColor(sf::Color::White);
        optionsItem.isSelected = false;
        optionsItem.callback = [this]()
        {
            showOptions();
        };
        bounds = optionsItem.text.getLocalBounds();
        optionsItem.text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        optionsItem.text.setPosition(windowSize.x / 2.f, windowSize.y / 2.f + 60.f);

        MenuItem exitItem;
        exitItem.text.setFont(m_font);
        exitItem.text.setString("Quitter");
        exitItem.text.setCharacterSize(36);
        exitItem.text.setFillColor(sf::Color::White);
        exitItem.isSelected = false;
        exitItem.callback = [this]()
        {
            exitGame();
        };
        bounds = exitItem.text.getLocalBounds();
        exitItem.text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        exitItem.text.setPosition(windowSize.x / 2.f, windowSize.y / 2.f + 120.f);

        // Ajouter les éléments au menu
        m_menuItems.push_back(newGameItem);
        m_menuItems.push_back(optionsItem);
        m_menuItems.push_back(exitItem);

        // Configurer le rectangle de fondu
        m_fadeRect.setSize(sf::Vector2f(windowSize.x, windowSize.y));
        m_fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_fadeAlpha)));

        // Charger et démarrer la musique de fond avec ResourceManager
        // Note: Nous utilisons directement m_music car SFML ne permet pas de récupérer la musique à partir d'un buffer
        // comme pour les sons, mais nous pouvons utiliser ResourceManager pour vérifier si le fichier existe d'abord

        // Le code suivant est commenté car les fichiers audio peuvent ne pas exister dans le projet
        // Décommentez cette section lorsque vous avez un fichier audio disponible

        /*
        std::string musicPath = "resources/musics/title_theme.ogg";
        // Utilisons ResourceManager pour vérifier si le fichier existe avant de le charger
        sf::SoundBuffer* testBuffer = resourceManager.loadSoundBuffer("titleThemeTest", musicPath);

        if (testBuffer) {
            // Si le buffer a été chargé avec succès, cela signifie que le fichier existe
            // Maintenant, nous pouvons charger la musique directement
            if (!m_music.openFromFile(musicPath))
            {
                std::cerr << "Erreur: Impossible de charger la musique du menu titre" << std::endl;
            }
            else
            {
                m_music.setLoop(true);
                m_music.setVolume(50.f);
                m_music.play();
            }

            // Libérer le buffer de test
            resourceManager.clearSounds();
        }
        */

        // Configurer les actions d'entrée avec InputHandler
        InputHandler &inputHandler = InputHandler::getInstance();
        inputHandler.bindKeyAction(sf::Keyboard::Up, "menuUp");
        inputHandler.bindKeyAction(sf::Keyboard::Down, "menuDown");
        inputHandler.bindKeyAction(sf::Keyboard::Return, "menuSelect");
        inputHandler.bindKeyAction(sf::Keyboard::Escape, "menuBack");

        return true;
    }

    bool TitleState::update(float deltaTime)
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
                m_menuItems[i].text.setFillColor(sf::Color(255, 200, 50)); // Jaune orangé
            }
            else
            {
                // Réinitialiser l'échelle et la couleur pour les éléments non sélectionnés
                m_menuItems[i].text.setScale(1.f, 1.f);
                m_menuItems[i].text.setFillColor(sf::Color::White);
            }
        }

        return true;
    }

    void TitleState::handleEvent(sf::Event &event)
    {
        // La gestion des événements spécifiques est maintenant déléguée à InputHandler
        // Cette méthode peut rester disponible pour des traitements d'événements spéciaux si nécessaire
        InputHandler::getInstance().processEvent(event);
    }

    bool TitleState::handleInput()
    {
        // Utilisation d'InputHandler pour gérer les entrées
        InputHandler &inputHandler = InputHandler::getInstance();

        // Vérifier les actions liées au menu
        if (inputHandler.isActionPressed("menuUp"))
        {
            selectPreviousItem();
        }
        else if (inputHandler.isActionPressed("menuDown"))
        {
            selectNextItem();
        }
        else if (inputHandler.isActionPressed("menuSelect"))
        {
            activateItem();
        }
        else if (inputHandler.isActionPressed("menuBack"))
        {
            exitGame();
        }

        return true;
    }

    bool TitleState::render()
    {
        // Délégation au render qui prend la fenêtre en paramètre
        if (getStateMachine())
        {
            sf::RenderWindow &window = getStateMachine()->getWindow();
            render(window);
            return true;
        }
        return false;
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