#pragma once

#include "../Core/Component.hpp"
#include "TiledMap.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace Orenji
{

    /**
     * @brief Composant pour gérer le rendu des cartes Tiled.
     *
     * Ce composant permet de rendre une carte Tiled complète ou partiellement,
     * avec la possibilité de configurer quels calques sont visibles.
     */
    class TileMapRenderer : public Component, public sf::Drawable
    {
    public:
        /**
         * @brief Constructeur par défaut.
         */
        TileMapRenderer();

        /**
         * @brief Constructeur avec carte Tiled.
         * @param map Pointeur vers la carte Tiled à rendre.
         */
        explicit TileMapRenderer(TiledMap *map);

        /**
         * @brief Destructeur.
         */
        ~TileMapRenderer() override = default;

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
         * @brief Obtient la carte Tiled utilisée.
         * @return Pointeur vers la carte Tiled.
         */
        TiledMap *getMap() const;

        /**
         * @brief Définit la région visible de la carte.
         * @param visibleArea Rectangle définissant la zone visible.
         */
        void setVisibleArea(const sf::FloatRect &visibleArea);

        /**
         * @brief Obtient la région visible actuelle.
         * @return Rectangle définissant la zone visible.
         */
        const sf::FloatRect &getVisibleArea() const;

        /**
         * @brief Active ou désactive un calque spécifique.
         * @param layerName Nom du calque.
         * @param visible true pour rendre le calque visible, false sinon.
         */
        void setLayerVisible(const std::string &layerName, bool visible);

        /**
         * @brief Vérifie si un calque est visible.
         * @param layerName Nom du calque.
         * @return true si le calque est visible, false sinon.
         */
        bool isLayerVisible(const std::string &layerName) const;

        /**
         * @brief Définit l'opacité d'un calque spécifique.
         * @param layerName Nom du calque.
         * @param opacity Valeur d'opacité (0.0f - 1.0f).
         */
        void setLayerOpacity(const std::string &layerName, float opacity);

        /**
         * @brief Obtient l'opacité d'un calque.
         * @param layerName Nom du calque.
         * @return Valeur d'opacité du calque.
         */
        float getLayerOpacity(const std::string &layerName) const;

        /**
         * @brief Active ou désactive le rendu des objets.
         * @param visible true pour rendre les objets visibles, false sinon.
         */
        void setObjectsVisible(bool visible);

        /**
         * @brief Vérifie si les objets sont visibles.
         * @return true si les objets sont visibles, false sinon.
         */
        bool areObjectsVisible() const;

    protected:
        /**
         * @brief Méthode de rendu SFML.
         * @param target Cible de rendu.
         * @param states États de rendu.
         */
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        TiledMap *m_map;             ///< Carte Tiled à rendre
        sf::FloatRect m_visibleArea; ///< Zone visible de la carte
        bool m_objectsVisible;       ///< Indique si les objets doivent être rendus

        std::vector<std::string> m_hiddenLayers;                 ///< Liste des calques cachés
        std::unordered_map<std::string, float> m_layerOpacities; ///< Opacités des calques
    };

} // namespace Orenji