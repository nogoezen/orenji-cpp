#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

namespace Orenji
{

    class StateManager;

    /**
     * @class State
     * @brief Classe de base pour tous les états du jeu
     */
    class State
    {
    public:
        /**
         * @brief Constructeur
         * @param stateManager Pointeur vers le gestionnaire d'états
         */
        State(StateManager &stateManager);

        /**
         * @brief Destructeur virtuel
         */
        virtual ~State() = default;

        /**
         * @brief Initialise l'état
         */
        virtual void initialize() = 0;

        /**
         * @brief Met à jour l'état
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        virtual void update(float deltaTime) = 0;

        /**
         * @brief Gère les événements
         * @param event Événement à traiter
         */
        virtual void handleEvent(const sf::Event &event) = 0;

        /**
         * @brief Dessine l'état
         * @param window Fenêtre sur laquelle dessiner
         */
        virtual void draw(sf::RenderWindow &window) = 0;

        /**
         * @brief Méthode appelée lorsque l'état est mis en pause
         */
        virtual void pause() {}

        /**
         * @brief Méthode appelée lorsque l'état est repris
         */
        virtual void resume() {}

        /**
         * @brief Vérifie si l'état est transparent (si les états en dessous doivent être rendus)
         * @return true si l'état est transparent, false sinon
         */
        virtual bool isTransparent() const { return false; }

    protected:
        StateManager &m_stateManager;
    };

} // namespace Orenji
