# TODO List pour le projet Orenji-CPP

## Adaptation aux nouvelles versions des librairies
- [x] SFML 3 - Mettre à jour les API et types
- [ ] Box2D 3 - Adapter le code au nouveau API
    - [ ] Mettre à jour PhysicsWorld.cpp pour utiliser les nouveaux types et fonctions
    - [ ] Mettre à jour Box2DTypes.hpp

## Erreurs de compilation identifiées
- [ ] Box2D:
  - [ ] Appels à des fonctions qui n'existent plus dans Box2D 3 (b2Body_GetDefinition, b2Body_SetDefinition)
  - [ ] Structure des données différente (les id comme b2JointId n'ont plus d'index)
  - [ ] Problèmes avec les b2BodyUserData
  - [ ] Plusieurs erreurs de redéfinition

- [x] SFML 3:
  - [x] PrimitiveType est maintenant dans un namespace (sf::PrimitiveType::Quads au lieu de sf::Quads)
  - [x] Mise à jour des méthodes liées aux transformations (setPosition, rotate, etc.)

- [ ] Duplication:
  - [ ] SimpleParticle défini dans ParticleComponent.hpp et ParticleSystem.hpp
  - [ ] EmissionParameters défini à deux endroits

- [x] Implémentations manquantes:
  - [x] MathUtils.hpp référencé mais absent ou mal localisé
  - [x] InputHandler

## Plan d'action pour résoudre les erreurs

### Box2D
- [ ] Analyser complètement la différence d'API entre Box2D 2 et 3
- [ ] Mettre à jour PhysicsWorld.cpp pour remplacer les fonctions obsolètes
- [ ] Adapter les structures de données dans Box2DTypes.hpp

### SFML 3
- [x] Mettre à jour les références à PrimitiveType (sf::PrimitiveType::Quads)
- [x] Vérifier et adapter toutes les méthodes de transformation
- [x] Vérifier les signatures des fonctions de rendu

### Duplication
- [ ] Refactoriser pour avoir une seule définition de SimpleParticle
- [ ] Refactoriser pour avoir une seule définition d'EmissionParameters

### Implémentations
- [x] Créer le fichier MathUtils.hpp avec les fonctions nécessaires
- [x] Implémenter InputHandler.cpp

## Conclusion et prochaines étapes

### Progrès actuels
- [x] Mise à jour pour SFML 3 presque complète
- [x] Implémentation des fichiers manquants (MathUtils, InputHandler)
- [ ] Adaptation pour Box2D encore à faire
- [ ] Problèmes de duplication non résolus

### Approche recommandée
1. Terminer l'adaptation SFML 3
2. Résoudre les problèmes de duplication
3. Adapter le code pour Box2D 3

### Objectifs à court terme
- [ ] Compiler sans erreurs avec DISABLE_BOX2D défini
- [ ] Résoudre les problèmes de duplication

### Objectifs à moyen terme
- [ ] Adapter complètement à Box2D 3
- [ ] Compiler sans erreurs avec toutes les fonctionnalités activées 