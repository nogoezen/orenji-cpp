#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "../include/Particles/ParticleSystem.hpp"
#include <Thor/Particles/Affectors.hpp>
#include <Thor/Animations/ColorAnimation.hpp>

int main()
{
    // Créer une fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Orenji Particle System Example", sf::Style::Default);
    window.setFramerateLimit(60);

    // Créer l'instance du système de particules
    Orenji::ParticleSystem &particleSystem = Orenji::ParticleSystem::getInstance();

    // Définir les positions pour les différents systèmes de particules
    sf::Vector2f explosionPos(200.f, 200.f);
    sf::Vector2f firePos(400.f, 300.f);
    sf::Vector2f smokePos(600.f, 400.f);
    sf::Vector2f sparklePos(500.f, 200.f);
    sf::Vector2f customPos(300.f, 400.f);

    // Créer les différents systèmes de particules
    particleSystem.createSystem("explosion", Orenji::ParticleSystem::ParticleType::EXPLOSION,
                                explosionPos, sf::Color::Yellow);

    particleSystem.createSystem("fire", Orenji::ParticleSystem::ParticleType::FIRE,
                                firePos, sf::Color::Red);

    particleSystem.createSystem("smoke", Orenji::ParticleSystem::ParticleType::SMOKE,
                                smokePos, sf::Color(100, 100, 100, 150));

    particleSystem.createSystem("sparkle", Orenji::ParticleSystem::ParticleType::SPARKLE,
                                sparklePos, sf::Color::White);

    // Créer un système personnalisé
    particleSystem.createCustomSystem("custom", customPos, 20.f, sf::seconds(2.f));

    // Configurer le système personnalisé
    particleSystem.addAffector("custom",
                               [](thor::Particle &particle, sf::Time dt)
                               {
                                   particle.velocity += sf::Vector2f(20.f, 0.f) * dt.asSeconds();
                               });

    // Configurer les couleurs du système personnalisé
    // Note: On utilise un animation affector avec color animation
    thor::ColorAnimation colorAnimation;
    colorAnimation.addColor(0.f, sf::Color::Blue);
    colorAnimation.addColor(1.f, sf::Color::Cyan);

    particleSystem.addAffector("custom",
                               [colorAnimation](thor::Particle &particle, sf::Time)
                               {
                                   float progress = 1.f - (particle.remainingLifetime / particle.totalLifetime);
                                   particle.color = colorAnimation.getColor(progress);
                               });

    // Créer une texture pour le système personnalisé (un cercle blanc)
    sf::Texture circleTexture;
    circleTexture.create(32, 32);
    {
        sf::CircleShape circle(16.f);
        circle.setFillColor(sf::Color::White);
        circle.setPosition(0.f, 0.f);

        sf::RenderTexture renderTexture;
        renderTexture.create(32, 32);
        renderTexture.clear(sf::Color::Transparent);
        renderTexture.draw(circle);
        renderTexture.display();

        circleTexture = renderTexture.getTexture();
    }

    // Appliquer la texture au système personnalisé
    particleSystem.setTexture("custom", circleTexture);

    // Charger une police pour afficher du texte
    sf::Font font;
    if (!font.loadFromFile("resources/fonts/VeniceClassic.ttf"))
    {
        std::cerr << "Impossible de charger la police!" << std::endl;
        return 1;
    }

    sf::Text instructionsText;
    instructionsText.setFont(font);
    instructionsText.setCharacterSize(14);
    instructionsText.setFillColor(sf::Color::White);
    instructionsText.setPosition(10.f, 10.f);
    instructionsText.setString(
        "1-5: Toggle particle systems\n"
        "Q: Emit explosion\n"
        "W: Move fire system\n"
        "E: Stop smoke system\n"
        "R: Restart smoke system\n"
        "Space: Emit custom burst\n"
        "Escape: Quit");

    // Variables pour suivre l'état des systèmes
    bool systems[5] = {true, true, true, true, true}; // État actif des systèmes

    // Horloge pour le temps delta
    sf::Clock clock;

    // Boucle principale
    while (window.isOpen())
    {
        // Calculer le temps delta
        float deltaTime = clock.restart().asSeconds();

        // Gérer les événements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    window.close();
                    break;

                // Activer/désactiver les systèmes
                case sf::Keyboard::Num1:
                    systems[0] = !systems[0];
                    particleSystem.setActive("explosion", systems[0]);
                    break;
                case sf::Keyboard::Num2:
                    systems[1] = !systems[1];
                    particleSystem.setActive("fire", systems[1]);
                    break;
                case sf::Keyboard::Num3:
                    systems[2] = !systems[2];
                    particleSystem.setActive("smoke", systems[2]);
                    break;
                case sf::Keyboard::Num4:
                    systems[3] = !systems[3];
                    particleSystem.setActive("sparkle", systems[3]);
                    break;
                case sf::Keyboard::Num5:
                    systems[4] = !systems[4];
                    particleSystem.setActive("custom", systems[4]);
                    break;

                // Actions spéciales
                case sf::Keyboard::Q:
                    // Créer une nouvelle explosion
                    particleSystem.emit("explosion", 100);
                    break;

                case sf::Keyboard::W:
                    // Déplacer le feu
                    firePos.x = (firePos.x < 600.f) ? firePos.x + 50.f : 100.f;
                    particleSystem.setPosition("fire", firePos);
                    break;

                case sf::Keyboard::E:
                    // Arrêter la fumée
                    particleSystem.stop("smoke");
                    break;

                case sf::Keyboard::R:
                    // Redémarrer la fumée
                    particleSystem.setActive("smoke", true);
                    break;

                case sf::Keyboard::Space:
                    // Émettre un burst de particules personnalisées
                    particleSystem.emit("custom", 50);
                    break;

                default:
                    break;
                }
            }
        }

        // Mettre à jour le système de particules
        particleSystem.update(deltaTime);

        // Effet de mouvement circulaire pour les étincelles
        static float angle = 0.f;
        angle += deltaTime * 0.5f;
        float radius = 100.f;
        sf::Vector2f newSparklePos(
            500.f + std::cos(angle) * radius,
            200.f + std::sin(angle) * radius);
        particleSystem.setPosition("sparkle", newSparklePos);

        // Nettoyer et dessiner
        window.clear(sf::Color(30, 30, 30));

        // Dessiner le système de particules
        window.draw(particleSystem);

        // Dessiner le texte d'instructions
        window.draw(instructionsText);

        // Afficher la position des systèmes (pour référence visuelle)
        sf::CircleShape positionMarker(5.f);
        positionMarker.setFillColor(sf::Color::Red);
        positionMarker.setOrigin(5.f, 5.f);

        positionMarker.setPosition(explosionPos);
        window.draw(positionMarker);

        positionMarker.setPosition(firePos);
        window.draw(positionMarker);

        positionMarker.setPosition(smokePos);
        window.draw(positionMarker);

        positionMarker.setPosition(newSparklePos);
        window.draw(positionMarker);

        positionMarker.setPosition(customPos);
        window.draw(positionMarker);

        window.display();
    }

    return 0;
}