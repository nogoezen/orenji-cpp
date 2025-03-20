#pragma once

#include "../Core/Component.hpp"
#include "TiledMap.hpp"
#include <SFML/Graphics.hpp>
#include <string>

namespace Orenji
{

    /**
     * @brief Composant pour associer une entité à un objet Tiled.
     *
     * Ce composant permet de maintenir une référence entre une entité du jeu
     * et un objet défini dans une carte Tiled, facilitant l'interaction entre
     * le moteur d'entités et les données de la carte.
     */
    class TileObject : public Component
    {
    public:
        /**
         * @brief Constructeur par défaut.
         */
        TileObject();

        /**
         * @brief Constructeur avec ID d'objet.
         * @param objectId ID de l'objet Tiled associé.
         */
        explicit TileObject(int objectId);

        /**
         * @brief Destructeur.
         */
        ~TileObject() override = default;

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
         * @brief Définit l'ID de l'objet Tiled associé.
         * @param objectId ID de l'objet.
         */
        void setObjectId(int objectId);

        /**
         * @brief Récupère l'ID de l'objet Tiled associé.
         * @return ID de l'objet.
         */
        int getObjectId() const;

        /**
         * @brief Définit la carte Tiled contenant l'objet.
         * @param map Pointeur vers la carte Tiled.
         */
        void setMap(TiledMap *map);

        /**
         * @brief Récupère la carte Tiled contenant l'objet.
         * @return Pointeur vers la carte Tiled.
         */
        TiledMap *getMap() const;

        /**
         * @brief Récupère l'objet Tiled associé.
         * @return Pointeur vers l'objet Tiled ou nullptr si non trouvé.
         */
        Object *getObject() const;

        /**
         * @brief Synchronise la position de l'entité avec celle de l'objet Tiled.
         */
        void syncPositionFromObject();

        /**
         * @brief Synchronise la position de l'objet Tiled avec celle de l'entité.
         */
        void syncPositionToObject();

        /**
         * @brief Vérifie si une propriété spécifique existe dans l'objet Tiled.
         * @param name Nom de la propriété.
         * @return true si la propriété existe, false sinon.
         */
        bool hasProperty(const std::string &name) const;

        /**
         * @brief Récupère une propriété de type chaîne de l'objet Tiled.
         * @param name Nom de la propriété.
         * @param defaultValue Valeur par défaut si la propriété n'existe pas.
         * @return Valeur de la propriété ou valeur par défaut.
         */
        std::string getStringProperty(const std::string &name, const std::string &defaultValue = "") const;

        /**
         * @brief Récupère une propriété de type entier de l'objet Tiled.
         * @param name Nom de la propriété.
         * @param defaultValue Valeur par défaut si la propriété n'existe pas.
         * @return Valeur de la propriété ou valeur par défaut.
         */
        int getIntProperty(const std::string &name, int defaultValue = 0) const;

        /**
         * @brief Récupère une propriété de type flottant de l'objet Tiled.
         * @param name Nom de la propriété.
         * @param defaultValue Valeur par défaut si la propriété n'existe pas.
         * @return Valeur de la propriété ou valeur par défaut.
         */
        float getFloatProperty(const std::string &name, float defaultValue = 0.0f) const;

        /**
         * @brief Récupère une propriété de type booléen de l'objet Tiled.
         * @param name Nom de la propriété.
         * @param defaultValue Valeur par défaut si la propriété n'existe pas.
         * @return Valeur de la propriété ou valeur par défaut.
         */
        bool getBoolProperty(const std::string &name, bool defaultValue = false) const;

    private:
        int m_objectId;  ///< ID de l'objet Tiled associé
        TiledMap *m_map; ///< Carte Tiled contenant l'objet
    };

} // namespace Orenji