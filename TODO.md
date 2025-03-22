# Projet Orenji-CPP - TODO List

## Corrections immédiates

- [ ] Corriger les erreurs de compilation dans `src/Scenes/MainMenuScene.cpp`:
  - Corriger les constructeurs manquants pour `sf::Text`
  - Corriger les appels à `setScale`, `setOrigin`, et `setPosition` pour utiliser correctement `sf::Vector2f`
  - Résoudre les problèmes avec les membres de `sf::Event`
  - Mettre à jour les constantes `sf::Keyboard` pour SFML 3
  - Remplacer `sf::Uint8` par le type correct dans SFML 3

## Implémentation des systèmes

### Système de Physique
- [ ] Implémenter `src/Physics/PhysicsSystem.cpp`
- [ ] Créer `include/Physics/Box2DWrapper.hpp` et son implémentation
- [ ] Créer `include/Physics/PhysicsComponent.hpp` et son implémentation

### Système de Rendu
- [ ] Implémenter `src/Graphics/RenderSystem.cpp`
- [ ] Créer `include/Graphics/SpriteComponent.hpp` et son implémentation
- [ ] Implémenter `src/Graphics/ParticleSystem.cpp`

### Système d'Intelligence Artificielle
- [ ] Implémenter `src/AI/AISystem.cpp`
- [ ] Créer `include/AI/BehaviorTree.hpp` et son implémentation
- [ ] Créer des comportements d'IA de base

### Interface Utilisateur
- [ ] Implémenter `src/UI/UIManager.cpp`
- [ ] Créer `include/UI/EditorUI.hpp` et son implémentation
- [ ] Intégrer TGUI avec l'interface utilisateur

### Ressources
- [ ] Implémenter `src/Resources/TiledMapLoader.cpp`
- [ ] Ajouter le support pour les fichiers JSON avec nlohmann/json
- [ ] Créer un système de chargement de configuration

### Scènes
- [ ] Implémenter `src/Scenes/GameScene.cpp`
- [ ] Créer `include/Scenes/PauseScene.hpp` et son implémentation
- [ ] Ajouter des transitions entre les scènes

## Ressources
- [ ] Ajouter une police par défaut dans `resources/fonts/`
- [ ] Ajouter des textures de base dans `resources/textures/`
- [ ] Ajouter des fichiers audio dans `resources/sounds/`
- [ ] Créer des cartes de test dans `resources/maps/`

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