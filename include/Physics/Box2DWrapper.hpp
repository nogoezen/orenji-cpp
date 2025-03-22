#pragma once

#include <memory>
#include <box2d/box2d.h>
#include "../Core/Entity.hpp"

namespace Physics
{
    // Constantes pour la conversion entre les unités de jeu et les unités Box2D
    constexpr float PIXELS_PER_METER = 32.0f;
    constexpr float METERS_PER_PIXEL = 1.0f / PIXELS_PER_METER;

    /**
     * @brief Classe wrapper pour Box2D
     */
    class Box2DWrapper
    {
    public:
        /**
         * @brief Constructeur
         * @param gravity Vecteur de gravité
         */
        Box2DWrapper(b2Vec2 gravity = {0.0f, 9.8f});

        /**
         * @brief Destructeur
         */
        ~Box2DWrapper();

        /**
         * @brief Mettre à jour la simulation physique
         * @param timeStep Pas de temps en secondes
         * @param velocityIterations Nombre d'itérations pour la résolution de la vélocité
         * @param positionIterations Nombre d'itérations pour la résolution de la position
         */
        void step(float timeStep, int velocityIterations = 6, int positionIterations = 2);

        /**
         * @brief Créer un corps statique
         * @param position Position en pixels
         * @param angle Angle en degrés
         * @param userData Données utilisateur (pointeur vers l'entité)
         * @return ID du corps créé
         */
        b2BodyId createStaticBody(b2Vec2 position, float angle = 0.0f, void *userData = nullptr);

        /**
         * @brief Créer un corps dynamique
         * @param position Position en pixels
         * @param angle Angle en degrés
         * @param userData Données utilisateur (pointeur vers l'entité)
         * @return ID du corps créé
         */
        b2BodyId createDynamicBody(b2Vec2 position, float angle = 0.0f, void *userData = nullptr);

        /**
         * @brief Ajouter une forme de boîte à un corps
         * @param bodyId ID du corps auquel ajouter la forme
         * @param width Largeur en pixels
         * @param height Hauteur en pixels
         * @param density Densité
         * @param friction Friction
         * @param restitution Restitution (rebond)
         * @param offset Décalage par rapport au centre du corps en pixels
         * @return ID de la forme créée
         */
        b2ShapeId addBoxFixture(b2BodyId bodyId, float width, float height,
                                float density = 1.0f, float friction = 0.3f,
                                float restitution = 0.0f, b2Vec2 offset = {0.0f, 0.0f});

        /**
         * @brief Ajouter une forme de cercle à un corps
         * @param bodyId ID du corps auquel ajouter la forme
         * @param radius Rayon en pixels
         * @param density Densité
         * @param friction Friction
         * @param restitution Restitution (rebond)
         * @param offset Décalage par rapport au centre du corps en pixels
         * @return ID de la forme créée
         */
        b2ShapeId addCircleFixture(b2BodyId bodyId, float radius,
                                   float density = 1.0f, float friction = 0.3f,
                                   float restitution = 0.0f, b2Vec2 offset = {0.0f, 0.0f});

        /**
         * @brief Détruire un corps
         * @param bodyId ID du corps à détruire
         */
        void destroyBody(b2BodyId bodyId);

        /**
         * @brief Convertir des pixels en mètres
         * @param pixels Valeur en pixels
         * @return Valeur en mètres
         */
        static float pixelsToMeters(float pixels);

        /**
         * @brief Convertir des mètres en pixels
         * @param meters Valeur en mètres
         * @return Valeur en pixels
         */
        static float metersToPixels(float meters);

        /**
         * @brief Convertir un vecteur de pixels en mètres
         * @param pixels Vecteur en pixels
         * @return Vecteur en mètres
         */
        static b2Vec2 pixelsToMeters(b2Vec2 pixels);

        /**
         * @brief Convertir un vecteur de mètres en pixels
         * @param meters Vecteur en mètres
         * @return Vecteur en pixels
         */
        static b2Vec2 metersToPixels(b2Vec2 meters);

        /**
         * @brief Obtenir l'ID du monde Box2D
         * @return ID du monde Box2D
         */
        b2WorldId getWorld() const;

    private:
        b2WorldId m_worldId;
    };

} // namespace Physics