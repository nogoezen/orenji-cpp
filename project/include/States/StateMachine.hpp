#pragma once

#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <SFML/Graphics.hpp>
#include "States/State.hpp"

namespace Orenji
{
    /**
     * @brief Machine à états pour gérer les différents états du jeu
     *
     * Cette classe gère la pile d'états du jeu, leur mise à jour et leur rendu.
     * Elle permet d'empiler, dépiler et remplacer des états.
     */
    class StateMachine
    {
    public:
        /**
         * @brief Constructeur
         */
        StateMachine();

        /**
         * @brief Destructeur
         */
        ~StateMachine();

        /**
         * @brief Ajoute un état sur la pile
         *
         * @param stateName Nom de l'état à empiler
         */
        void pushState(const std::string &stateName);

        /**
         * @brief Retire l'état au sommet de la pile
         */
        void popState();

        /**
         * @brief Remplace l'état au sommet de la pile
         *
         * @param stateName Nom de l'état de remplacement
         */
        void changeState(const std::string &stateName);

        /**
         * @brief Retire tous les états de la pile
         */
        void clearStates();

        /**
         * @brief Vérifie si la pile est vide
         *
         * @return true si la pile est vide, false sinon
         */
        bool isEmpty() const;

        /**
         * @brief Enregistre un type d'état dans la machine
         *
         * @tparam T Type d'état à enregistrer (doit hériter de State)
         * @param stateName Nom unique pour identifier cet état
         */
        template <typename T>
        void registerState(const std::string &stateName)
        {
            static_assert(std::is_base_of<State, T>::value, "T must derive from State");
            m_factories[stateName] = [this]() -> StatePtr
            {
                return std::make_shared<T>(*this);
            };
        }

        /**
         * @brief Met à jour l'état ou les états actifs
         *
         * @param dt Temps écoulé depuis la dernière mise à jour (en secondes)
         */
        void update(float dt);

        /**
         * @brief Gère les événements pour l'état ou les états actifs
         *
         * @param event Événement SFML à traiter
         */
        void handleEvent(const sf::Event &event);

        /**
         * @brief Dessine l'état ou les états actifs
         *
         * @param target Cible de rendu
         */
        void draw(sf::RenderTarget &target);

    private:
        /**
         * @brief Traite les modifications en attente sur la pile d'états
         */
        void applyPendingChanges();

    private:
        std::vector<StatePtr> m_states;                               ///< Pile d'états
        std::map<std::string, std::function<StatePtr()>> m_factories; ///< Fabriques d'états

        // Actions en attente sur la pile d'états
        struct PendingChange
        {
            enum class Action
            {
                Push,
                Pop,
                Change,
                Clear
            };

            Action action;
            std::string stateName;
        };

        std::vector<PendingChange> m_pendingChanges; ///< Liste des actions en attente
    };

} // namespace Orenji