#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace Orenji
{

    /**
     * @class State
     * @brief Interface de base pour les états du jeu
     */
    class State
    {
    public:
        /**
         * @brief Constructeur virtuel par défaut
         */
        State() = default;

        /**
         * @brief Destructeur virtuel
         */
        virtual ~State() = default;

        /**
         * @brief Initialise l'état
         * @return true si l'initialisation a réussi, false sinon
         */
        virtual bool initialize() = 0;

        /**
         * @brief Met à jour l'état
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        virtual void update(float deltaTime) = 0;

        /**
         * @brief Gère les événements
         * @param event L'événement SFML à traiter
         */
        virtual void handleEvent(const sf::Event &event) = 0;

        /**
         * @brief Dessine l'état
         * @param window La fenêtre SFML sur laquelle dessiner
         */
        virtual void render(sf::RenderWindow &window) = 0;

        /**
         * @brief Appelé lorsque l'état est activé (devient l'état courant)
         */
        virtual void onActive() {}

        /**
         * @brief Appelé lorsque l'état est désactivé (un autre état devient l'état courant)
         */
        virtual void onInactive() {}

        /**
         * @brief Vérifie si l'état doit être retiré
         * @return true si l'état doit être retiré, false sinon
         */
        virtual bool shouldRemove() const { return m_shouldRemove; }

    protected:
        bool m_shouldRemove = false;
    };

    /**
     * @typedef StatePtr
     * @brief Pointeur vers un état
     */
    using StatePtr = std::unique_ptr<State>;

} // namespace Orenji