#pragma once

#include "../Core/Component.hpp"
#include "TiledMap.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace Orenji
{

    /**
     * @brief Composant pour rendre une carte Tiled.
     *
     * Ce composant permet de gérer le rendu des différentes couches d'une carte Tiled,
     * avec des options de visibilité, de défilement parallaxe, et d'optimisation du rendu.
     */
    class TiledMapRenderer : public Component
    {
    public:
        /**
         * @brief Constructeur par défaut.
         */
        TiledMapRenderer();

        /**
         * @brief Constructeur avec carte Tiled.
         * @param map Pointeur vers la carte Tiled.
         */
        explicit TiledMapRenderer(TiledMap *map);

        /**
         * @brief Destructeur.
         */
        ~TiledMapRenderer() override = default;

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
         * @brief Dessine la carte Tiled.
         * @param target Cible de rendu.
         * @param states États de rendu.
         */
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;

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
         * @brief Définit les calques à afficher.
         * @param layerNames Liste des noms de calques.
         */
        void setVisibleLayers(const std::vector<std::string> &layerNames);

        /**
         * @brief Active ou désactive l'affichage d'un calque.
         * @param layerName Nom du calque.
         * @param visible true pour afficher, false pour masquer.
         */
        void setLayerVisible(const std::string &layerName, bool visible);

        /**
         * @brief Vérifie si un calque est visible.
         * @param layerName Nom du calque.
         * @return true si le calque est visible, false sinon.
         */
        bool isLayerVisible(const std::string &layerName) const;

        /**
         * @brief Définit le facteur de parallaxe pour un calque.
         * @param layerName Nom du calque.
         * @param factorX Facteur de parallaxe horizontal.
         * @param factorY Facteur de parallaxe vertical.
         */
        void setLayerParallax(const std::string &layerName, float factorX, float factorY);

        /**
         * @brief Obtient le facteur de parallaxe d'un calque.
         * @param layerName Nom du calque.
         * @return Facteur de parallaxe (x, y).
         */
        sf::Vector2f getLayerParallax(const std::string &layerName) const;

        /**
         * @brief Active ou désactive l'affichage des objets.
         * @param visible true pour afficher, false pour masquer.
         */
        void setObjectsVisible(bool visible);

        /**
         * @brief Vérifie si les objets sont visibles.
         * @return true si les objets sont visibles, false sinon.
         */
        bool areObjectsVisible() const;

        /**
         * @brief Définit la région de la carte à afficher.
         * @param view Rectangle définissant la vue.
         */
        void setView(const sf::FloatRect &view);

        /**
         * @brief Obtient la région de la carte affichée.
         * @return Rectangle définissant la vue.
         */
        sf::FloatRect getView() const;

        /**
         * @brief Active ou désactive l'optimisation du rendu.
         *
         * Lorsque activée, seules les tuiles visibles dans la vue sont rendues.
         * @param enabled true pour activer, false pour désactiver.
         */
        void setRenderOptimization(bool enabled);

        /**
         * @brief Vérifie si l'optimisation du rendu est activée.
         * @return true si l'optimisation est activée, false sinon.
         */
        bool isRenderOptimizationEnabled() const;

    private:
        TiledMap *m_map;                                               ///< Carte Tiled associée
        std::unordered_map<std::string, bool> m_layerVisibility;       ///< Visibilité des calques
        std::unordered_map<std::string, sf::Vector2f> m_layerParallax; ///< Facteurs de parallaxe
        bool m_objectsVisible;                                         ///< Si les objets sont visibles
        sf::FloatRect m_view;                                          ///< Vue actuelle
        bool m_renderOptimizationEnabled;                              ///< Si l'optimisation du rendu est activée

        /**
         * @brief Calcule la région de tuiles visible dans la vue actuelle.
         * @return Paire de coordonnées (début, fin) des tuiles visibles.
         */
        std::pair<sf::Vector2i, sf::Vector2i> getVisibleTileRange() const;

        /**
         * @brief Dessine un calque spécifique de la carte.
         * @param target Cible de rendu.
         * @param states États de rendu.
         * @param layerName Nom du calque à dessiner.
         */
        void drawLayer(sf::RenderTarget &target, sf::RenderStates states, const std::string &layerName) const;

        /**
         * @brief Dessine les objets de la carte.
         * @param target Cible de rendu.
         * @param states États de rendu.
         */
        void drawObjects(sf::RenderTarget &target, sf::RenderStates states) const;
    };

} // namespace Orenji