# Guide de Migration - SFML 3.0 et Box2D 3.0

Ce document détaille les modifications nécessaires pour mettre à jour le moteur Orenji-CPP vers SFML 3.0 et Box2D 3.0.

## Migration vers SFML 3.0

SFML 3.0 introduit plusieurs changements d'API importants par rapport à la version 2.x.

### Principaux changements

#### Classe `sf::Vector2f` et constructeurs

- Les constructeurs prennent maintenant un seul paramètre (une paire de coordonnées) au lieu de deux paramètres séparés
- Avant: `sf::Vector2f(x, y)`
- Après: `sf::Vector2f({x, y})`

```cpp
// SFML 2.x
sf::Vector2f position(10.0f, 20.0f);

// SFML 3.0
sf::Vector2f position({10.0f, 20.0f});
```

#### Classe `sf::VideoMode`

- Le constructeur prend maintenant un `sf::Vector2u` au lieu de paramètres séparés
- Avant: `sf::VideoMode(width, height)`
- Après: `sf::VideoMode({width, height})`

```cpp
// SFML 2.x
sf::VideoMode mode(800, 600);

// SFML 3.0
sf::VideoMode mode({800, 600});
```

#### Classes `sf::Rect`, `sf::FloatRect`, `sf::IntRect`

- Les propriétés `left`, `top`, `width`, `height` sont remplacées par `position` et `size`
- Avant: `rect.left`, `rect.top`, `rect.width`, `rect.height`
- Après: `rect.position.x`, `rect.position.y`, `rect.size.x`, `rect.size.y`

```cpp
// SFML 2.x
sf::FloatRect rect(10.0f, 20.0f, 30.0f, 40.0f);
float left = rect.left;
float right = rect.left + rect.width;

// SFML 3.0
sf::FloatRect rect({10.0f, 20.0f}, {30.0f, 40.0f});
float left = rect.position.x;
float right = rect.position.x + rect.size.x;
```

#### Gestion des événements

- `pollEvent()` retourne maintenant un `std::optional<sf::Event>` au lieu de prendre une référence en paramètre
- Les types d'événements ne sont plus accessibles via `sf::Event::[Type]`, mais via `sf::EventType::[Type]`
- Les membres ne sont plus accessibles via des méthodes "is" mais directement via des propriétés

```cpp
// SFML 2.x
sf::Event event;
while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        window.close();
    }
}

// SFML 3.0
if (auto event = window.pollEvent()) {
    if (event->type == sf::EventType::Closed) {
        window.close();
    }
    if (event->type == sf::EventType::KeyPressed && event->key.code == sf::Keyboard::Key::Escape) {
        window.close();
    }
}
```

#### Autres changements importants

- `sf::Uint8` est remplacé par `uint8_t` (C++ standard)
- La méthode `mapCoordsToPixel` retourne maintenant un `sf::Vector2i` au lieu d'un `sf::Vector2f`
- Les sprites et textures ont besoin d'une construction explicite et ne peuvent plus être créés vides

## Migration vers Box2D 3.0

Box2D 3.0 adopte une approche complètement différente, orientée vers un modèle de programmation basé sur des ID plutôt que sur des pointeurs d'objets.

### Principaux changements

#### Création d'objets

- Les objets (World, Body, Fixture) sont maintenant référencés par des ID (`b2WorldId`, `b2BodyId`, etc.) au lieu de pointeurs
- La création se fait via des fonctions globales au lieu de méthodes d'objet

```cpp
// Box2D 2.x
b2World* world = new b2World(b2Vec2(0.0f, 9.8f));
b2Body* body = world->CreateBody(&bodyDef);

// Box2D 3.0
b2WorldDef worldDef;
worldDef.gravity = {0.0f, 9.8f};
b2WorldId worldId = b2CreateWorld(&worldDef);
b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
```

#### Définition de corps et propriétés

- `b2BodyDef` a changé avec de nombreuses propriétés renommées ou restructurées
- `angle` est maintenant défini via `transform.q` (quaternion)
- `bullet` est renommé en `isBullet`
- `allowSleep` est renommé en `enableSleep`

```cpp
// Box2D 2.x
b2BodyDef bodyDef;
bodyDef.type = b2_dynamicBody;
bodyDef.position.Set(0.0f, 0.0f);
bodyDef.angle = angle * b2_pi / 180.0f;
bodyDef.bullet = true;
bodyDef.allowSleep = false;

// Box2D 3.0
b2BodyDef bodyDef;
bodyDef.type = b2_dynamicBody;
bodyDef.position = {0.0f, 0.0f};
// Angle défini via quaternion
float angleRad = angle * B2_pi / 180.0f;
bodyDef.transform.q.s = sinf(angleRad);
bodyDef.transform.q.c = cosf(angleRad);
bodyDef.isBullet = true;
bodyDef.enableSleep = false;
```

#### Formes géométriques

- Les classes `b2CircleShape`, `b2PolygonShape` sont remplacées par des structures `b2Circle`, `b2Polygon`
- Les méthodes pour définir les formes sont maintenant des fonctions globales
- La création des formes utilise de nouvelles structures comme `b2Hull`

```cpp
// Box2D 2.x
b2CircleShape circle;
circle.m_radius = 1.0f;
circle.m_p.Set(0.0f, 0.0f);

// Box2D 3.0
b2Circle circle;
circle.radius = 1.0f;
circle.center = {0.0f, 0.0f};
```

Pour les polygones:

```cpp
// Box2D 2.x
b2PolygonShape box;
box.SetAsBox(1.0f, 1.0f);

// Box2D 3.0
b2Hull hull;
hull.points[0] = {-1.0f, -1.0f};
hull.points[1] = {1.0f, -1.0f};
hull.points[2] = {1.0f, 1.0f};
hull.points[3] = {-1.0f, 1.0f};
hull.count = 4;

b2Hull canonicalHull;
b2ComputeHull(&hull, &canonicalHull);

b2Polygon polygon;
b2CreatePolygonFromHull(&canonicalHull, &polygon);
```

#### Manipulation du monde physique

- Les méthodes d'objet sont remplacées par des fonctions globales

```cpp
// Box2D 2.x
world->Step(timeStep, velocityIterations, positionIterations);
world->SetGravity(b2Vec2(0.0f, 9.8f));

// Box2D 3.0
b2World_Step(worldId, timeStep, 1); // subStepCount = 1
b2World_SetGravity(worldId, {0.0f, 9.8f});
```

## Astuces pour la migration

1. Commencez par convertir les structures de base comme `sf::Vector2f`, `sf::FloatRect` et `sf::VideoMode`
2. Mettez à jour la gestion des événements pour utiliser le nouveau modèle basé sur `std::optional`
3. Pour Box2D, créez d'abord une couche d'abstraction (wrapper) qui cache la complexité de l'API
4. Testez chaque composant individuellement avant d'intégrer tous les changements

## Fonctionnalités des nouvelles versions

### SFML 3.0

- Meilleure prise en charge du HDPI et des écrans haute résolution
- Mise à jour du système audio avec plus de formats supportés
- Amélioration des performances de rendu
- Support pour les shaders GLSL modernes

### Box2D 3.0

- Performances améliorées grâce à un design plus orienté données
- Meilleure stabilité pour les simulations complexes
- Support pour les opérations en parallèle
- Meilleure gestion de la mémoire 