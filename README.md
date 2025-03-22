# Orenji-CPP Game Engine

Orenji-CPP est un moteur de jeu 2D en C++ conçu pour simplifier le développement de jeux tout en offrant des performances optimales et une architecture flexible.

## Fonctionnalités

- Architecture ECS (Entity-Component-System) flexible
- Moteur physique 2D basé sur Box2D 3.0
- Rendu graphique avec SFML 3.0
- Support des cartes Tiled pour le design de niveaux
- Gestion des ressources (textures, sons, polices)
- Système d'interface utilisateur avec TGUI
- Système d'IA pour les entités de jeu
- Système de caméra avec effets (zoom, shake)

## Prérequis

- Compilateur C++17 compatible (GCC 9+, Clang 10+, MSVC 2019+)
- CMake 3.16+
- SFML 3.0
- Box2D 3.0
- TGUI 1.0+
- Tileson (inclus comme sous-module)
- JSON for Modern C++ (inclus comme sous-module)

## Installation

### Cloner le dépôt

```bash
git clone https://github.com/nogoezen/orenji-cpp.git
cd orenji-cpp
git submodule update --init --recursive
```

### Compilation

#### Windows

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

Ou utilisez le script batch fourni :

```bash
.\build.bat
```

#### Linux/macOS

```bash
mkdir build
cd build
cmake ..
make
```

## Utilisation

### Création d'un projet simple

```cpp
#include <Engine.hpp>
#include <Scenes/MainMenuScene.hpp>

int main()
{
    // Initialiser le moteur
    Core::Engine engine;
    
    // Créer et ajouter une scène
    auto mainMenu = std::make_shared<Scenes::MainMenuScene>(engine);
    engine.setScene(mainMenu);
    
    // Lancer le moteur
    engine.run();
    
    return 0;
}
```

### Création d'une entité avec des composants physiques

```cpp
#include <Engine.hpp>
#include <ECS/Entity.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/SpriteComponent.hpp>
#include <Components/PhysicsComponent.hpp>

// Dans votre scène
void MyGameScene::initialize()
{
    // Créer une entité joueur
    auto player = m_registry.create();
    
    // Ajouter un composant de transformation
    auto& transform = m_registry.emplace<Components::TransformComponent>(player);
    transform.position = {100.0f, 100.0f};
    transform.rotation = 0.0f;
    
    // Ajouter un composant de sprite
    auto& sprite = m_registry.emplace<Components::SpriteComponent>(player);
    sprite.setTexture(m_engine.getResourceManager().getTexture("player"));
    
    // Ajouter un composant physique
    auto& physics = m_registry.emplace<Components::PhysicsComponent>(player);
    physics.bodyType = Physics::BodyType::Dynamic;
    physics.init(m_physicsSystem.getBox2DWrapper(), {0.5f, 1.0f});
}
```

### Chargement d'une carte Tiled

```cpp
#include <Resources/TiledMapLoader.hpp>

// Dans votre scène
void MyGameScene::loadMap(const std::string& mapPath)
{
    Resources::TiledMapLoader mapLoader(m_engine.getResourceManager());
    m_map = mapLoader.loadMap(mapPath);
    
    // Créer des entités à partir des objets de la carte
    auto objects = m_map->getLayer("Objects")->getObjects();
    for (const auto& object : objects)
    {
        if (object.getType() == "Spawn")
        {
            m_playerSpawnPoint = {object.getPosition().x, object.getPosition().y};
        }
        else if (object.getType() == "Enemy")
        {
            spawnEnemy(object.getPosition().x, object.getPosition().y);
        }
    }
}
```

## Architecture

Le moteur Orenji-CPP est organisé comme suit :

```
orenji-cpp/
├── include/              # Headers publics
│   ├── Components/       # Composants ECS
│   ├── Core/             # Classes principales du moteur
│   ├── ECS/              # Système Entity-Component-System
│   ├── Physics/          # Système physique et wrapper Box2D
│   ├── Rendering/        # Classes de rendu
│   ├── Resources/        # Gestion des ressources
│   ├── Scenes/           # Base de scènes et scènes communes
│   └── UI/               # Système d'interface utilisateur
├── src/                  # Implémentations
├── examples/             # Exemples d'utilisation
├── docs/                 # Documentation
├── lib/                  # Bibliothèques externes
├── assets/               # Ressources pour les exemples
├── tests/                # Tests unitaires et d'intégration
├── CMakeLists.txt        # Configuration CMake principale
└── README.md             # Ce fichier
```

## Documentation

Pour une documentation complète de l'API, consultez les fichiers en-tête dans le répertoire `include/`. Des exemples d'utilisation sont disponibles dans le répertoire `examples/`.

Un guide de migration détaillé pour SFML 3.0 et Box2D 3.0 est disponible dans `docs/Migration.md`.

## Exemples

Le moteur inclut plusieurs exemples pour démontrer ses fonctionnalités :

- `examples/HelloWorld` : Simple fenêtre avec un sprite
- `examples/GameMapExample` : Chargement et affichage d'une carte Tiled
- `examples/PhysicsExample` : Démonstration du système physique
- `examples/ECSExample` : Utilisation du système ECS
- `examples/UIExample` : Création d'interfaces utilisateur

Pour compiler et exécuter les exemples :

```bash
cd build/examples
./GameMapExample
```

## Contribution

Les contributions sont les bienvenues ! Pour contribuer :

1. Forkez le dépôt
2. Créez une branche pour votre fonctionnalité (`git checkout -b feature/ma-fonctionnalite`)
3. Committez vos modifications (`git commit -am 'Ajout de ma fonctionnalité'`)
4. Poussez vers la branche (`git push origin feature/ma-fonctionnalite`)
5. Créez une Pull Request

## Licence

Ce projet est distribué sous la licence MIT. Voir le fichier `LICENSE` pour plus d'informations. 