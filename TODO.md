# Projet Orenji-CPP - TODO List

## Corrections et Systèmes de Base

### Corrections immédiates
- [x] Corriger les erreurs dans `src/Scenes/MainMenuScene.cpp` pour SFML 3:
  - Constructeurs `sf::Text`
  - Méthodes `setScale`, `setOrigin`, `setPosition`
  - Membres `sf::Event`
  - Constantes `sf::Keyboard`
  - Types `sf::Uint8`
- [ ] Corriger les erreurs de compatibilité SFML 3 dans les exemples:
  - `sf::VideoMode` nécessite maintenant `sf::Vector2u` au lieu de `(int, int)`
  - `sf::Vertex` constructeur modifié
  - `sf::Event` doit être construit avec un sous-type
  - `pollEvent()` ne prend plus de paramètre et retourne un `std::optional<Event>`
  - `sf::Keyboard::Key::X` au lieu de `sf::Keyboard::X`
  - `sf::Mouse::Button::Left` au lieu de `sf::Mouse::Left`
  - `sf::Text` nécessite des paramètres à la construction
  - `sf::Font::loadFromFile` remplacé par `openFromFile`
  - `sf::BlendMode` constants modifiés

### Système de Physique
- [x] Implémenter `Box2DWrapper` et intégration SFML 3
- [x] Créer `TiledMapCollider` pour Box2D
- [x] Implémenter composants physiques de base
- [ ] Ajouter filtrage des collisions
- [ ] Intégrer débogage visuel des collisions
- [ ] Implémenter collisions entité-entité et entité-map

### Système d'Entités
- [x] Implémenter classes de base Entity et EntityManager
- [x] Ajouter récupération d'entités par composants
- [ ] Implémenter nettoyage des entités
- [ ] Créer Player et Enemy de base

### Système de Rendu
- [x] Implémenter RenderSystem et SpriteComponent
- [x] Créer système de particules (SFML 3 compatible)
- [x] Optimiser avec texture batching et view culling
- [x] Implémenter système d'animation

### Système de Cartes
- [x] Intégrer Tileson pour chargement des cartes Tiled
- [x] Implémenter TiledMapLoader et GameMap
- [x] Support des calques avec parallax
- [x] Support des objets pour collisions
- [ ] Ajouter transitions entre niveaux

### Caméra
- [x] Implémenter système de caméra avec suivi
- [x] Ajouter limites de caméra
- [x] Intégrer effets (shake, zoom, pan)
- [x] Implémenter mouvement fluide

### Intelligence Artificielle
- [x] Implémenter le pathfinding A*
- [x] Créer une architecture pour les behavior trees
- [x] Intégrer BehaviorTree.CPP
- [x] Ajouter des comportements prédéfinis (poursuite, patrouille, etc.)
- [x] Créer AIComponent pour lier entités aux arbres de comportement
- [ ] Tester et déboguer les comportements d'IA
- [ ] Créer une interface visuelle pour la création d'arbres de comportement

### Interface Utilisateur
- [x] Implémenter UIManager (base)
- [ ] Créer interface d'éditeur
- [ ] Intégrer TGUI
- [ ] Ajouter menus (pause, options)

### Ressources
- [x] Implémenter ResourceManager pour SFML 3
- [x] Ajouter ressources de base:
  - Police VeniceClassic.ttf
  - Textures title-bg.png
  - Audio 012-Theme01.mid
- [x] Ajouter ressources pour particules:
  - Textures: particle.png, particle_star.png, particle_flame.png
  - Configurations: fire.txt, smoke.txt, explosion.txt
- [ ] Ajouter sprites (joueur, ennemis)
- [ ] Créer tilesets et cartes
- [ ] Ajouter effets sonores et musiques

## Configuration et Tests

### Build
- [x] Mettre à jour build.bat
- [x] Configurer CMake pour SFML 3
- [x] Résoudre problèmes de liaison
- [x] Ajouter support pour BehaviorTree.CPP dans CMakeLists.txt
- [x] Ajouter support pour le système de particules dans le build
- [ ] Corriger les erreurs de compilation des exemples

### Tests et Optimisation
- [x] Créer démo du système de particules
- [ ] Corriger les erreurs dans la démo de particules pour SFML 3
- [ ] Créer tests unitaires
- [ ] Tester sur différentes plateformes
- [ ] Ajouter benchmarks
- [ ] Optimiser performances générales

### Documentation
- [ ] Écrire README détaillé
- [ ] Documenter API (Doxygen)
- [ ] Créer guide d'utilisation
- [ ] Documenter intégration Tiled
- [ ] Documenter utilisation des arbres de comportement
- [ ] Documenter système de particules et effets

## Fonctionnalités Additionnelles
- [ ] Système de sauvegarde/chargement
- [x] Système d'animation
- [x] Système de particules configurable
- [ ] Gestionnaire d'événements personnalisé
- [ ] Éditeur de niveaux