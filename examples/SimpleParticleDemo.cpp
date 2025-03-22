#include <SFML/Graphics.hpp>
#include "Particles/ParticleSystem.hpp"
#include <iostream>

// Exemple simple d'utilisation du système de particules Orenji
// Ce code explique les concepts de base et montre comment créer
// un effet de particules simple avec des commentaires explicatifs

using namespace Orenji;

int main()
{
    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(800, 600), "Démonstration Simple du Système de Particules");
    window.setFramerateLimit(60);

    // 1. Obtenir l'instance du système de particules (singleton)
    ParticleSystem &particleSystem = ParticleSystem::getInstance();

    // 2. Créer un émetteur de particules avec un nom et une capacité
    // Le premier argument est un identifiant unique pour cet émetteur
    // Le second argument est le nombre maximal de particules que cet émetteur peut gérer
    particleSystem.createEmitter("basic", 500);

    // 3. Obtenir une référence à l'émetteur pour le configurer
    ParticleEmitter *emitter = particleSystem.getEmitter("basic");

    // 4. Configurer les propriétés de l'émetteur

    // La forme de l'émetteur détermine d'où les particules sont générées
    emitter->setShape(EmitterShape::Circle);

    // La taille de l'émetteur détermine la zone d'émission
    emitter->setSize(sf::Vector2f(20.f, 20.f));

    // Définir la durée de vie des particules (en secondes)
    emitter->setMinLife(1.0f); // Durée de vie minimale
    emitter->setMaxLife(3.0f); // Durée de vie maximale

    // Définir la taille des particules
    emitter->setMinStartSize(5.f);  // Taille minimale au début
    emitter->setMaxStartSize(10.f); // Taille maximale au début
    emitter->setMinEndSize(0.f);    // Taille minimale à la fin
    emitter->setMaxEndSize(2.f);    // Taille maximale à la fin

    // Définir la vitesse des particules
    emitter->setMinVelocity(20.f); // Vitesse minimale
    emitter->setMaxVelocity(50.f); // Vitesse maximale

    // Définir l'angle d'émission des particules (en degrés)
    emitter->setMinAngle(0.f);   // Angle minimal
    emitter->setMaxAngle(360.f); // Angle maximal (360 = dans toutes les directions)

    // Définir la vitesse de rotation des particules
    emitter->setMinRotationSpeed(-5.f); // Vitesse de rotation minimale
    emitter->setMaxRotationSpeed(5.f);  // Vitesse de rotation maximale

    // Définir les couleurs des particules
    emitter->setStartColor(sf::Color(255, 200, 50, 200)); // Couleur au début
    emitter->setEndColor(sf::Color(255, 50, 0, 0));       // Couleur à la fin (transparente)

    // 5. Ajouter un affecteur (fonction qui influence les particules pendant leur vie)
    // Cette fonction est appelée pour chaque particule à chaque frame
    emitter->addAffector([](Particle &particle, float dt)
                         {
        // Ajoute un effet de gravité
        particle.velocity.y += 40.f * dt;
        
        // Ajoute un léger effet de vent
        particle.velocity.x += 5.f * dt; });

    // 6. Positionner l'émetteur au centre de l'écran
    particleSystem.setEmitterPosition("basic", sf::Vector2f(400.f, 300.f));

    // Texte d'instructions
    sf::Font font;
    if (!font.loadFromFile("resources/fonts/arial.ttf"))
    {
        std::cerr << "Erreur: Impossible de charger la police" << std::endl;
        // Continuons quand même
    }

    sf::Text instructions;
    instructions.setFont(font);
    instructions.setCharacterSize(14);
    instructions.setFillColor(sf::Color::White);
    instructions.setPosition(10.f, 10.f);
    instructions.setString(
        "Démonstration du Système de Particules Orenji\n\n"
        "Cliquez pour déplacer l'émetteur\n"
        "Espace: Émettre une rafale de particules\n"
        "R: Réinitialiser l'émetteur");

    // Horloge pour le calcul du deltaTime
    sf::Clock clock;

    // Boucle principale
    while (window.isOpen())
    {
        // Gestion des événements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    // Émet une rafale de 50 particules
                    particleSystem.emit("basic", 50);
                }
                else if (event.key.code == sf::Keyboard::R)
                {
                    // Réinitialise l'émetteur (supprime toutes les particules existantes)
                    particleSystem.clearEmitter("basic");
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // Déplace l'émetteur à la position de la souris
                    sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                                          static_cast<float>(event.mouseButton.y));
                    particleSystem.setEmitterPosition("basic", mousePos);
                }
            }
        }

        // Calcul du deltaTime
        float deltaTime = clock.restart().asSeconds();

        // 7. Émission continue de particules (faible taux)
        // Émet un nombre de particules proportionnel au temps écoulé
        particleSystem.emit("basic", static_cast<unsigned int>(10.f * deltaTime));

        // 8. Mise à jour du système de particules
        // Cette étape est cruciale - elle met à jour la position, la couleur,
        // et d'autres propriétés de toutes les particules
        particleSystem.update(deltaTime);

        // Rendu
        window.clear(sf::Color(30, 30, 40));

        // 9. Dessiner le système de particules
        window.draw(particleSystem);

        // Dessiner les instructions
        window.draw(instructions);

        window.display();
    }

    // 10. Libérer les ressources du système de particules
    ParticleSystem::release();

    return 0;
}