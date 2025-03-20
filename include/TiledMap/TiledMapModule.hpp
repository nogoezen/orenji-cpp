#pragma once

#include "../Core/Module.hpp"
#include "TiledMap.hpp"
#include "TiledMapRenderer.hpp"
#include "TiledMapCollision.hpp"
#include "TiledMapFactory.hpp"
#include "TileObject.hpp"

namespace Orenji
{

    /**
     * @brief Module intégrant les fonctionnalités de Tiled Map dans le moteur.
     *
     * Ce module enregistre tous les composants et systèmes nécessaires pour utiliser
     * des cartes Tiled dans le moteur de jeu.
     */
    class TiledMapModule : public Module
    {
    public:
        /**
         * @brief Constructeur par défaut.
         */
        TiledMapModule();

        /**
         * @brief Destructeur.
         */
        ~TiledMapModule() override = default;

        /**
         * @brief Initialise le module.
         * @param engine Pointeur vers le moteur.
         */
        void initialize(Engine *engine) override;

        /**
         * @brief Termine le module.
         */
        void shutdown() override;

        /**
         * @brief Récupère le nom du module.
         * @return Nom du module.
         */
        std::string getName() const override;

        /**
         * @brief Enregistre les types de composants Tiled dans le moteur.
         * @param engine Pointeur vers le moteur.
         */
        void registerComponents(Engine *engine);

        /**
         * @brief Charge une carte Tiled.
         * @param filename Chemin vers le fichier de carte (.tmx).
         * @return Pointeur vers la carte chargée ou nullptr en cas d'échec.
         */
        static std::unique_ptr<TiledMap> loadMap(const std::string &filename);

        /**
         * @brief Crée une entité avec un renderer de carte.
         * @param entityManager Gestionnaire d'entités.
         * @param map Pointeur vers la carte Tiled.
         * @return Pointeur vers l'entité créée.
         */
        static Entity *createMapEntity(EntityManager *entityManager, TiledMap *map);

        /**
         * @brief Configure la collision pour une carte.
         * @param mapEntity Entité contenant la carte.
         * @param collisionLayers Liste des noms de calques de collision.
         * @return Pointeur vers le composant de collision.
         */
        static TiledMapCollision *setupMapCollision(Entity *mapEntity, const std::vector<std::string> &collisionLayers);
    };

} // namespace Orenji