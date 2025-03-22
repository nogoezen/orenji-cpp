#pragma once

#include "../Core/System.hpp"
#include "Box2DWrapper.hpp"
#include <memory>

namespace Physics
{
    class PhysicsSystem : public Core::System
    {
    public:
        /**
         * @brief Constructeur
         * @param entityManager Gestionnaire d'entités
         * @param gravity Vecteur de gravité du monde (par défaut 0, 9.8)
         */
        PhysicsSystem(Core::EntityManager &entityManager, b2Vec2 gravity = {0.0f, 9.8f});

        /**
         * @brief Destructeur
         */
        virtual ~PhysicsSystem();

        /**
         * @brief Initialiser le système
         */
        virtual void init() override;

        /**
         * @brief Mettre à jour le système
         * @param deltaTime Temps écoulé depuis la dernière mise à jour en secondes
         */
        virtual void update(float deltaTime) override;

        /**
         * @brief Obtenir le wrapper Box2D
         * @return Référence vers le wrapper Box2D
         */
        Box2DWrapper &getPhysics();

        /**
         * @brief Définir la gravité du monde
         * @param gravity Vecteur de gravité
         */
        void setGravity(b2Vec2 gravity);

        /**
         * @brief Obtenir la gravité du monde
         * @return Vecteur de gravité
         */
        b2Vec2 getGravity() const;

        /**
         * @brief Définir si les corps doivent dormir lorsqu'ils sont au repos
         * @param enableSleep True pour permettre le sommeil, false sinon
         */
        void setAllowSleep(bool enableSleep);

        /**
         * @brief Définir la précision du solveur de la simulation physique
         * @param velocityIterations Nombre d'itérations de vitesse
         * @param positionIterations Nombre d'itérations de position
         */
        void setIterations(int velocityIterations, int positionIterations);

    private:
        std::unique_ptr<Box2DWrapper> m_physics;
        int m_velocityIterations;
        int m_positionIterations;
    };

} // namespace Physics