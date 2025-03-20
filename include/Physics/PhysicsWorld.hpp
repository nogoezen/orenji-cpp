#pragma once
#include <box2d/box2d.h>
#include <memory>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include "Box2DTypes.hpp"

namespace Orenji
{

    // Constantes pour la conversion entre pixels et mètres Box2D
    constexpr float PIXELS_PER_METER = 30.0f;
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

    // Type d'alias pour les fonctions de rappel de contact
    using BeginContactCallback = std::function<void(b2Contact *)>;
    using EndContactCallback = std::function<void(b2Contact *)>;
    using PreSolveCallback = std::function<void(b2Contact *, const b2Manifold *)>;
    using PostSolveCallback = std::function<void(b2Contact *, const b2ContactImpulse *)>;

    /**
     * @brief Classe d'écoute pour les événements de contact de Box2D
     *
     * Cette classe permet de définir des fonctions de rappel pour les différentes
     * phases de contact entre les corps physiques.
     */
    class ContactListener
    {
    public:
        // Méthodes de rappel pour les contacts
        void BeginContact(b2Contact *contact);
        void EndContact(b2Contact *contact);
        void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
        void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);

        // Méthodes pour définir les fonctions de rappel
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

    /**
     * @brief Adaptateur pour Box2D
     *
     * Structure pour adapter les callbacks avec Box2D 2.4.x
     */
    struct b2ContactListenerWrapper
    {
        void *userData;
        void (*beginContactFcn)(void *userdata, void *contact);
        void (*endContactFcn)(void *userdata, void *contact);
        void (*preSolveFcn)(void *userdata, void *contact, void *oldManifold);
        void (*postSolveFcn)(void *userdata, void *contact, void *impulse);
    };

    /**
     * @brief Classe de gestion du monde physique avec Box2D
     *
     * Cette classe encapsule le monde Box2D et fournit des méthodes utilitaires
     * pour gérer les corps, les fixtures et les interactions physiques.
     */
    class PhysicsWorld
    {
    public:
        /**
         * @brief Constructeur
         * @param gravity La force de gravité (par défaut (0, 9.8f))
         */
        PhysicsWorld(const sf::Vector2f &gravity = sf::Vector2f(0.0f, 9.8f));

        /**
         * @brief Destructeur
         */
        ~PhysicsWorld();

        /**
         * @brief Obtient l'instance du singleton
         * @return Référence vers l'instance unique
         */
        static PhysicsWorld &getInstance();

        /**
         * @brief Convertit une position en mètres (Box2D) en pixels (SFML)
         * @param meterPos Position en mètres
         * @return Position en pixels
         */
        static sf::Vector2f metersToPixels(const b2Vec2 &meterPos);

        /**
         * @brief Convertit une position en pixels (SFML) en mètres (Box2D)
         * @param pixelPos Position en pixels
         * @return Position en mètres
         */
        static b2Vec2 pixelsToMeters(const sf::Vector2f &pixelPos);

        /**
         * @brief Convertit une valeur en mètres en pixels
         * @param meterValue Valeur en mètres
         * @return Valeur en pixels
         */
        static float metersToPixels(float meterValue);

        /**
         * @brief Convertit une valeur en pixels en mètres
         * @param pixelValue Valeur en pixels
         * @return Valeur en mètres
         */
        static float pixelsToMeters(float pixelValue);

        /**
         * @brief Met à jour la simulation physique
         * @param deltaTime Temps écoulé depuis la dernière mise à jour en secondes
         */
        void update(float deltaTime);

        /**
         * @brief Crée un corps physique
         * @param position Position initiale en pixels
         * @param type Type de corps (statique, dynamique, cinématique)
         * @return Identifiant du corps créé
         */
        b2BodyId createBody(const sf::Vector2f &position, b2BodyType type);

        /**
         * @brief Détruit un corps physique
         * @param body Identifiant du corps à détruire
         */
        void destroyBody(b2BodyId body);

        /**
         * @brief Ajoute une fixture rectangulaire à un corps
         * @param body Identifiant du corps
         * @param size Dimensions du rectangle en pixels
         * @param density Densité de la fixture (masse volumique)
         * @param friction Coefficient de friction
         * @param restitution Coefficient de restitution (rebond)
         * @param categoryBits Bits de catégorie pour le filtrage des collisions
         * @param maskBits Bits de masque pour le filtrage des collisions
         * @param isSensor Si true, la fixture est un capteur (ne génère pas de collisions physiques)
         * @return Identifiant de la fixture créée
         */
        b2FixtureId addBoxFixture(b2BodyId body, const sf::Vector2f &size,
                                  float density = 1.0f, float friction = 0.3f, float restitution = 0.0f,
                                  uint16_t categoryBits = 0x0001, uint16_t maskBits = 0xFFFF, bool isSensor = false);

        /**
         * @brief Ajoute une fixture circulaire à un corps
         * @param body Identifiant du corps
         * @param radius Rayon du cercle en pixels
         * @param density Densité de la fixture (masse volumique)
         * @param friction Coefficient de friction
         * @param restitution Coefficient de restitution (rebond)
         * @param categoryBits Bits de catégorie pour le filtrage des collisions
         * @param maskBits Bits de masque pour le filtrage des collisions
         * @param isSensor Si true, la fixture est un capteur (ne génère pas de collisions physiques)
         * @return Identifiant de la fixture créée
         */
        b2FixtureId addCircleFixture(b2BodyId body, float radius,
                                     float density = 1.0f, float friction = 0.3f, float restitution = 0.0f,
                                     uint16_t categoryBits = 0x0001, uint16_t maskBits = 0xFFFF, bool isSensor = false);

        /**
         * @brief Obtient l'écouteur de contact
         * @return Référence vers l'écouteur de contact
         */
        ContactListener &getContactListener();

        /**
         * @brief Active ou désactive le rendu de debug
         * @param enabled True pour activer, false pour désactiver
         */
        void setDebugDrawEnabled(bool enabled) { m_debugDrawEnabled = enabled; }

        /**
         * @brief Vérifie si le rendu de debug est activé
         * @return True si activé, false sinon
         */
        bool isDebugDrawEnabled() const { return m_debugDrawEnabled; }

        /**
         * @brief Dessine les formes des corps physiques (debug)
         * @param window Fenêtre de rendu SFML
         */
        void debugDraw(sf::RenderWindow &window);

        // Accesseur au monde Box2D
        b2WorldId getWorld() const { return m_world; }

    private:
        // Instance unique (singleton)
        static std::unique_ptr<PhysicsWorld> s_instance;

        // Monde Box2D
        b2WorldId m_world;

        // Écouteur de contact
        ContactListener m_contactListener;

        // Pour le debug drawing
        sf::VertexArray m_debugLines;
        bool m_debugDrawEnabled;

        // Paramètres de la simulation
        const int m_velocityIterations = 8;
        const int m_positionIterations = 3;
    };

} // namespace Orenji