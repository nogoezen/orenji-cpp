#pragma once
#include <memory>
#include <string>

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

        virtual bool onEnter() = 0;
        virtual bool onExit() = 0;
        virtual bool update(float deltaTime) = 0;
        virtual bool render() = 0;
        virtual bool handleInput() = 0;

        StateStatus getStatus() const;
        void setStatus(StateStatus status);

        void setParent(StateMachine *parent);
        StateMachine *getParent() const;

    private:
        StateStatus m_status;
        StateMachine *m_parent;
    };

} // namespace Orenji