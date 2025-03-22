#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "State/StateManager.hpp"
#include "Scene/SceneManager.hpp"
#include "Config.hpp"

namespace Orenji
{

    /**
     * @class Game
     * @brief Classe principale du moteur de jeu
     */
    class Game
    {
    public:
        /**
         * @struct Settings
         * @brief Structure de configuration du jeu
         */
        struct Settings
        {
            std::string windowTitle = "Orenji Engine"; ///< Titre de la fenêtre
            unsigned int windowWidth = 800;            ///< Largeur de la fenêtre
            unsigned int windowHeight = 600;           ///< Hauteur de la fenêtre
            unsigned int frameRateLimit = 60;          ///< Limite de FPS
            bool verticalSync = true;                  ///< Synchronisation verticale
            bool fullscreen = false;                   ///< Mode plein écran
        };

        /**
         * @brief Obtient l'instance unique du jeu (singleton)
         * @return Référence vers l'instance unique du jeu
         */
        static Game &getInstance();

        /**
         * @brief Initialise le jeu
         * @param settings Configuration du jeu
         * @return true si l'initialisation a réussi, false sinon
         */
        bool initialize(const Settings &settings = Settings());

        /**
         * @brief Lance la boucle de jeu
         */
        void run();

        /**
         * @brief Arrête la boucle de jeu
         */
        void quit();

        /**
         * @brief Obtient la fenêtre du jeu
         * @return Référence vers la fenêtre du jeu
         */
        sf::RenderWindow &getWindow();

        /**
         * @brief Obtient le gestionnaire d'états
         * @return Référence vers le gestionnaire d'états
         */
        StateManager &getStateManager();

        /**
         * @brief Obtient le gestionnaire de scènes
         * @return Référence vers le gestionnaire de scènes
         */
        SceneManager &getSceneManager();

        /**
         * @brief Obtient le temps écoulé depuis la dernière mise à jour
         * @return Temps écoulé en secondes
         */
        float getDeltaTime() const;

    private:
        /**
         * @brief Constructeur (privé pour le pattern singleton)
         */
        Game();

        /**
         * @brief Destructeur
         */
        ~Game();

        /**
         * @brief Traite les événements
         */
        void processEvents();

        /**
         * @brief Met à jour le jeu
         */
        void update();

        /**
         * @brief Dessine le jeu
         */
        void render();

        std::unique_ptr<sf::RenderWindow> m_window; ///< Fenêtre du jeu
        StateManager m_stateManager;                ///< Gestionnaire d'états
        SceneManager m_sceneManager;                ///< Gestionnaire de scènes
        sf::Clock m_clock;                          ///< Horloge pour le calcul du temps
        float m_deltaTime;                          ///< Temps écoulé depuis la dernière mise à jour
        bool m_running;                             ///< Indique si le jeu est en cours d'exécution
    };

} // namespace Orenji