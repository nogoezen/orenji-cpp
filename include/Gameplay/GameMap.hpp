#pragma once

#include "../Resources/TiledMapLoader.hpp"
#include "../Physics/TiledMapCollider.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

namespace Gameplay
{
    /**
     * @brief Classe représentant une carte de jeu avec rendu et collisions
     */
    class GameMap : public sf::Drawable
    {
    public:
        /**
         * @brief Constructeur
         * @param tiledMapLoader Le chargeur de cartes Tiled
         * @param physicsSystem Le système physique (nullptr si pas de physique)
         */
        GameMap(Resources::TiledMapLoader &tiledMapLoader, Physics::Box2DWrapper *physicsWrapper = nullptr);

        /**
         * @brief Destructeur
         */
        ~GameMap();

        /**
         * @brief Charger une carte à partir d'un fichier
         * @param mapFilePath Chemin vers le fichier de carte Tiled (JSON)
         * @param createCollisions Créer automatiquement les collisions
         * @return Vrai si la carte a été chargée avec succès
         */
        bool loadFromFile(const std::string &mapFilePath, bool createCollisions = true);

        /**
         * @brief Mettre à jour la carte
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        void update(float deltaTime);

        /**
         * @brief Obtenir la taille de la carte en pixels
         * @return Taille de la carte
         */
        sf::Vector2f getSize() const;

        /**
         * @brief Obtenir la taille des tuiles en pixels
         * @return Taille des tuiles
         */
        sf::Vector2i getTileSize() const;

        /**
         * @brief Convertir des coordonnées pixels en coordonnées de tuile
         * @param pixelCoords Coordonnées en pixels
         * @return Coordonnées de tuile (arrondies)
         */
        sf::Vector2i pixelToTileCoords(const sf::Vector2f &pixelCoords) const;

        /**
         * @brief Convertir des coordonnées de tuile en coordonnées pixels
         * @param tileCoords Coordonnées de tuile
         * @return Coordonnées en pixels (coin supérieur gauche de la tuile)
         */
        sf::Vector2f tileToPixelCoords(const sf::Vector2i &tileCoords) const;

        /**
         * @brief Obtenir tous les objets d'un type spécifique
         * @param type Type des objets à récupérer
         * @return Vecteur d'objets du type spécifié
         */
        std::vector<Resources::MapObject> getObjectsByType(const std::string &type) const;

        /**
         * @brief Obtenir tous les objets avec un nom spécifique
         * @param name Nom des objets à récupérer
         * @return Vecteur d'objets avec le nom spécifié
         */
        std::vector<Resources::MapObject> getObjectsByName(const std::string &name) const;

        /**
         * @brief Obtenir le système de collision de la carte
         * @return Pointeur vers le système de collision (peut être nullptr)
         */
        Physics::TiledMapCollider *getCollider();

    private:
        Resources::TiledMapLoader &m_tiledMapLoader;
        Physics::Box2DWrapper *m_physicsWrapper;
        std::unique_ptr<Physics::TiledMapCollider> m_collider;
        bool m_loaded;

        /**
         * @brief Dessiner la carte sur une cible de rendu
         * @param target Cible de rendu
         * @param states États de rendu
         */
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    };

} // namespace Gameplay