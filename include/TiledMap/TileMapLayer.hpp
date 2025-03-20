#pragma once

#include "../Core/Component.hpp"
#include "TiledMap.hpp"
#include <SFML/Graphics.hpp>
#include <string>

namespace Orenji
{

    /**
     * @brief Composant pour gérer une couche spécifique d'une carte Tiled.
     *
     * Ce composant permet d'attacher une couche spécifique d'une carte Tiled à une entité
     * pour faciliter le rendu et la manipulation des calques de manière indépendante.
     */
    class TileMapLayer : public Component
    {
    public:
        /**
         * @brief Constructeur par défaut.
         */
        TileMapLayer();

        /**
         * @brief Constructeur avec nom de calque.
         * @param layerName Nom du calque à utiliser.
         */
        TileMapLayer(const std::string &layerName);

        /**
         * @brief Destructeur.
         */
        ~TileMapLayer() override = default;

        /**
         * @brief Initialise le composant.
         */
        void initialize() override;

        /**
         * @brief Met à jour le composant.
         * @param deltaTime Temps écoulé depuis la dernière mise à jour.
         */
        void update(float deltaTime) override;

        /**
         * @brief Configure la carte Tiled à utiliser.
         * @param map Pointeur vers la carte Tiled.
         */
        void setMap(TiledMap *map);

        /**
         * @brief Définit le nom du calque à utiliser.
         * @param layerName Nom du calque.
         */
        void setLayerName(const std::string &layerName);

        /**
         * @brief Obtient le nom du calque actuel.
         * @return Nom du calque.
         */
        const std::string &getLayerName() const;

        /**
         * @brief Obtient le calque associé.
         * @return Pointeur vers le calque ou nullptr si non trouvé.
         */
        Layer *getLayer();

        /**
         * @brief Définit la visibilité du calque.
         * @param visible Valeur de visibilité.
         */
        void setVisible(bool visible);

        /**
         * @brief Vérifie si le calque est visible.
         * @return true si visible, false sinon.
         */
        bool isVisible() const;

        /**
         * @brief Définit l'opacité du calque.
         * @param opacity Valeur d'opacité (0.0f - 1.0f).
         */
        void setOpacity(float opacity);

        /**
         * @brief Obtient l'opacité du calque.
         * @return Valeur d'opacité.
         */
        float getOpacity() const;

    private:
        TiledMap *m_map;         ///< Carte Tiled associée
        std::string m_layerName; ///< Nom du calque
        Layer *m_layer;          ///< Pointeur vers le calque (non possédé)
        bool m_visible;          ///< Visibilité du calque
        float m_opacity;         ///< Opacité du calque
    };

} // namespace Orenji