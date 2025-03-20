# Orenji-CPP

Orenji est un petit moteur de jeu basé sur SFML, offrant des fonctionnalités supplémentaires pour faciliter le développement de jeux 2D.

## Modules disponibles

- **Core**: Fonctionnalités de base du moteur (gestion des ressources, etc.)
- **TiledMap**: Chargement et affichage de cartes créées avec l'éditeur Tiled
- **Physics**: Système de physique basé sur Box2D (version 2.4.x)
- **Animation**: Système d'animation utilisant la bibliothèque Thor
- **Particles**: Système de particules basé sur Thor

## Dépendances

- [SFML](https://www.sfml-dev.org/) 2.5+
- [Box2D](https://box2d.org/) 2.4.x
- [TMXLite](https://github.com/fallahn/tmxlite)
- [Thor](https://github.com/Bromeon/Thor)

## Installation

1. Clonez ce dépôt
2. Assurez-vous d'avoir installé les dépendances
3. Utilisez CMake pour générer le projet
4. Compilez avec votre compilateur préféré

```bash
git clone https://github.com/yourusername/orenji-cpp.git
cd orenji-cpp
mkdir build && cd build
cmake ..
make
```

## Module TiledMap

Le module TiledMap permet de charger et d'afficher des cartes créées avec l'éditeur [Tiled](https://www.mapeditor.org/).

### Utilisation

```cpp
#include <Orenji/TiledMap/TiledMapLoader.hpp>

// Définir le répertoire racine des cartes
Orenji::TiledMapLoader::setRootDirectory("resources/maps/");

// Charger une carte
std::shared_ptr<Orenji::TiledMap> map = Orenji::TiledMapLoader::load("level1.tmx");

// Vérifier si le chargement a réussi
if (!map) {
    // Gestion d'erreur
    return;
}

// Accéder aux propriétés de la carte
int width = map->getWidth();
int height = map->getHeight();
int tileWidth = map->getTileWidth();
int tileHeight = map->getTileHeight();

// Accéder aux couches
const auto& tileLayers = map->getTileLayers();
const auto& objectLayers = map->getObjectLayers();
const auto& imageLayers = map->getImageLayers();

// Décharger une carte spécifique
Orenji::TiledMapLoader::unload("level1.tmx");

// Décharger toutes les cartes
Orenji::TiledMapLoader::unloadAll();
```

## Module Physics

Le module Physics fournit une intégration avec Box2D pour la simulation physique 2D.

### Utilisation

```cpp
#include <Orenji/Physics/PhysicsWorld.hpp>
#include <Orenji/Physics/PhysicsComponent.hpp>

// Créer un monde physique
Orenji::PhysicsWorld::getInstance().initialize();

// Créer un composant physique pour une entité
auto physicsComponent = new Orenji::PhysicsComponent(&entity);

// Configurer un corps dynamique
physicsComponent->setBodyType(Orenji::PhysicsBodyType::DYNAMIC);
physicsComponent->setPosition(sf::Vector2f(100.f, 100.f));

// Ajouter une fixture rectangulaire
physicsComponent->addRectangleFixture(sf::Vector2f(50.f, 50.f), 1.0f, 0.3f, 0.1f);

// Appliquer des forces
physicsComponent->applyForce(sf::Vector2f(10.f, 0.f));
physicsComponent->applyImpulse(sf::Vector2f(0.f, -5.f));

// Mettre à jour le monde physique
Orenji::PhysicsWorld::getInstance().update(1.0f/60.0f);
```

## Module Animation

Le module Animation utilise la bibliothèque Thor pour gérer les animations de sprites.

### Utilisation

```cpp
#include <Orenji/Animation/AnimationManager.hpp>

// Obtenir l'instance du gestionnaire d'animations
auto& animManager = Orenji::AnimationManager::getInstance();

// Créer une animation à partir d'une sprite sheet
sf::Texture texture;
texture.loadFromFile("spritesheet.png");

// Créer une animation en boucle avec 8 frames, chaque frame durant 0.1 secondes
animManager.createAnimation("player_run", texture, sf::Vector2i(32, 32), 8, 0.1f, 
                           Orenji::AnimationManager::AnimationType::LOOP, 0);

// Ajouter une frame individuelle
animManager.addFrame("player_run", sf::IntRect(0, 32, 32, 32));

// Jouer l'animation sur un sprite
sf::Sprite playerSprite;
animManager.playAnimation("player_run", playerSprite);

// Mettre à jour les animations (dans la boucle de jeu)
float deltaTime = clock.restart().asSeconds();
animManager.update(deltaTime);

// Arrêter une animation
animManager.stopAnimation("player_run");

// Vérifier si une animation est en cours
bool isPlaying = animManager.isPlaying("player_run");
```

## Module Particles

Le module Particles utilise Thor pour créer et gérer des systèmes de particules.

### Utilisation

```cpp
#include <Orenji/Particles/ParticleSystem.hpp>

// Obtenir l'instance du système de particules
auto& particles = Orenji::ParticleSystem::getInstance();

// Créer différents types de systèmes prédéfinis
particles.createSystem("explosion", Orenji::ParticleSystem::ParticleType::EXPLOSION, 
                      sf::Vector2f(100.f, 100.f), sf::Color::Yellow);

particles.createSystem("fire", Orenji::ParticleSystem::ParticleType::FIRE, 
                      sf::Vector2f(200.f, 200.f), sf::Color::Red);

particles.createSystem("smoke", Orenji::ParticleSystem::ParticleType::SMOKE, 
                      sf::Vector2f(300.f, 100.f), sf::Color(100, 100, 100, 150));

// Créer un système personnalisé
particles.createCustomSystem("custom", sf::Vector2f(400.f, 300.f), 30.f, sf::seconds(2.f));

// Ajouter des effets (affectors)
particles.addAffector("custom", thor::ForceAffector(sf::Vector2f(0.f, 10.f)));

// Définir une texture
sf::Texture particleTexture;
particleTexture.loadFromFile("particle.png");
particles.setTexture("custom", particleTexture);

// Émettre des particules
particles.emit("explosion", 100);

// Mettre à jour les systèmes (dans la boucle de jeu)
float deltaTime = clock.restart().asSeconds();
particles.update(deltaTime);

// Dessiner les particules
window.draw(particles);

// Arrêter un système
particles.stop("fire");

// Supprimer un système
particles.removeSystem("smoke");
```

## Exemples

Des exemples d'utilisation sont disponibles dans le dossier `examples/` :

- TiledMapExample : Démontre le chargement et l'affichage d'une carte Tiled
- Box2DExample : Montre l'utilisation du module Physics
- ParticleExample : Illustre les différents types de systèmes de particules

Pour compiler et exécuter les exemples :

```bash
cd build
cmake .. -DBUILD_EXAMPLES=ON
make
./examples/TiledMapExample
```

## Licence

Ce projet est sous licence MIT. Voir le fichier LICENSE pour plus de détails.
