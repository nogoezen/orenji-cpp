#include <SFML/Graphics.hpp>
#include "Particles/ParticleSystem.hpp"
#include "Particles/ParticleComponent.hpp"
#include "Utils/MathUtils.hpp"
#include <iostream>

// Exemple d'utilisation du système de particules
// Ce code démontre comment créer différents effets de particules
// Utilisez les touches 1-5 pour changer d'effet:
// 1: Feu
// 2: Fumée
// 3: Explosion
// 4: Fontaine
// 5: Étincelles

using namespace Orenji;

// Fonction qui crée un effet de feu
void setupFireEffect(ParticleEmitter *emitter)
{
    emitter->setShape(EmitterShape::Rectangle);
    emitter->setSize(sf::Vector2f(30.f, 5.f));
    emitter->setMinLife(0.5f);
    emitter->setMaxLife(1.5f);
    emitter->setMinStartSize(30.f);
    emitter->setMaxStartSize(40.f);
    emitter->setMinEndSize(5.f);
    emitter->setMaxEndSize(10.f);
    emitter->setMinVelocity(20.f);
    emitter->setMaxVelocity(50.f);
    emitter->setMinAngle(260.f);
    emitter->setMaxAngle(280.f);
    emitter->setMinRotationSpeed(-10.f);
    emitter->setMaxRotationSpeed(10.f);
    emitter->setStartColor(sf::Color(255, 180, 0, 200));
    emitter->setEndColor(sf::Color(200, 0, 0, 0));

    // Ajoute un affecteur pour simuler la chaleur (déplacement vers le haut et oscillation)
    emitter->addAffector([](Particle &particle, float dt)
                         {
        float oscillation = std::sin(particle.life * 10.f) * 0.2f;
        particle.velocity.x += oscillation; });
}

// Fonction qui crée un effet de fumée
void setupSmokeEffect(ParticleEmitter *emitter)
{
    emitter->setShape(EmitterShape::Circle);
    emitter->setSize(sf::Vector2f(10.f, 10.f));
    emitter->setMinLife(2.0f);
    emitter->setMaxLife(4.0f);
    emitter->setMinStartSize(10.f);
    emitter->setMaxStartSize(20.f);
    emitter->setMinEndSize(50.f);
    emitter->setMaxEndSize(70.f);
    emitter->setMinVelocity(10.f);
    emitter->setMaxVelocity(20.f);
    emitter->setMinAngle(260.f);
    emitter->setMaxAngle(280.f);
    emitter->setMinRotationSpeed(-5.f);
    emitter->setMaxRotationSpeed(5.f);
    emitter->setStartColor(sf::Color(200, 200, 200, 150));
    emitter->setEndColor(sf::Color(150, 150, 150, 0));

    // Ajoute un affecteur pour simuler le vent
    emitter->addAffector([](Particle &particle, float dt)
                         {
        float windFactor = particle.life / particle.totalLife;
        particle.velocity.x += 5.f * windFactor * dt; });
}

// Fonction qui crée un effet d'explosion
void setupExplosionEffect(ParticleEmitter *emitter)
{
    emitter->setShape(EmitterShape::Point);
    emitter->setMinLife(0.5f);
    emitter->setMaxLife(1.0f);
    emitter->setMinStartSize(5.f);
    emitter->setMaxStartSize(10.f);
    emitter->setMinEndSize(1.f);
    emitter->setMaxEndSize(2.f);
    emitter->setMinVelocity(100.f);
    emitter->setMaxVelocity(200.f);
    emitter->setMinAngle(0.f);
    emitter->setMaxAngle(360.f);
    emitter->setMinRotationSpeed(-90.f);
    emitter->setMaxRotationSpeed(90.f);
    emitter->setStartColor(sf::Color(255, 200, 0, 255));
    emitter->setEndColor(sf::Color(255, 0, 0, 0));

    // Ajoute un affecteur pour simuler la gravité et le ralentissement
    emitter->addAffector([](Particle &particle, float dt)
                         {
        particle.velocity.y += 50.f * dt; // Gravité
        float slowdown = 0.98f;
        particle.velocity *= slowdown; });
}

// Fonction qui crée un effet de fontaine
void setupFountainEffect(ParticleEmitter *emitter)
{
    emitter->setShape(EmitterShape::Point);
    emitter->setMinLife(1.0f);
    emitter->setMaxLife(2.0f);
    emitter->setMinStartSize(3.f);
    emitter->setMaxStartSize(5.f);
    emitter->setMinEndSize(1.f);
    emitter->setMaxEndSize(2.f);
    emitter->setMinVelocity(100.f);
    emitter->setMaxVelocity(150.f);
    emitter->setMinAngle(260.f);
    emitter->setMaxAngle(280.f);
    emitter->setMinRotationSpeed(0.f);
    emitter->setMaxRotationSpeed(0.f);
    emitter->setStartColor(sf::Color(50, 150, 255, 200));
    emitter->setEndColor(sf::Color(200, 200, 255, 0));

    // Ajoute un affecteur pour simuler la gravité
    emitter->addAffector([](Particle &particle, float dt)
                         {
                             particle.velocity.y += 200.f * dt; // Gravité plus forte
                         });
}

// Fonction qui crée un effet d'étincelles
void setupSparkEffect(ParticleEmitter *emitter)
{
    emitter->setShape(EmitterShape::Point);
    emitter->setMinLife(0.2f);
    emitter->setMaxLife(0.5f);
    emitter->setMinStartSize(2.f);
    emitter->setMaxStartSize(3.f);
    emitter->setMinEndSize(0.5f);
    emitter->setMaxEndSize(1.f);
    emitter->setMinVelocity(50.f);
    emitter->setMaxVelocity(100.f);
    emitter->setMinAngle(0.f);
    emitter->setMaxAngle(360.f);
    emitter->setMinRotationSpeed(0.f);
    emitter->setMaxRotationSpeed(0.f);
    emitter->setStartColor(sf::Color(255, 255, 50, 255));
    emitter->setEndColor(sf::Color(255, 50, 0, 0));

    // Ajoute un affecteur pour simuler la gravité et le scintillement
    emitter->addAffector([](Particle &particle, float dt)
                         {
        particle.velocity.y += 20.f * dt; // Légère gravité
        
        // Scintillement
        float factor = Utils::MathUtils::randFloat(0.8f, 1.0f);
        particle.color.r = static_cast<uint8_t>(particle.color.r * factor);
        particle.color.g = static_cast<uint8_t>(particle.color.g * factor);
        particle.color.b = static_cast<uint8_t>(particle.color.b * factor); });
}

int main()
{
    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(800, 600), "Exemples d'Effets de Particules");
    window.setFramerateLimit(60);

    // Création du système de particules et des émetteurs
    ParticleSystem &particleSystem = ParticleSystem::getInstance();

    // Création des émetteurs pour chaque type d'effet
    particleSystem.createEmitter("fire", 1000);
    particleSystem.createEmitter("smoke", 500);
    particleSystem.createEmitter("explosion", 200);
    particleSystem.createEmitter("fountain", 1000);
    particleSystem.createEmitter("spark", 500);

    // Positionnement des émetteurs
    particleSystem.setEmitterPosition("fire", sf::Vector2f(200.f, 500.f));
    particleSystem.setEmitterPosition("smoke", sf::Vector2f(400.f, 500.f));
    particleSystem.setEmitterPosition("explosion", sf::Vector2f(600.f, 400.f));
    particleSystem.setEmitterPosition("fountain", sf::Vector2f(600.f, 500.f));
    particleSystem.setEmitterPosition("spark", sf::Vector2f(300.f, 300.f));

    // Configuration des effets
    setupFireEffect(particleSystem.getEmitter("fire"));
    setupSmokeEffect(particleSystem.getEmitter("smoke"));
    setupExplosionEffect(particleSystem.getEmitter("explosion"));
    setupFountainEffect(particleSystem.getEmitter("fountain"));
    setupSparkEffect(particleSystem.getEmitter("spark"));

    // Variables pour l'effet actif et le déclenchement d'explosion
    int activeEffect = 1; // 1=fire, 2=smoke, 3=explosion, 4=fountain, 5=spark
    bool explosionActive = false;
    sf::Clock explosionClock;

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
        "Appuyez sur les touches 1-5 pour changer d'effet:\n"
        "1: Feu\n"
        "2: Fumée\n"
        "3: Explosion (clic pour déclencher)\n"
        "4: Fontaine\n"
        "5: Étincelles");

    sf::Text activeText;
    activeText.setFont(font);
    activeText.setCharacterSize(18);
    activeText.setFillColor(sf::Color::Yellow);
    activeText.setPosition(10.f, 120.f);
    activeText.setString("Effet actif: Feu");

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
                // Changement d'effet avec les touches 1-5
                if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num5)
                {
                    activeEffect = event.key.code - sf::Keyboard::Num1 + 1;

                    switch (activeEffect)
                    {
                    case 1:
                        activeText.setString("Effet actif: Feu");
                        break;
                    case 2:
                        activeText.setString("Effet actif: Fumée");
                        break;
                    case 3:
                        activeText.setString("Effet actif: Explosion (clic pour déclencher)");
                        break;
                    case 4:
                        activeText.setString("Effet actif: Fontaine");
                        break;
                    case 5:
                        activeText.setString("Effet actif: Étincelles");
                        break;
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (activeEffect == 3) // Explosion
                    {
                        // Position de la souris
                        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                                              static_cast<float>(event.mouseButton.y));

                        // Déplace l'émetteur d'explosion à la position de la souris
                        particleSystem.setEmitterPosition("explosion", mousePos);

                        // Active l'explosion
                        explosionActive = true;
                        explosionClock.restart();

                        // Émet une rafale de particules
                        particleSystem.emit("explosion", 100);
                    }
                }
            }
        }

        // Calcul du deltaTime
        float deltaTime = clock.restart().asSeconds();

        // Émission continue pour les effets non explosifs
        if (activeEffect == 1) // Feu
            particleSystem.emit("fire", static_cast<unsigned int>(30.f * deltaTime));
        else if (activeEffect == 2) // Fumée
            particleSystem.emit("smoke", static_cast<unsigned int>(10.f * deltaTime));
        else if (activeEffect == 4) // Fontaine
            particleSystem.emit("fountain", static_cast<unsigned int>(40.f * deltaTime));
        else if (activeEffect == 5) // Étincelles
            particleSystem.emit("spark", static_cast<unsigned int>(20.f * deltaTime));

        // Arrête l'explosion après un certain temps
        if (explosionActive && explosionClock.getElapsedTime().asSeconds() > 0.2f)
        {
            explosionActive = false;
        }

        // Mise à jour du système de particules
        particleSystem.update(deltaTime);

        // Rendu
        window.clear(sf::Color(20, 20, 30));

        // Dessine le système de particules
        window.draw(particleSystem);

        // Dessine les instructions
        window.draw(instructions);
        window.draw(activeText);

        window.display();
    }

    // Libère les ressources du système de particules
    ParticleSystem::release();

    return 0;
}