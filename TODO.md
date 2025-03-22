# Projet Orenji-CPP - TODO List

## Corrections et Systèmes de Base

### Corrections immédiates
- [x] Corriger les erreurs dans `src/Scenes/MainMenuScene.cpp` pour SFML 3:
  - Constructeurs `sf::Text`
  - Méthodes `setScale`, `setOrigin`, `setPosition`
  - Membres `sf::Event`
  - Constantes `sf::Keyboard`
  - Types `sf::Uint8`

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
- [ ] Implémenter RenderSystem et SpriteComponent
- [ ] Créer système de particules
- [ ] Optimiser avec texture batching et view culling

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
- [ ] Ajouter sprites (joueur, ennemis)
- [ ] Créer tilesets et cartes
- [ ] Ajouter effets sonores et musiques

## Configuration et Tests

### Build
- [x] Mettre à jour build.bat
- [x] Configurer CMake pour SFML 3
- [x] Résoudre problèmes de liaison

### Tests et Optimisation
- [ ] Créer tests unitaires
- [ ] Tester sur différentes plateformes
- [ ] Ajouter benchmarks
- [ ] Optimiser performances générales

### Documentation
- [ ] Écrire README détaillé
- [ ] Documenter API (Doxygen)
- [ ] Créer guide d'utilisation
- [ ] Documenter intégration Tiled

## Fonctionnalités Additionnelles
- [ ] Système de sauvegarde/chargement
- [ ] Système d'animation
- [ ] Gestionnaire d'événements personnalisé
- [ ] Éditeur de niveaux