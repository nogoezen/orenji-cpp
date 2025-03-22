# Changements d'API dans Box2D 3.0

Ce document détaille les principales modifications de l'API dans Box2D 3.0 par rapport à Box2D 2.x. Il sert de référence pour mettre à jour votre code.

## 1. Changement fondamental : Pointeurs vs IDs

La différence la plus importante dans Box2D 3.0 est le passage d'un système orienté-objet avec des pointeurs à un système orienté-données avec des identifiants.

### Monde physique

**Box2D 2.x**:
```cpp
b2World* world = new b2World(b2Vec2(0.0f, 9.8f));
// ...
delete world;
```

**Box2D 3.0**:
```cpp
b2WorldDef worldDef;
worldDef.gravity = {0.0f, 9.8f};
b2WorldId worldId = b2CreateWorld(&worldDef);
// ...
b2DestroyWorld(worldId);
```

### Corps physiques

**Box2D 2.x**:
```cpp
b2BodyDef bodyDef;
bodyDef.type = b2_dynamicBody;
bodyDef.position = b2Vec2(0.0f, 0.0f);
b2Body* body = world->CreateBody(&bodyDef);
// ...
world->DestroyBody(body);
```

**Box2D 3.0**:
```cpp
b2BodyDef bodyDef;
bodyDef.type = b2_dynamicBody;
bodyDef.position = {0.0f, 0.0f};
b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
// ...
b2DestroyBody(bodyId);
```

## 2. Définitions de corps

### Propriétés de base

**Box2D 2.x**:
```cpp
b2BodyDef bodyDef;
bodyDef.type = b2_dynamicBody;
bodyDef.position.Set(0.0f, 0.0f);
bodyDef.angle = 45.0f * b2_pi / 180.0f;
bodyDef.linearVelocity.Set(1.0f, 0.0f);
bodyDef.angularVelocity = 0.0f;
bodyDef.linearDamping = 0.0f;
bodyDef.angularDamping = 0.0f;
bodyDef.allowSleep = true;
bodyDef.awake = true;
bodyDef.fixedRotation = false;
bodyDef.bullet = false;
bodyDef.active = true;
bodyDef.userData = nullptr;
```

**Box2D 3.0**:
```cpp
b2BodyDef bodyDef;
bodyDef.type = b2_dynamicBody;
bodyDef.position = {0.0f, 0.0f};
// Angle défini via quaternion
float angle = 45.0f * B2_pi / 180.0f;
bodyDef.transform.q.s = sinf(angle / 2.0f);
bodyDef.transform.q.c = cosf(angle / 2.0f);
bodyDef.linearVelocity = {1.0f, 0.0f};
bodyDef.angularVelocity = 0.0f;
bodyDef.linearDamping = 0.0f;
bodyDef.angularDamping = 0.0f;
bodyDef.enableSleep = true;     // renommé
bodyDef.isAwake = true;         // renommé
bodyDef.lockRotation = false;   // renommé
bodyDef.isBullet = false;       // renommé
bodyDef.isEnabled = true;       // renommé
bodyDef.userData.pointer = 0;   // changement de structure
```

### Gestion de l'angle

**Box2D 2.x**:
```cpp
bodyDef.angle = 45.0f * b2_pi / 180.0f;
```

**Box2D 3.0**:
```cpp
float angle = 45.0f * B2_pi / 180.0f;
// Utilisation d'un quaternion
bodyDef.transform.q.s = sinf(angle / 2.0f);
bodyDef.transform.q.c = cosf(angle / 2.0f);
```

## 3. Formes (Shapes)

### Cercles

**Box2D 2.x**:
```cpp
b2CircleShape circle;
circle.m_radius = 1.0f;
circle.m_p.Set(0.0f, 0.0f);  // Position du centre relative au corps
```

**Box2D 3.0**:
```cpp
b2Circle circle;
circle.radius = 1.0f;
circle.center = {0.0f, 0.0f};  // Position du centre relative au corps
```

### Boîtes (Rectangles)

**Box2D 2.x**:
```cpp
b2PolygonShape box;
box.SetAsBox(1.0f, 0.5f);  // Demi-largeur et demi-hauteur
```

**Box2D 3.0**:
```cpp
// Définir les points du Hull manuellement
b2Hull hull;
hull.points[0] = {-1.0f, -0.5f};
hull.points[1] = {1.0f, -0.5f};
hull.points[2] = {1.0f, 0.5f};
hull.points[3] = {-1.0f, 0.5f};
hull.count = 4;

// Calculer le Hull canonique (optionnel mais recommandé)
b2Hull canonicalHull;
b2ComputeHull(&hull, &canonicalHull);

// Créer le polygon à partir du Hull
b2Polygon polygon;
b2CreatePolygonFromHull(&canonicalHull, &polygon);
```

### Polygones

**Box2D 2.x**:
```cpp
b2PolygonShape polygon;
b2Vec2 vertices[3];
vertices[0].Set(0.0f, 0.0f);
vertices[1].Set(1.0f, 0.0f);
vertices[2].Set(0.0f, 1.0f);
polygon.Set(vertices, 3);
```

**Box2D 3.0**:
```cpp
b2Hull hull;
hull.points[0] = {0.0f, 0.0f};
hull.points[1] = {1.0f, 0.0f};
hull.points[2] = {0.0f, 1.0f};
hull.count = 3;

b2Hull canonicalHull;
b2ComputeHull(&hull, &canonicalHull);

b2Polygon polygon;
b2CreatePolygonFromHull(&canonicalHull, &polygon);
```

## 4. Fixtures

### Création de fixtures

**Box2D 2.x**:
```cpp
b2FixtureDef fixtureDef;
fixtureDef.shape = &shape;
fixtureDef.density = 1.0f;
fixtureDef.friction = 0.3f;
fixtureDef.restitution = 0.5f;
fixtureDef.isSensor = false;
b2Fixture* fixture = body->CreateFixture(&fixtureDef);
```

**Box2D 3.0**:
```cpp
b2FixtureDef fixtureDef;
fixtureDef.shape = &polygon;  // ou &circle
fixtureDef.density = 1.0f;
fixtureDef.friction = 0.3f;
fixtureDef.restitution = 0.5f;
fixtureDef.isSensor = false;
b2ShapeId shapeId = b2CreateShape(bodyId, &fixtureDef);
```

### Destruction de fixtures

**Box2D 2.x**:
```cpp
body->DestroyFixture(fixture);
```

**Box2D 3.0**:
```cpp
b2DestroyShape(shapeId);
```

## 5. Interactions avec les corps

### Accès aux propriétés

**Box2D 2.x**:
```cpp
b2Vec2 position = body->GetPosition();
float angle = body->GetAngle();
b2Vec2 velocity = body->GetLinearVelocity();
float angularVelocity = body->GetAngularVelocity();
```

**Box2D 3.0**:
```cpp
b2BodyState bodyState = b2Body_GetState(bodyId);
b2Vec2 position = bodyState.transform.p;
// Conversion du quaternion en angle
float angle = 2.0f * atan2f(bodyState.transform.q.s, bodyState.transform.q.c);
b2Vec2 velocity = bodyState.linearVelocity;
float angularVelocity = bodyState.angularVelocity;
```

### Modification des propriétés

**Box2D 2.x**:
```cpp
body->SetTransform(b2Vec2(1.0f, 2.0f), 0.0f);
body->SetLinearVelocity(b2Vec2(1.0f, 0.0f));
body->SetAngularVelocity(1.0f);
body->ApplyForce(b2Vec2(10.0f, 0.0f), body->GetWorldCenter(), true);
body->ApplyTorque(5.0f, true);
body->ApplyLinearImpulse(b2Vec2(0.0f, 5.0f), body->GetWorldCenter(), true);
body->ApplyAngularImpulse(1.0f, true);
```

**Box2D 3.0**:
```cpp
// Définir la position et l'angle
float angle = 0.0f;
b2CosSin cs = {cosf(angle), sinf(angle)};
b2Transform transform = {b2Vec2(1.0f, 2.0f), cs};
b2Body_SetTransform(bodyId, &transform);

// Définir les vélocités
b2Body_SetLinearVelocity(bodyId, {1.0f, 0.0f});
b2Body_SetAngularVelocity(bodyId, 1.0f);

// Appliquer des forces et impulsions
b2Vec2 worldPosition = b2Body_GetWorldCenter(bodyId);
b2Body_ApplyForce(bodyId, {10.0f, 0.0f}, worldPosition, true);
b2Body_ApplyTorque(bodyId, 5.0f, true);
b2Body_ApplyLinearImpulse(bodyId, {0.0f, 5.0f}, worldPosition, true);
b2Body_ApplyAngularImpulse(bodyId, 1.0f, true);
```

## 6. Simulation du monde

### Avancer le temps de simulation

**Box2D 2.x**:
```cpp
float timeStep = 1.0f / 60.0f;
int32 velocityIterations = 6;
int32 positionIterations = 2;
world->Step(timeStep, velocityIterations, positionIterations);
```

**Box2D 3.0**:
```cpp
float timeStep = 1.0f / 60.0f;
int32_t subStepCount = 1;
b2World_Step(worldId, timeStep, subStepCount);
```

### Gestion de la gravité

**Box2D 2.x**:
```cpp
world->SetGravity(b2Vec2(0.0f, -10.0f));
b2Vec2 gravity = world->GetGravity();
```

**Box2D 3.0**:
```cpp
b2World_SetGravity(worldId, {0.0f, -10.0f});
b2Vec2 gravity = b2World_GetGravity(worldId);
```

## 7. Contact et collision

### Accès aux contacts

**Box2D 2.x**:
```cpp
class MyContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact) override
    {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();
        // Gérer le contact
    }
};

MyContactListener contactListener;
world->SetContactListener(&contactListener);
```

**Box2D 3.0**:
```cpp
b2ContactEvents contactEvents;

// Configurer les callbacks
contactEvents.beginFcn = [](b2ContactBeginEvent* event, void* userData) {
    b2ShapeId shapeIdA = event->shapeIdA;
    b2ShapeId shapeIdB = event->shapeIdB;
    // Gérer le contact
};

// Enregistrer les événements
b2SetContactEvents(worldId, &contactEvents, nullptr);
```

## 8. Requêtes et tests

### Requêtes dans une zone (AABB)

**Box2D 2.x**:
```cpp
class MyQueryCallback : public b2QueryCallback
{
public:
    bool ReportFixture(b2Fixture* fixture) override
    {
        // Traiter le fixture
        return true; // Continuer la requête
    }
};

b2AABB aabb;
aabb.lowerBound.Set(0.0f, 0.0f);
aabb.upperBound.Set(10.0f, 10.0f);
MyQueryCallback callback;
world->QueryAABB(&callback, aabb);
```

**Box2D 3.0**:
```cpp
b2AABB aabb;
aabb.min = {0.0f, 0.0f};
aabb.max = {10.0f, 10.0f};

struct Context
{
    // Vos données
};
Context context;

b2QueryCallback callback = [](b2ShapeId shapeId, void* context) {
    Context* ctx = static_cast<Context*>(context);
    // Traiter la forme
    return true; // Continuer la requête
};

b2World_Query(worldId, &aabb, callback, &context);
```

### Lancer de rayon (Raycast)

**Box2D 2.x**:
```cpp
class MyRayCastCallback : public b2RayCastCallback
{
public:
    float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
                      const b2Vec2& normal, float fraction) override
    {
        // Traiter l'intersection
        return fraction; // Continuer jusqu'à la fin
    }
};

b2Vec2 point1(0.0f, 0.0f);
b2Vec2 point2(10.0f, 10.0f);
MyRayCastCallback callback;
world->RayCast(&callback, point1, point2);
```

**Box2D 3.0**:
```cpp
b2Vec2 point1 = {0.0f, 0.0f};
b2Vec2 point2 = {10.0f, 10.0f};

struct Context
{
    // Vos données
};
Context context;

b2RaycastCallback callback = [](const b2RaycastHit* hit, void* context) {
    Context* ctx = static_cast<Context*>(context);
    // Traiter l'intersection
    return hit->fraction; // Continuer jusqu'à la fin
};

b2World_RayCast(worldId, &point1, &point2, callback, &context);
```

## Conseils de migration

1. Créez une couche d'abstraction (wrapper) autour de Box2D pour isoler les changements d'API
2. Convertissez d'abord la création et la gestion du monde physique
3. Mettez à jour les définitions de corps et de formes
4. Adaptez la gestion des fixtures et contacts
5. Remplacez toutes les références aux pointeurs par des IDs
6. Mettez à jour les callbacks pour utiliser les nouvelles signatures de fonctions

## Avantages de Box2D 3.0

- Architecture orientée données qui permet de meilleures performances
- Meilleure séparation entre la simulation et les lectures d'état
- Possibilité de simulation parallèle
- API plus cohérente et robuste
- Meilleure stabilité pour les simulations complexes 