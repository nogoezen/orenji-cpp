#include "State/GameState.hpp"
#include <iostream>
#include <random>

namespace Orenji
{
    GameState::GameState(const std::string &level)
        : m_currentLevel(level),
          m_isPaused(false),
          m_isGameOver(false),
          m_gameTime(0.0f),
          m_score(0)
    {
    }

    bool GameState::initialize()
    {
        // Charge la police
        if (!m_font.loadFromFile("resources/fonts/VeinceClassic.ttf"))
        {
            std::cerr << "Erreur lors du chargement de la police\n";
            // Créer une police par défaut pour les exemples si le chargement échoue
            std::vector<uint8_t> fontData(1000, 0);
            if (!m_font.loadFromMemory(fontData.data(), fontData.size()))
            {
                return false;
            }
        }

        // Configure le texte d'état
        m_statusText.setFont(m_font);
        m_statusText.setCharacterSize(36);
        m_statusText.setFillColor(sf::Color::White);
        m_statusText.setOutlineColor(sf::Color::Black);
        m_statusText.setOutlineThickness(2.0f);
        m_statusText.setString("");

        // Configure le texte du score
        m_scoreText.setFont(m_font);
        m_scoreText.setCharacterSize(24);
        m_scoreText.setFillColor(sf::Color::White);
        m_scoreText.setOutlineColor(sf::Color::Black);
        m_scoreText.setOutlineThickness(1.0f);
        m_scoreText.setString("Score: 0");
        m_scoreText.setPosition(20.0f, 20.0f);

        // Configure l'overlay de pause
        m_pauseOverlay.setFillColor(sf::Color(0, 0, 0, 128));
        m_pauseOverlay.setSize(sf::Vector2f(800.0f, 600.0f));
        m_pauseOverlay.setPosition(0, 0);

        // Configure le joueur
        m_player.setSize(sf::Vector2f(50.0f, 50.0f));
        m_player.setFillColor(sf::Color::Green);
        m_player.setPosition(375.0f, 500.0f);

        // Crée quelques obstacles aléatoires
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> xDist(50.0f, 750.0f);
        std::uniform_real_distribution<float> yDist(50.0f, 400.0f);
        std::uniform_real_distribution<float> sizeDist(30.0f, 100.0f);

        for (int i = 0; i < 5; ++i)
        {
            sf::RectangleShape obstacle;
            float size = sizeDist(gen);
            obstacle.setSize(sf::Vector2f(size, size));
            obstacle.setFillColor(sf::Color::Red);
            obstacle.setPosition(xDist(gen), yDist(gen));
            m_obstacles.push_back(obstacle);
        }

        // Charge le niveau si spécifié
        if (!m_currentLevel.empty())
        {
            loadLevel(m_currentLevel);
        }

        return true;
    }

    void GameState::update(float deltaTime)
    {
        if (m_isPaused || m_isGameOver)
        {
            return;
        }

        // Met à jour le temps de jeu
        m_gameTime += deltaTime;

        // Met à jour le score toutes les secondes
        static float timeSinceLastScoreUpdate = 0.0f;
        timeSinceLastScoreUpdate += deltaTime;
        if (timeSinceLastScoreUpdate >= 1.0f)
        {
            m_score += 10;
            m_scoreText.setString("Score: " + std::to_string(m_score));
            timeSinceLastScoreUpdate = 0.0f;
        }

        // Déplace le joueur légèrement pour simuler un mouvement
        float playerSpeed = 100.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            m_player.move(-playerSpeed * deltaTime, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            m_player.move(playerSpeed * deltaTime, 0);
        }

        // Garde le joueur dans les limites de l'écran
        sf::Vector2f playerPos = m_player.getPosition();
        if (playerPos.x < 0)
        {
            m_player.setPosition(0, playerPos.y);
        }
        else if (playerPos.x > 750.0f)
        {
            m_player.setPosition(750.0f, playerPos.y);
        }

        // Détection des collisions simple
        sf::FloatRect playerBounds = m_player.getGlobalBounds();
        for (const auto &obstacle : m_obstacles)
        {
            if (playerBounds.intersects(obstacle.getGlobalBounds()))
            {
                m_isGameOver = true;
                m_statusText.setString("Game Over!");
                sf::FloatRect textBounds = m_statusText.getLocalBounds();
                m_statusText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
                m_statusText.setPosition(400.0f, 300.0f);
                break;
            }
        }
    }

    void GameState::handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::Type::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Key::Escape)
            {
                // Bascule entre pause et reprise
                setPaused(!m_isPaused);
            }
            else if (event.key.code == sf::Keyboard::Key::R)
            {
                // Réinitialise le jeu
                if (m_isGameOver)
                {
                    m_isGameOver = false;
                    m_score = 0;
                    m_gameTime = 0.0f;
                    m_scoreText.setString("Score: 0");
                    m_statusText.setString("");
                    m_player.setPosition(375.0f, 500.0f);
                }
            }
        }
    }

    void GameState::render(sf::RenderWindow &window)
    {
        // Dessine les obstacles
        for (const auto &obstacle : m_obstacles)
        {
            window.draw(obstacle);
        }

        // Dessine le joueur
        window.draw(m_player);

        // Dessine le score
        window.draw(m_scoreText);

        // Dessine l'overlay de pause si nécessaire
        if (m_isPaused)
        {
            window.draw(m_pauseOverlay);
            window.draw(m_statusText);
        }

        // Dessine l'écran de game over si nécessaire
        if (m_isGameOver)
        {
            window.draw(m_pauseOverlay);
            window.draw(m_statusText);
        }
    }

    void GameState::onActive()
    {
        // Rien à faire pour le moment
    }

    void GameState::onInactive()
    {
        // Met automatiquement le jeu en pause lorsqu'il devient inactif
        m_isPaused = true;
    }

    bool GameState::loadLevel(const std::string &levelName)
    {
        // Ici, nous pourrions charger un niveau à partir d'un fichier
        // Pour l'exemple, nous allons simplement réinitialiser le jeu
        m_currentLevel = levelName;
        m_score = 0;
        m_gameTime = 0.0f;
        m_isGameOver = false;
        m_isPaused = false;
        m_scoreText.setString("Score: 0");
        m_statusText.setString("");

        // On pourrait charger les obstacles depuis un fichier ici
        // Pour l'exemple, on garde les obstacles générés aléatoirement

        return true;
    }

    void GameState::setPaused(bool paused)
    {
        m_isPaused = paused;
        if (m_isPaused)
        {
            m_statusText.setString("Pause");
            sf::FloatRect textBounds = m_statusText.getLocalBounds();
            m_statusText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
            m_statusText.setPosition(400.0f, 300.0f);
        }
        else
        {
            m_statusText.setString("");
        }
    }

    bool GameState::isPaused() const
    {
        return m_isPaused;
    }

} // namespace Orenji