#pragma once

#include "../Core/Scene.hpp"
#include "TiledMap.hpp"
#include "TiledMapRenderer.hpp"
#include "TiledMapCollision.hpp"
#include "TiledMapFactory.hpp"
#include <string>
#include <memory>

namespace Orenji
{

    /**
     * @brief Classe de base pour créer des scènes utilisant une carte Tiled.
     *
     * Cette classe simplifie la création de scènes basées sur des cartes Tiled
     * en fournissant des fonctionnalités pour charger la carte, configurer le rendu,
     * gérer les collisions et instancier des entités depuis les objets de la carte.
     */
    class TiledMapScene : public Scene
    {
    public:
        /**
         * @brief Constructeur par défaut.
         */
        TiledMapScene();

        /**
         * @brief Constructeur avec nom de fichier de carte.
         * @param mapFilename Chemin vers le fichier de la carte Tiled (.tmx).
         */
        explicit TiledMapScene(const std::string &mapFilename);

        /**
         * @brief Destructeur.
         */
        ~TiledMapScene() override = default;

        /**
         * @brief Charge une carte Tiled à partir d'un fichier.
         * @param filename Chemin vers le fichier de la carte (.tmx).
         * @return true si la carte a été chargée avec succès, false sinon.
         */
        bool loadMap(const std::string &filename);

        /**
         * @brief Initialise la scène.
         *
         * Cette méthode est appelée lors de l'entrée dans la scène.
         */
        void initialize() override;

        /**
         * @brief Met à jour la scène.
         * @param deltaTime Temps écoulé depuis la dernière mise à jour.
         */
        void update(float deltaTime) override;

        /**
         * @brief Dessine la scène.
         * @param target Cible de rendu.
         * @param states États de rendu.
         */
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        /**
         * @brief Récupère la carte Tiled.
         * @return Pointeur vers la carte Tiled.
         */
        TiledMap *getMap();

        /**
         * @brief Récupère le renderer de la carte.
         * @return Pointeur vers le renderer de la carte.
         */
        TiledMapRenderer *getRenderer();

        /**
         * @brief Récupère le gestionnaire de collisions de la carte.
         * @return Pointeur vers le gestionnaire de collisions.
         */
        TiledMapCollision *getCollision();

        /**
         * @brief Récupère la factory d'entités de la carte.
         * @return Pointeur vers la factory d'entités.
         */
        TiledMapFactory *getFactory();

        /**
         * @brief Configure les calques de collisions.
         * @param collisionLayers Noms des calques de collisions.
         */
        void setCollisionLayers(const std::vector<std::string> &collisionLayers);

        /**
         * @brief Configure les calques visibles.
         * @param visibleLayers Noms des calques visibles.
         */
        void setVisibleLayers(const std::vector<std::string> &visibleLayers);

        /**
         * @brief Active ou désactive la visibilité des objets.
         * @param visible true pour afficher les objets, false pour les masquer.
         */
        void setObjectsVisible(bool visible);

        /**
         * @brief Crée les entités à partir des objets de la carte.
         *
         * Cette méthode parcourt tous les objets de la carte et crée les entités
         * correspondantes en utilisant la factory d'entités.
         */
        virtual void createEntitiesFromMap();

        /**
         * @brief Méthode appelée lorsqu'une entité est créée à partir d'un objet de la carte.
         *
         * Cette méthode peut être surchargée pour personnaliser les entités créées.
         * @param entity Pointeur vers l'entité créée.
         * @param object Pointeur vers l'objet Tiled source.
         */
        virtual void onEntityCreated(Entity *entity, Object *object);

    protected:
        /**
         * @brief Enregistre les types d'entités pour la factory.
         *
         * Cette méthode doit être surchargée pour enregistrer les fonctions de création
         * d'entités spécifiques dans la factory.
         */
        virtual void registerEntityTypes();

        /**
         * @brief Crée l'entité de caméra pour la scène.
         * @return Pointeur vers l'entité de caméra créée.
         */
        virtual Entity *createCameraEntity();

        /**
         * @brief Configure la caméra en fonction de la carte.
         * @param cameraEntity Pointeur vers l'entité de caméra.
         */
        virtual void setupCamera(Entity *cameraEntity);

    private:
        std::unique_ptr<TiledMap> m_map; ///< Carte Tiled
        Entity *m_mapEntity;             ///< Entité contenant les composants de la carte
        TiledMapRenderer *m_renderer;    ///< Composant de rendu de la carte
        TiledMapCollision *m_collision;  ///< Composant de collision de la carte
        TiledMapFactory *m_factory;      ///< Factory d'entités de la carte
        Entity *m_cameraEntity;          ///< Entité de caméra
    };

} // namespace Orenji