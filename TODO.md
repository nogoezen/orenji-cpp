# Projet Moteur de Jeu 2D Moderne

## Vue d'ensemble
Création d'un moteur de jeu 2D moderne utilisant les technologies suivantes :
- SFML 3 pour le rendu graphique et la gestion des événements
- Box2D 3 pour la simulation physique
- Tiled/Tiledson pour la gestion des cartes
- BehaviorTree.CPP pour les arbres de comportement
- TGUI pour l'interface utilisateur
- nlohmann/json pour la gestion des données JSON
- OpenSteer pour le comportement de direction
- Recast & Detour pour la navigation et la recherche de chemin

## État d'avancement

Nous avons terminé l'implémentation du système de particules avec les classes suivantes :
- `Particle`: Structure représentant une particule avec toutes ses propriétés
- `ParticleEmitter`: Gère l'émission et la configuration des particules
- `ParticleSystem`: Singleton gérant tous les émetteurs et optimisant le rendu
- `ParticleComponent`: Composant liant les particules aux entités du jeu

Ces implémentations sont compatibles avec SFML 3 et incluent les fonctionnalités suivantes :
- Différentes formes d'émetteurs (point, ligne, rectangle, cercle, anneau)
- Support pour différents types de déclenchement (continu, rafale, événement)
- Interpolation des propriétés (taille, couleur) pendant la vie de la particule
- Système d'affecteurs pour modifier le comportement des particules
- Optimisation du rendu avec VertexArray utilisant des triangles

## Architecture du Moteur

### 1. Système de State
- **Objectifs** :
  - Gérer les différents états du jeu (Menu principal, Jeu, Pause, etc.)
  - Permettre des transitions fluides entre les états
  - Support pour les états empilés (stack-based)
- **Composants à implémenter** :
  - Classe `StateManager`
  - Interface `State`
  - Transitions entre états
  - Persistance d'état

### 2. Système de Scene
- **Objectifs** :
  - Organisation hiérarchique des objets de jeu
  - Gestion efficace des entités et composants
  - Support pour le chargement/déchargement dynamique
- **Composants à implémenter** :
  - Classe `SceneManager`
  - Classe `Scene`
  - Système de nœuds et graphe de scène
  - Système de chargement de scènes depuis des fichiers JSON

### 3. Système de Particules
- **Objectifs** :
  - Créer un système de particules complet basé sur SFML 3
  - Support pour différents types d'émetteurs
  - Effets visuels variés (feu, fumée, eau, etc.)
- **Composants à implémenter** :
  - Classe `ParticleSystem`
  - Classe `ParticleEmitter`
  - Différents types de particules et comportements
  - Optimisation du rendu avec VertexArray

### 4. Système d'IA
- **Objectifs** :
  - Intégrer BehaviorTree.CPP pour les arbres de comportement
  - Utiliser OpenSteer pour les comportements de direction
  - Implémenter Recast & Detour pour la navigation
- **Composants à implémenter** :
  - Classe `AIManager`
  - Interface pour les comportements d'IA
  - Système de navigation et évitement d'obstacles
  - Éditeur visuel d'arbres de comportement

### 5. Système Physique
- **Objectifs** :
  - Intégrer Box2D 3 pour la simulation physique
  - Créer des wrappers faciles à utiliser
  - Support pour les débogueurs visuels
- **Composants à implémenter** :
  - Classe `PhysicsWorld`
  - Composants physiques pour les entités
  - Callbacks de collision
  - Outils de débogage

### 6. Système d'Interface Utilisateur
- **Objectifs** :
  - Intégrer TGUI pour les interfaces utilisateur
  - Créer un système de HUD dans le jeu
  - Support pour des menus et dialogues
- **Composants à implémenter** :
  - Classe `UIManager`
  - Système de thèmes et styles
  - Widgets personnalisés
  - Support pour le chargement d'UI depuis des fichiers

### 7. Système de Ressources
- **Objectifs** :
  - Gestion efficace des assets (textures, sons, polices, etc.)
  - Chargement asynchrone
  - Support pour le hot-reloading
- **Composants à implémenter** :
  - Classe `ResourceManager`
  - Système de cache
  - Chargement optimisé des ressources

### 8. Système de Données
- **Objectifs** :
  - Utiliser nlohmann/json pour charger et sauvegarder des données
  - Support pour la sérialisation/désérialisation des objets du jeu
  - Gestion des sauvegardes
- **Composants à implémenter** :
  - Classe `DataManager`
  - Interfaces de sérialisation
  - Utilitaires de lecture/écriture JSON

## Plan d'Implémentation

### Phase 1: Structure de Base
1. ✅ Mise en place de la structure du projet avec build.bat (au lieu de CMake)
2. ✅ Adaptation pour SFML 3 et Box2D 3
3. 🔄 Création du système de state
4. 🔄 Implémentation de la boucle de jeu principale

### Phase 2: Systèmes Fondamentaux
1. 🔄 Implémentation du système de scene
2. 🔄 Développement du système de ressources
3. 🔄 Création du système de données
4. 🔄 Intégration de Box2D 3 avec le système physique

### Phase 3: Systèmes Avancés
1. ✅ Développement du système de particules basé sur SFML 3
2. 🔄 Intégration de TGUI pour l'interface utilisateur
3. 🔄 Implémentation du système de carte avec Tiledson
4. 🔄 Mise en place des outils de débogage

### Phase 4: Système d'IA
1. 🔄 Intégration de BehaviorTree.CPP
2. 🔄 Implémentation des comportements avec OpenSteer
3. 🔄 Ajout de la navigation avec Recast & Detour
4. 🔄 Création d'un éditeur d'arbres de comportement

### Phase 5: Optimisation et Polissage
1. 🔄 Optimisation des performances
2. 🔄 Amélioration du système de rendu
3. 🔄 Tests et débogage
4. 🔄 Documentation

## Tâches Immédiates

### Mise en place du projet
- [x] Créer la structure de base du projet
- [x] Configurer le script build.bat pour la compilation
- [x] Intégrer SFML 3 et adapter le code
- [ ] Intégrer Box2D 3 et adapter le code
- [ ] Intégrer nlohmann/json et tester avec les fichiers JSON existants

### Système de State
- [ ] Implémenter la classe StateManager
- [ ] Créer l'interface State
- [ ] Implémenter les états de base (MenuState, GameState, PauseState)
- [ ] Tester les transitions entre états

### Système de Scene
- [ ] Implémenter la classe SceneManager
- [ ] Créer la classe Scene et le système de nœuds
- [ ] Développer le système d'entités et composants
- [ ] Tester le chargement et le rendu d'une scène simple

### Système de Particules
- [x] Implémenter la classe ParticleSystem
- [x] Créer différents types d'émetteurs
- [x] Optimiser le rendu avec VertexArray
- [x] Implémenter la classe ParticleEmitter
- [x] Implémenter le ParticleComponent
- [x] Adapter pour SFML 3 (uint8_t, sf::degrees(), Vertex, Triangles)
- [ ] Créer des exemples d'effets de particules (feu, fumée, explosion, etc.)

### Prochaines étapes
1. Créer des exemples d'effets de particules
2. Intégrer Box2D 3 et adapter le code
3. Développer le système de ressources pour gérer les textures des particules
