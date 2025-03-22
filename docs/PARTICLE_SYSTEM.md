# Système de Particules Orenji

Ce document explique comment utiliser le système de particules de Orenji Engine, une solution légère et performante pour ajouter des effets visuels à vos jeux.

## Table des matières
1. [Introduction](#introduction)
2. [Architecture](#architecture)
3. [Concepts clés](#concepts-clés)
4. [Utilisation basique](#utilisation-basique)
5. [Effets avancés](#effets-avancés)
6. [Exemples](#exemples)
7. [Référence API](#référence-api)

## Introduction

Le système de particules Orenji permet de créer facilement des effets visuels comme du feu, de la fumée, des explosions, de la pluie, et bien plus encore. Il utilise SFML pour le rendu et est conçu pour être à la fois simple à utiliser et efficace.

## Architecture

Le système de particules est composé des éléments suivants:

- **ParticleSystem** : Singleton qui gère tous les émetteurs de particules
- **ParticleEmitter** : Gère la génération et les propriétés des particules
- **Particle** : Structure qui contient les données d'une particule individuelle

## Concepts clés

### Particules
Chaque particule possède:
- Une position
- Une vitesse
- Une couleur
- Une taille
- Une rotation
- Une durée de vie

### Émetteurs
Les émetteurs contrôlent comment et où les particules sont générées:
- Forme d'émission (point, cercle, rectangle)
- Taille de la zone d'émission
- Propriétés des particules (durée de vie, taille, vitesse, etc.)
- Taux d'émission

### Affecteurs
Les affecteurs sont des fonctions lambda qui modifient les particules pendant leur durée de vie:
- Gravité
- Vent
- Rotation
- Changements de couleur ou de taille
- Tout autre comportement personnalisé

## Utilisation basique

### 1. Initialisation du système

```cpp
// Obtenir l'instance du système de particules
ParticleSystem& particleSystem = ParticleSystem::getInstance();

// Créer un émetteur avec un nom et une capacité maximale
particleSystem.createEmitter("fire", 1000);
```

### 2. Configuration de l'émetteur

```cpp
// Obtenir une référence à l'émetteur
ParticleEmitter* emitter = particleSystem.getEmitter("fire");

// Configurer les propriétés de base
emitter->setShape(EmitterShape::Circle);
emitter->setSize(sf::Vector2f(10.f, 10.f));
emitter->setMinLife(1.0f);
emitter->setMaxLife(2.0f);
emitter->setMinStartSize(10.f);
emitter->setMaxStartSize(20.f);
emitter->setMinEndSize(0.f);
emitter->setMaxEndSize(5.f);
emitter->setMinVelocity(20.f);
emitter->setMaxVelocity(50.f);
emitter->setMinAngle(260.f);
emitter->setMaxAngle(280.f);
emitter->setStartColor(sf::Color(255, 200, 50, 200));
emitter->setEndColor(sf::Color(200, 0, 0, 0));
```

### 3. Ajout d'un affecteur

```cpp
// Ajouter un affecteur pour simuler la gravité
emitter->addAffector([](Particle& particle, float dt) {
    particle.velocity.y += 50.f * dt;
});
```

### 4. Boucle de jeu

```cpp
// Dans votre boucle de jeu principale
while (window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();
    
    // Émettre des particules
    particleSystem.emit("fire", 10 * deltaTime);
    
    // Mettre à jour le système de particules
    particleSystem.update(deltaTime);
    
    // Dessiner le système de particules
    window.clear();
    window.draw(particleSystem);
    window.display();
}
```

### 5. Nettoyage

```cpp
// À la fin du programme
ParticleSystem::release();
```

## Effets avancés

### Contrôle des émissions

```cpp
// Émettre une rafale de particules
particleSystem.emit("explosion", 100);

// Arrêter l'émission
particleSystem.stopEmitter("fire");

// Supprimer toutes les particules d'un émetteur
particleSystem.clearEmitter("smoke");
```

### Affecteurs complexes

```cpp
// Affecteur pour simuler le vent avec turbulence
emitter->addAffector([](Particle& particle, float dt) {
    // Force de base du vent
    float windForce = 20.f * dt;
    
    // Turbulence basée sur la position et le temps
    float turbulence = std::sin(particle.position.y * 0.01f + 
                              particle.life * 2.f) * 5.f * dt;
    
    // Application du vent
    particle.velocity.x += windForce + turbulence;
});

// Affecteur pour faire scintiller les particules
emitter->addAffector([](Particle& particle, float dt) {
    // Facteur aléatoire entre 0.8 et 1.0
    float factor = 0.8f + (std::rand() % 20) / 100.f;
    
    // Application du scintillement
    particle.color.r = static_cast<uint8_t>(particle.color.r * factor);
    particle.color.g = static_cast<uint8_t>(particle.color.g * factor);
    particle.color.b = static_cast<uint8_t>(particle.color.b * factor);
});
```

## Exemples

Le moteur inclut plusieurs exemples pour démontrer l'utilisation du système de particules:

1. **SimpleParticleDemo** - Démontre les bases du système
2. **ParticleExample** - Présente plusieurs effets (feu, fumée, explosion, fontaine, étincelles)

Pour compiler les exemples, activez l'option `BUILD_EXAMPLES` dans CMake:

```bash
cmake -DBUILD_EXAMPLES=ON ..
```

## Référence API

### ParticleSystem

```cpp
// Singleton - Obtention de l'instance
static ParticleSystem& getInstance();

// Libération des ressources
static void release();

// Création et gestion des émetteurs
ParticleEmitter* createEmitter(const std::string& name, unsigned int maxParticles);
ParticleEmitter* getEmitter(const std::string& name);
void removeEmitter(const std::string& name);

// Contrôle de l'émission
void emit(const std::string& name, unsigned int count);
void stopEmitter(const std::string& name);
void startEmitter(const std::string& name);
void clearEmitter(const std::string& name);

// Positionnement
void setEmitterPosition(const std::string& name, const sf::Vector2f& position);

// Mise à jour et rendu
void update(float deltaTime);
void draw(sf::RenderTarget& target, sf::RenderStates states) const;
```

### ParticleEmitter

```cpp
// Configuration de la forme
void setShape(EmitterShape shape);
void setSize(const sf::Vector2f& size);

// Configuration de la durée de vie
void setMinLife(float minLife);
void setMaxLife(float maxLife);

// Configuration de la taille
void setMinStartSize(float size);
void setMaxStartSize(float size);
void setMinEndSize(float size);
void setMaxEndSize(float size);

// Configuration de la vitesse
void setMinVelocity(float velocity);
void setMaxVelocity(float velocity);

// Configuration de l'angle
void setMinAngle(float angle);
void setMaxAngle(float angle);

// Configuration de la rotation
void setMinRotationSpeed(float speed);
void setMaxRotationSpeed(float speed);

// Configuration des couleurs
void setStartColor(const sf::Color& color);
void setEndColor(const sf::Color& color);

// Gestion des affecteurs
void addAffector(const ParticleAffector& affector);
void clearAffectors();

// Contrôle de l'émission
void emit(unsigned int count);
void start();
void stop();
void clear();

// Positionnement
void setPosition(const sf::Vector2f& position);
``` 