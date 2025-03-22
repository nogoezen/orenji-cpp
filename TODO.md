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

## Erreurs de compilation identifiées

### 1. Problèmes avec la mise à jour Box2D
- **Erreurs** : Multiples erreurs liées à l'utilisation de l'API Box2D 3
- **Détails** :
  - Les structures comme `b2FixtureId`, `b2BodyId` ont changé (problème avec `.index` vs `.index1`)
  - Plusieurs fonctions (`b2Body_GetFixtureAtIndex`, `b2Fixture_GetShape`, etc.) non trouvées ou changées
  - Type `b2FixtureId` n'est pas défini correctement dans `PhysicsComponent` et `PhysicsWorld`
  - Appels ambigus comme `b2Body_ApplyForce` et `b2Body_SetFixedRotation` qui ont une surcharge
  - Fonctions comme `b2Body_GetDefinition` et `b2Body_SetDefinition` non trouvées

### 2. Problèmes de compatibilité SFML 3
- **Erreurs** : Les API de SFML 3 sont différentes de SFML 2
- **Détails** :
  - Changements dans `sf::Transformable::setRotation` qui prend maintenant un `sf::Angle` au lieu d'un `float`
  - Erreurs dans `sf::Vertex` qui a une signature de constructeur différente
  - Type `sf::Lines` remplacé par `sf::PrimitiveType::Lines`
  - Problèmes avec `getPosition()` qui retourne une valeur et non une référence

### 3. Problèmes de duplication de définitions
- **Erreurs** : Plusieurs structures et méthodes redéfinies
- **Détails** :
  - Structure `SimpleParticle` définie à la fois dans `ParticleComponent.hpp` et `ParticleSystem.hpp`
  - Redéfinition de méthodes comme `setDebugDraw`, `isDebugDrawEnabled` et `getBody`
  - Problèmes avec le constructeur privé `ParticleSystem::ParticleSystem()` lors de l'utilisation de `std::make_unique`

### 4. Problèmes avec les modèles (classes City, Player, Ship, Fleet)
- **Erreurs** : Problèmes de visibilité et de namespace dans les modèles
- **Détails** :
  - La classe `City` manque des méthodes comme `hasGood`, `getGoodQuantity`, `addGood`, etc.
  - La classe `Player` manque des méthodes comme `getFaction`, `getMoney`, `getShip`, etc.
  - Erreurs de namespace avec `Ship` vs `Orenji::Ship`
  - Problèmes de type avec les collections comme `std::vector<std::shared_ptr<Ship>>`

### 5. Utilitaires manquants
- **Erreurs** : Références à des méthodes d'utilitaires non définies
- **Détails** :
  - Classe `MathUtils` non trouvée, avec des appels à `MathUtils::randInt`, `MathUtils::randFloat`, etc.
  - Problèmes de conversion de types dans les méthodes mathématiques et de rendu

## Plan d'action pour résoudre les erreurs

### 1. Mise à jour Box2D
- **Tâches** :
  - Étudier la documentation de Box2D 3 pour comprendre les changements d'API
  - Mettre à jour les structures `b2FixtureId` et `b2BodyId` pour utiliser `.index1` au lieu de `.index`
  - Mettre à jour les signatures des méthodes dans `PhysicsWorld` et `PhysicsComponent`
  - Résoudre les ambiguïtés d'appels en qualifiant complètement les fonctions (ex: `Orenji::box2d::b2Body_ApplyForce` vs `::b2Body_ApplyForce`)
  - Ajouter une directive conditionnelle `#ifdef BOX2D_V3` pour assurer la compatibilité avec les différentes versions

### 2. Compatibilité SFML 3
- **Tâches** :
  - Mettre à jour les appels à `setRotation` pour utiliser `sf::degrees(angle)`
  - Corriger les constructeurs de `sf::Vertex` pour utiliser la nouvelle syntaxe
  - Remplacer `sf::Lines` par `sf::PrimitiveType::Lines`
  - Gérer correctement les retours de `getPosition()` en faisant une copie locale

### 3. Problèmes de duplication
- **Tâches** :
  - Déplacer la définition de `SimpleParticle` dans un fichier commun (ex: `include/Particles/ParticleTypes.hpp`)
  - Supprimer les redéfinitions de méthodes comme `setDebugDraw` en conservant uniquement la version inline dans le fichier header
  - Modifier le pattern singleton de `ParticleSystem` pour permettre l'utilisation de `std::make_unique`

### 4. Intégration des modèles
- **Tâches** :
  - Compléter l'implémentation des classes `City` et `Player` avec les méthodes manquantes
  - Corriger les problèmes de namespace en utilisant `Orenji::Ship` au lieu de `Ship`
  - Corriger les déclarations de collections comme `std::vector<std::shared_ptr<Orenji::Ship>>`
  - Assurer la cohérence entre les déclarations et les définitions des méthodes

### 5. Implémentation des utilitaires
- **Tâches** :
  - Créer ou compléter la classe `MathUtils` dans `include/Utils/MathUtils.hpp`
  - Implémenter les fonctions manquantes comme `randInt`, `randFloat`, etc.
  - Assurer que les méthodes de conversion de types sont correctement implémentées

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

## Conclusion et prochaines étapes

Le projet Orenji-CPP a progressé significativement avec l'implémentation de plusieurs fonctionnalités clés et la résolution de nombreux problèmes. Cependant, les erreurs de compilation identifiées montrent que le passage aux nouvelles versions des bibliothèques (SFML 3 et Box2D 3) a introduit des incompatibilités importantes qui doivent être résolues.

### Démarche recommandée
1. **Triage des erreurs par priorité** : Commencer par traiter les problèmes bloquants comme les incompatibilités Box2D qui empêchent la compilation du système physique.
2. **Approche modulaire** : Résoudre les problèmes fichier par fichier, en commençant par les classes de base (comme `PhysicsWorld` et `Entity`).
3. **Tests incrémentaux** : Après chaque correction majeure, effectuer une compilation partielle pour vérifier les progrès.
4. **Documentation simultanée** : Documenter les changements d'API et les adaptations effectuées pour faciliter la maintenance future.

### Stratégies possibles
- **Option 1 - Mise à jour complète** : Adapter entièrement le code pour utiliser SFML 3 et Box2D 3, ce qui implique une refonte importante des systèmes physiques et graphiques.
- **Option 2 - Compatibilité rétroactive** : Utiliser des directives conditionnelles pour supporter à la fois les anciennes et les nouvelles versions des bibliothèques, ce qui complexifie le code mais offre plus de flexibilité.
- **Option 3 - Downgrade temporaire** : Revenir temporairement à SFML 2 et Box2D 2.4 pour assurer la compatibilité, puis planifier une migration progressive.

L'option 1 semble la plus adaptée à long terme, car elle permettra de profiter pleinement des améliorations apportées par les nouvelles versions des bibliothèques, même si cela demande un investissement initial plus important.

### Objectifs à court terme (1-2 semaines)
1. Corriger les erreurs liées à Box2D 3 dans `PhysicsWorld` et `PhysicsComponent`
2. Résoudre les problèmes de compatibilité SFML 3 dans les classes graphiques
3. Éliminer les problèmes de duplication et de namespace pour améliorer la cohérence du code
4. Implémenter la classe `MathUtils` pour les fonctions utilitaires manquantes

### Objectifs à moyen terme (1-2 mois)
1. Compléter la documentation de l'architecture et des classes principales
2. Améliorer le système de build pour faciliter la configuration sur différents environnements
3. Implémenter des tests unitaires pour les composants critiques
4. Optimiser les performances des systèmes de rendu et de physique

En suivant cette démarche, le projet pourra devenir une base solide pour développer des jeux 2D avec une architecture moderne et des performances optimales.

## Adaptations nécessaires pour les nouvelles versions de librairies

### SFML 3
- [x] Corriger l'utilisation des angles (radians vs degrés)
- [x] Mettre à jour les fonctions de transformation
- [x] Remplacer `sf::Quads` par `sf::PrimitiveType::Quads`
- [x] Remplacer `sf::Lines` par `sf::PrimitiveType::Lines`
- [ ] Vérifier les autres types primitifs similaires
- [ ] Vérifier la compatibilité avec le système de rendu

### Box2D 3
- [ ] Mettre à jour la création de monde et la gestion des corps physiques
  - [ ] Remplacer les pointeurs par des identifiants (b2WorldId, b2BodyId, etc.)
  - [ ] Mettre à jour les fonctions d'accès (GetBodyAngle, etc.)
  - [ ] Revoir le système de collision et de contact
- [ ] Adapter la classe ContactListener
- [ ] Mettre à jour les fonctions utilitaires dans Box2DTypes.hpp
- [ ] Vérifier les transformations de coordonnées (mètres/pixels)

## Erreurs de compilation identifiées

### Box2D
- Erreurs de référence non résolues pour les fonctions Box2D
- Problèmes d'incompatibilité entre les pointeurs et les identifiants
- ContactListener nécessite une mise à jour complète

### SFML
- ~~Problèmes avec `sf::Vertex` et `sf::PrimitiveType`~~
- ~~Erreurs liées aux constructeurs de `sf::Vertex`~~
- ~~Problèmes de compatibilité avec les types primitifs comme `sf::Quads` et `sf::Lines`~~

### Erreur de duplication
- ~~Duplication de `SimpleParticle` entre `ParticleComponent.hpp` et `ParticleSystem.hpp`~~
- ~~Duplication de `EmissionParameters`~~

### Erreurs spécifiques au système de particules
- ~~Erreur de compilation due à Thor (résolu avec `DISABLE_THOR`)~~
- ~~Erreurs d'inclusion de fichiers relatifs à MathUtils~~
- ~~Implémentation manquante pour les particules sans Thor~~

## Plan d'action pour résoudre les erreurs

### 1. Mise à jour Box2D
- [x] Mise à jour de Box2DTypes.hpp
- [x] Mise à jour de PhysicsWorld.cpp
- [x] Mise à jour de PhysicsWorld.hpp
- [ ] Vérifier les autres fichiers qui utilisent Box2D

### 2. Compatibilité SFML 3
- [x] Mise à jour des types primitifs dans ParticleComponent.cpp
- [x] Mise à jour des constructeurs de sf::Vertex
- [x] Mise à jour de PhysicsWorld.cpp pour les fonctions de dessin
- [ ] Vérifier les autres utilisations de sf::Vertex dans la codebase

### 3. Résolution des duplications
- [x] Définir les structures dans ParticleTypes.hpp
- [x] Supprimer les définitions dupliquées dans ParticleComponent.hpp
- [x] Adapter les imports existants

### 4. Intégration des modèles manquants
- [x] Créer le fichier MathUtils.hpp
- [x] Implémenter les fonctions utilitaires mathématiques

### 5. Implémentation des utilitaires
- [x] Compléter l'implémentation de ParticleComponent.cpp sans Thor
- [x] Ajouter la méthode updateVertices()
- [ ] Finaliser ParticleSystem.cpp

## Conclusion et prochaines étapes

### Progrès réalisés
- Système de particules amélioré avec et sans Thor
- Adaptation partielle à Box2D 3
- Résolution des problèmes de types SFML 3

### Problèmes restants
- Compléter l'adaptation à Box2D 3
- Finaliser l'intégration avec SFML 3
- Nettoyer les avertissements restants

### Priorités
1. Terminer les adaptations pour Box2D 3
2. Tester la compilation complète
3. Corriger les erreurs restantes

### Objectifs à court terme
- Compiler et exécuter le jeu
- Ajouter de nouvelles fonctionnalités

### Objectifs à moyen terme
- Améliorer les graphismes
- Améliorer les effets sonores
 