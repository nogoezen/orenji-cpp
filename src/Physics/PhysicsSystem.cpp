#include "../../include/Physics/PhysicsSystem.hpp"
#include "../../include/Core/EntityManager.hpp"
#include "../../include/Physics/PhysicsComponent.hpp"
#include <iostream>

namespace Physics
{
    // Structure pour stocker les données nécessaires au callback de collision
    struct ContactListener
    {
        Core::EntityManager *entityManager;
        std::vector<std::tuple<Core::EntityId, Core::EntityId, b2ContactId>> beginContacts;
        std::vector<std::tuple<Core::EntityId, Core::EntityId, b2ContactId>> endContacts;
    };

    // Callback pour les événements de début de contact
    void BeginContactCallback(b2ContactId contactId, void *userData)
    {
        ContactListener *listener = static_cast<ContactListener *>(userData);
        if (!listener || !listener->entityManager)
            return;

        // Récupérer les corps impliqués dans le contact
        b2BodyId bodyIdA = b2Contact_GetBodyA(contactId);
        b2BodyId bodyIdB = b2Contact_GetBodyB(contactId);

        // Récupérer les entités associées aux corps
        void *userDataA = b2Body_GetUserData(bodyIdA);
        void *userDataB = b2Body_GetUserData(bodyIdB);

        if (!userDataA || !userDataB)
            return;

        // Convertir les données utilisateur en entityId
        Core::EntityId entityIdA = reinterpret_cast<Core::EntityId>(userDataA);
        Core::EntityId entityIdB = reinterpret_cast<Core::EntityId>(userDataB);

        // Stocker le contact pour traitement ultérieur
        listener->beginContacts.emplace_back(entityIdA, entityIdB, contactId);
    }

    // Callback pour les événements de fin de contact
    void EndContactCallback(b2ContactId contactId, void *userData)
    {
        ContactListener *listener = static_cast<ContactListener *>(userData);
        if (!listener || !listener->entityManager)
            return;

        // Récupérer les corps impliqués dans le contact
        b2BodyId bodyIdA = b2Contact_GetBodyA(contactId);
        b2BodyId bodyIdB = b2Contact_GetBodyB(contactId);

        // Récupérer les entités associées aux corps
        void *userDataA = b2Body_GetUserData(bodyIdA);
        void *userDataB = b2Body_GetUserData(bodyIdB);

        if (!userDataA || !userDataB)
            return;

        // Convertir les données utilisateur en entityId
        Core::EntityId entityIdA = reinterpret_cast<Core::EntityId>(userDataA);
        Core::EntityId entityIdB = reinterpret_cast<Core::EntityId>(userDataB);

        // Stocker le contact pour traitement ultérieur
        listener->endContacts.emplace_back(entityIdA, entityIdB, contactId);
    }

    PhysicsSystem::PhysicsSystem(Core::EntityManager &entityManager, b2Vec2 gravity)
        : Core::System(entityManager), m_physics(std::make_unique<Box2DWrapper>(gravity)), m_velocityIterations(6), m_positionIterations(2)
    {
        std::cout << "PhysicsSystem created" << std::endl;
    }

    PhysicsSystem::~PhysicsSystem()
    {
        std::cout << "PhysicsSystem destroyed" << std::endl;
    }

    void PhysicsSystem::init()
    {
        std::cout << "PhysicsSystem initialized" << std::endl;

        // Créer le listener de contact
        ContactListener *listener = new ContactListener();
        listener->entityManager = &getEntityManager();

        // Configurer les callbacks du monde Box2D
        b2WorldId worldId = m_physics->getWorld();
        b2World_SetBeginContactCallback(worldId, BeginContactCallback, listener);
        b2World_SetEndContactCallback(worldId, EndContactCallback, listener);
    }

    void PhysicsSystem::update(float deltaTime)
    {
        // Faire avancer la simulation physique
        m_physics->step(deltaTime, m_velocityIterations, m_positionIterations);

        // Récupérer le listener de contact
        ContactListener *listener = static_cast<ContactListener *>(b2World_GetUserData(m_physics->getWorld()));
        if (!listener)
            return;

        // Traiter tous les contacts qui ont commencé
        for (auto &[entityIdA, entityIdB, contactId] : listener->beginContacts)
        {
            // Récupérer les composants physiques des entités
            auto entityA = getEntityManager().getEntity(entityIdA);
            auto entityB = getEntityManager().getEntity(entityIdB);

            if (!entityA || !entityB)
                continue;

            auto physicsCompA = entityA->getComponent<PhysicsComponent>();
            auto physicsCompB = entityB->getComponent<PhysicsComponent>();

            if (physicsCompA)
                physicsCompA->onCollisionBegin(entityIdB, contactId);

            if (physicsCompB)
                physicsCompB->onCollisionBegin(entityIdA, contactId);
        }

        // Traiter tous les contacts qui ont terminé
        for (auto &[entityIdA, entityIdB, contactId] : listener->endContacts)
        {
            // Récupérer les composants physiques des entités
            auto entityA = getEntityManager().getEntity(entityIdA);
            auto entityB = getEntityManager().getEntity(entityIdB);

            if (!entityA || !entityB)
                continue;

            auto physicsCompA = entityA->getComponent<PhysicsComponent>();
            auto physicsCompB = entityB->getComponent<PhysicsComponent>();

            if (physicsCompA)
                physicsCompA->onCollisionEnd(entityIdB, contactId);

            if (physicsCompB)
                physicsCompB->onCollisionEnd(entityIdA, contactId);
        }

        // Effacer les contacts traités
        listener->beginContacts.clear();
        listener->endContacts.clear();
    }

    Box2DWrapper &PhysicsSystem::getPhysics()
    {
        return *m_physics;
    }

    void PhysicsSystem::setGravity(b2Vec2 gravity)
    {
        b2WorldId worldId = m_physics->getWorld();
        if (b2World_IsValid(worldId))
        {
            b2World_SetGravity(worldId, gravity);
        }
    }

    b2Vec2 PhysicsSystem::getGravity() const
    {
        b2WorldId worldId = m_physics->getWorld();
        if (b2World_IsValid(worldId))
        {
            return b2World_GetGravity(worldId);
        }
        return {0.0f, 0.0f};
    }

    void PhysicsSystem::setAllowSleep(bool enableSleep)
    {
        b2WorldId worldId = m_physics->getWorld();
        if (b2World_IsValid(worldId))
        {
            b2WorldData *worldData = b2World_GetData(worldId);
            if (worldData)
            {
                worldData->enableSleep = enableSleep;
            }
        }
    }

    void PhysicsSystem::setIterations(int velocityIterations, int positionIterations)
    {
        m_velocityIterations = velocityIterations;
        m_positionIterations = positionIterations;
    }

    void PhysicsSystem::enableDebugDraw(bool enabled)
    {
        m_physics->setDebugDraw(enabled);
    }

    void PhysicsSystem::debugDraw(sf::RenderTarget &target, sf::RenderStates states)
    {
        m_physics->debugDraw(target, states);
    }

} // namespace Physics