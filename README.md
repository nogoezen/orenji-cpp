# Orenji-CPP Game Engine

Orenji-CPP est un moteur de jeu 2D moderne en C++ conçu pour une architecture ECS (Entity-Component-System) et utilisant des technologies comme SFML 3.0 et Box2D 3.0 pour le rendu et la physique.

## Caractéristiques

- **Architecture ECS** flexible pour une meilleure organisation du code
- **Physique 2D** avec Box2D 3.0 
- **Rendu graphique** basé sur SFML 3.0
- **Support des cartes Tiled** pour la conception de niveaux
- **Gestion des ressources** intégrée (textures, sons, polices)
- **Système d'interface utilisateur** avec TGUI
- **Système d'intelligence artificielle** pour les entités de jeu
- **Système de caméra** avec effets (zoom, secousse, panoramique)

## Architecture

Le moteur est organisé autour des modules suivants :

- **Core** : Moteur principal, gestionnaire d'entités, scènes, composants
- **Physics** : Système de physique utilisant Box2D
- **Graphics** : Système de rendu
- **Resources** : Chargement et gestion des ressources
- **UI** : Interface utilisateur
- **AI** : Comportements et intelligence artificielle
- **Gameplay** : Classes spécifiques au gameplay comme les cartes

## Dépendances

Le projet utilise les bibliothèques externes suivantes :

- [SFML 3.0](https://github.com/SFML/SFML) - Librairie multimédia
- [Box2D 3.0](https://box2d.org/) - Moteur de physique 2D
- [TGUI](https://tgui.eu/) - UI pour SFML
- [Tileson](https://github.com/SSBMTonberry/tileson) - Parser pour les cartes Tiled

## Compilation

### Prérequis

- CMake 3.12 ou supérieur
- Compilateur C++17 (GCC, Clang ou MSVC)
- Git pour cloner le projet

### Instructions

1. Clonez le dépôt :
   ```bash
   git clone https://github.com/yourusername/orenji-cpp.git
   cd orenji-cpp
   ```

2. Exécutez le script de compilation :
   - Sur Windows :
     ```bash
     .\build.bat
     ```
   - Sur Linux/Mac :
     ```bash
     ./build.sh
     ```

Le script configure CMake, compile le projet, et copie les ressources nécessaires dans le dossier de build.

## Utilisation

### Création d'un jeu simple

```cpp
#include <SFML/Graphics.hpp>
#include "include/Engine.hpp"
#include "include/Gameplay/GameMap.hpp"

// Exemple simple d'utilisation du moteur
int main()
{
    // Initialiser le moteur
    Core::Engine engine("Mon Jeu", 800, 600);
    engine.initialize();
    
    // Créer une scène
    auto scene = std::make_shared<MyGameScene>(engine);
    engine.setScene(scene);
    
    // Lancer le jeu
    engine.run();
    
    return 0;
}
```

### Chargement d'une carte Tiled

```cpp
// Dans votre scène
void MyGameScene::init()
{
    auto& resourceManager = m_engine.getResourceManager();
    Resources::TiledMapLoader mapLoader(resourceManager);
    
    // Initialiser la carte
    m_gameMap = std::make_unique<Gameplay::GameMap>(mapLoader, m_engine.getPhysicsSystem());
    
    // Charger depuis un fichier Tiled (JSON)
    m_gameMap->loadFromFile("resources/maps/level1.json", true);
}
```

## Exemples

Le projet inclut plusieurs exemples montrant différentes fonctionnalités du moteur :

- **GameMapExample** : Utilisation des cartes Tiled
- **PhysicsExample** : Démonstration du système physique
- **AIExample** : Comportements d'intelligence artificielle

## État du développement

Le projet est actuellement en développement actif avec des mises à jour pour :

- Support complet de Box2D 3.0
- Optimisation des performances
- Ajout de nouvelles fonctionnalités comme le scripting

## Licence

Ce projet est sous licence MIT - voir le fichier LICENSE pour plus de détails.

## Contribution

Les contributions sont les bienvenues ! N'hésitez pas à ouvrir une issue ou une pull request.

1. Forkez le projet
2. Créez votre branche (`git checkout -b feature/amazing-feature`)
3. Committez vos changements (`git commit -m 'Add some amazing feature'`)
4. Pushez vers la branche (`git push origin feature/amazing-feature`)
5. Ouvrez une Pull Request 