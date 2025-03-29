# Projet Orenji-CPP - TODO List

## Corrections et Systèmes de Base

### Corrections immédiates
- [x] Corriger les erreurs dans `src/Scenes/MainMenuScene.cpp` pour SFML 3:
  - Constructeurs `sf::Text`
  - Méthodes `setScale`, `setOrigin`, `setPosition`
  - Membres `sf::Event`
  - Constantes `sf::Keyboard`
  - Types `sf::Uint8`
- [x] Corriger les erreurs de compatibilité SFML 3 dans les exemples:
  - [x] `sf::VideoMode` nécessite maintenant `sf::Vector2u` au lieu de `(int, int)`
  - [x] `sf::Vertex` constructeur modifié
  - [x] `sf::Event` doit être construit avec un sous-type
  - [x] `pollEvent()` ne prend plus de paramètre et retourne un `std::optional<Event>`
  - [x] `sf::Keyboard::Key::X` au lieu de `sf::Keyboard::X`
  - [x] `sf::Mouse::Button::Left` au lieu de `sf::Mouse::Left`
  - [x] `sf::Text` nécessite des paramètres à la construction
  - [x] `sf::Font::loadFromFile` remplacé par `openFromFile`
  - [x] `sf::BlendMode` constants modifiés
  - [x] Remplacer `sf::Uint8` par `std::uint8_t`
  - [x] Mettre à jour la méthode `draw` de `sf::RenderTarget`

### Corrections du Système de Particules
- [x] Corriger les erreurs dans `ParticleSystem.cpp`:
  - [x] Remplacer `sf::Quads` par `sf::PrimitiveType::Triangles`
  - [x] Corriger les membres non déclarés (`m_emissionTimer`, `m_enabled`, etc.)
  - [x] Mettre à jour les constantes `sf::BlendMode`
  - [x] Corriger les méthodes manquantes (`setEmitterArea`, `setParticleAcceleration`, etc.)
  - [x] Remplacer `sf::Uint8` par `std::uint8_t`
  - [x] Mettre à jour les signatures de méthodes pour SFML 3
  - [x] Implémenter `setBlendMode` et `setCircularEmitter`
  - [x] Corriger la méthode `draw` pour le nouveau système de rendu de SFML 3

### Système de Physique
- [x] Implémenter `Box2DWrapper` et intégration SFML 3
- [x] Créer `TiledMapCollider` pour Box2D
- [x] Implémenter composants physiques de base
- [x] Ajouter filtrage des collisions
- [x] Intégrer débogage visuel des collisions
- [x] Implémenter collisions entité-entité et entité-map
- [x] Mettre à jour pour utiliser le système `b2BodyId` de Box2D 3.0

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
- [x] Corriger les erreurs de compilation des exemples

### Tests et Optimisation
- [x] Créer démo du système de particules
- [x] Corriger les erreurs dans la démo de particules pour SFML 3
- [x] Créer démo du système de physique avec collision filtering
- [ ] Créer tests unitaires
- [ ] Tester sur différentes plateformes
- [ ] Ajouter benchmarks
- [ ] Optimiser performances générales

### Documentation
- [x] Écrire README détaillé
- [ ] Documenter API (Doxygen)
- [ ] Créer guide d'utilisation
- [ ] Documenter intégration Tiled
- [ ] Documenter utilisation des arbres de comportement
- [ ] Documenter système de particules et effets
- [x] Documenter système de physique et collision filtering

## Fonctionnalités Additionnelles
- [ ] Système de sauvegarde/chargement
- [x] Système d'animation
- [x] Système de particules configurable
- [ ] Gestionnaire d'événements personnalisé
- [ ] Éditeur de niveaux