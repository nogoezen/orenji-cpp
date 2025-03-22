#pragma once

#include "State/State.hpp"
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <functional>

namespace Orenji
{
    /**
     * @class StateManager
     * @brief Gère les différents états du jeu
     *
     * Cette classe implémente un système de gestion d'états avec un pattern stack.
     * Les états peuvent être empilés, dépilés, et remplacés.
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
         * @brief Enregistre un type d'état avec une fonction factory
         * @param id Identifiant du type d'état
         * @param factory Fonction factory qui crée une instance de l'état
         */
        template <typename T>
        void registerState(const std::string &id);

        /**
         * @brief Pousse un nouvel état sur la pile
         * @param id Identifiant du type d'état à créer
         * @param replace Indique si le nouvel état doit remplacer l'état actuel
         * @param args Arguments supplémentaires à passer au constructeur de l'état
         * @return true si l'état a été ajouté, false sinon
         */
        template <typename... Args>
        bool pushState(const std::string &id, bool replace = false, Args &&...args);

        /**
         * @brief Retire l'état au sommet de la pile
         * @return true si un état a été retiré, false sinon
         */
        bool popState();

        /**
         * @brief Vide la pile d'états
         */
        void clearStates();

        /**
         * @brief Vérifie si la pile d'états est vide
         * @return true si la pile est vide, false sinon
         */
        bool isEmpty() const;

        /**
         * @brief Obtient l'état au sommet de la pile
         * @return Référence vers l'état au sommet de la pile, nullptr si la pile est vide
         */
        State *getCurrentState();

        /**
         * @brief Met à jour l'état actuel
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        void update(float deltaTime);

        /**
         * @brief Gère les événements pour l'état actuel
         * @param event L'événement SFML à traiter
         */
        void handleEvent(const sf::Event &event);

        /**
         * @brief Dessine les états
         * @param window La fenêtre SFML sur laquelle dessiner
         */
        void render(sf::RenderWindow &window);

        /**
         * @brief Traite les changements d'états demandés
         * Cette méthode doit être appelée après la mise à jour et le rendu
         * pour effectuer les changements d'états en attente.
         */
        void processStateChanges();

    private:
        // Pile d'états
        std::vector<StatePtr> m_states;

        // Map des fabriques d'états
        std::map<std::string, std::function<StatePtr()>> m_factories;

        // Actions en attente pour les changements d'états
        enum class Action
        {
            Push,
            Pop,
            Clear
        };
        struct PendingChange
        {
            Action action;
            std::string stateId;
            bool replace;
        };
        std::vector<PendingChange> m_pendingChanges;

        // Flag indiquant si un changement d'état est en cours
        bool m_isProcessingChanges;
    };

    // Implémentation des méthodes template
    template <typename T>
    void StateManager::registerState(const std::string &id)
    {
        m_factories[id] = []() -> StatePtr
        {
            return std::make_unique<T>();
        };
    }

    template <typename... Args>
    bool StateManager::pushState(const std::string &id, bool replace, Args &&...args)
    {
        if (m_isProcessingChanges)
        {
            // Si on est en train de traiter des changements, on ajoute à la liste d'attente
            m_pendingChanges.push_back({Action::Push, id, replace});
            return true;
        }

        // Vérifie si le type d'état est enregistré
        auto found = m_factories.find(id);
        if (found == m_factories.end())
        {
            return false;
        }

        // Crée le nouvel état
        StatePtr state = found->second();

        // Initialise l'état
        if (!state->initialize())
        {
            return false;
        }

        // Désactive l'état actuel si la pile n'est pas vide
        if (!m_states.empty())
        {
            if (replace)
            {
                m_states.back()->onInactive();
                m_states.pop_back();
            }
            else
            {
                m_states.back()->onInactive();
            }
        }

        // Ajoute le nouvel état et l'active
        m_states.push_back(std::move(state));
        m_states.back()->onActive();

        return true;
    }

} // namespace Orenji