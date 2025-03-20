#pragma once

#include "../Core/Entity.hpp"
#include "../Core/EntityManager.hpp"
#include "TiledMap.hpp"
#include <string>
#include <functional>
#include <unordered_map>

namespace Orenji
{

    /**
     * @brief Classe utilitaire pour créer des entités basées sur des objets Tiled.
     *
     * Cette classe facilite la création automatique d'entités de jeu à partir
     * des objets définis dans une carte Tiled, en fonction de leur type ou propriétés.
     */
    class TiledMapFactory
    {
    public:
        /**
         * @brief Type de fonction de création d'entité.
         *
         * Les fonctions de ce type sont appelées pour créer une entité à partir d'un objet Tiled.
         * @param object Objet Tiled servant de base pour l'entité.
         * @param manager Gestionnaire d'entités où l'entité sera ajoutée.
         * @return Pointeur vers l'entité créée ou nullptr en cas d'échec.
         */
        using EntityCreationFunction = std::function<Entity *(const Object &object, EntityManager &manager)>;

        /**
         * @brief Constructeur par défaut.
         */
        TiledMapFactory();

        /**
         * @brief Enregistre une fonction de création pour un type d'objet spécifique.
         * @param objectType Type d'objet Tiled.
         * @param creationFunction Fonction de création à associer.
         */
        void registerType(const std::string &objectType, EntityCreationFunction creationFunction);

        /**
         * @brief Enregistre une fonction de création pour un préfixe de nom d'objet.
         * @param namePrefix Préfixe de nom d'objet Tiled.
         * @param creationFunction Fonction de création à associer.
         */
        void registerNamePrefix(const std::string &namePrefix, EntityCreationFunction creationFunction);

        /**
         * @brief Enregistre une fonction de création pour des objets avec une propriété spécifique.
         * @param propertyName Nom de la propriété.
         * @param propertyValue Valeur de la propriété (en format chaîne).
         * @param creationFunction Fonction de création à associer.
         */
        void registerProperty(const std::string &propertyName, const std::string &propertyValue, EntityCreationFunction creationFunction);

        /**
         * @brief Crée les entités pour tous les objets d'une carte Tiled.
         * @param map Carte Tiled contenant les objets.
         * @param manager Gestionnaire d'entités où les entités seront ajoutées.
         * @return Nombre d'entités créées.
         */
        int createEntities(TiledMap &map, EntityManager &manager);

        /**
         * @brief Crée une entité pour un objet Tiled spécifique.
         * @param object Objet Tiled servant de base pour l'entité.
         * @param manager Gestionnaire d'entités où l'entité sera ajoutée.
         * @return Pointeur vers l'entité créée ou nullptr si aucune fonction de création n'est enregistrée.
         */
        Entity *createEntity(const Object &object, EntityManager &manager);

    private:
        std::unordered_map<std::string, EntityCreationFunction> m_typeCreators;                                      ///< Créateurs par type d'objet
        std::unordered_map<std::string, EntityCreationFunction> m_namePrefixCreators;                                ///< Créateurs par préfixe de nom
        std::unordered_map<std::string, std::unordered_map<std::string, EntityCreationFunction>> m_propertyCreators; ///< Créateurs par propriété
    };

} // namespace Orenji