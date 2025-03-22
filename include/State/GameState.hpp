#pragma once

#include "State/State.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace Orenji
{
    /**
     * @class GameState
     * @brief État principal du jeu
     *
     * Représente l'état où le joueur joue réellement au jeu.
     */
    class GameState : public State
    {
    public:
        /**
         * @brief Constructeur
         * @param level Niveau à charger (optionnel)
         */
        GameState(const std::string &level = "");

        /**
         * @brief Destructeur
         */
        virtual ~GameState() = default;

        /**
         * @brief Initialise l'état
         * @return true si l'initialisation a réussi, false sinon
         */
        virtual bool initialize() override;

        /**
         * @brief Met à jour l'état
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        virtual void update(float deltaTime) override;

        /**
         * @brief Gère les événements
         * @param event L'événement SFML à traiter
         */
        virtual void handleEvent(const sf::Event &event) override;

        /**
         * @brief Dessine l'état
         * @param window La fenêtre SFML sur laquelle dessiner
         */
        virtual void render(sf::RenderWindow &window) override;

        /**
         * @brief Appelé lorsque l'état devient actif
         */
        virtual void onActive() override;

        /**
         * @brief Appelé lorsque l'état devient inactif
         */
        virtual void onInactive() override;

        /**
         * @brief Charge un niveau
         * @param levelName Nom du niveau à charger
         * @return true si le niveau a été chargé avec succès, false sinon
         */
        bool loadLevel(const std::string &levelName);

        /**
         * @brief Met en pause ou reprend le jeu
         * @param paused true pour mettre en pause, false pour reprendre
         */
        void setPaused(bool paused);

        /**
         * @brief Vérifie si le jeu est en pause
         * @return true si le jeu est en pause, false sinon
         */
        bool isPaused() const;

    private:
        std::string m_currentLevel; ///< Nom du niveau chargé
        bool m_isPaused;            ///< Indique si le jeu est en pause
        bool m_isGameOver;          ///< Indique si le jeu est terminé
        float m_gameTime;           ///< Temps de jeu écoulé

        // Ressources graphiques
        sf::Font m_font;                   ///< Police utilisée
        sf::Text m_statusText;             ///< Texte d'état (pause, game over, etc.)
        sf::Text m_scoreText;              ///< Texte du score
        sf::RectangleShape m_pauseOverlay; ///< Overlay lorsque le jeu est en pause

        // Éléments de jeu fictifs (à remplacer par les vrais éléments du jeu)
        sf::RectangleShape m_player;                 ///< Rectangle représentant le joueur
        std::vector<sf::RectangleShape> m_obstacles; ///< Obstacles
        int m_score;                                 ///< Score du joueur
    };

} // namespace Orenji