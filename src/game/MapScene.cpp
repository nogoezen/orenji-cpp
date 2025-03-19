#include "MapScene.h"
#include "../core/Game.h"
#include <iostream>

MapScene::MapScene(std::shared_ptr<Game> game, const std::string &mapFilePath)
    : m_game(game), m_playerPosition(0.0f, 0.0f), m_playerSpeed(200.0f)
{
    initWindow();

    // Charger la carte Tiled
    std::cout << "Chargement de la carte: " << mapFilePath << std::endl;
    try
    {
        m_map = std::make_unique<TiledMap>(mapFilePath);
        std::cout << "Carte chargée avec succès: " << m_map->getWidth() << "x" << m_map->getHeight()
                  << " tuiles de " << m_map->getTileWidth() << "x" << m_map->getTileHeight() << " pixels." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erreur lors du chargement de la carte: " << e.what() << std::endl;
    }

    // Initialiser la caméra
    m_camera.setSize(static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y));
    m_camera.setCenter(m_playerPosition);
}

MapScene::~MapScene()
{
    // Pas besoin de nettoyer explicitement, les destructeurs des membres s'en chargeront
}

void MapScene::initWindow()
{
    // Créer une fenêtre de 1280x720 pixels, avec un titre
    m_window.create(sf::VideoMode(1280, 720), "Uncharted Waters - Map View", sf::Style::Titlebar | sf::Style::Close);
    m_window.setFramerateLimit(60);
}

void MapScene::run()
{
    sf::Clock clock;

    while (m_window.isOpen())
    {
        // Calculer le temps écoulé depuis la dernière frame
        float deltaTime = clock.restart().asSeconds();

        handleEvents();
        update(deltaTime);
        render();
    }
}

void MapScene::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                m_window.close();
            }
        }
    }

    // Gestion du déplacement du joueur
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        m_playerPosition.y -= m_playerSpeed * 0.016f; // Approximation pour 60 FPS
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        m_playerPosition.y += m_playerSpeed * 0.016f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        m_playerPosition.x -= m_playerSpeed * 0.016f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        m_playerPosition.x += m_playerSpeed * 0.016f;
    }
}

void MapScene::update(float deltaTime)
{
    // Limiter la position du joueur à la taille de la carte
    if (m_map)
    {
        m_playerPosition.x = std::max(0.0f, std::min(m_playerPosition.x, static_cast<float>(m_map->getPixelWidth())));
        m_playerPosition.y = std::max(0.0f, std::min(m_playerPosition.y, static_cast<float>(m_map->getPixelHeight())));
    }

    // Mettre à jour la caméra pour suivre le joueur
    m_camera.setCenter(m_playerPosition);
    m_window.setView(m_camera);
}

void MapScene::render()
{
    m_window.clear(sf::Color(0, 0, 50));

    // Dessiner la carte
    if (m_map)
    {
        m_window.draw(*m_map);
    }

    // Dessiner le joueur (un simple cercle pour l'instant)
    sf::CircleShape playerShape(16.0f);
    playerShape.setFillColor(sf::Color::Red);
    playerShape.setOrigin(16.0f, 16.0f);
    playerShape.setPosition(m_playerPosition);
    m_window.draw(playerShape);

    m_window.display();
}