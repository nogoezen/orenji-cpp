# Projet Orenji-CPP - TODO List

## Corrections immédiates

- [x] Corriger les erreurs de compilation dans `src/Scenes/MainMenuScene.cpp`:
  - [x] Corriger les constructeurs manquants pour `sf::Text`
  - [x] Corriger les appels à `setScale`, `setOrigin`, et `setPosition` pour utiliser correctement `sf::Vector2f`
  - [x] Résoudre les problèmes avec les membres de `sf::Event`
  - [x] Mettre à jour les constantes `sf::Keyboard` pour SFML 3
  - [x] Remplacer `sf::Uint8` par le type correct dans SFML 3

## Implémentation des systèmes

### Système de Physique
- [x] Implémenter `src/Physics/PhysicsSystem.cpp`
- [x] Créer `include/Physics/Box2DWrapper.hpp` et son implémentation
- [x] Créer `include/Physics/PhysicsComponent.hpp` et son implémentation
- [ ] Ajouter gestion des collisions avancées
- [ ] Intégrer le débogage visuel des collisions

### Système de Rendu
- [ ] Implémenter `src/Graphics/RenderSystem.cpp`
- [ ] Créer `include/Graphics/SpriteComponent.hpp` et son implémentation
- [ ] Implémenter `src/Graphics/ParticleSystem.cpp`

### Système d'Intelligence Artificielle
- [ ] Implémenter `src/AI/AISystem.cpp`
- [ ] Créer `include/AI/BehaviorTree.hpp` et son implémentation
- [ ] Créer des comportements d'IA de base

### Interface Utilisateur
- [x] Implémenter `src/UI/UIManager.cpp` (base fonctionnelle)
- [ ] Créer `include/UI/EditorUI.hpp` et son implémentation
- [ ] Intégrer TGUI avec l'interface utilisateur

### Ressources
- [x] Implémenter `src/Resources/ResourceManager.cpp` pour SFML 3
- [x] Implémenter `src/Resources/TiledMapLoader.cpp`
- [ ] Ajouter le support pour les fichiers JSON avec nlohmann/json
- [ ] Créer un système de chargement de configuration

### Scènes
- [x] Corriger `src/Scenes/MainMenuScene.cpp` pour SFML 3
- [x] Implémenter `src/Scenes/GameScene.cpp` (base fonctionnelle)
- [ ] Créer `include/Scenes/PauseScene.hpp` et son implémentation
- [ ] Ajouter des transitions entre les scènes

## Ressources
- [x] Ajouter une police par défaut dans `resources/fonts/` (VeniceClassic.ttf)
- [x] Ajouter des textures de base dans `resources/textures/` (title-bg.png)
- [x] Ajouter des fichiers audio dans `resources/sounds/` (012-Theme01.mid)
- [ ] Créer des cartes de test dans `resources/maps/`

## Configuration de build
- [x] Mettre à jour `build.bat` pour copier correctement les ressources
- [x] Configurer CMakeLists.txt pour utiliser correctement SFML 3
- [x] Résoudre les problèmes de liaison avec les bibliothèques 

## Optimisations et tests
- [ ] Créer un système de debug pour visualiser les collisions
- [ ] Ajouter des logs et traçage d'erreurs
- [ ] Optimiser les performances du moteur de rendu
- [ ] Créer des tests unitaires

## Documentation
- [ ] Écrire un README.md détaillé
- [ ] Documenter l'API avec des commentaires Doxygen
- [ ] Créer des exemples d'utilisation

## Fonctionnalités supplémentaires
- [ ] Ajouter un système de sauvegarde/chargement
- [ ] Implémenter un système d'animation
- [ ] Ajouter un système de gestion d'événements personnalisé
- [ ] Créer un éditeur de niveaux simple

## Things To Do For Orenji-CPP - Updated Progress

### Immediate Tasks
- [x] Fix compilation errors in `src/Scenes/MainMenuScene.cpp`:
  - [x] Fix missing constructors for `sf::Text`
  - [x] Fix method calls for `setScale`, `setOrigin`, and `setPosition`
  - [x] Fix `sf::Event` members
  - [x] Fix `sf::Keyboard` constants for SFML 3
  - [x] Fix `sf::Uint8` with correct type for SFML 3
- [x] Implement UI Manager in `src/UI/UIManager.cpp` - Functional base implemented
- [x] Implement `src/Resources/ResourceManager.cpp` for SFML 3 - Completed
- [x] Implement `src/Scenes/GameScene.cpp` - Functional base implemented
- [x] Fix `src/Scenes/MainMenuScene.cpp` for SFML 3 - Corrected
- [x] Integrate Tileson library for Tiled map support
- [x] Create `TiledMapLoader` class for loading and rendering Tiled maps
- [x] Create `TiledMapCollider` for Box2D collision with Tiled maps
- [x] Create `GameMap` class to unify map rendering and collision functionality
- [x] Implement camera system in `src/Core/Camera.cpp` 
- [ ] Implement player entity in `src/Gameplay/Player.cpp`
- [ ] Implement enemy base class in `src/Gameplay/Enemies/Enemy.cpp`

### Entity System
- [x] Implement entity base in `src/Core/Entity.cpp`
- [x] Implement entity management in `src/Core/EntityManager.cpp`
- [x] Add method to retrieve entities with specific components
- [ ] Implement despawn and cleanup of entities
- [ ] Implement entity-to-entity collisions
- [ ] Implement entity-to-map collisions using Box2D

### Physics System
- [x] Update `Box2DWrapper` to work with SFML 3
- [x] Create `TiledMapCollider` for Box2D integration with Tiled maps
- [ ] Implement collision filtering
- [ ] Create physics component for entities
- [ ] Add debug rendering for Box2D objects

### Maps and Levels
- [x] Integrate Tileson for loading Tiled map files
- [x] Create `TiledMapLoader` for rendering Tiled maps
- [x] Implement map layer support with parallax scrolling
- [x] Add object layers support for collisions and game objects
- [ ] Implement level transitions
- [ ] Create sample levels

### Resources Added
- [x] Default font added: VeniceClassic.ttf
- [x] Basic textures added: title-bg.png
- [x] Audio files added: 012-Theme01.mid
- [ ] Add player sprites
- [ ] Add enemy sprites
- [ ] Add tile sets for maps
- [ ] Add background music for game levels
- [ ] Add sound effects

### Camera System
- [x] Create base camera class with follow functionality
- [x] Implement camera bounds to prevent showing outside the map
- [x] Add camera effects (shake, zoom, pan)
- [x] Implement smooth camera movement
- [x] Create camera example program

### Application Flow
- [x] Implement basic game loop
- [x] Create main menu scene
- [ ] Implement scene transitions
- [ ] Add pause menu
- [ ] Create options menu
- [ ] Implement game save/load system

### Optimizations
- [ ] Profile and optimize rendering
- [ ] Implement texture batching
- [ ] Optimize collision detection
- [ ] Add view culling for large maps

### Documentation
- [ ] Create API documentation
- [ ] Document entity system
- [ ] Document resource management
- [ ] Create user guide for Tiled integration

### Testing
- [ ] Create unit tests for core systems
- [ ] Test on different platforms
- [ ] Add performance benchmarks 