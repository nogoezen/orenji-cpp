#include "../../include/TiledMap/TileMapRenderer.hpp"
#include <algorithm>

namespace Orenji
{

    TileMapRenderer::TileMapRenderer()
        : m_map(nullptr), m_objectsVisible(false)
    {
        // Initialiser la zone visible à un rectangle vide
        m_visibleArea = sf::FloatRect(0, 0, 0, 0);
    }

    TileMapRenderer::TileMapRenderer(TiledMap *map)
        : m_map(map), m_objectsVisible(false)
    {
        // Initialiser la zone visible à la taille de la carte si elle est définie
        if (m_map)
        {
            m_visibleArea = sf::FloatRect(0, 0, m_map->getWidthPixels(), m_map->getHeightPixels());
        }
        else
        {
            m_visibleArea = sf::FloatRect(0, 0, 0, 0);
        }
    }

    void TileMapRenderer::initialize()
    {
        // Rien à initialiser spécifiquement
    }

    void TileMapRenderer::update(float deltaTime)
    {
        // Rien à mettre à jour dynamiquement
    }

    void TileMapRenderer::setMap(TiledMap *map)
    {
        m_map = map;

        // Réinitialiser la zone visible à la taille de la nouvelle carte
        if (m_map)
        {
            m_visibleArea = sf::FloatRect(0, 0, m_map->getWidthPixels(), m_map->getHeightPixels());
        }
        else
        {
            m_visibleArea = sf::FloatRect(0, 0, 0, 0);
        }

        // Réinitialiser les calques cachés et les opacités
        m_hiddenLayers.clear();
        m_layerOpacities.clear();
    }

    TiledMap *TileMapRenderer::getMap() const
    {
        return m_map;
    }

    void TileMapRenderer::setVisibleArea(const sf::FloatRect &visibleArea)
    {
        m_visibleArea = visibleArea;
    }

    const sf::FloatRect &TileMapRenderer::getVisibleArea() const
    {
        return m_visibleArea;
    }

    void TileMapRenderer::setLayerVisible(const std::string &layerName, bool visible)
    {
        // Si la carte n'est pas définie, ne rien faire
        if (!m_map)
            return;

        // Trouver le calque
        Layer *layer = m_map->getLayer(layerName);
        if (!layer)
            return;

        // Vérifier si le calque est dans la liste des calques cachés
        auto it = std::find(m_hiddenLayers.begin(), m_hiddenLayers.end(), layerName);

        if (visible)
        {
            // Si le calque doit être visible et qu'il est dans la liste des cachés, le retirer
            if (it != m_hiddenLayers.end())
            {
                m_hiddenLayers.erase(it);
            }

            // Appliquer la visibilité au calque
            layer->visible = true;
        }
        else
        {
            // Si le calque doit être caché et qu'il n'est pas dans la liste, l'ajouter
            if (it == m_hiddenLayers.end())
            {
                m_hiddenLayers.push_back(layerName);
            }

            // Appliquer la visibilité au calque
            layer->visible = false;
        }
    }

    bool TileMapRenderer::isLayerVisible(const std::string &layerName) const
    {
        // Si la carte n'est pas définie, considérer le calque comme invisible
        if (!m_map)
            return false;

        // Vérifier si le calque est dans la liste des calques cachés
        auto it = std::find(m_hiddenLayers.begin(), m_hiddenLayers.end(), layerName);
        if (it != m_hiddenLayers.end())
        {
            return false;
        }

        // Trouver le calque et vérifier sa visibilité
        for (const auto &layer : m_map->getLayers())
        {
            if (layer.name == layerName)
            {
                return layer.visible;
            }
        }

        // Le calque n'existe pas, considérer comme invisible
        return false;
    }

    void TileMapRenderer::setLayerOpacity(const std::string &layerName, float opacity)
    {
        // Si la carte n'est pas définie, ne rien faire
        if (!m_map)
            return;

        // Limiter l'opacité entre 0 et 1
        opacity = std::max(0.0f, std::min(1.0f, opacity));

        // Stocker l'opacité dans notre map
        m_layerOpacities[layerName] = opacity;

        // Trouver le calque et appliquer l'opacité
        Layer *layer = m_map->getLayer(layerName);
        if (layer)
        {
            layer->opacity = opacity;

            // Mettre à jour la couleur des vertices si c'est un calque de tuiles
            if (layer->type == "tilelayer")
            {
                sf::Color color = sf::Color::White;
                color.a = static_cast<sf::Uint8>(255 * opacity);

                for (size_t i = 0; i < layer->vertices.getVertexCount(); ++i)
                {
                    sf::Vertex &vertex = layer->vertices[i];
                    vertex.color.a = color.a;
                }
            }
        }
    }

    float TileMapRenderer::getLayerOpacity(const std::string &layerName) const
    {
        // Vérifier si nous avons une opacité personnalisée pour ce calque
        auto it = m_layerOpacities.find(layerName);
        if (it != m_layerOpacities.end())
        {
            return it->second;
        }

        // Sinon, récupérer l'opacité directement du calque
        if (m_map)
        {
            Layer *layer = m_map->getLayer(layerName);
            if (layer)
            {
                return layer->opacity;
            }
        }

        // Par défaut, opacité complète
        return 1.0f;
    }

    void TileMapRenderer::setObjectsVisible(bool visible)
    {
        m_objectsVisible = visible;
    }

    bool TileMapRenderer::areObjectsVisible() const
    {
        return m_objectsVisible;
    }

    void TileMapRenderer::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        // Si la carte n'est pas définie, ne rien dessiner
        if (!m_map)
            return;

        // Récupérer les propriétés de la zone de rendu
        sf::Vector2f viewCenter = target.getView().getCenter();
        sf::Vector2f viewSize = target.getView().getSize();

        // Calculer le rectangle de la vue actuelle
        sf::FloatRect viewRect(
            viewCenter.x - viewSize.x / 2.0f,
            viewCenter.y - viewSize.y / 2.0f,
            viewSize.x,
            viewSize.y);

        // Utiliser soit la zone visible définie, soit la zone de la vue actuelle
        sf::FloatRect visibleRect = (m_visibleArea.width > 0 && m_visibleArea.height > 0) ? m_visibleArea : viewRect;

        // Appliquer la transformation de l'entité
        if (hasEntity())
        {
            sf::Transform entityTransform;
            sf::Vector2f position = getEntity()->getPosition();
            float rotation = getEntity()->getRotation();
            sf::Vector2f scale = getEntity()->getScale();

            entityTransform.translate(position);
            entityTransform.rotate(rotation);
            entityTransform.scale(scale);

            states.transform *= entityTransform;
        }

        // Dessiner les calques de tuiles
        for (const auto &layer : m_map->getLayers())
        {
            // Vérifier si le calque est de type tilelayer et s'il est visible
            if (layer.type != "tilelayer" || !layer.visible)
                continue;

            // Vérifier si le calque est dans la liste des calques cachés
            auto it = std::find(m_hiddenLayers.begin(), m_hiddenLayers.end(), layer.name);
            if (it != m_hiddenLayers.end())
                continue;

            // Dessiner le calque
            sf::RenderStates layerStates = states;
            layerStates.transform *= layer.renderStates.transform;
            layerStates.texture = layer.renderStates.texture;

            target.draw(layer.vertices, layerStates);
        }

        // Dessiner les objets si nécessaire
        if (m_objectsVisible)
        {
            for (const auto &object : m_map->getObjects())
            {
                if (object.visible && object.shape)
                {
                    // Vérifier si l'objet est dans la zone visible
                    sf::FloatRect objectRect = object.bounds;
                    if (visibleRect.intersects(objectRect))
                    {
                        target.draw(*object.shape, states);
                    }
                }
            }
        }
    }

} // namespace Orenji