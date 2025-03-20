#include "../../include/TiledMap/TiledMapModule.hpp"
#include "../../include/Core/Engine.hpp"
#include "../../include/Core/Entity.hpp"
#include "../../include/Core/EntityManager.hpp"
#include "../../include/TiledMap/TiledMapLoader.hpp"

namespace Orenji
{

    TiledMapModule::TiledMapModule()
    {
    }

    void TiledMapModule::initialize(Engine *engine)
    {
        if (!engine)
        {
            return;
        }

        // Enregistrer les composants
        registerComponents(engine);
    }

    void TiledMapModule::shutdown()
    {
        // Rien à nettoyer spécifiquement
    }

    std::string TiledMapModule::getName() const
    {
        return "TiledMapModule";
    }

    void TiledMapModule::registerComponents(Engine *engine)
    {
        if (!engine)
        {
            return;
        }

        // Enregistrer les types de composants
        engine->getComponentRegistry()->registerComponent<TiledMapRenderer>();
        engine->getComponentRegistry()->registerComponent<TiledMapCollision>();
        engine->getComponentRegistry()->registerComponent<TileObject>();
    }

    std::unique_ptr<TiledMap> TiledMapModule::loadMap(const std::string &filename)
    {
        // Utiliser TiledMapLoader pour charger la carte
        auto sharedMap = TiledMapLoader::load(filename);

        if (!sharedMap)
        {
            return nullptr;
        }

        // Convertir le std::shared_ptr en std::unique_ptr
        return std::make_unique<TiledMap>(*sharedMap);
    }

    Entity *TiledMapModule::createMapEntity(EntityManager *entityManager, TiledMap *map)
    {
        if (!entityManager || !map)
        {
            return nullptr;
        }

        // Créer une entité pour la carte
        Entity *entity = entityManager->createEntity();
        entity->setName("TiledMap");

        // Ajouter le composant de rendu
        TiledMapRenderer *renderer = entity->addComponent<TiledMapRenderer>(map);

        return entity;
    }

    TiledMapCollision *TiledMapModule::setupMapCollision(Entity *mapEntity, const std::vector<std::string> &collisionLayers)
    {
        if (!mapEntity)
        {
            return nullptr;
        }

        // Récupérer le renderer pour obtenir la carte
        TiledMapRenderer *renderer = mapEntity->getComponent<TiledMapRenderer>();
        if (!renderer)
        {
            return nullptr;
        }

        // Obtenir la carte
        TiledMap *map = renderer->getMap();
        if (!map)
        {
            return nullptr;
        }

        // Ajouter le composant de collision
        TiledMapCollision *collision = mapEntity->addComponent<TiledMapCollision>(map);

        // Configurer les calques de collision
        for (const auto &layer : collisionLayers)
        {
            collision->addCollisionLayer(layer);
        }

        return collision;
    }

} // namespace Orenji