#pragma once

#include <string>
#include <memory>
#include <vector>
#include <behaviortree_cpp/behavior_tree.h>
#include "../../Core/EntityManager.hpp"
#include "../../Core/TransformComponent.hpp"
#include "../Components/AIComponent.hpp"
#include "../Pathfinding/AStar.hpp"

namespace Orenji
{
    namespace AI
    {

        /**
         * @brief Classe de base pour les nœuds d'action personnalisés
         */
        class BaseAIAction : public BT::ActionNodeBase
        {
        public:
            BaseAIAction(const std::string &name, const BT::NodeConfig &config)
                : ActionNodeBase(name, config) {}

            static BT::PortsList providedPorts() { return {}; }
        };

        /**
         * @brief Actions de déplacement vers une position
         */
        class MoveToPosition : public BaseAIAction
        {
        public:
            MoveToPosition(const std::string &name, const BT::NodeConfig &config);

            static BT::PortsList providedPorts();

            BT::NodeStatus tick() override;

        private:
            float m_speed;
            Pathfinding::AStar m_pathfinder;
            std::vector<sf::Vector2f> m_currentPath;
        };

        /**
         * @brief Action pour vérifier si la cible est visible
         */
        class IsTargetVisible : public BT::ConditionNode
        {
        public:
            IsTargetVisible(const std::string &name, const BT::NodeConfig &config);

            static BT::PortsList providedPorts();

            BT::NodeStatus tick() override;
        };

        /**
         * @brief Action pour vérifier si la cible a été vue récemment
         */
        class WasTargetSeen : public BT::ConditionNode
        {
        public:
            WasTargetSeen(const std::string &name, const BT::NodeConfig &config);

            static BT::PortsList providedPorts();

            BT::NodeStatus tick() override;
        };

        /**
         * @brief Action pour obtenir la position de la cible
         */
        class GetTargetPosition : public BaseAIAction
        {
        public:
            GetTargetPosition(const std::string &name, const BT::NodeConfig &config);

            static BT::PortsList providedPorts();

            BT::NodeStatus tick() override;
        };

        /**
         * @brief Action pour obtenir les points de patrouille
         */
        class GetPatrolPoints : public BaseAIAction
        {
        public:
            GetPatrolPoints(const std::string &name, const BT::NodeConfig &config);

            static BT::PortsList providedPorts();

            BT::NodeStatus tick() override;
        };

        /**
         * @brief Action pour obtenir le prochain point de patrouille
         */
        class GetNextPatrolPoint : public BaseAIAction
        {
        public:
            GetNextPatrolPoint(const std::string &name, const BT::NodeConfig &config);

            static BT::PortsList providedPorts();

            BT::NodeStatus tick() override;
        };

        /**
         * @brief Action pour vérifier si une position est atteinte
         */
        class IsPositionReached : public BT::ConditionNode
        {
        public:
            IsPositionReached(const std::string &name, const BT::NodeConfig &config);

            static BT::PortsList providedPorts();

            BT::NodeStatus tick() override;
        };

        /**
         * @brief Action pour attendreun certain temps
         */
        class Wait : public BaseAIAction
        {
        public:
            Wait(const std::string &name, const BT::NodeConfig &config);

            static BT::PortsList providedPorts();

            BT::NodeStatus tick() override;

        private:
            int m_duration_ms;
            std::chrono::time_point<std::chrono::steady_clock> m_startTime;
            bool m_initialized;
        };

        // Enregistrement des nœuds personnalisés
        inline void RegisterCustomNodes(BT::BehaviorTreeFactory &factory)
        {
            factory.registerNodeType<MoveToPosition>("MoveToPosition");
            factory.registerNodeType<IsTargetVisible>("IsTargetVisible");
            factory.registerNodeType<WasTargetSeen>("WasTargetSeen");
            factory.registerNodeType<GetTargetPosition>("GetTargetPosition");
            factory.registerNodeType<GetPatrolPoints>("GetPatrolPoints");
            factory.registerNodeType<GetNextPatrolPoint>("GetNextPatrolPoint");
            factory.registerNodeType<IsPositionReached>("IsPositionReached");
            factory.registerNodeType<Wait>("Wait");
        }

    } // namespace AI
} // namespace Orenji