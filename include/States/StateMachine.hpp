#pragma once
#include <memory>
#include <vector>
#include <stack>
#include "State.hpp"

namespace Orenji
{

    class StateMachine
    {
    public:
        StateMachine();
        ~StateMachine();

        void pushState(std::shared_ptr<State> state);
        void popState();
        void changeState(std::shared_ptr<State> state);

        std::shared_ptr<State> getCurrentState() const;
        bool isEmpty() const;

        void update(float deltaTime);
        void render();
        void handleInput();

    private:
        std::stack<std::shared_ptr<State>> m_states;
        std::shared_ptr<State> m_pendingState;
        bool m_isAdding;
        bool m_isRemoving;
        bool m_isReplacing;

        void applyPendingChanges();
    };

} // namespace Orenji