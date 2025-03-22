#pragma once

#include "Scene/Scene.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

namespace Orenji
{
    /**
     * @class SceneManager
     * @brief Gère les différentes scènes du jeu
     *
     * Cette classe permet de charger, activer et désactiver des scènes.
     * Elle gère la transition entre les scènes et leur cycle de vie.
     */
    class SceneManager
    {
    public:
        using ScenePtr = std::unique_ptr<Scene>;
        using SceneFactory = std::function<ScenePtr()>;

        /**
         * @brief Constructeur
         */
        SceneManager();

        /**
         * @brief Destructeur
         */
        ~SceneManager();

        /**
         * @brief Initialise le gestionnaire de scènes
         * @return true si l'initialisation a réussi, false sinon
         */
        bool initialize();

        /**
         * @brief Enregistre un type de scène avec une fabrique
         * @param id Identifiant unique pour ce type de scène
         */
        template <typename T>
        void registerScene(const std::string &id);

        /**
         * @brief Enregistre une scène prédéfinie
         * @param id Identifiant unique pour la scène
         * @param scene Scène à enregistrer
         */
        void addScene(const std::string &id, ScenePtr scene);

        /**
         * @brief Crée et ajoute une nouvelle scène du type enregistré
         * @param id Identifiant du type de scène
         * @param sceneName Nom à donner à la scène
         * @return Pointeur vers la scène créée, nullptr si échec
         */
        Scene *createScene(const std::string &id, const std::string &sceneName);

        /**
         * @brief Active une scène
         * @param id Identifiant de la scène à activer
         * @param exclusive Si true, désactive toutes les autres scènes
         * @return true si la scène a été activée, false sinon
         */
        bool activateScene(const std::string &id, bool exclusive = true);

        /**
         * @brief Désactive une scène
         * @param id Identifiant de la scène à désactiver
         * @return true si la scène a été désactivée, false sinon
         */
        bool deactivateScene(const std::string &id);

        /**
         * @brief Supprime une scène
         * @param id Identifiant de la scène à supprimer
         * @return true si la scène a été supprimée, false sinon
         */
        bool removeScene(const std::string &id);

        /**
         * @brief Récupère une scène par son identifiant
         * @param id Identifiant de la scène
         * @return Pointeur vers la scène, nullptr si non trouvée
         */
        Scene *getScene(const std::string &id);

        /**
         * @brief Vérifie si une scène est active
         * @param id Identifiant de la scène
         * @return true si la scène est active, false sinon
         */
        bool isSceneActive(const std::string &id) const;

        /**
         * @brief Met à jour toutes les scènes actives
         * @param dt Delta time (temps écoulé depuis la dernière mise à jour)
         */
        void update(float dt);

        /**
         * @brief Dessine toutes les scènes actives
         * @param target Cible de rendu
         */
        void render(sf::RenderTarget &target);

        /**
         * @brief Charge une scène depuis un fichier JSON
         * @param filename Chemin vers le fichier JSON
         * @param id Identifiant à donner à la scène chargée
         * @return true si le chargement a réussi, false sinon
         */
        bool loadSceneFromFile(const std::string &filename, const std::string &id);

        /**
         * @brief Sauvegarde une scène dans un fichier JSON
         * @param id Identifiant de la scène à sauvegarder
         * @param filename Chemin vers le fichier JSON
         * @return true si la sauvegarde a réussi, false sinon
         */
        bool saveSceneToFile(const std::string &id, const std::string &filename);

    private:
        std::unordered_map<std::string, ScenePtr> m_scenes;        ///< Scènes gérées
        std::unordered_map<std::string, SceneFactory> m_factories; ///< Fabriques de scènes
        std::vector<std::string> m_activeScenes;                   ///< Liste des scènes actives
    };

    // Implémentation du template
    template <typename T>
    void SceneManager::registerScene(const std::string &id)
    {
        static_assert(std::is_base_of<Scene, T>::value, "T doit hériter de Scene");
        m_factories[id] = []()
        { return std::make_unique<T>(); };
    }

} // namespace Orenji