# TODO - Projet Orenji-CPP

## Contexte général
Le projet Orenji-CPP est un moteur de jeu/RPG basé sur SFML qui rencontre actuellement des problèmes de compilation liés principalement à l'intégration de Box2D et d'autres bibliothèques externes. Le projet a été partiellement adapté pour permettre l'affichage de l'écran titre, mais plusieurs modules sont désactivés et nécessitent des corrections pour être pleinement fonctionnels.

## Problèmes à résoudre

### 1. Intégration de Box2D 2.4.x
- **Contexte** : Le projet utilise une ancienne version de Box2D, mais la structure du code fait référence à des éléments de la version 2.4.x (comme `b2BodyId`, `b2FixtureId` et `b2WorldId`).
- **Tâches** :
  - Mettre à jour les fichiers `Box2DTypes.hpp` pour être compatibles avec Box2D 2.4.x
  - Adapter les wrappers de `PhysicsComponent.cpp` et `PhysicsWorld.cpp` pour la nouvelle API
  - Vérifier et corriger les signatures des méthodes utilisant des identifiants Box2D
  - Mettre à jour les références à `b2Body*` vers `b2BodyId` et autres types similaires

### 2. Résolution des problèmes de compilation de la bibliothèque Thor
- **Contexte** : Le composant `ParticleComponent` utilise la bibliothèque Thor qui cause des erreurs de compilation.
- **Tâches** :
  - Vérifier la compatibilité de Thor avec la version actuelle de SFML 
  - Mettre à jour l'API de Thor si nécessaire
  - Alternative: Créer un système de particules simple intégré au projet

### 3. Implémentation des modèles manquants
- **Contexte** : Les fichiers relatifs aux modèles (`City.h`, `Ship.h`, etc.) sont manquants, ce qui empêche la compilation complète.
- **Tâches** :
  - Créer les classes `City`, `Ship` et autres modèles manquants
  - Implémenter les fonctionnalités de base pour ces modèles
  - S'assurer de leur intégration avec le reste du système de jeu

### 4. Implémentation du mode de jeu complet
- **Contexte** : Actuellement, seul l'écran titre fonctionne. Le mode de jeu est désactivé par la directive `DISABLE_GAMESTATE`.
- **Tâches** :
  - Terminer l'implémentation de `GameState.cpp` 
  - Intégrer correctement les systèmes d'entités et de composants
  - Créer le système de rendu des niveaux
  - Mettre en place la gestion des personnages jouables

### 5. Correction des avertissements de compilation
- **Contexte** : Plusieurs avertissements apparaissent pendant la compilation, notamment concernant l'ordre d'initialisation des variables et les comparaisons entre types signés et non signés.
- **Tâches** :
  - Corriger l'ordre d'initialisation dans les constructeurs (ex: `Game::m_isRunning` et `Game::m_deltaTime`)
  - Remplacer les comparaisons entre `size_t` et `int` par des conversions appropriées
  - Supprimer ou utiliser les variables inutilisées (ex: `entity` dans `RenderSystem.cpp`)

### 6. Amélioration du système de build
- **Contexte** : Le script de build actuel a été modifié pour exclure les fichiers problématiques, mais idéalement, tous les fichiers devraient être compilés.
- **Tâches** :
  - Restaurer le script de build pour inclure tous les fichiers une fois les problèmes résolus
  - Optimiser la détection des dépendances
  - Ajouter la prise en charge de CMake pour améliorer la portabilité

### 7. Configuration correcte des DLL SFML
- **Contexte** : Les DLL SFML ne sont pas trouvées lors de la copie dans le dossier bin.
- **Tâches** :
  - Vérifier l'emplacement des DLL SFML
  - S'assurer que le chemin est correctement défini dans le script de build
  - Créer un système pour gérer différentes configurations (Debug/Release)

### 8. Implémentation du système de trading
- **Contexte** : Le fichier `TradingSystem.h` est manquant.
- **Tâches** :
  - Créer l'interface pour le système de trading
  - Implémenter les mécanismes d'échange entre entités
  - Intégrer avec le système d'économie du jeu

### 9. Documentation du code
- **Contexte** : Le code manque de documentation, ce qui rend difficile la compréhension de l'architecture.
- **Tâches** :
  - Documenter les classes principales et leur fonctionnement
  - Créer des diagrammes UML pour visualiser les relations entre composants
  - Expliquer l'architecture ECS (Entity-Component-System) utilisée

### 10. Tests unitaires
- **Contexte** : Pas de tests unitaires visibles dans le projet.
- **Tâches** :
  - Mettre en place un framework de test (Google Test, Catch2, etc.)
  - Créer des tests pour les composants principaux
  - Intégrer les tests au système de build

## Priorités suggérées

1. Correction des problèmes Box2D - Essentiel pour le système de physique
2. Implémentation du mode de jeu - Fonctionnalité principale du projet
3. Correction des avertissements - Pour assurer la qualité du code
4. Implémentation des modèles manquants - Nécessaire pour le gameplay
5. Résolution des problèmes Thor - Pour les effets visuels
6. Amélioration du système de build - Pour faciliter le développement futur

## Notes techniques
- La version de SFML utilisée semble être 2.x
- Le projet utilise une architecture Entity-Component-System (ECS)
- Le script de compilation actuel utilise GCC (g++)
- Le projet cible Windows, des modifications seraient nécessaires pour d'autres plateformes 