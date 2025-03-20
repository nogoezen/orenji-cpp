#include "../../include/TiledMap/TileMapLayer.hpp"

namespace Orenji
{

    TileMapLayer::TileMapLayer()
        : m_map(nullptr), m_layerName(""), m_layer(nullptr),
          m_visible(true), m_opacity(1.0f)
    {
    }

    TileMapLayer::TileMapLayer(const std::string &layerName)
        : m_map(nullptr), m_layerName(layerName), m_layer(nullptr),
          m_visible(true), m_opacity(1.0f)
    {
    }

    void TileMapLayer::initialize()
    {
        // Si nous avons une carte et un nom de calque, essayer de trouver le calque
        if (m_map && !m_layerName.empty())
        {
            m_layer = m_map->getLayer(m_layerName);

            // Si le calque est trouvé, initialiser les propriétés
            if (m_layer)
            {
                m_visible = m_layer->visible;
                m_opacity = m_layer->opacity;
            }
        }
    }

    void TileMapLayer::update(float deltaTime)
    {
        // Si le calque existe et que nos propriétés locales ont changé, les appliquer
        if (m_layer)
        {
            if (m_layer->visible != m_visible)
            {
                m_layer->visible = m_visible;
            }

            if (m_layer->opacity != m_opacity)
            {
                m_layer->opacity = m_opacity;

                // Mettre à jour la couleur des vertices si l'opacité a changé
                if (m_layer->type == "tilelayer")
                {
                    sf::Color color = sf::Color::White;
                    color.a = static_cast<sf::Uint8>(255 * m_opacity);

                    for (size_t i = 0; i < m_layer->vertices.getVertexCount(); ++i)
                    {
                        sf::Vertex &vertex = m_layer->vertices[i];
                        vertex.color.a = color.a;
                    }
                }
            }
        }
    }

    void TileMapLayer::setMap(TiledMap *map)
    {
        m_map = map;

        // Réinitialiser le calque
        m_layer = nullptr;

        // Si nous avons un nom de calque, essayer de trouver le calque
        if (m_map && !m_layerName.empty())
        {
            m_layer = m_map->getLayer(m_layerName);

            // Si le calque est trouvé, initialiser les propriétés
            if (m_layer)
            {
                m_visible = m_layer->visible;
                m_opacity = m_layer->opacity;
            }
        }
    }

    void TileMapLayer::setLayerName(const std::string &layerName)
    {
        m_layerName = layerName;

        // Réinitialiser le calque
        m_layer = nullptr;

        // Si nous avons une carte, essayer de trouver le calque
        if (m_map && !m_layerName.empty())
        {
            m_layer = m_map->getLayer(m_layerName);

            // Si le calque est trouvé, initialiser les propriétés
            if (m_layer)
            {
                m_visible = m_layer->visible;
                m_opacity = m_layer->opacity;
            }
        }
    }

    const std::string &TileMapLayer::getLayerName() const
    {
        return m_layerName;
    }

    Layer *TileMapLayer::getLayer()
    {
        return m_layer;
    }

    void TileMapLayer::setVisible(bool visible)
    {
        m_visible = visible;

        // Si le calque existe, appliquer la visibilité
        if (m_layer)
        {
            m_layer->visible = m_visible;
        }
    }

    bool TileMapLayer::isVisible() const
    {
        return m_visible;
    }

    void TileMapLayer::setOpacity(float opacity)
    {
        // Limiter l'opacité entre 0 et 1
        m_opacity = std::max(0.0f, std::min(1.0f, opacity));

        // Si le calque existe, appliquer l'opacité
        if (m_layer)
        {
            m_layer->opacity = m_opacity;

            // Mettre à jour la couleur des vertices si c'est un calque de tuiles
            if (m_layer->type == "tilelayer")
            {
                sf::Color color = sf::Color::White;
                color.a = static_cast<sf::Uint8>(255 * m_opacity);

                for (size_t i = 0; i < m_layer->vertices.getVertexCount(); ++i)
                {
                    sf::Vertex &vertex = m_layer->vertices[i];
                    vertex.color.a = color.a;
                }
            }
        }
    }

    float TileMapLayer::getOpacity() const
    {
        return m_opacity;
    }

} // namespace Orenji