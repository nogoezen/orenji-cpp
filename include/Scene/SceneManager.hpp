#pragma once

#include "Scene/Scene.hpp"
#include <map>
#include <string>
#include <memory>

namespace Orenji
{

    /**
     * @class SceneManager
     * @brief Gère les scènes du jeu
     */
    class SceneManager
    {
    public:
        /**
         * @brief Constructeur par défaut
         */
        SceneManager();

        /**
         * @brief Destructeur
         */
        ~SceneManager();

        /**
         * @brief Ajoute une scène au gestionnaire
         * @param name Nom de la scène
         * @param scene Pointeur vers la scène
         * @return true si la scène a été ajoutée, false sinon
         */
        bool addScene(const std::string &name, ScenePtr scene);

        /**
         * @brief Crée une nouvelle scène et l'ajoute au gestionnaire
         * @param name Nom de la scène
         * @return Pointeur vers la scène créée, nullptr si échec
         */
        ScenePtr createScene(const std::string &name);

        /**
         * @brief Supprime une scène du gestionnaire
         * @param name Nom de la scène
         * @return true si la scène a été supprimée, false sinon
         */
        bool removeScene(const std::string &name);

        /**
         * @brief Obtient une scène par son nom
         * @param name Nom de la scène
         * @return Pointeur vers la scène, nullptr si non trouvée
         */
        ScenePtr getScene(const std::string &name) const;

        /**
         * @brief Définit la scène active
         * @param name Nom de la scène
         * @return true si la scène a été activée, false sinon
         */
        bool setActiveScene(const std::string &name);

        /**
         * @brief Obtient la scène active
         * @return Pointeur vers la scène active, nullptr si aucune
         */
        ScenePtr getActiveScene() const;

        /**
         * @brief Vérifie si une scène existe
         * @param name Nom de la scène
         * @return true si la scène existe, false sinon
         */
        bool hasScene(const std::string &name) const;

        /**
         * @brief Met à jour la scène active
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        void update(float deltaTime);

        /**
         * @brief Dessine la scène active
         * @param window La fenêtre SFML sur laquelle dessiner
         */
        void render(sf::RenderWindow &window);

    private:
        std::map<std::string, ScenePtr> m_scenes; ///< Map des scènes par nom
        std::string m_activeSceneName;            ///< Nom de la scène active
    };

} // namespace Orenji