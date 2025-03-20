#pragma once

#include "../Core/Component.hpp"
#include "TiledMap.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace Orenji
{

    /**
     * @brief Composant pour gérer les collisions avec une carte Tiled.
     *
     * Ce composant permet de faciliter la détection de collisions entre une entité
     * et les différents éléments d'une carte Tiled (tuiles et objets).
     */
    class TiledMapCollision : public Component
    {
    public:
        /**
         * @brief Constructeur par défaut.
         */
        TiledMapCollision();

        /**
         * @brief Constructeur avec carte Tiled.
         * @param map Pointeur vers la carte Tiled.
         */
        explicit TiledMapCollision(TiledMap *map);

        /**
         * @brief Destructeur.
         */
        ~TiledMapCollision() override = default;

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
         * @brief Définit les calques à considérer pour les collisions.
         * @param layerNames Liste des noms de calques.
         */
        void setCollisionLayers(const std::vector<std::string> &layerNames);

        /**
         * @brief Ajoute un calque à la liste des calques de collision.
         * @param layerName Nom du calque.
         */
        void addCollisionLayer(const std::string &layerName);

        /**
         * @brief Active ou désactive les collisions avec les tuiles.
         * @param enabled true pour activer, false pour désactiver.
         */
        void setTileCollisionsEnabled(bool enabled);

        /**
         * @brief Active ou désactive les collisions avec les objets.
         * @param enabled true pour activer, false pour désactiver.
         */
        void setObjectCollisionsEnabled(bool enabled);

        /**
         * @brief Vérifie si une position spécifique entre en collision avec une tuile.
         * @param position Position à vérifier.
         * @return true si une collision est détectée, false sinon.
         */
        bool checkCollisionAtPosition(const sf::Vector2f &position) const;

        /**
         * @brief Vérifie si un rectangle entre en collision avec des tuiles.
         * @param rect Rectangle à vérifier.
         * @return true si une collision est détectée, false sinon.
         */
        bool checkCollisionWithRect(const sf::FloatRect &rect) const;

        /**
         * @brief Vérifie si une entité entre en collision avec des tuiles.
         * @param entity Pointeur vers l'entité à vérifier.
         * @return true si une collision est détectée, false sinon.
         */
        bool checkEntityCollision(Entity *entity) const;

        /**
         * @brief Obtient tous les objets en collision avec un rectangle.
         * @param rect Rectangle à vérifier.
         * @return Vecteur de pointeurs vers les objets en collision.
         */
        std::vector<Object *> getObjectsInRect(const sf::FloatRect &rect) const;

        /**
         * @brief Obtient tous les objets d'un type spécifique en collision avec un rectangle.
         * @param rect Rectangle à vérifier.
         * @param objectType Type d'objets à rechercher.
         * @return Vecteur de pointeurs vers les objets en collision.
         */
        std::vector<Object *> getObjectsOfTypeInRect(const sf::FloatRect &rect, const std::string &objectType) const;

        /**
         * @brief Vérifie si un rectangle peut se déplacer dans une direction sans collision.
         * @param rect Rectangle à déplacer.
         * @param movement Vecteur de déplacement souhaité.
         * @param slidingEnabled Si true, permet un glissement le long des murs.
         * @return Vecteur de déplacement ajusté pour éviter les collisions.
         */
        sf::Vector2f resolveCollision(const sf::FloatRect &rect, const sf::Vector2f &movement, bool slidingEnabled = true) const;

    private:
        TiledMap *m_map;                            ///< Carte Tiled associée
        std::vector<std::string> m_collisionLayers; ///< Calques à considérer pour les collisions
        bool m_tileCollisionsEnabled;               ///< Si les collisions avec les tuiles sont activées
        bool m_objectCollisionsEnabled;             ///< Si les collisions avec les objets sont activées

        /**
         * @brief Vérifie si une tuile à une position donnée est collidable.
         * @param tileX Coordonnée X de la tuile.
         * @param tileY Coordonnée Y de la tuile.
         * @return true si la tuile est collidable, false sinon.
         */
        bool isTileCollidable(int tileX, int tileY) const;
    };

} // namespace Orenji