#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

namespace Orenji
{
    class StateMachine;

    /**
     * @brief Classe de base pour les états du jeu
     *
     * Cette classe fournit l'interface de base pour tous les états
     * utilisés dans la machine à états du jeu.
     */
    class State
    {
    public:
        /**
         * @brief Constructeur
         *
         * @param machine Référence vers la machine à états
         */
        State(StateMachine &machine);

        /**
         * @brief Destructeur virtuel
         */
        virtual ~State() = default;

        /**
         * @brief Initialise l'état
         *
         * Cette méthode est appelée lorsque l'état devient actif
         */
        virtual void onEnter() {}

        /**
         * @brief Nettoie l'état
         *
         * Cette méthode est appelée lorsque l'état n'est plus actif
         */
        virtual void onExit() {}

        /**
         * @brief Met à jour l'état
         *
         * @param dt Temps écoulé depuis la dernière mise à jour (en secondes)
         */
        virtual void update(float dt) = 0;

        /**
         * @brief Gère les événements
         *
         * @param event Événement SFML à traiter
         */
        virtual void handleEvent(const sf::Event &event) = 0;

        /**
         * @brief Dessine l'état
         *
         * @param target Cible de rendu
         */
        virtual void draw(sf::RenderTarget &target) = 0;

        /**
         * @brief Vérifie si l'état est transparent
         *
         * Un état transparent permet de voir les états qui se trouvent en-dessous
         *
         * @return true si l'état est transparent, false sinon
         */
        virtual bool isTransparent() const { return false; }

        /**
         * @brief Vérifie si l'état est exclusif
         *
         * Un état exclusif empêche la mise à jour des états qui se trouvent en-dessous
         *
         * @return true si l'état est exclusif, false sinon
         */
        virtual bool isExclusive() const { return true; }

        /**
         * @brief Récupère l'identifiant de l'état
         *
         * @return Identifiant de l'état
         */
        virtual std::string getStateID() const = 0;

    protected:
        StateMachine &m_machine; ///< Référence vers la machine à états
    };

    /**
     * @brief Alias pour un pointeur intelligent vers un State
     */
    using StatePtr = std::shared_ptr<State>;

} // namespace Orenji