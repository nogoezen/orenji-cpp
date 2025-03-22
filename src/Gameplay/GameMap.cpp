#include "../../include/Gameplay/GameMap.hpp"
#include <iostream>

namespace Gameplay
{
    GameMap::GameMap(Resources::TiledMapLoader &tiledMapLoader, Physics::Box2DWrapper *physicsWrapper)
        : m_tiledMapLoader(tiledMapLoader), m_physicsWrapper(physicsWrapper), m_loaded(false)
    {
        // Créer le système de collision si un wrapper physique est fourni
        if (m_physicsWrapper)
        {
            m_collider = std::make_unique<Physics::TiledMapCollider>(*m_physicsWrapper);
        }
    }

    GameMap::~GameMap()
    {
    }

    bool GameMap::loadFromFile(const std::string &mapFilePath, bool createCollisions)
    {
        // Charger la carte avec le TiledMapLoader
        if (!m_tiledMapLoader.loadMap(mapFilePath))
        {
            std::cerr << "Failed to load map file: " << mapFilePath << std::endl;
            return false;
        }

        // Créer les collisions si demandé et si un système physique est disponible
        if (createCollisions && m_collider)
        {
            // Créer des collisions pour tous les objets marqués comme collidables
            m_collider->createCollisionsFromMap(m_tiledMapLoader);
        }

        m_loaded = true;
        return true;
    }

    void GameMap::update(float deltaTime)
    {
        // Mise à jour de la carte (animations, etc.) si nécessaire
    }

    sf::Vector2f GameMap::getSize() const
    {
        return m_tiledMapLoader.getMapSize();
    }

    sf::Vector2i GameMap::getTileSize() const
    {
        return m_tiledMapLoader.getTileSize();
    }

    sf::Vector2i GameMap::pixelToTileCoords(const sf::Vector2f &pixelCoords) const
    {
        sf::Vector2i tileSize = getTileSize();
        return sf::Vector2i(
            static_cast<int>(pixelCoords.x / tileSize.x),
            static_cast<int>(pixelCoords.y / tileSize.y));
    }

    sf::Vector2f GameMap::tileToPixelCoords(const sf::Vector2i &tileCoords) const
    {
        sf::Vector2i tileSize = getTileSize();
        return sf::Vector2f(
            tileCoords.x * tileSize.x,
            tileCoords.y * tileSize.y);
    }

    std::vector<Resources::MapObject> GameMap::getObjectsByType(const std::string &type) const
    {
        return m_tiledMapLoader.getObjectsByType(type);
    }

    std::vector<Resources::MapObject> GameMap::getObjectsByName(const std::string &name) const
    {
        return m_tiledMapLoader.getObjectsByName(name);
    }

    Physics::TiledMapCollider *GameMap::getCollider()
    {
        return m_collider.get();
    }

    void GameMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (m_loaded)
        {
            // Utiliser le système de rendu intégré au TiledMapLoader
            m_tiledMapLoader.draw(target, states);
        }
    }

} // namespace Gameplay