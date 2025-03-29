#pragma once

#include "../../Core/Component.hpp"
#include <string>
#include <memory>

namespace BT
{
    class Blackboard;
}

namespace AI
{
    // Déclaration anticipée
    class BehaviorTree;

    namespace Components
    {
        /**
         * @brief Component for AI entities
         */
        class AIComponent : public Core::Component
        {
        public:
            /**
             * @brief Constructor
             */
            AIComponent();

            /**
             * @brief Destructor
             */
            virtual ~AIComponent() = default;

            /**
             * @brief Set the behavior tree for this entity
             * @param behaviorTree Shared pointer to the behavior tree
             */
            void setBehaviorTree(std::shared_ptr<BehaviorTree> behaviorTree);

            /**
             * @brief Get the behavior tree
             * @return Shared pointer to the behavior tree
             */
            std::shared_ptr<BehaviorTree> getBehaviorTree() const;

            /**
             * @brief Set the blackboard value
             * @param key Key of the value
             * @param value Value to set
             */
            template <typename T>
            void setBlackboardValue(const std::string &key, const T &value);

            /**
             * @brief Get the blackboard value
             * @param key Key of the value
             * @param value Reference to store the value
             * @return True if the value exists
             */
            template <typename T>
            bool getBlackboardValue(const std::string &key, T &value) const;

            /**
             * @brief Enable or disable the AI
             * @param enabled Whether the AI is enabled
             */
            void setEnabled(bool enabled);

            /**
             * @brief Check if the AI is enabled
             * @return True if enabled
             */
            bool isEnabled() const;

        private:
            std::shared_ptr<BehaviorTree> m_behaviorTree;
            bool m_enabled;
        };

        template <typename T>
        void AIComponent::setBlackboardValue(const std::string &key, const T &value)
        {
            if (m_behaviorTree)
            {
                auto blackboard = m_behaviorTree->getBlackboard();
                if (blackboard)
                {
                    blackboard->set(key, value);
                }
            }
        }

        template <typename T>
        bool AIComponent::getBlackboardValue(const std::string &key, T &value) const
        {
            if (m_behaviorTree)
            {
                auto blackboard = m_behaviorTree->getBlackboard();
                if (blackboard)
                {
                    return blackboard->get(key, value);
                }
            }

            return false;
        }

    } // namespace Components
} // namespace AI