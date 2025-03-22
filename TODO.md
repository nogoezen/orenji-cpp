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

## Erreurs de compilation détaillées (dernière exécution)

### Erreurs Box2D 3
1. Structure des identifiants incompatible:
   - Remplacer `bodyId.index` par `bodyId.index1` dans toutes les fonctions `IsValid`
   - De même pour `fixtureId.index`, `jointId.index`, `worldId.index` et `shapeId.index`

2. Fonctions manquantes dans Box2D 3:
   - `b2Body_GetDefinition` et `b2Body_SetDefinition` n'existent pas dans Box2D 3
   - `b2BodyDef` a changé: `bullet` est devenu `isBullet`
   - `b2BodyUserData` n'est pas défini
   - Problème avec `GetBodyFixtureList`, `GetContactBodyA`, etc.

3. Types manquants ou renommés:
   - `b2ContactId` n'est pas déclaré
   - `b2FixtureId` n'est pas reconnu dans plusieurs contextes
   - `b2ContactEvent` vs `b2ContactEvents`
   - `b2StackAllocator` manquant

4. Problèmes avec les transformations:
   - Erreur lors de l'appel à `b2Body_SetTransform` qui attend maintenant différents arguments
   - `b2Body_GetAngle` remplacé par autre chose (peut-être `b2Rot_GetAngle`)

### Erreurs SFML 3
1. Problèmes de conversion de types:
   - Impossible de convertir `float` en `sf::Angle` dans `setRotation`
   - `transform.getPosition()` retourne une valeur et non une référence
   - Signature différente pour certains constructeurs

### Erreurs du système de particules
1. Redéfinitions multiples:
   - `SimpleParticle` défini à la fois dans `ParticleComponent.hpp` et `ParticleSystem.hpp`
   - `EmissionParameters` défini dans les deux fichiers
   - `AffectorType` et `ParticleTriggerType` également dupliqués

2. Problèmes d'implémentation:
   - Méthode `draw` marquée comme `override` mais ne surcharge rien
   - Membres non déclarés comme `m_particleComponents`, `position`, `texture`, etc.
   - Classe `ParticleSystem` avec constructeur privé et problème avec `std::make_unique`

### Erreurs des modèles (Ship, Fleet, City)
1. Problèmes de namespace:
   - `Ship` utilisé au lieu de `Orenji::Ship` dans `Fleet.hpp`
   - Erreurs avec les templates comme `std::shared_ptr<Ship>`

2. Méthodes manquantes:
   - `City` n'a pas les méthodes `hasGood`, `getGoodQuantity`, `removeGood`, etc.
   - `Player` n'a pas les méthodes `getFaction`, `getMoney`, `getShip`, etc.

3. Erreurs d'ordre d'initialisation et de types:
   - Warnings d'ordre d'initialisation dans `Player`
   - Erreurs de types incompatibles dans les opérateurs ternaires

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

- [x] Créer le fichier MathUtils.hpp si nécessaire
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
- [ ] Redéfinitions dans les classes ParticleSystem et ParticleComponent

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
- [ ] Problèmes avec ParticleSystem:
  - [ ] Redéfinition de SimpleParticle, EmissionParameters et autres structures
  - [ ] Configuration incorrecte des constructeurs et des méthodes
- [ ] Plusieurs avertissements concernant l'initialisation de variables

## Système de particules
- [x] Mise à jour pour SFML 3
- [x] Résolution des problèmes de compatibilité
- [ ] Correction des redéfinitions entre ParticleSystem et ParticleComponent

## Système physique
- [x] Adaptation pour Box2D 3
- [x] Mise à jour des callbacks de collision
- [x] Correction des erreurs liées aux userData
- [ ] Problèmes restants avec PhysicsComponent:
  - [ ] getTransformable non défini dans Entity
  - [ ] Problèmes avec b2FixtureId
  - [ ] Erreurs de conversion pour b2Body_SetTransform

## Prochaines étapes:
1. Finaliser les corrections du système de particules pour éliminer les redéfinitions
2. Compléter l'adaptation de PhysicsComponent pour Box2D 3
3. Mettre à jour TradingSystem pour utiliser correctement les classes avec leur namespace
4. Résoudre les avertissements d'initialisation des variables
5. Effectuer des tests complets pour s'assurer que tous les systèmes fonctionnent correctement ensemble

## Tests et validation
- [ ] Tests des composants mis à jour
- [ ] Vérification de la compatibilité cross-platform

## Documentation
- [ ] Mise à jour des commentaires de code
- [ ] Documentation des changements pour SFML 3 et Box2D 3 