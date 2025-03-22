# TODO - Projet Orenji-CPP

## Contexte général
Le projet Orenji-CPP est un moteur de jeu/RPG basé sur SFML qui a été corrigé pour résoudre les principaux problèmes de compilation et fonctionnalités manquantes. Le projet peut maintenant afficher l'écran titre et passer au mode jeu. Des améliorations et optimisations supplémentaires peuvent encore être apportées.


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

### 4. Intégration des modèles
- [ ] Corriger les erreurs dans Ship
- [ ] Corriger les erreurs dans Fleet et City
- [ ] Ajouter les méthodes manquantes dans les modèles

### 5. Implémentation des utilitaires
- [x] Créer le fichier MathUtils.hpp
- [x] Implémenter les fonctions utilitaires mathématiques

## Conclusion et prochaines étapes

### Progrès réalisés
- Système de particules amélioré avec et sans Thor
- Adaptation partielle à Box2D 3
- Résolution des problèmes de types SFML 3
- Résolution des problèmes de duplication dans le système de particules

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

## Plan d'action détaillé pour la prochaine étape

Pour résoudre les erreurs Box2D les plus critiques:

1. Modifier `Box2DTypes.hpp`:
   - Remplacer toutes les occurrences de `.index` par `.index1` pour les identifiants Box2D
   - Rechercher des alternatives aux fonctions `b2Body_GetDefinition` et `b2Body_SetDefinition`
   - Implémenter une solution pour `b2BodyUserData`
   - Revoir les signatures des fonctions `GetBodyFixtureList`, `GetContactBodyA`, etc.

2. Adapter les transformations dans `PhysicsComponent.cpp`:
   - Corriger l'utilisation de `b2Body_SetTransform`
   - Remplacer `b2Body_GetAngle` par la fonction appropriée
   - Convertir correctement les angles avec `sf::degrees()`

3. Résoudre les problèmes de duplication:
   - Nettoyer `ParticleSystem.hpp` pour utiliser les types définis dans `ParticleTypes.hpp`
   - Assurer la cohérence dans l'utilisation des namespaces

4. Implémenter les méthodes manquantes dans les modèles:
   - Ajouter les méthodes requises dans `City` ou adapter le code pour utiliser d'autres méthodes
   - Corriger les problèmes de namespace dans `Fleet` et `Player`

# Tasks pour l'adaptation du code à SFML 3 et Box2D 3

## Problèmes identifiés

- [x] SFML 3 change la façon dont les primitives sont utilisées (`sf::Lines` -> `sf::PrimitiveType::Lines`)
- [x] SFML 3 change certains aspects de l'API `VertexArray` et `Vertex`
- [x] Box2D 3 utilise un système d'identifiants différent (b2BodyId, b2JointId, etc.)
- [x] Box2D 3 a changé la façon dont les callbacks de collision sont gérés
- [x] Box2D 3 utilise des méthodes différentes pour accéder aux données utilisateur
- [x] Problèmes de namespace avec les modèles Ship, Fleet et City

## Erreurs de compilation

### Erreurs liées à Box2D

- [x] Identifier `b2BodyId` utilise `index1` au lieu de `index`
- [x] Modifier les fonctions `IsValid` pour vérifier `index1` au lieu de `index`
- [x] Rechercher toutes les utilisations de `b2Body_GetDefinition` et `b2Body_SetDefinition` et les remplacer
- [x] Adapter les méthodes `GetBodyAngle`, `SetBodyFixedRotation`, `SetBodyBullet` et autres helpers
- [x] Mise à jour de la gestion des UserData pour utiliser le nouveau système
- [x] Adapter les transformations dans `PhysicsComponent.cpp`
- [x] Mettre à jour les callbacks de collision dans ContactListener

### Erreurs liées à SFML

- [x] Mettre à jour les références à `sf::Lines` vers `sf::PrimitiveType::Lines`
- [x] Mettre à jour les références à `sf::Quads` vers `sf::PrimitiveType::Quads`
- [x] Adapter les conversions d'angle avec `sf::degrees()`

### Erreurs dans le système de particules

- [x] Résoudre les redéfinitions entre ParticleComponent et ParticleSystem
- [x] Ajouter la méthode `updateVertices()` dans ParticleComponent
- [x] Créer le fichier ParticleTypes.hpp pour centraliser les définitions communes
- [x] Mettre à jour ParticleComponent.cpp pour être compatible avec les changements

### Erreurs dans les modèles (Ship, Fleet, City)

- [x] Résoudre les erreurs de namespace dans Ship.cpp
- [x] Ajouter les méthodes manquantes dans Ship (`setType(Type)`, `addCrewMember`)
- [x] Ajouter le namespace Orenji dans Fleet.hpp et Fleet.cpp
- [ ] Corriger les warnings d'ordre d'initialisation dans City

## Plan d'action pour résoudre les erreurs

### 1. Mise à jour Box2D

- [x] Modifier `Box2DTypes.hpp` pour la compatibilité avec Box2D 3
- [x] Adapter les transformations dans `PhysicsComponent.cpp`
- [x] Mettre à jour le ContactListener pour utiliser les nouveaux types

### 2. Compatibilité SFML 3

- [x] Mettre à jour les références aux PrimitiveTypes dans tous les fichiers
- [x] Adapter les références aux angles et transformations

### 3. Résolution des duplications

- [x] Vérifier les conflits entre ParticleComponent et ParticleSystem
- [x] Restructurer les composants de particules si nécessaire

### 4. Intégration des modèles

- [x] Corriger les erreurs dans Ship
- [x] Corriger les erreurs dans Fleet
- [ ] Corriger les erreurs dans City
- [ ] Ajouter les méthodes manquantes dans les modèles restants

### 5. Implémentation des utilitaires

- [x] Créer le fichier MathUtils.hpp
- [x] Adapter les namespace pour les utilitaires mathématiques

## Erreurs spécifiques rencontrées lors de la dernière compilation

Les erreurs actuellement identifiées lors de la dernière compilation incluent:

1. ~~Erreurs liées aux identifiants Box2D (`index` vs `index1`)~~
2. ~~Erreurs liées aux transformations Box2D~~
3. ~~Erreurs liées aux données utilisateur Box2D~~
4. ~~Erreurs liées au modèle Ship et à ses méthodes manquantes~~
5. ~~Erreurs liées au modèle Fleet et à l'absence de namespace~~
6. Erreurs liées aux warnings d'initialisation dans City et autres modèles

## Conclusion et prochaines étapes

Nous avons progressé dans l'adaptation du code pour SFML 3 et Box2D 3:

- ✅ Les problèmes principaux de Box2D ont été résolus (PhysicsComponent, Box2DTypes)
- ✅ Les problèmes liés à SFML 3 ont été corrigés (PrimitiveTypes, angles)
- ✅ Les problèmes du système de particules ont été adressés
- ✅ La mise à jour des callbacks de collision a été effectuée
- ✅ Les erreurs liées au modèle Ship ont été corrigées
- ✅ Les erreurs liées au modèle Fleet ont été corrigées

Prochaines étapes:

1. Corriger les erreurs dans le modèle City
2. Résoudre les warnings d'ordre d'initialisation
3. Effectuer des tests pour vérifier que la physique fonctionne correctement
4. Documenter les changements majeurs effectués

Il est recommandé de continuer en se concentrant sur les erreurs du modèle City et les warnings d'initialisation, qui sont les principaux problèmes restants. 

# Orenji-CPP Project Status and TODO List

## Adaptation pour SFML 3 et Box2D 3

### Problèmes identifiés
- [x] SFML 3 change la façon dont les primitives sont utilisées (`sf::Lines` -> `sf::PrimitiveType::Lines`)
- [x] SFML 3 change certains aspects de l'API `VertexArray` et `Vertex`
- [x] Box2D 3 utilise un système d'identifiants différent (b2BodyId, b2JointId, etc.)
- [x] Box2D 3 a changé la façon dont les callbacks de collision sont gérés
- [x] Box2D 3 utilise des méthodes différentes pour accéder aux données utilisateur
- [x] Problèmes de namespace avec les modèles Ship, Fleet et City

### Erreurs de compilation
- [x] `b2JointId`, `b2WorldId`, et `b2ShapeId` n'ont pas de membre `index`
- [x] Les fonctions `b2Body_GetDefinition` et `b2Body_SetDefinition` ne sont pas reconnues
- [x] Erreur avec `b2BodyUserData` et son utilisation dans `SetBodyUserData`
- [x] Problèmes de namespace avec les modèles Ship, Fleet et City
- [x] Classes manquantes dans les modèles (Fleet et City nécessitent des méthodes supplémentaires pour le commerce)
- [x] Problèmes avec la classe Player (référence aux classes non qualifiées par le namespace Orenji)
- [x] Redéfinitions dans les classes ParticleSystem et ParticleComponent

### Plan d'action
- [x] Mise à jour de Box2D
  - [x] Adaptation des structures pour utiliser les nouveaux identifiants
  - [x] Mise à jour des fonctions de callback pour les collisions
  - [x] Résolution des problèmes avec les userData
- [x] Compatibilité SFML 3
  - [x] Mise à jour des références à `sf::PrimitiveType::Lines` au lieu de `sf::Lines`
  - [x] Adaptation des structures VertexArray et Vertex
- [x] Correction des duplications de définitions
  - [x] Résolution des problèmes de redéfinition entre les différents en-têtes
- [x] Intégration des modèles
  - [x] Ajout des méthodes manquantes dans Ship
  - [x] Mise à jour des namespaces pour Fleet et City
  - [x] Implémentation des méthodes de commerce pour City
  - [x] Ajout du namespace Orenji pour Character
  - [x] Mise à jour de la classe Player pour utiliser le namespace Orenji
  - [ ] Résolution des problèmes avec TradeGood et TradingSystem
- [ ] Implémentation des utilitaires
  - [ ] Ajout des classes auxiliaires pour la gestion du jeu
  - [ ] Tests des composants mis à jour

### Erreurs spécifiques rencontrées lors de la dernière compilation
- [x] Problèmes avec le modèle Fleet - résolu en ajoutant le namespace Orenji
- [x] Problèmes avec le modèle City - résolu en ajoutant le namespace Orenji et les méthodes manquantes (hasGood, getGoodQuantity, removeGood)
- [x] Problèmes avec le modèle Character - résolu en ajoutant le namespace Orenji
- [x] Problèmes avec la classe Player - résolu en ajoutant le namespace Orenji et en qualifiant les références aux autres classes
- [ ] Problèmes avec TradeGood et TradingSystem:
  - [ ] Mise à jour des références pour utiliser les classes avec leur namespace Orenji complet
  - [ ] Adaptation des méthodes pour correspondre aux interfaces mises à jour
- [x] Problèmes avec ParticleSystem:
  - [x] Redéfinition de SimpleParticle, EmissionParameters et autres structures
  - [x] Configuration incorrecte des constructeurs et des méthodes
- [ ] Plusieurs avertissements concernant l'initialisation de variables

## Système de particules
- [x] Mise à jour pour SFML 3
- [x] Résolution des problèmes de compatibilité
- [x] Correction des redéfinitions entre ParticleSystem et ParticleComponent
- [x] Création d'un fichier ParticleTypes.hpp pour centraliser les définitions
- [x] Implémentation d'une version alternative sans Thor
- [x] Mise à jour de tous les fichiers liés au système de particules pour utiliser les nouvelles définitions

## Système physique
- [x] Adaptation pour Box2D 3
- [x] Mise à jour des callbacks de collision
- [x] Correction des erreurs liées aux userData
- [ ] Problèmes restants avec PhysicsComponent:
  - [ ] getTransformable non défini dans Entity
  - [ ] Problèmes avec b2FixtureId
  - [ ] Erreurs de conversion pour b2Body_SetTransform

## Prochaines étapes:
1. Finaliser l'adaptation du système physique pour Box2D 3
2. Résoudre les problèmes avec TradeGood et TradingSystem
3. Tester la compilation et la fonctionnalité du système de particules
4. Nettoyer les avertissements restants

## Tests et validation
- [ ] Tests des composants mis à jour
- [ ] Vérification de la compatibilité cross-platform

## Documentation
- [ ] Mise à jour des commentaires de code
- [ ] Documentation des changements pour SFML 3 et Box2D 3 