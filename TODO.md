# TODO - Projet Orenji-CPP

## Contexte général
Le projet Orenji-CPP est un moteur de jeu/RPG basé sur SFML qui a été corrigé pour résoudre les principaux problèmes de compilation et fonctionnalités manquantes. Le projet peut maintenant afficher l'écran titre et passer au mode jeu. Des améliorations et optimisations supplémentaires peuvent encore être apportées.

## Problèmes résolus

### ✅ 1. Intégration de Box2D 2.4.x
- **Contexte** : Le projet a été mis à jour pour utiliser Box2D 2.4.x.
- **Réalisations** :
  - Les fichiers `Box2DTypes.hpp` ont été mis à jour pour être compatibles avec Box2D 2.4.x
  - Les wrappers de `PhysicsComponent.cpp` et `PhysicsWorld.cpp` ont été adaptés pour la nouvelle API
  - Les signatures des méthodes utilisant des identifiants Box2D ont été corrigées

### ✅ 2. Résolution des problèmes de compilation de la bibliothèque Thor
- **Contexte** : Une alternative au composant `ParticleComponent` a été implémentée pour fonctionner avec ou sans Thor.
- **Réalisations** :
  - Création d'un système de particules simple intégré au projet qui ne dépend pas de Thor
  - Mise en place d'une compilation conditionnelle avec `#ifndef DISABLE_THOR`
  - La compilaton peut maintenant se faire que Thor soit disponible ou non

### ✅ 3. Implémentation des modèles manquants
- **Contexte** : Les fichiers manquants ont été créés et implémentés.
- **Réalisations** :
  - Création des classes `City`, `Ship`, `TradeGood` et autres modèles manquants
  - Implémentation des fonctionnalités de base pour ces modèles
  - Intégration avec le reste du système de jeu

### ✅ 4. Implémentation du mode de jeu complet
- **Contexte** : Le mode de jeu (GameState) est désormais fonctionnel.
- **Réalisations** :
  - Implémentation complète de `GameState.cpp`
  - Intégration des systèmes d'entités et de composants
  - Système de rendu des niveaux fonctionnel
  - Gestion des personnages jouables

### ✅ 5. Correction des avertissements de compilation
- **Contexte** : Les avertissements liés à l'ordre d'initialisation et aux variables non utilisées ont été corrigés.
- **Réalisations** :
  - Correction de l'ordre d'initialisation dans les constructeurs (ex: `Game::m_isRunning` et `Game::m_deltaTime`)
  - Ajout de cast pour les variables non utilisées (ex: `entity` dans `RenderSystem.cpp`)
  - Initialisation complète des membres dans le constructeur de `Ship`

### ✅ 7. Configuration correcte des DLL SFML
- **Contexte** : Les DLL SFML ne sont pas trouvées lors de la copie dans le dossier bin.
- **Réalisations** :
  - Création d'un script `copy_dlls.bat` pour copier manuellement les DLL
  - Amélioration du CMakeLists.txt pour détecter et copier automatiquement les DLL
  - Gestion distincte des configurations Debug et Release

### ✅ 8. Implémentation du système de trading
- **Contexte** : Le système de trading est maintenant fonctionnel.
- **Réalisations** :
  - Création de l'interface pour le système de trading via `TradingSystem`
  - Implémentation des mécanismes d'échange entre entités
  - Intégration avec le système d'économie du jeu

### ✅ 11. Correction des erreurs de compilation identifiées
- **Contexte** : Plusieurs erreurs de compilation ont été identifiées et corrigées.
- **Réalisations** :
  - Création du fichier manquant `include/utils/MathUtils.hpp` avec les fonctions mathématiques nécessaires
  - Correction des chemins d'inclusion dans les fichiers `Ship.cpp`, `TradeGood.cpp` et `TradingSystem.cpp`
  - Correction de l'ordre d'initialisation dans les constructeurs de la classe `City`
  - Vérification du système de build et configurations des chemins d'inclusion

### ✅ 12. Installation des bibliothèques requises
- **Contexte** : Les bibliothèques externes SFML, Box2D, Thor et TMXLite ont été installées via MSYS2 UCRT64.
- **Réalisations** :
  - Installation des bibliothèques à l'aide de pacman dans MSYS2
  - Configuration correcte des chemins d'inclusion et des bibliothèques
  - Détection automatique des bibliothèques installées dans le script de build

### ✅ 13. Mise à jour vers les dernières versions des bibliothèques
- **Contexte** : Le projet a été mis à jour pour utiliser les dernières versions des bibliothèques.
- **Réalisations** :
  - Support de SFML 3 avec les nouvelles fonctionnalités audio et graphiques
  - Support de Box2D 3 avec la nouvelle API physique
  - Support des dernières versions de Thor et TMXLite
  - Ajout de directives conditionnelles pour assurer la compatibilité (SFML_V3, BOX2D_V3)
  - Mise à jour du script build.bat pour détecter et utiliser les bonnes versions des DLLs

## Problèmes restants à résoudre

### 6. Amélioration du système de build
- **Contexte** : Le système de build a été amélioré mais des optimisations sont encore possibles.
- **Tâches** :
  - Optimiser la détection des dépendances
  - Améliorer la portabilité pour différentes plateformes

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

### 14. Adaptation du code pour les nouvelles versions des bibliothèques
- **Contexte** : Le code doit être adapté pour tirer pleinement parti des nouvelles versions des bibliothèques.
- **Tâches** :
  - Mettre à jour les classes utilisant SFML pour exploiter les nouvelles fonctionnalités de SFML 3
  - Adapter le code de physique pour utiliser correctement Box2D 3
  - Mettre à jour les renderers pour utiliser les nouvelles capacités graphiques
  - Assurer la compatibilité avec les anciennes versions via des directives de préprocesseur

## Priorités suggérées

1. Adaptation du code pour les nouvelles versions des bibliothèques (point 14)
2. Documentation du code - Pour faciliter la maintenance future
3. Amélioration du système de build - Pour faciliter le développement futur
4. Tests unitaires - Pour garantir la stabilité

## Notes techniques
- La version de SFML utilisée est désormais SFML 3
- Box2D a été mis à jour vers la version 3
- Les dernières versions de Thor et TMXLite sont utilisées
- Le projet utilise une architecture Entity-Component-System (ECS)
- Le système de build utilise CMake avec des adaptations pour les nouvelles versions de bibliothèques
- Le projet cible principalement Windows avec MSYS2 UCRT64, mais peut être adapté pour d'autres plateformes
 