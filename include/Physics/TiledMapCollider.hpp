#pragma once

#include "../Resources/TiledMapLoader.hpp"
#include "Box2DWrapper.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Physics
{
    /**
     * @brief Classe permettant de créer des objets de collision à partir d'une carte Tiled
     */
    class TiledMapCollider
    {
    public:
        /**
         * @brief Constructeur
         * @param physics Référence vers le wrapper Box2D
         */
        TiledMapCollider(Box2DWrapper &physics);

        /**
         * @brief Destructeur
         */
        ~TiledMapCollider();

        /**
         * @brief Créer les corps statiques pour tous les objets collidables d'une carte
         * @param map Référence vers le chargeur de carte Tiled
         * @return Nombre d'objets de collision créés
         */
        int createCollisionsFromMap(const Resources::TiledMapLoader &map);

        /**
         * @brief Créer les corps statiques pour tous les objets d'une couche spécifique
         * @param map Référence vers le chargeur de carte Tiled
         * @param layerName Nom de la couche contenant les objets
         * @return Nombre d'objets de collision créés
         */
        int createCollisionsFromLayer(const Resources::TiledMapLoader &map, const std::string &layerName);

        /**
         * @brief Créer les corps statiques pour tous les objets d'un type spécifique
         * @param map Référence vers le chargeur de carte Tiled
         * @param type Type des objets à utiliser pour les collisions
         * @return Nombre d'objets de collision créés
         */
        int createCollisionsFromType(const Resources::TiledMapLoader &map, const std::string &type);

        /**
         * @brief Supprimer tous les corps de collision créés
         */
        void clear();

        /**
         * @brief Obtenir le nombre d'objets de collision créés
         * @return Nombre d'objets de collision
         */
        size_t getCollisionCount() const;

    private:
        Box2DWrapper &m_physics;
        std::vector<b2Body *> m_collisionBodies;

        /**
         * @brief Créer un corps de collision à partir d'un objet de carte
         * @param obj Objet de carte
         * @return Pointeur vers le corps créé ou nullptr en cas d'échec
         */
        b2Body *createCollisionBody(const Resources::MapObject &obj);
    };

} // namespace Physics