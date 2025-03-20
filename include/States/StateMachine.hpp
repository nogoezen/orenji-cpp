#pragma once
#include <memory>
#include <vector>
#include <stack>
#include <SFML/Graphics.hpp>
#include "State.hpp"

namespace Orenji
{

    class StateMachine
    {
    public:
        StateMachine();
        ~StateMachine();

        void pushState(std::unique_ptr<State> state);
        void popState();
        void changeState(std::unique_ptr<State> state);

        State *getCurrentState() const;
        bool isEmpty() const;

        void update(float deltaTime);
        void render();
        void handleInput();

        // Méthodes pour accéder à la fenêtre
        void setWindow(sf::RenderWindow &window);
        sf::RenderWindow &getWindow() const;

        // Méthode pour récupérer le temps écoulé depuis le démarrage
        float getElapsedTime() const;

    private:
        std::stack<std::unique_ptr<State>> m_states;
        std::unique_ptr<State> m_pendingState;
        bool m_isAdding;
        bool m_isRemoving;
        bool m_isReplacing;
        sf::RenderWindow *m_window;
        sf::Clock m_globalClock;

        void applyPendingChanges();
    };

} // namespace Orenji