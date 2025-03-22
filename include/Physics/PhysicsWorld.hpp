#pragma once
#include <box2d/box2d.h>
#include <memory>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include <vector>
#include "Box2DTypes.hpp"

namespace Orenji
{
    // Forward declarations
    class ContactListener;

    /**
     * @brief Classe singleton gérant le monde physique Box2D.
     *
     * Cette classe s'occupe de la création et la gestion du monde Box2D,
     * ainsi que des corps et fixtures qui y sont présents.
     */
    class PhysicsWorld
    {
    public:
        // Constantes de conversion entre pixels et mètres
        static constexpr float PIXELS_PER_METER = 32.0f;
        static constexpr float METERS_PER_PIXEL = 1.0f / PIXELS_PER_METER;

        /**
         * @brief Classe pour gérer les callbacks de collision Box2D.
         */
        class ContactListener
        {
        public:
            ContactListener();
            ~ContactListener() = default;

            // Méthodes appelées par Box2D lors des collisions
            void BeginContact(void *contact);
            void EndContact(void *contact);
            void PreSolve(void *contact, const void *oldManifold);
            void PostSolve(void *contact, const void *impulse);

            // Méthodes pour définir les callbacks utilisateur
            using BeginContactCallback = std::function<void(void *)>;
            using EndContactCallback = std::function<void(void *)>;
            using PreSolveCallback = std::function<void(void *, const void *)>;
            using PostSolveCallback = std::function<void(void *, const void *)>;

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
         * @brief Obtient l'instance unique du monde physique.
         *
         * @return Référence à l'instance du PhysicsWorld.
         */
        static PhysicsWorld &getInstance();

        // Destructeur
        ~PhysicsWorld();

        /**
         * @brief Initialise le monde physique.
         *
         * @param gravity Vecteur de gravité (en pixels/s²).
         */
        void initialize(const sf::Vector2f &gravity = sf::Vector2f(0.0f, 9.8f * PIXELS_PER_METER));

        /**
         * @brief Convertit une distance en mètres vers des pixels.
         *
         * @param meters Distance en mètres.
         * @return Distance en pixels.
         */
        static float metersToPixels(float meters);

        /**
         * @brief Convertit un vecteur en mètres vers des pixels.
         *
         * @param metersVec Vecteur en mètres.
         * @return Vecteur en pixels.
         */
        static sf::Vector2f metersToPixels(const b2Vec2 &metersVec);

        /**
         * @brief Convertit une distance en pixels vers des mètres.
         *
         * @param pixels Distance en pixels.
         * @return Distance en mètres.
         */
        static float pixelsToMeters(float pixels);

        /**
         * @brief Convertit un vecteur en pixels vers des mètres.
         *
         * @param pixelsVec Vecteur en pixels.
         * @return Vecteur en mètres.
         */
        static b2Vec2 pixelsToMeters(const sf::Vector2f &pixelsVec);

        /**
         * @brief Met à jour le monde physique.
         *
         * @param timeStep Pas de temps pour la simulation (en secondes).
         */
        void update(float timeStep);

        /**
         * @brief Crée un corps physique.
         *
         * @param position Position initiale du corps (en pixels).
         * @param type Type du corps (dynamique, statique, cinématique).
         * @return Identifiant du corps créé.
         */
        b2BodyId createBody(const sf::Vector2f &position, b2BodyType type = b2_dynamicBody);

        /**
         * @brief Détruit un corps physique.
         *
         * @param body Identifiant du corps à détruire.
         */
        void destroyBody(b2BodyId body);

        /**
         * @brief Ajoute une fixture rectangulaire à un corps.
         *
         * @param body Corps auquel ajouter la fixture.
         * @param size Dimensions du rectangle (en pixels).
         * @param density Densité de la fixture.
         * @param friction Coefficient de friction.
         * @param categoryBits Bits de catégorie pour le filtrage des collisions.
         * @param maskBits Bits de masque pour le filtrage des collisions.
         * @param isSensor Si true, la fixture agit comme un capteur.
         * @return Identifiant de la fixture créée.
         */
        b2FixtureId addBoxFixture(b2BodyId body, const sf::Vector2f &size,
                                 float density = 1.0f, float friction = 0.3f,
                                 uint16_t categoryBits = 0x0001, uint16_t maskBits = 0xFFFF,
                                 bool isSensor = false);

        /**
         * @brief Ajoute une fixture circulaire à un corps.
         *
         * @param body Corps auquel ajouter la fixture.
         * @param radius Rayon du cercle (en pixels).
         * @param density Densité de la fixture.
         * @param friction Coefficient de friction.
         * @param categoryBits Bits de catégorie pour le filtrage des collisions.
         * @param maskBits Bits de masque pour le filtrage des collisions.
         * @param isSensor Si true, la fixture agit comme un capteur.
         * @return Identifiant de la fixture créée.
         */
        b2FixtureId addCircleFixture(b2BodyId body, float radius,
                                    float density = 1.0f, float friction = 0.3f,
                                    uint16_t categoryBits = 0x0001, uint16_t maskBits = 0xFFFF,
                                    bool isSensor = false);

        /**
         * @brief Définit le callback pour le début d'un contact.
         *
         * @param callback Fonction à appeler au début d'un contact.
         */
        void setBeginContactCallback(ContactListener::BeginContactCallback callback);

        /**
         * @brief Définit le callback pour la fin d'un contact.
         *
         * @param callback Fonction à appeler à la fin d'un contact.
         */
        void setEndContactCallback(ContactListener::EndContactCallback callback);

        /**
         * @brief Définit le callback avant la résolution d'un contact.
         *
         * @param callback Fonction à appeler avant la résolution.
         */
        void setPreSolveCallback(ContactListener::PreSolveCallback callback);

        /**
         * @brief Définit le callback après la résolution d'un contact.
         *
         * @param callback Fonction à appeler après la résolution.
         */
        void setPostSolveCallback(ContactListener::PostSolveCallback callback);

        /**
         * @brief Active ou désactive le dessin de debug.
         *
         * @param enabled Si true, active le dessin de debug.
         */
        void setDebugDraw(bool enabled) { m_debugDrawEnabled = enabled; }

        /**
         * @brief Vérifie si le dessin de debug est activé.
         *
         * @return true si le dessin de debug est activé.
         */
        bool isDebugDrawEnabled() const { return m_debugDrawEnabled; }

        /**
         * @brief Dessine les formes physiques pour debug.
         *
         * @param window La fenêtre SFML sur laquelle dessiner.
         */
        void debugDraw(sf::RenderWindow &window);
        
        /**
         * @brief Obtient l'identifiant du monde Box2D.
         *
         * @return Identifiant du monde Box2D.
         */
        b2WorldId getWorldId() const { return m_world; }
        
        /**
         * @brief Définit la gravité du monde.
         *
         * @param gravity Vecteur de gravité (en pixels/s²).
         */
        void setGravity(const sf::Vector2f &gravity);
        
        /**
         * @brief Obtient la gravité du monde.
         *
         * @return Vecteur de gravité (en pixels/s²).
         */
        sf::Vector2f getGravity() const;
        
        /**
         * @brief Définit le nombre d'itérations de sous-étapes pour la résolution.
         *
         * @param count Nombre de sous-étapes.
         */
        void setSubStepCount(int count) { m_subStepCount = count; }
        
        /**
         * @brief Obtient le nombre d'itérations de sous-étapes.
         *
         * @return Nombre de sous-étapes.
         */
        int getSubStepCount() const { return m_subStepCount; }

    private:
        // Constructeur privé (singleton)
        PhysicsWorld();
        PhysicsWorld(const PhysicsWorld &) = delete;
        PhysicsWorld &operator=(const PhysicsWorld &) = delete;

        // Instance unique
        static PhysicsWorld *s_instance;

        // Monde Box2D
        b2WorldId m_world;
        ContactListener m_contactListener;

        // Paramètres de simulation
        int m_velocityIterations;
        int m_positionIterations;
        int m_subStepCount;
        bool m_debugDrawEnabled;
    };

} // namespace Orenji