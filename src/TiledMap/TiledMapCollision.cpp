#include "../../include/TiledMap/TiledMapCollision.hpp"
#include "../../include/Core/Entity.hpp"

namespace Orenji
{

    TiledMapCollision::TiledMapCollision()
        : m_map(nullptr), m_tileCollisionsEnabled(true), m_objectCollisionsEnabled(true)
    {
    }

    TiledMapCollision::TiledMapCollision(TiledMap *map)
        : m_map(map), m_tileCollisionsEnabled(true), m_objectCollisionsEnabled(true)
    {
    }

    void TiledMapCollision::initialize()
    {
        // Rien à initialiser spécifiquement
    }

    void TiledMapCollision::update(float deltaTime)
    {
        // Rien à mettre à jour dynamiquement
    }

    void TiledMapCollision::setMap(TiledMap *map)
    {
        m_map = map;
    }

    TiledMap *TiledMapCollision::getMap() const
    {
        return m_map;
    }

    void TiledMapCollision::setCollisionLayers(const std::vector<std::string> &layerNames)
    {
        m_collisionLayers = layerNames;
    }

    void TiledMapCollision::addCollisionLayer(const std::string &layerName)
    {
        // Vérifier si le calque n'est pas déjà dans la liste
        if (std::find(m_collisionLayers.begin(), m_collisionLayers.end(), layerName) == m_collisionLayers.end())
        {
            m_collisionLayers.push_back(layerName);
        }
    }

    void TiledMapCollision::setTileCollisionsEnabled(bool enabled)
    {
        m_tileCollisionsEnabled = enabled;
    }

    void TiledMapCollision::setObjectCollisionsEnabled(bool enabled)
    {
        m_objectCollisionsEnabled = enabled;
    }

    bool TiledMapCollision::checkCollisionAtPosition(const sf::Vector2f &position) const
    {
        if (!m_map || !m_tileCollisionsEnabled)
        {
            return false;
        }

        // Convertir la position en coordonnées de tuile
        sf::Vector2i tilePos = m_map->worldToTile(position);

        // Vérifier si la tuile est collidable
        return isTileCollidable(tilePos.x, tilePos.y);
    }

    bool TiledMapCollision::checkCollisionWithRect(const sf::FloatRect &rect) const
    {
        if (!m_map || !m_tileCollisionsEnabled)
        {
            return false;
        }

        // Convertir les coins du rectangle en coordonnées de tuiles
        sf::Vector2i topLeft = m_map->worldToTile(sf::Vector2f(rect.left, rect.top));
        sf::Vector2i bottomRight = m_map->worldToTile(sf::Vector2f(rect.left + rect.width, rect.top + rect.height));

        // S'assurer que les coordonnées sont dans les limites de la carte
        topLeft.x = std::max(0, std::min(topLeft.x, m_map->getWidth() - 1));
        topLeft.y = std::max(0, std::min(topLeft.y, m_map->getHeight() - 1));
        bottomRight.x = std::max(0, std::min(bottomRight.x, m_map->getWidth() - 1));
        bottomRight.y = std::max(0, std::min(bottomRight.y, m_map->getHeight() - 1));

        // Vérifier chaque tuile dans le rectangle
        for (int y = topLeft.y; y <= bottomRight.y; ++y)
        {
            for (int x = topLeft.x; x <= bottomRight.x; ++x)
            {
                if (isTileCollidable(x, y))
                {
                    return true;
                }
            }
        }

        // Vérifier les collisions avec les objets si activées
        if (m_objectCollisionsEnabled)
        {
            for (const auto &object : m_map->getObjects())
            {
                if (object.visible && rect.intersects(object.bounds))
                {
                    // Si l'objet a une propriété "collidable" à false, l'ignorer
                    if (object.properties.contains("collidable"))
                    {
                        const auto &prop = object.properties["collidable"];
                        if (prop.is_boolean() && !prop.get<bool>())
                        {
                            continue;
                        }
                    }

                    return true;
                }
            }
        }

        return false;
    }

    bool TiledMapCollision::checkEntityCollision(Entity *entity) const
    {
        if (!entity || !m_map)
        {
            return false;
        }

        // Obtenir le rectangle englobant de l'entité
        sf::FloatRect entityBounds;
        entityBounds.left = entity->getPosition().x;
        entityBounds.top = entity->getPosition().y;

        // Si l'entité a un sprite ou une forme, utiliser sa taille
        sf::Vector2f size(32.0f, 32.0f); // Taille par défaut
        // TODO: Obtenir la taille réelle du sprite ou de la forme de l'entité

        entityBounds.width = size.x;
        entityBounds.height = size.y;

        // Centrer le rectangle sur la position de l'entité
        entityBounds.left -= size.x / 2.0f;
        entityBounds.top -= size.y / 2.0f;

        // Vérifier les collisions avec ce rectangle
        return checkCollisionWithRect(entityBounds);
    }

    std::vector<Object *> TiledMapCollision::getObjectsInRect(const sf::FloatRect &rect) const
    {
        std::vector<Object *> result;

        if (!m_map || !m_objectCollisionsEnabled)
        {
            return result;
        }

        for (auto &object : m_map->getObjects())
        {
            if (object.visible && rect.intersects(object.bounds))
            {
                result.push_back(&object);
            }
        }

        return result;
    }

    std::vector<Object *> TiledMapCollision::getObjectsOfTypeInRect(const sf::FloatRect &rect, const std::string &objectType) const
    {
        std::vector<Object *> result;

        if (!m_map || !m_objectCollisionsEnabled)
        {
            return result;
        }

        for (auto &object : m_map->getObjects())
        {
            if (object.visible && object.type == objectType && rect.intersects(object.bounds))
            {
                result.push_back(&object);
            }
        }

        return result;
    }

    sf::Vector2f TiledMapCollision::resolveCollision(const sf::FloatRect &rect, const sf::Vector2f &movement, bool slidingEnabled) const
    {
        if (!m_map || (!m_tileCollisionsEnabled && !m_objectCollisionsEnabled))
        {
            return movement;
        }

        // Copie du rectangle pour le déplacer
        sf::FloatRect movedRect = rect;

        // Essayer le mouvement complet
        movedRect.left += movement.x;
        movedRect.top += movement.y;

        // Si pas de collision, retourner le mouvement original
        if (!checkCollisionWithRect(movedRect))
        {
            return movement;
        }

        sf::Vector2f resultMovement = movement;

        // Si le glissement est activé, essayer les mouvements séparés
        if (slidingEnabled)
        {
            // Essayer le mouvement horizontal
            sf::FloatRect horizontalRect = rect;
            horizontalRect.left += movement.x;

            if (!checkCollisionWithRect(horizontalRect))
            {
                resultMovement.y = 0;
                return resultMovement;
            }

            // Essayer le mouvement vertical
            sf::FloatRect verticalRect = rect;
            verticalRect.top += movement.y;

            if (!checkCollisionWithRect(verticalRect))
            {
                resultMovement.x = 0;
                return resultMovement;
            }

            // Si les deux mouvements séparés causent des collisions, bloquer complètement
            return sf::Vector2f(0, 0);
        }
        else
        {
            // Sans glissement, bloquer complètement le mouvement
            return sf::Vector2f(0, 0);
        }
    }

    bool TiledMapCollision::isTileCollidable(int tileX, int tileY) const
    {
        if (!m_map)
        {
            return false;
        }

        // Vérifier les limites de la carte
        if (tileX < 0 || tileY < 0 || tileX >= m_map->getWidth() || tileY >= m_map->getHeight())
        {
            return false;
        }

        // Si aucun calque spécifique n'est défini, vérifier tous les calques
        if (m_collisionLayers.empty())
        {
            Tile *tile = m_map->getTileAt(tileX, tileY);
            return tile && tile->collidable;
        }

        // Sinon, vérifier uniquement les calques spécifiés
        for (const auto &layerName : m_collisionLayers)
        {
            Tile *tile = m_map->getTileAt(tileX, tileY, layerName);
            if (tile && tile->collidable)
            {
                return true;
            }
        }

        return false;
    }

} // namespace Orenji