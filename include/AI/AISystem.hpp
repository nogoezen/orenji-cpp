#pragma once

#include "Components/AIComponent.hpp"
#include <string>
#include <memory>
#include <unordered_map>
#include <behaviortree_cpp/behavior_tree.h>

namespace Orenji
{
    namespace Core
    {
        class EntityManager;
        typedef unsigned int EntityId;
    }

    namespace AI
    {

        /**
         * @brief Système gérant les comportements d'IA des entités
         */
        class AISystem
        {
        public:
            AISystem();
            ~AISystem();

            /**
             * @brief Initialise le système d'IA
             */
            bool init();

            /**
             * @brief Met à jour tous les comportements d'IA
             * @param deltaTime Temps écoulé depuis la dernière mise à jour
             */
            void update(float deltaTime);

            /**
             * @brief Crée un arbre de comportement à partir d'un fichier XML
             * @param name Nom de l'arbre de comportement
             * @param filePath Chemin du fichier XML
             * @return true si la création a réussi, false sinon
             */
            bool createBehaviorTreeFromFile(const std::string &name, const std::string &filePath);

            /**
             * @brief Assigne un arbre de comportement à une entité
             * @param entityId ID de l'entité
             * @param behaviorTreeName Nom de l'arbre de comportement
             * @return true si l'assignation a réussi, false sinon
             */
            bool assignBehaviorTree(Core::EntityId entityId, const std::string &behaviorTreeName);

            /**
             * @brief Met à jour le blackboard d'une entité avec ses paramètres actuels
             * @param entityId ID de l'entité
             */
            void updateEntityBlackboard(Core::EntityId entityId);

        private:
            bool m_initialized;
            std::shared_ptr<BT::BehaviorTreeFactory> m_factory;
            std::unordered_map<std::string, BT::Tree> m_behaviorTrees;
        };

    } // namespace AI
} // namespace Orenji