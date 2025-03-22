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

### 3. Système de Particules (✅ Terminé)
- Implémentation complète avec :
  - `Particle`: Structure représentant une particule
  - `ParticleEmitter`: Gestion de l'émission et configuration
  - `ParticleSystem`: Singleton gérant les émetteurs et le rendu
  - `ParticleComponent`: Liaison avec les entités du jeu
- Fonctionnalités :
  - Différentes formes d'émetteurs (point, ligne, rectangle, cercle)
  - Types de déclenchement (continu, rafale, événement)
  - Interpolation des propriétés
  - Système d'affecteurs
  - Optimisation du rendu avec VertexArray/Triangles
- Documentation et exemples créés :
  - Guide d'utilisation (docs/PARTICLE_SYSTEM.md)
  - SimpleParticleDemo - exemple basique
  - ParticleExample - démonstration d'effets variés
- ✅ Exemples corrigés pour SFML 3:
  - Mise à jour des constructeurs de classes (VideoMode, Text, etc.)
  - Utilisation des namespaces scoped (Event::Type, Keyboard::Key, etc.)
  - Correction des méthodes et signatures qui ont changé

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
- **Problèmes à résoudre** :
  - Erreurs de compilation avec Box2D 3
  - Adapter les types du wrapper à la nouvelle API Box2D

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
  - Gestion efficace des assets
  - Chargement asynchrone
  - Support pour le hot-reloading
- **Composants à implémenter** :
  - Classe `ResourceManager`
  - Système de cache
  - Chargement optimisé des ressources

### 8. Système de Données
- **Objectifs** :
  - Utiliser nlohmann/json pour la gestion des données
  - Support pour la sérialisation des objets
  - Gestion des sauvegardes
- **Composants à implémenter** :
  - Classe `DataManager`
  - Interfaces de sérialisation
  - Utilitaires JSON

## État d'avancement

### Terminé (✅)
- Structure de base du projet avec build.bat
- Adaptation pour SFML 3
- Système de particules complet
- Correction des exemples pour compatibilité SFML 3

### En cours (🔄)
- Système de state
- Système de scene
- Intégration Box2D 3
- Système de ressources
- Système de données
- Interface utilisateur avec TGUI
- Système de carte avec Tiledson

### À faire
- Intégration BehaviorTree.CPP
- Implémentation OpenSteer
- Navigation avec Recast & Detour
- Éditeur d'arbres de comportement
- Optimisations et tests
- Documentation complète

## Bugs connus
- Erreurs de compilation avec Box2D 3
- Problèmes d'intégration avec les modèles de données
- ✅ Les exemples de particules montrent des erreurs de linter avec SFML 3 (RÉSOLU)

## Prochaines étapes immédiates
1. ✅ Corriger les erreurs de compilation dans les exemples de particules pour SFML 3 (TERMINÉ)
2. Résoudre les problèmes d'intégration avec Box2D 3
3. Améliorer le système de particules avec support pour textures
4. Préparer l'intégration avec le système de scene et d'entités

## Améliorations futures
- Optimisation des performances du système de particules
- Support pour textures et dégradés complexes
- Sauvegarde/chargement des configurations d'émetteurs
- Interface d'édition des systèmes de particules
- Ajout d'effets de post-traitement pour les particules
- Intégration avec le système de lumières dynamiques
