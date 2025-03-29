#include <SFML/Graphics.hpp>
#include "Graphics/ParticleSystem.hpp"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

// Démonstration du système de particules
int main()
{
    // Créer la fenêtre
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1280, 720)), "Orenji Particle System Demo");
    window.setFramerateLimit(60);

    // Initialiser les systèmes de particules
    std::vector<std::shared_ptr<Orenji::Graphics::ParticleSystem>> particleSystems;

    // Créer le système de feu
    auto fireSystem = std::make_shared<Orenji::Graphics::ParticleSystem>();
    if (!fireSystem->loadFromFile("resources/effects/fire.txt"))
    {
        std::cerr << "Failed to load fire effect" << std::endl;
        return 1;
    }
    fireSystem->setTexture("resources/textures/particle.png");
    fireSystem->setEmitterPosition(sf::Vector2f(300, 600));
    particleSystems.push_back(fireSystem);

    // Créer le système de fumée
    auto smokeSystem = std::make_shared<Orenji::Graphics::ParticleSystem>();
    if (!smokeSystem->loadFromFile("resources/effects/smoke.txt"))
    {
        std::cerr << "Failed to load smoke effect" << std::endl;
        return 1;
    }
    smokeSystem->setTexture("resources/textures/particle.png");
    smokeSystem->setEmitterPosition(sf::Vector2f(300, 600));
    particleSystems.push_back(smokeSystem);

    // Créer le système d'explosion (désactivé par défaut)
    auto explosionSystem = std::make_shared<Orenji::Graphics::ParticleSystem>();
    if (!explosionSystem->loadFromFile("resources/effects/explosion.txt"))
    {
        std::cerr << "Failed to load explosion effect" << std::endl;
        return 1;
    }
    explosionSystem->setTexture("resources/textures/particle.png");
    explosionSystem->setEmitterPosition(sf::Vector2f(600, 400));
    explosionSystem->enableEmitter(false); // Désactivé au départ
    particleSystems.push_back(explosionSystem);

    // Créer un système de particules personnalisé (neige)
    auto snowSystem = std::make_shared<Orenji::Graphics::ParticleSystem>();
    snowSystem->setTexture("resources/textures/particle.png");
    snowSystem->setEffect(Orenji::Graphics::ParticleEffect::SNOW);
    snowSystem->setEmissionRate(50.0f);
    snowSystem->setEmitterPosition(sf::Vector2f(640, 0));
    snowSystem->setRectangularEmitter(sf::Vector2f(0, 0), sf::Vector2f(1280, 1));
    snowSystem->setParticleLifetime(5.0f, 10.0f);
    snowSystem->setParticleVelocity(sf::Vector2f(-20, 30), sf::Vector2f(20, 80));
    snowSystem->setParticleSize(2.0f, 7.0f);
    snowSystem->setParticleColors(sf::Color(255, 255, 255, 200), sf::Color(200, 200, 255, 50));
    particleSystems.push_back(snowSystem);

    // Police pour le texte
    sf::Font font;
    if (!font.openFromFile("resources/fonts/arial.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
        // Continuer quand même, le texte ne sera pas affiché
    }

    // Créer le texte d'instructions
    sf::Text instructionsText(font, "Contrôles:\n"
                                    "Espace: Déclencher une explosion\n"
                                    "Clic gauche: Déplacer les émetteurs de feu et de fumée\n"
                                    "F: Activer/désactiver le feu\n"
                                    "S: Activer/désactiver la fumée\n"
                                    "N: Activer/désactiver la neige\n"
                                    "Échap: Quitter",
                              18);
    instructionsText.setPosition(sf::Vector2f(10, 10));
    instructionsText.setFillColor(sf::Color::White);

    // Horloge pour deltaTime
    sf::Clock clock;

    // Boucle principale
    while (window.isOpen())
    {
        // Calculer le temps delta
        float deltaTime = clock.restart().asSeconds();

        // Gestion des événements
        auto event = window.pollEvent();
        while (event.has_value())
        {
            auto &e = event.value();

            if (std::holds_alternative<sf::Event::Closed>(e))
            {
                window.close();
            }
            else if (std::holds_alternative<sf::Event::KeyPressed>(e))
            {
                auto keyEvent = std::get<sf::Event::KeyPressed>(e);

                // Quitter avec Échap
                if (keyEvent.scancode == sf::Keyboard::Scan::Escape)
                    window.close();
                // Déclencher une explosion avec Espace
                else if (keyEvent.scancode == sf::Keyboard::Scan::Space)
                {
                    explosionSystem->clear();
                    explosionSystem->enableEmitter(true);
                    explosionSystem->emit(200); // Émettre beaucoup de particules d'un coup

                    // Désactiver l'émetteur après un bref délai
                    std::thread([&explosionSystem]()
                                {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        explosionSystem->enableEmitter(false); })
                        .detach();
                }
                // Activer/désactiver le feu
                else if (keyEvent.scancode == sf::Keyboard::Scan::F)
                    fireSystem->enableEmitter(!fireSystem->isEmitterEnabled());
                // Activer/désactiver la fumée
                else if (keyEvent.scancode == sf::Keyboard::Scan::S)
                    smokeSystem->enableEmitter(!smokeSystem->isEmitterEnabled());
                // Activer/désactiver la neige
                else if (keyEvent.scancode == sf::Keyboard::Scan::N)
                    snowSystem->enableEmitter(!snowSystem->isEmitterEnabled());
            }
            else if (std::holds_alternative<sf::Event::MouseButtonPressed>(e))
            {
                auto mouseEvent = std::get<sf::Event::MouseButtonPressed>(e);

                if (mouseEvent.button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos(mouseEvent.pos.x, mouseEvent.pos.y);
                    fireSystem->setEmitterPosition(mousePos);
                    smokeSystem->setEmitterPosition(mousePos);
                }
                else if (mouseEvent.button == sf::Mouse::Button::Right)
                {
                    sf::Vector2f mousePos(mouseEvent.pos.x, mouseEvent.pos.y);
                    explosionSystem->setEmitterPosition(mousePos);
                    explosionSystem->clear();
                    explosionSystem->enableEmitter(true);
                    explosionSystem->emit(200);

                    // Désactiver l'émetteur après un bref délai
                    std::thread([&explosionSystem]()
                                {
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        explosionSystem->enableEmitter(false); })
                        .detach();
                }
            }

            event = window.pollEvent();
        }

        // Mise à jour des systèmes de particules
        for (auto &system : particleSystems)
        {
            system->update(deltaTime);
        }

        // Dessiner
        window.clear(sf::Color(30, 30, 40));

        // Dessiner les systèmes de particules
        for (auto &system : particleSystems)
        {
            window.draw(*system);
        }

        // Dessiner les instructions
        window.draw(instructionsText);

        window.display();
    }

    return 0;
}