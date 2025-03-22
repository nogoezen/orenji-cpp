#pragma once

#include "State/State.hpp"
#include <map>
#include <stack>
#include <string>
#include <functional>
#include <memory>

namespace Orenji
{

    /**
     * @class StateManager
     * @brief Gère la pile d'états du jeu
     */
    class StateManager
    {
    public:
        /**
         * @brief Constructeur
         */
        StateManager();

        /**
         * @brief Destructeur
         */
        ~StateManager();

        /**
         * @brief Enregistre un type d'état avec une factory
         * @param id Identifiant du type d'état
         * @param factory Fonction de création de l'état
         */
        template <typename T>
        void registerState(const std::string &id)
        {
            m_factories[id] = [this]() -> StatePtr
            {
                return std::make_unique<T>();
            };
        }

        /**
         * @brief Pousse un nouvel état sur la pile
         * @param id Identifiant du type d'état à créer
         * @return true si l'état a été créé et initialisé avec succès, false sinon
         */
        bool pushState(const std::string &id);

        /**
         * @brief Retire l'état au sommet de la pile
         */
        void popState();

        /**
         * @brief Change l'état courant en retirant l'état courant et en poussant un nouvel état
         * @param id Identifiant du type d'état à créer
         * @return true si l'état a été créé et initialisé avec succès, false sinon
         */
        bool changeState(const std::string &id);

        /**
         * @brief Retire tous les états de la pile
         */
        void clearStates();

        /**
         * @brief Met à jour l'état au sommet de la pile
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        void update(float deltaTime);

        /**
         * @brief Gère les événements pour l'état au sommet de la pile
         * @param event L'événement SFML à traiter
         */
        void handleEvent(const sf::Event &event);

        /**
         * @brief Dessine tous les états de la pile (du bas vers le haut)
         * @param window La fenêtre SFML sur laquelle dessiner
         */
        void render(sf::RenderWindow &window);

        /**
         * @brief Vérifie si la pile d'états est vide
         * @return true si la pile est vide, false sinon
         */
        bool isEmpty() const;

    private:
        /**
         * @brief Type de factory pour créer un état
         */
        using StateFactory = std::function<StatePtr()>;

        std::map<std::string, StateFactory> m_factories; ///< Map des factories d'états
        std::stack<StatePtr> m_states;                   ///< Pile des états
        bool m_pendingChange;                            ///< Indique s'il y a un changement d'état en attente
        std::string m_pendingStateId;                    ///< ID de l'état en attente
    };

} // namespace Orenji