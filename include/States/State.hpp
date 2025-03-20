#pragma once
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

namespace Orenji
{

    class StateMachine;

    class State
    {
    public:
        enum class StateStatus
        {
            Running,
            Paused,
            Finished
        };

        State();
        virtual ~State();

        virtual bool initialize() { return true; }

        virtual bool onEnter() { return true; }
        virtual bool onExit() { return true; }
        virtual bool update(float deltaTime) = 0;
        virtual bool render() = 0;
        virtual bool handleInput() { return true; }

        StateStatus getStatus() const;
        void setStatus(StateStatus status);

        void setStateMachine(StateMachine *stateMachine);
        StateMachine *getStateMachine() const;

        virtual void render(sf::RenderWindow &window) {}

    private:
        StateStatus m_status;
        StateMachine *m_stateMachine;
    };

} // namespace Orenji