#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace Orenji
{

    // Déclarations avancées
    class Entity;
    using EntityPtr = std::shared_ptr<Entity>;

    /**
     * @class Scene
     * @brief Représente une scène de jeu avec des entités
     */
    class Scene
    {
    public:
        /**
         * @brief Constructeur
         * @param name Nom de la scène
         */
        explicit Scene(const std::string &name = "DefaultScene");

        /**
         * @brief Destructeur virtuel
         */
        virtual ~Scene();

        /**
         * @brief Initialise la scène
         * @return true si l'initialisation a réussi, false sinon
         */
        virtual bool initialize();

        /**
         * @brief Met à jour la scène et ses entités
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        virtual void update(float deltaTime);

        /**
         * @brief Dessine la scène et ses entités
         * @param window La fenêtre SFML sur laquelle dessiner
         */
        virtual void render(sf::RenderWindow &window);

        /**
         * @brief Ajoute une entité à la scène
         * @param entity L'entité à ajouter
         * @return Pointeur vers l'entité ajoutée
         */
        EntityPtr addEntity(EntityPtr entity);

        /**
         * @brief Crée et ajoute une nouvelle entité à la scène
         * @param name Nom de l'entité
         * @return Pointeur vers l'entité créée
         */
        EntityPtr createEntity(const std::string &name = "Entity");

        /**
         * @brief Trouve une entité par son ID
         * @param id ID de l'entité
         * @return Pointeur vers l'entité trouvée, nullptr si non trouvée
         */
        EntityPtr findEntityById(unsigned int id) const;

        /**
         * @brief Trouve une entité par son nom
         * @param name Nom de l'entité
         * @return Pointeur vers la première entité trouvée avec ce nom, nullptr si non trouvée
         */
        EntityPtr findEntityByName(const std::string &name) const;

        /**
         * @brief Supprime une entité de la scène
         * @param id ID de l'entité à supprimer
         * @return true si l'entité a été supprimée, false sinon
         */
        bool removeEntity(unsigned int id);

        /**
         * @brief Supprime toutes les entités de la scène
         */
        void clearEntities();

        /**
         * @brief Obtient le nom de la scène
         * @return Nom de la scène
         */
        const std::string &getName() const { return m_name; }

        /**
         * @brief Définit le nom de la scène
         * @param name Nouveau nom de la scène
         */
        void setName(const std::string &name) { m_name = name; }

        /**
         * @brief Obtient le nombre d'entités dans la scène
         * @return Nombre d'entités
         */
        size_t getEntityCount() const { return m_entities.size(); }

    private:
        std::string m_name;                ///< Nom de la scène
        std::vector<EntityPtr> m_entities; ///< Liste des entités dans la scène
        unsigned int m_nextEntityId;       ///< ID pour la prochaine entité
    };

    /**
     * @typedef ScenePtr
     * @brief Pointeur vers une scène
     */
    using ScenePtr = std::shared_ptr<Scene>;

} // namespace Orenji