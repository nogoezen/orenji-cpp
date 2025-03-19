#include "TitleScreen.h"
#include "../core/Game.h"
#include "../utils/FileSystem.h"
#include <iostream>
#include <filesystem>
#include <cmath>

TitleScreen::TitleScreen(std::shared_ptr<Game> game)
    : m_game(game), m_selectedIndex(0), m_isStartScreen(true), m_textAlpha(0), m_fadeIn(true)
{
    initWindow();
    loadResources();
    createStartScreen();
}

TitleScreen::~TitleScreen()
{
    // Pas besoin de nettoyer explicitement les ressources SFML
    // car leurs destructeurs s'en chargeront
}

void TitleScreen::initWindow()
{
    // Créer une fenêtre de 1280x720 pixels, avec un titre
    m_window.create(sf::VideoMode(1280, 720), "Uncharted Waters", sf::Style::Titlebar | sf::Style::Close);
    m_window.setFramerateLimit(60);
}

void TitleScreen::loadResources()
{
    // Charger l'image de fond
    std::string backgroundPath = "assets/img/MainMenuBG.png";
    if (!m_backgroundTexture.loadFromFile(backgroundPath))
    {
        std::cerr << "Erreur: Impossible de charger l'image de fond: " << backgroundPath << std::endl;
    }
    m_backgroundSprite.setTexture(m_backgroundTexture);

    // Ajuster l'image pour qu'elle remplisse l'écran
    float scaleX = m_window.getSize().x / static_cast<float>(m_backgroundTexture.getSize().x);
    float scaleY = m_window.getSize().y / static_cast<float>(m_backgroundTexture.getSize().y);
    m_backgroundSprite.setScale(scaleX, scaleY);

    // Charger la police
    std::string fontPath = "assets/font/VeniceClassic.ttf";
    if (!m_font.loadFromFile(fontPath))
    {
        std::cerr << "Erreur: Impossible de charger la police: " << fontPath << std::endl;

        // Essayer avec une police système
        if (std::filesystem::exists("C:/Windows/Fonts/arial.ttf"))
        {
            if (!m_font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
            {
                std::cerr << "Erreur: Impossible de charger la police système." << std::endl;
            }
        }
    }
}

void TitleScreen::createStartScreen()
{
    // Configurer le texte "Appuyez sur une touche pour continuer"
    m_pressAnyKeyText.setFont(m_font);
    m_pressAnyKeyText.setString("APPUYEZ SUR UNE TOUCHE POUR CONTINUER");
    m_pressAnyKeyText.setCharacterSize(32);
    m_pressAnyKeyText.setStyle(sf::Text::Bold);
    m_pressAnyKeyText.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(m_textAlpha)));
    m_pressAnyKeyText.setOutlineColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_textAlpha)));
    m_pressAnyKeyText.setOutlineThickness(2.0f);

    // Centrer le texte
    m_pressAnyKeyText.setOrigin(m_pressAnyKeyText.getLocalBounds().width / 2, m_pressAnyKeyText.getLocalBounds().height / 2);
    m_pressAnyKeyText.setPosition(m_window.getSize().x / 2, m_window.getSize().y * 0.75f);

    // Ajouter le titre pour l'écran de démarrage
    sf::Text title;
    title.setFont(m_font);
    title.setString("UNCHARTED WATERS");
    title.setCharacterSize(72);
    title.setStyle(sf::Text::Bold);
    title.setFillColor(sf::Color(255, 215, 0));
    title.setOutlineColor(sf::Color(139, 69, 19));
    title.setOutlineThickness(3.0f);
    title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
    title.setPosition(m_window.getSize().x / 2, m_window.getSize().y / 3);

    m_menuItems.clear();
    m_menuItems.push_back(title);
}

void TitleScreen::createMenuItems()
{
    const std::vector<std::string> menuLabels = {
        "Nouvelle partie",
        hasSaveGame() ? "Continuer partie" : "Continuer partie (non disponible)",
        "Test carte Tiled",
        "Options",
        "Crédits",
        "Quitter"};

    m_menuItems.clear();

    for (size_t i = 0; i < menuLabels.size(); ++i)
    {
        sf::Text text;
        text.setFont(m_font);
        text.setString(menuLabels[i]);
        text.setCharacterSize(32);

        // Positionner les éléments au centre
        float x = m_window.getSize().x / 2;
        float y = m_window.getSize().y / 2 + i * 60;

        text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
        text.setPosition(x, y);

        // Couleurs modifiées pour plus d'impact visuel
        if (i == m_selectedIndex)
        {
            // Utiliser un bleu vif pour l'élément sélectionné
            text.setFillColor(sf::Color(50, 150, 255));
            text.setStyle(sf::Text::Bold);

            // Ajouter un contour blanc pour plus de contraste
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(1.5f);
        }
        else
        {
            // Utiliser un bleu plus clair pour les autres éléments
            text.setFillColor(sf::Color(150, 200, 255));
            text.setOutlineThickness(0);
        }

        m_menuItems.push_back(text);
    }

    // Ajouter le titre avec un effet plus impressionnant
    sf::Text title;
    title.setFont(m_font);
    title.setString("UNCHARTED WATERS");
    title.setCharacterSize(72);
    title.setStyle(sf::Text::Bold);

    // Utiliser un dégradé doré pour le titre
    title.setFillColor(sf::Color(255, 215, 0));
    title.setOutlineColor(sf::Color(139, 69, 19));
    title.setOutlineThickness(3.0f);

    title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
    title.setPosition(m_window.getSize().x / 2, m_window.getSize().y / 5);

    // Ajouter le sous-titre avec un style amélioré
    sf::Text subtitle;
    subtitle.setFont(m_font);
    subtitle.setString("Un jeu de commerce maritime");
    subtitle.setCharacterSize(32);
    subtitle.setFillColor(sf::Color(173, 216, 230));
    subtitle.setStyle(sf::Text::Italic);
    subtitle.setOrigin(subtitle.getLocalBounds().width / 2, subtitle.getLocalBounds().height / 2);
    subtitle.setPosition(m_window.getSize().x / 2, m_window.getSize().y / 5 + 80);

    // Ajouter une version
    sf::Text version;
    version.setFont(m_font);
    version.setString("Version 1.0");
    version.setCharacterSize(18);
    version.setFillColor(sf::Color(200, 200, 200, 128));
    version.setPosition(m_window.getSize().x - 120, m_window.getSize().y - 40);

    // Ajouter le titre et le sous-titre au début de la liste
    m_menuItems.insert(m_menuItems.begin(), version);
    m_menuItems.insert(m_menuItems.begin(), subtitle);
    m_menuItems.insert(m_menuItems.begin(), title);
}

void TitleScreen::run()
{
    while (m_window.isOpen())
    {
        handleEvents();
        update();
        render();
    }
}

void TitleScreen::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }
        else if (m_isStartScreen)
        {
            // Sur l'écran de démarrage, n'importe quelle touche ou clic fait passer au menu principal
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed)
            {
                m_isStartScreen = false;
                createMenuItems();
            }
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Up)
            {
                // Naviguer vers le haut dans le menu (en sautant les 2 premiers items qui sont le titre et le sous-titre)
                m_selectedIndex = std::max(0, m_selectedIndex - 1);
                createMenuItems();
            }
            else if (event.key.code == sf::Keyboard::Down)
            {
                // Naviguer vers le bas dans le menu
                m_selectedIndex = std::min(static_cast<int>(m_menuItems.size() - 3), m_selectedIndex + 1);
                createMenuItems();
            }
            else if (event.key.code == sf::Keyboard::Return)
            {
                // Activer l'élément sélectionné
                switch (m_selectedIndex)
                {
                case 0:
                    startNewGame();
                    break;
                case 1:
                    continueGame();
                    break;
                case 2:
                    testTiledMap();
                    break;
                case 3:
                    showOptions();
                    break;
                case 4:
                    showCredits();
                    break;
                case 5:
                    exit();
                    break;
                }
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                m_window.close();
            }
        }
    }
}

void TitleScreen::update()
{
    if (m_isStartScreen)
    {
        // Animation de pulsation pour le texte "Appuyez sur une touche"
        if (m_fadeIn)
        {
            m_textAlpha += 3.0f;
            if (m_textAlpha >= 255.0f)
            {
                m_textAlpha = 255.0f;
                m_fadeIn = false;
            }
        }
        else
        {
            m_textAlpha -= 3.0f;
            if (m_textAlpha <= 50.0f)
            {
                m_textAlpha = 50.0f;
                m_fadeIn = true;
            }
        }

        m_pressAnyKeyText.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(m_textAlpha)));
        m_pressAnyKeyText.setOutlineColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_textAlpha)));

        // Animation du titre
        static float time = 0.0f;
        time += 0.02f;

        if (!m_menuItems.empty())
        {
            sf::Text &title = m_menuItems[0];
            title.setPosition(m_window.getSize().x / 2,
                              m_window.getSize().y / 3 + sin(time) * 5.0f);

            float colorPulse = (sin(time * 0.5f) + 1.0f) / 2.0f;
            sf::Color titleColor(
                255,
                215 + colorPulse * 40,
                colorPulse * 50);
            title.setFillColor(titleColor);
        }
    }
    else
    {
        // Animation simple pour le titre (oscillation légère)
        static float time = 0.0f;
        time += 0.02f;

        // Faire osciller légèrement le titre
        if (m_menuItems.size() > 2)
        {
            sf::Text &title = m_menuItems[0];
            title.setPosition(m_window.getSize().x / 2,
                              m_window.getSize().y / 5 + sin(time) * 5.0f);

            // Animation de couleur pour le titre
            float colorPulse = (sin(time * 0.5f) + 1.0f) / 2.0f;
            sf::Color titleColor(
                255,
                215 + colorPulse * 40,
                colorPulse * 50);
            title.setFillColor(titleColor);
        }
    }
}

void TitleScreen::render()
{
    m_window.clear();

    // Dessiner l'arrière-plan
    m_window.draw(m_backgroundSprite);

    // Ajouter un filtre semi-transparent pour améliorer la lisibilité du texte
    sf::RectangleShape overlay(sf::Vector2f(m_window.getSize().x, m_window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 50, 100));
    m_window.draw(overlay);

    // Dessiner tous les éléments du menu
    for (const auto &item : m_menuItems)
    {
        m_window.draw(item);
    }

    // Si on est sur l'écran de démarrage, afficher le texte "Appuyez sur une touche"
    if (m_isStartScreen)
    {
        m_window.draw(m_pressAnyKeyText);
    }

    // Afficher tout
    m_window.display();
}

bool TitleScreen::hasSaveGame() const
{
    return FileSystem::fileExists("bin/save.json");
}

void TitleScreen::startNewGame()
{
    if (m_game)
    {
        m_window.close();
        m_game->startNewGame();
    }
    else
    {
        std::cerr << "Erreur: Gestionnaire de jeu non initialisé" << std::endl;
    }
}

void TitleScreen::continueGame()
{
    if (m_game && hasSaveGame())
    {
        m_window.close();
        m_game->loadGame("bin/save.json");
    }
    else
    {
        std::cerr << "Erreur: Impossible de charger la partie" << std::endl;
    }
}

void TitleScreen::showOptions()
{
    // À implémenter
    std::cout << "Menu Options (non implémenté)" << std::endl;
}

void TitleScreen::showCredits()
{
    // À implémenter
    std::cout << "Crédits (non implémentés)" << std::endl;
}

void TitleScreen::exit()
{
    m_window.close();
}

void TitleScreen::testTiledMap()
{
    if (m_game)
    {
        m_window.close();
        // Lancer la scène de carte avec un fichier de test
        m_game->launchMapScene("assets/maps/test_map.json");
    }
    else
    {
        std::cerr << "Erreur: Gestionnaire de jeu non initialisé" << std::endl;
    }
}