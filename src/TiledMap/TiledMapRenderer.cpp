#include "../../include/TiledMap/TiledMapRenderer.hpp"

namespace Orenji
{

    TiledMapRenderer::TiledMapRenderer()
        : m_map(nullptr), m_objectsVisible(false), m_renderOptimizationEnabled(true)
    {
        m_view = sf::FloatRect(0, 0, 800, 600); // Vue par défaut
    }

    TiledMapRenderer::TiledMapRenderer(TiledMap *map)
        : m_map(map), m_objectsVisible(false), m_renderOptimizationEnabled(true)
    {
        m_view = sf::FloatRect(0, 0, 800, 600); // Vue par défaut

        // Si la carte est valide, initialiser la visibilité et le parallaxe pour tous les calques
        if (m_map)
        {
            for (const auto &layer : m_map->getLayers())
            {
                m_layerVisibility[layer.name] = layer.visible;
                m_layerParallax[layer.name] = sf::Vector2f(1.0f, 1.0f);
            }
        }
    }

    void TiledMapRenderer::initialize()
    {
        // Si la carte est valide, initialiser la visibilité et le parallaxe pour tous les calques
        if (m_map)
        {
            for (const auto &layer : m_map->getLayers())
            {
                if (m_layerVisibility.find(layer.name) == m_layerVisibility.end())
                {
                    m_layerVisibility[layer.name] = layer.visible;
                }

                if (m_layerParallax.find(layer.name) == m_layerParallax.end())
                {
                    m_layerParallax[layer.name] = sf::Vector2f(1.0f, 1.0f);
                }
            }
        }
    }

    void TiledMapRenderer::update(float deltaTime)
    {
        // Rien à mettre à jour dynamiquement
    }

    void TiledMapRenderer::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (!m_map)
        {
            return;
        }

        // Si l'entité a une transformation, l'appliquer
        if (Entity *entity = getEntity())
        {
            states.transform *= entity->getTransform();
        }

        // Dessiner les calques dans l'ordre
        for (const auto &layer : m_map->getLayers())
        {
            if (isLayerVisible(layer.name))
            {
                drawLayer(target, states, layer.name);
            }
        }

        // Dessiner les objets si nécessaire
        if (m_objectsVisible)
        {
            drawObjects(target, states);
        }
    }

    void TiledMapRenderer::setMap(TiledMap *map)
    {
        m_map = map;

        // Réinitialiser les configurations
        m_layerVisibility.clear();
        m_layerParallax.clear();

        // Si la carte est valide, initialiser la visibilité et le parallaxe
        if (m_map)
        {
            for (const auto &layer : m_map->getLayers())
            {
                m_layerVisibility[layer.name] = layer.visible;
                m_layerParallax[layer.name] = sf::Vector2f(1.0f, 1.0f);
            }
        }
    }

    TiledMap *TiledMapRenderer::getMap() const
    {
        return m_map;
    }

    void TiledMapRenderer::setVisibleLayers(const std::vector<std::string> &layerNames)
    {
        // Masquer tous les calques d'abord
        for (auto &[name, visible] : m_layerVisibility)
        {
            visible = false;
        }

        // Activer les calques spécifiés
        for (const auto &name : layerNames)
        {
            m_layerVisibility[name] = true;
        }
    }

    void TiledMapRenderer::setLayerVisible(const std::string &layerName, bool visible)
    {
        m_layerVisibility[layerName] = visible;
    }

    bool TiledMapRenderer::isLayerVisible(const std::string &layerName) const
    {
        auto it = m_layerVisibility.find(layerName);
        if (it != m_layerVisibility.end())
        {
            return it->second;
        }

        // Si le calque n'est pas explicitement configuré, vérifier s'il existe dans la carte
        if (m_map)
        {
            for (const auto &layer : m_map->getLayers())
            {
                if (layer.name == layerName)
                {
                    return layer.visible;
                }
            }
        }

        return false;
    }

    void TiledMapRenderer::setLayerParallax(const std::string &layerName, float factorX, float factorY)
    {
        m_layerParallax[layerName] = sf::Vector2f(factorX, factorY);
    }

    sf::Vector2f TiledMapRenderer::getLayerParallax(const std::string &layerName) const
    {
        auto it = m_layerParallax.find(layerName);
        if (it != m_layerParallax.end())
        {
            return it->second;
        }

        return sf::Vector2f(1.0f, 1.0f);
    }

    void TiledMapRenderer::setObjectsVisible(bool visible)
    {
        m_objectsVisible = visible;
    }

    bool TiledMapRenderer::areObjectsVisible() const
    {
        return m_objectsVisible;
    }

    void TiledMapRenderer::setView(const sf::FloatRect &view)
    {
        m_view = view;
    }

    sf::FloatRect TiledMapRenderer::getView() const
    {
        return m_view;
    }

    void TiledMapRenderer::setRenderOptimization(bool enabled)
    {
        m_renderOptimizationEnabled = enabled;
    }

    bool TiledMapRenderer::isRenderOptimizationEnabled() const
    {
        return m_renderOptimizationEnabled;
    }

    std::pair<sf::Vector2i, sf::Vector2i> TiledMapRenderer::getVisibleTileRange() const
    {
        if (!m_map)
        {
            return std::make_pair(sf::Vector2i(0, 0), sf::Vector2i(0, 0));
        }

        // Convertir les coordonnées de la vue en coordonnées de tuiles
        sf::Vector2i topLeft = m_map->worldToTile(sf::Vector2f(m_view.left, m_view.top));
        sf::Vector2i bottomRight = m_map->worldToTile(sf::Vector2f(m_view.left + m_view.width, m_view.top + m_view.height));

        // Ajouter une marge pour les tuiles partiellement visibles
        topLeft.x = std::max(0, topLeft.x - 1);
        topLeft.y = std::max(0, topLeft.y - 1);
        bottomRight.x = std::min(m_map->getWidth() - 1, bottomRight.x + 1);
        bottomRight.y = std::min(m_map->getHeight() - 1, bottomRight.y + 1);

        return std::make_pair(topLeft, bottomRight);
    }

    void TiledMapRenderer::drawLayer(sf::RenderTarget &target, sf::RenderStates states, const std::string &layerName) const
    {
        if (!m_map)
        {
            return;
        }

        // Récupérer le calque
        const Layer *layer = nullptr;
        for (const auto &l : m_map->getLayers())
        {
            if (l.name == layerName)
            {
                layer = &l;
                break;
            }
        }

        if (!layer)
        {
            return;
        }

        // Appliquer le facteur de parallaxe
        sf::Vector2f parallax = getLayerParallax(layerName);
        sf::Transform parallaxTransform;
        parallaxTransform.scale(parallax.x, parallax.y);
        states.transform *= parallaxTransform;

        // Déterminer la plage de tuiles à dessiner
        sf::Vector2i start(0, 0);
        sf::Vector2i end(m_map->getWidth() - 1, m_map->getHeight() - 1);

        // Si l'optimisation est activée, ne dessiner que les tuiles visibles
        if (m_renderOptimizationEnabled)
        {
            auto range = getVisibleTileRange();
            start = range.first;
            end = range.second;
        }

        // Dessiner les tuiles du calque
        for (int y = start.y; y <= end.y; ++y)
        {
            for (int x = start.x; x <= end.x; ++x)
            {
                Tile *tile = m_map->getTileAt(x, y, layerName);
                if (tile && tile->visible)
                {
                    // Créer le sprite de la tuile
                    sf::Sprite tileSprite;
                    tileSprite.setTexture(*tile->texture);
                    tileSprite.setTextureRect(tile->textureRect);

                    // Positionner la tuile
                    sf::Vector2f position = m_map->tileToWorld(sf::Vector2i(x, y));
                    tileSprite.setPosition(position);

                    // Appliquer les transformations de la tuile (rotation, flip, etc.)
                    if (tile->flipHorizontally)
                    {
                        tileSprite.setScale(-1.0f, tileSprite.getScale().y);
                        tileSprite.setPosition(position.x + m_map->getTileWidth(), position.y);
                    }

                    if (tile->flipVertically)
                    {
                        tileSprite.setScale(tileSprite.getScale().x, -1.0f);
                        tileSprite.setPosition(position.x, position.y + m_map->getTileHeight());
                    }

                    if (tile->rotation != 0)
                    {
                        tileSprite.setOrigin(m_map->getTileWidth() / 2.0f, m_map->getTileHeight() / 2.0f);
                        tileSprite.setRotation(tile->rotation);
                        tileSprite.setPosition(position.x + m_map->getTileWidth() / 2.0f, position.y + m_map->getTileHeight() / 2.0f);
                    }

                    // Dessiner la tuile
                    target.draw(tileSprite, states);
                }
            }
        }
    }

    void TiledMapRenderer::drawObjects(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (!m_map)
        {
            return;
        }

        // Parcourir tous les objets
        for (const auto &obj : m_map->getObjects())
        {
            if (!obj.visible)
            {
                continue;
            }

            // Si l'optimisation est activée, vérifier si l'objet est dans la vue
            if (m_renderOptimizationEnabled)
            {
                if (!m_view.intersects(obj.bounds))
                {
                    continue;
                }
            }

            // Dessiner l'objet selon son type
            if (obj.shape == ObjectShape::Rectangle)
            {
                // Dessiner un rectangle
                sf::RectangleShape rect;
                rect.setPosition(obj.bounds.left, obj.bounds.top);
                rect.setSize(sf::Vector2f(obj.bounds.width, obj.bounds.height));

                // Appliquer les styles
                rect.setFillColor(sf::Color(255, 255, 255, 80)); // Semi-transparent
                rect.setOutlineColor(sf::Color::Red);
                rect.setOutlineThickness(1.0f);

                target.draw(rect, states);
            }
            else if (obj.shape == ObjectShape::Ellipse)
            {
                // Dessiner une ellipse
                sf::CircleShape ellipse;
                float radius = std::min(obj.bounds.width, obj.bounds.height) / 2.0f;
                ellipse.setRadius(radius);
                ellipse.setPosition(obj.bounds.left, obj.bounds.top);
                ellipse.setScale(obj.bounds.width / (radius * 2.0f), obj.bounds.height / (radius * 2.0f));

                // Appliquer les styles
                ellipse.setFillColor(sf::Color(255, 255, 255, 80)); // Semi-transparent
                ellipse.setOutlineColor(sf::Color::Green);
                ellipse.setOutlineThickness(1.0f);

                target.draw(ellipse, states);
            }
            else if (obj.shape == ObjectShape::Polygon || obj.shape == ObjectShape::Polyline)
            {
                // Dessiner un polygone ou une polyligne
                sf::ConvexShape polygon;
                polygon.setPointCount(obj.points.size());

                for (size_t i = 0; i < obj.points.size(); ++i)
                {
                    polygon.setPoint(i, sf::Vector2f(obj.bounds.left + obj.points[i].x, obj.bounds.top + obj.points[i].y));
                }

                // Appliquer les styles
                if (obj.shape == ObjectShape::Polygon)
                {
                    polygon.setFillColor(sf::Color(255, 255, 255, 80)); // Semi-transparent
                }
                else
                {
                    polygon.setFillColor(sf::Color::Transparent);
                }

                polygon.setOutlineColor(sf::Color::Blue);
                polygon.setOutlineThickness(1.0f);

                target.draw(polygon, states);
            }
            else if (obj.shape == ObjectShape::Point)
            {
                // Dessiner un point
                sf::CircleShape point;
                point.setRadius(3.0f);
                point.setOrigin(3.0f, 3.0f);
                point.setPosition(obj.bounds.left + obj.bounds.width / 2.0f, obj.bounds.top + obj.bounds.height / 2.0f);
                point.setFillColor(sf::Color::Yellow);

                target.draw(point, states);
            }
            else if (obj.shape == ObjectShape::Tile && obj.gid > 0)
            {
                // Dessiner une tuile
                const Tileset *tileset = m_map->getTilesetForGid(obj.gid);
                if (tileset && tileset->texture)
                {
                    sf::Sprite sprite;
                    sprite.setTexture(*tileset->texture);

                    // Calculer le rectangle de texture
                    int localId = obj.gid - tileset->firstGid;
                    int tilesPerRow = tileset->columns;
                    int tileX = localId % tilesPerRow;
                    int tileY = localId / tilesPerRow;

                    sf::IntRect textureRect(
                        tileset->margin + tileX * (tileset->tileWidth + tileset->spacing),
                        tileset->margin + tileY * (tileset->tileHeight + tileset->spacing),
                        tileset->tileWidth,
                        tileset->tileHeight);

                    sprite.setTextureRect(textureRect);
                    sprite.setPosition(obj.bounds.left, obj.bounds.top);

                    // Ajuster la taille si nécessaire
                    if (obj.bounds.width != tileset->tileWidth || obj.bounds.height != tileset->tileHeight)
                    {
                        float scaleX = obj.bounds.width / tileset->tileWidth;
                        float scaleY = obj.bounds.height / tileset->tileHeight;
                        sprite.setScale(scaleX, scaleY);
                    }

                    target.draw(sprite, states);
                }
            }

            // Dessiner le nom de l'objet si présent
            if (!obj.name.empty())
            {
                // Cette partie nécessiterait une classe de texte avec une police chargée
                // Pour l'instant, on ne dessine pas le texte
            }
        }
    }

} // namespace Orenji