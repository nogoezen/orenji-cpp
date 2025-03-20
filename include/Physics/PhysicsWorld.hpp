#pragma once
#include <box2d/box2d.h>
#include <memory>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>

namespace Orenji
{

    // Constantes pour la conversion entre pixels et mètres Box2D
    constexpr float PIXELS_PER_METER = 32.0f;
    constexpr float METERS_PER_PIXEL = 1.0f / PIXELS_PER_METER;

    // Types de collisions
    enum class CollisionCategory
    {
        NONE = 0,
        PLAYER = 1 << 0,
        ENEMY = 1 << 1,
        OBSTACLE = 1 << 2,
        SENSOR = 1 << 3,
        PROJECTILE = 1 << 4,
        ITEM = 1 << 5,
        ALL = 0xFFFF
    };

    // Gestionnaire de contacts Box2D
    class ContactListener : public b2ContactListener
    {
    public:
        using BeginContactCallback = std::function<void(b2Contact *)>;
        using EndContactCallback = std::function<void(b2Contact *)>;
        using PreSolveCallback = std::function<void(b2Contact *, const b2Manifold *)>;
        using PostSolveCallback = std::function<void(b2Contact *, const b2ContactImpulse *)>;

        void BeginContact(b2Contact *contact) override;
        void EndContact(b2Contact *contact) override;
        void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) override;
        void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) override;

        void SetBeginContactCallback(BeginContactCallback callback);
        void SetEndContactCallback(EndContactCallback callback);
        void SetPreSolveCallback(PreSolveCallback callback);
        void SetPostSolveCallback(PostSolveCallback callback);

    private:
        BeginContactCallback m_beginContactCallback;
        EndContactCallback m_endContactCallback;
        PreSolveCallback m_preSolveCallback;
        PostSolveCallback m_postSolveCallback;
    };

    // Gestionnaire principal du monde physique
    class PhysicsWorld
    {
    public:
        PhysicsWorld(const sf::Vector2f &gravity = sf::Vector2f(0.0f, 9.8f));
        ~PhysicsWorld();

        // Singleton
        static PhysicsWorld &getInstance();

        // Conversion entre pixels SFML et mètres Box2D
        static sf::Vector2f metersToPixels(const b2Vec2 &meterPos);
        static b2Vec2 pixelsToMeters(const sf::Vector2f &pixelPos);
        static float metersToPixels(float meterValue);
        static float pixelsToMeters(float pixelValue);

        // Mettre à jour le monde physique
        void update(float deltaTime);

        // Créer des corps physiques
        b2Body *createBody(const sf::Vector2f &position, b2BodyType type);
        void destroyBody(b2Body *body);

        // Ajouter des formes aux corps
        b2Fixture *addBoxFixture(b2Body *body, const sf::Vector2f &size, float density = 1.0f,
                                 float friction = 0.3f, float restitution = 0.1f,
                                 uint16_t categoryBits = static_cast<uint16_t>(CollisionCategory::ALL),
                                 uint16_t maskBits = static_cast<uint16_t>(CollisionCategory::ALL),
                                 bool isSensor = false);

        b2Fixture *addCircleFixture(b2Body *body, float radius, float density = 1.0f,
                                    float friction = 0.3f, float restitution = 0.1f,
                                    uint16_t categoryBits = static_cast<uint16_t>(CollisionCategory::ALL),
                                    uint16_t maskBits = static_cast<uint16_t>(CollisionCategory::ALL),
                                    bool isSensor = false);

        // Accès au contact listener
        ContactListener &getContactListener();

        // Debug drawing pour visualiser les formes Box2D
        void debugDraw(sf::RenderWindow &window);

    private:
        static std::unique_ptr<PhysicsWorld> s_instance;

        std::unique_ptr<b2World> m_world;
        ContactListener m_contactListener;

        // Pour le debug drawing
        sf::VertexArray m_debugLines;
        bool m_debugDrawEnabled;

        // Paramètres de la simulation
        const int m_velocityIterations = 8;
        const int m_positionIterations = 3;
    };

} // namespace Orenji