#include "AI/Components/AIComponent.hpp"
#include "AI/BehaviorTree.hpp"

namespace AI
{
    namespace Components
    {
        AIComponent::AIComponent()
            : m_enabled(true)
        {
        }

        void AIComponent::setBehaviorTree(std::shared_ptr<BehaviorTree> behaviorTree)
        {
            m_behaviorTree = behaviorTree;
        }

        std::shared_ptr<BehaviorTree> AIComponent::getBehaviorTree() const
        {
            return m_behaviorTree;
        }

        void AIComponent::setEnabled(bool enabled)
        {
            m_enabled = enabled;
        }

        bool AIComponent::isEnabled() const
        {
            return m_enabled;
        }

    } // namespace Components
} // namespace AI