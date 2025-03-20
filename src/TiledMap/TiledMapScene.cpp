#include "../../include/TiledMap/TiledMapScene.hpp"
#include "../../include/Core/Entity.hpp"
#include "../../include/Core/EntityManager.hpp"
#include "../../include/Components/Camera.hpp"
#include "../../include/TiledMap/TileObject.hpp"
#include "../../include/TiledMap/TiledMapLoader.hpp"

namespace Orenji
{

    TiledMapScene::TiledMapScene()
        : m_mapEntity(nullptr), m_renderer(nullptr), m_collision(nullptr), m_factory(nullptr), m_cameraEntity(nullptr)
    {
    }

    TiledMapScene::TiledMapScene(const std::string &mapFilename)
        : m_mapEntity(nullptr), m_renderer(nullptr), m_collision(nullptr), m_factory(nullptr), m_cameraEntity(nullptr)
    {
        loadMap(mapFilename);
    }

    bool TiledMapScene::loadMap(const std::string &filename)
    {
        // Utiliser TiledMapLoader pour charger la carte
        auto loadedMap = TiledMapLoader::load(filename);

        if (!loadedMap)
        {
            return false;
        }

        // Stocker la carte chargée
        m_map.reset(new TiledMap(*loadedMap));

        return true;
    }

    void TiledMapScene::initialize()
    {
        // Initialiser la scène de base
        Scene::initialize();

        // Créer une entité pour contenir les composants de la carte
        m_mapEntity = getEntityManager()->createEntity();
        m_mapEntity->setName("TiledMap");

        // Ajouter le composant de rendu
        m_renderer = m_mapEntity->addComponent<TiledMapRenderer>();
        m_renderer->setMap(m_map.get());

        // Ajouter le composant de collision
        m_collision = m_mapEntity->addComponent<TiledMapCollision>();
        m_collision->setMap(m_map.get());

        // Créer la factory d'entités
        m_factory = new TiledMapFactory();
        m_factory->setMap(m_map.get());
        m_factory->setEntityManager(getEntityManager());

        // Enregistrer les types d'entités
        registerEntityTypes();

        // Créer les entités à partir de la carte
        createEntitiesFromMap();

        // Créer la caméra
        m_cameraEntity = createCameraEntity();
        if (m_cameraEntity)
        {
            setupCamera(m_cameraEntity);
        }
    }

    void TiledMapScene::update(float deltaTime)
    {
        // Mettre à jour la scène de base
        Scene::update(deltaTime);

        // Mettre à jour la vue du renderer si une caméra est présente
        if (m_renderer && m_cameraEntity)
        {
            Camera *camera = m_cameraEntity->getComponent<Camera>();
            if (camera)
            {
                sf::View view = camera->getView();
                sf::FloatRect viewRect(view.getCenter() - view.getSize() / 2.f, view.getSize());
                m_renderer->setView(viewRect);
            }
        }
    }

    void TiledMapScene::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        // La scène de base s'occupe de dessiner toutes les entités
        Scene::draw(target, states);
    }

    TiledMap *TiledMapScene::getMap()
    {
        return m_map.get();
    }

    TiledMapRenderer *TiledMapScene::getRenderer()
    {
        return m_renderer;
    }

    TiledMapCollision *TiledMapScene::getCollision()
    {
        return m_collision;
    }

    TiledMapFactory *TiledMapScene::getFactory()
    {
        return m_factory;
    }

    void TiledMapScene::setCollisionLayers(const std::vector<std::string> &collisionLayers)
    {
        if (m_collision)
        {
            m_collision->clearCollisionLayers();
            for (const auto &layer : collisionLayers)
            {
                m_collision->addCollisionLayer(layer);
            }
        }
    }

    void TiledMapScene::setVisibleLayers(const std::vector<std::string> &visibleLayers)
    {
        if (m_renderer)
        {
            m_renderer->setVisibleLayers(visibleLayers);
        }
    }

    void TiledMapScene::setObjectsVisible(bool visible)
    {
        if (m_renderer)
        {
            m_renderer->setObjectsVisible(visible);
        }
    }

    void TiledMapScene::createEntitiesFromMap()
    {
        if (!m_map || !m_factory)
        {
            return;
        }

        // Utiliser la factory pour créer les entités
        m_factory->createEntities();
    }

    void TiledMapScene::onEntityCreated(Entity *entity, Object *object)
    {
        // Par défaut, ajouter un composant TileObject à l'entité
        if (entity && object)
        {
            TileObject *tileObj = entity->addComponent<TileObject>(object->id);
            tileObj->setMap(m_map.get());

            // Synchroniser la position
            tileObj->syncPositionFromObject();
        }
    }

    void TiledMapScene::registerEntityTypes()
    {
        // Par défaut, aucun type n'est enregistré
        // Cette méthode doit être surchargée par les classes dérivées
    }

    Entity *TiledMapScene::createCameraEntity()
    {
        Entity *camera = getEntityManager()->createEntity();
        camera->setName("Camera");

        // Ajouter un composant Camera
        camera->addComponent<Camera>();

        return camera;
    }

    void TiledMapScene::setupCamera(Entity *cameraEntity)
    {
        if (!cameraEntity || !m_map)
        {
            return;
        }

        Camera *camera = cameraEntity->getComponent<Camera>();
        if (!camera)
        {
            return;
        }

        // Configurer la caméra pour voir toute la carte
        sf::Vector2f mapSize(
            m_map->getWidth() * m_map->getTileWidth(),
            m_map->getHeight() * m_map->getTileHeight());

        // Positionner la caméra au centre de la carte
        cameraEntity->setPosition(mapSize.x / 2.f, mapSize.y / 2.f);

        // Configurer la vue
        sf::View view;
        view.setSize(mapSize);
        view.setCenter(mapSize.x / 2.f, mapSize.y / 2.f);
        camera->setView(view);
    }

} // namespace Orenji