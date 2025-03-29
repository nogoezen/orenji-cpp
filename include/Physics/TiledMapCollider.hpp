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
         * @param category Catégorie de collision pour les objets de la carte
         * @param mask Masque de collision pour les objets de la carte
         * @return Nombre d'objets de collision créés
         */
        int createCollisionsFromMap(const Resources::TiledMapLoader &map,
                                    CollisionCategory category = CollisionCategory::Platform,
                                    CollisionCategory mask = CollisionCategory::All);

        /**
         * @brief Créer les corps statiques pour tous les objets d'une couche spécifique
         * @param map Référence vers le chargeur de carte Tiled
         * @param layerName Nom de la couche contenant les objets
         * @param category Catégorie de collision pour les objets de la couche
         * @param mask Masque de collision pour les objets de la couche
         * @return Nombre d'objets de collision créés
         */
        int createCollisionsFromLayer(const Resources::TiledMapLoader &map, const std::string &layerName,
                                      CollisionCategory category = CollisionCategory::Platform,
                                      CollisionCategory mask = CollisionCategory::All);

        /**
         * @brief Créer les corps statiques pour tous les objets d'un type spécifique
         * @param map Référence vers le chargeur de carte Tiled
         * @param type Type des objets à utiliser pour les collisions
         * @param category Catégorie de collision pour les objets du type
         * @param mask Masque de collision pour les objets du type
         * @return Nombre d'objets de collision créés
         */
        int createCollisionsFromType(const Resources::TiledMapLoader &map, const std::string &type,
                                     CollisionCategory category = CollisionCategory::Platform,
                                     CollisionCategory mask = CollisionCategory::All);

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
        std::vector<b2BodyId> m_collisionBodies;

        /**
         * @brief Créer un corps de collision à partir d'un objet de carte
         * @param obj Objet de carte
         * @param category Catégorie de collision pour l'objet
         * @param mask Masque de collision pour l'objet
         * @return ID du corps créé ou 0 en cas d'échec
         */
        b2BodyId createCollisionBody(const Resources::MapObject &obj,
                                     CollisionCategory category,
                                     CollisionCategory mask);
    };

} // namespace Physics