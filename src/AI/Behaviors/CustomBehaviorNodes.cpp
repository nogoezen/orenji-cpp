#include "AI/Behaviors/CustomBehaviorNodes.hpp"
#include <chrono>
#include <cmath>
#include <iostream>

namespace Orenji
{
    namespace AI
    {

        // MoveToPosition
        MoveToPosition::MoveToPosition(const std::string &name, const BT::NodeConfig &config)
            : BaseAIAction(name, config), m_speed(1.0f)
        {
        }

        BT::PortsList MoveToPosition::providedPorts()
        {
            return {
                BT::InputPort<float>("speed"),
                BT::InputPort<Core::EntityId>("entity_id"),
                BT::InputPort<float>("target_x"),
                BT::InputPort<float>("target_y")};
        }

        BT::NodeStatus MoveToPosition::tick()
        {
            Core::EntityId entityId;
            if (!getInput("entity_id", entityId))
            {
                return BT::NodeStatus::FAILURE;
            }

            float targetX, targetY;
            if (!getInput("target_x", targetX) || !getInput("target_y", targetY))
            {
                return BT::NodeStatus::FAILURE;
            }

            float speed = 1.0f;
            getInput("speed", speed);
            m_speed = speed;

            auto *entityManager = Core::EntityManager::getInstance();
            if (!entityManager->hasEntity(entityId))
            {
                return BT::NodeStatus::FAILURE;
            }

            auto transform = entityManager->getComponent<Core::TransformComponent>(entityId);
            if (!transform)
            {
                return BT::NodeStatus::FAILURE;
            }

            // Position actuelle
            sf::Vector2f currentPos = transform->getPosition();
            sf::Vector2f targetPos(targetX, targetY);

            // Calculer le chemin si nécessaire
            if (m_currentPath.empty())
            {
                // Utiliser A* pour calculer le chemin
                m_currentPath = m_pathfinder.findPath(currentPos, targetPos);
                if (m_currentPath.empty())
                {
                    return BT::NodeStatus::FAILURE;
                }
            }

            // Suivre le chemin
            if (!m_currentPath.empty())
            {
                sf::Vector2f nextPoint = m_currentPath[0];
                sf::Vector2f direction = nextPoint - currentPos;
                float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

                // Si on est proche du point, passer au suivant
                if (distance < 5.0f)
                {
                    m_currentPath.erase(m_currentPath.begin());
                    if (m_currentPath.empty())
                    {
                        // Si on a atteint la destination
                        return BT::NodeStatus::SUCCESS;
                    }
                    nextPoint = m_currentPath[0];
                    direction = nextPoint - currentPos;
                    distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                }

                // Normaliser et appliquer la vitesse
                if (distance > 0)
                {
                    direction /= distance;
                }

                // Mettre à jour la position
                sf::Vector2f newPos = currentPos + direction * m_speed;
                transform->setPosition(newPos);

                // Définir l'orientation de l'entité
                float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;
                transform->setRotation(angle);

                return BT::NodeStatus::RUNNING;
            }

            return BT::NodeStatus::SUCCESS;
        }

        // IsTargetVisible
        IsTargetVisible::IsTargetVisible(const std::string &name, const BT::NodeConfig &config)
            : ConditionNode(name, config)
        {
        }

        BT::PortsList IsTargetVisible::providedPorts()
        {
            return {
                BT::InputPort<Core::EntityId>("entity_id"),
                BT::InputPort<Core::EntityId>("target_id"),
                BT::InputPort<float>("view_distance"),
                BT::InputPort<float>("view_angle")};
        }

        BT::NodeStatus IsTargetVisible::tick()
        {
            Core::EntityId entityId, targetId;
            if (!getInput("entity_id", entityId) || !getInput("target_id", targetId))
            {
                return BT::NodeStatus::FAILURE;
            }

            float viewDistance = 300.0f;
            float viewAngle = 120.0f;
            getInput("view_distance", viewDistance);
            getInput("view_angle", viewAngle);

            auto *entityManager = Core::EntityManager::getInstance();
            if (!entityManager->hasEntity(entityId) || !entityManager->hasEntity(targetId))
            {
                return BT::NodeStatus::FAILURE;
            }

            auto entityTransform = entityManager->getComponent<Core::TransformComponent>(entityId);
            auto targetTransform = entityManager->getComponent<Core::TransformComponent>(targetId);
            if (!entityTransform || !targetTransform)
            {
                return BT::NodeStatus::FAILURE;
            }

            sf::Vector2f entityPos = entityTransform->getPosition();
            sf::Vector2f targetPos = targetTransform->getPosition();
            float entityRotation = entityTransform->getRotation();

            // Vérifier la distance
            sf::Vector2f diff = targetPos - entityPos;
            float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            if (distance > viewDistance)
            {
                return BT::NodeStatus::FAILURE;
            }

            // Vérifier l'angle
            float angle = std::atan2(diff.y, diff.x) * 180.0f / 3.14159f;
            float angleDiff = std::abs(angle - entityRotation);
            while (angleDiff > 180.0f)
                angleDiff = std::abs(angleDiff - 360.0f);

            if (angleDiff > viewAngle / 2.0f)
            {
                return BT::NodeStatus::FAILURE;
            }

            // La cible est visible
            return BT::NodeStatus::SUCCESS;
        }

        // WasTargetSeen
        WasTargetSeen::WasTargetSeen(const std::string &name, const BT::NodeConfig &config)
            : ConditionNode(name, config)
        {
        }

        BT::PortsList WasTargetSeen::providedPorts()
        {
            return {
                BT::InputPort<Core::EntityId>("entity_id"),
                BT::InputPort<float>("memory_time")};
        }

        BT::NodeStatus WasTargetSeen::tick()
        {
            Core::EntityId entityId;
            if (!getInput("entity_id", entityId))
            {
                return BT::NodeStatus::FAILURE;
            }

            float memoryTime = 5.0f; // Par défaut, se souvient pendant 5 secondes
            getInput("memory_time", memoryTime);

            auto *entityManager = Core::EntityManager::getInstance();
            if (!entityManager->hasEntity(entityId))
            {
                return BT::NodeStatus::FAILURE;
            }

            auto aiComponent = entityManager->getComponent<AIComponent>(entityId);
            if (!aiComponent)
            {
                return BT::NodeStatus::FAILURE;
            }

            // Vérifier si la cible a été vue récemment
            float lastSeenTime = 0.0f;
            if (!aiComponent->getBlackboardValue("last_seen_time", lastSeenTime))
            {
                return BT::NodeStatus::FAILURE;
            }

            float currentTime = 0.0f;
            if (!aiComponent->getBlackboardValue("current_time", currentTime))
            {
                return BT::NodeStatus::FAILURE;
            }

            if (currentTime - lastSeenTime <= memoryTime)
            {
                return BT::NodeStatus::SUCCESS;
            }

            return BT::NodeStatus::FAILURE;
        }

        // GetTargetPosition
        GetTargetPosition::GetTargetPosition(const std::string &name, const BT::NodeConfig &config)
            : BaseAIAction(name, config)
        {
        }

        BT::PortsList GetTargetPosition::providedPorts()
        {
            return {
                BT::InputPort<Core::EntityId>("entity_id"),
                BT::InputPort<Core::EntityId>("target_id"),
                BT::OutputPort<float>("target_x"),
                BT::OutputPort<float>("target_y")};
        }

        BT::NodeStatus GetTargetPosition::tick()
        {
            Core::EntityId entityId, targetId;
            if (!getInput("entity_id", entityId) || !getInput("target_id", targetId))
            {
                return BT::NodeStatus::FAILURE;
            }

            auto *entityManager = Core::EntityManager::getInstance();
            if (!entityManager->hasEntity(entityId) || !entityManager->hasEntity(targetId))
            {
                return BT::NodeStatus::FAILURE;
            }

            auto targetTransform = entityManager->getComponent<Core::TransformComponent>(targetId);
            if (!targetTransform)
            {
                return BT::NodeStatus::FAILURE;
            }

            sf::Vector2f targetPos = targetTransform->getPosition();
            setOutput("target_x", targetPos.x);
            setOutput("target_y", targetPos.y);

            // Mise à jour du dernier emplacement connu
            auto aiComponent = entityManager->getComponent<AIComponent>(entityId);
            if (aiComponent)
            {
                aiComponent->setBlackboardValue("last_known_x", targetPos.x);
                aiComponent->setBlackboardValue("last_known_y", targetPos.y);

                float currentTime = 0.0f;
                if (aiComponent->getBlackboardValue("current_time", currentTime))
                {
                    aiComponent->setBlackboardValue("last_seen_time", currentTime);
                }
            }

            return BT::NodeStatus::SUCCESS;
        }

        // GetPatrolPoints
        GetPatrolPoints::GetPatrolPoints(const std::string &name, const BT::NodeConfig &config)
            : BaseAIAction(name, config)
        {
        }

        BT::PortsList GetPatrolPoints::providedPorts()
        {
            return {
                BT::InputPort<Core::EntityId>("entity_id")};
        }

        BT::NodeStatus GetPatrolPoints::tick()
        {
            Core::EntityId entityId;
            if (!getInput("entity_id", entityId))
            {
                return BT::NodeStatus::FAILURE;
            }

            auto *entityManager = Core::EntityManager::getInstance();
            if (!entityManager->hasEntity(entityId))
            {
                return BT::NodeStatus::FAILURE;
            }

            auto aiComponent = entityManager->getComponent<AIComponent>(entityId);
            if (!aiComponent)
            {
                return BT::NodeStatus::FAILURE;
            }

            auto transform = entityManager->getComponent<Core::TransformComponent>(entityId);
            if (!transform)
            {
                return BT::NodeStatus::FAILURE;
            }

            // Initialisation des points de patrouille autour de la position initiale
            sf::Vector2f basePos = transform->getPosition();

            // Vérifier si les points existent déjà
            std::vector<sf::Vector2f> patrolPoints;
            if (!aiComponent->getBlackboardValue("patrol_points", patrolPoints))
            {
                // Créer des points de patrouille en cercle autour de la position
                patrolPoints.clear();
                int numPoints = 4;
                float radius = 200.0f;

                for (int i = 0; i < numPoints; ++i)
                {
                    float angle = i * 2.0f * 3.14159f / numPoints;
                    sf::Vector2f point(
                        basePos.x + radius * std::cos(angle),
                        basePos.y + radius * std::sin(angle));
                    patrolPoints.push_back(point);
                }

                // Ajouter les points au blackboard
                aiComponent->setBlackboardValue("patrol_points", patrolPoints);
                aiComponent->setBlackboardValue("current_patrol_index", 0);
            }

            return BT::NodeStatus::SUCCESS;
        }

        // GetNextPatrolPoint
        GetNextPatrolPoint::GetNextPatrolPoint(const std::string &name, const BT::NodeConfig &config)
            : BaseAIAction(name, config)
        {
        }

        BT::PortsList GetNextPatrolPoint::providedPorts()
        {
            return {
                BT::InputPort<Core::EntityId>("entity_id"),
                BT::OutputPort<float>("target_x"),
                BT::OutputPort<float>("target_y")};
        }

        BT::NodeStatus GetNextPatrolPoint::tick()
        {
            Core::EntityId entityId;
            if (!getInput("entity_id", entityId))
            {
                return BT::NodeStatus::FAILURE;
            }

            auto *entityManager = Core::EntityManager::getInstance();
            if (!entityManager->hasEntity(entityId))
            {
                return BT::NodeStatus::FAILURE;
            }

            auto aiComponent = entityManager->getComponent<AIComponent>(entityId);
            if (!aiComponent)
            {
                return BT::NodeStatus::FAILURE;
            }

            // Récupérer les points de patrouille
            std::vector<sf::Vector2f> patrolPoints;
            if (!aiComponent->getBlackboardValue("patrol_points", patrolPoints) || patrolPoints.empty())
            {
                return BT::NodeStatus::FAILURE;
            }

            // Récupérer l'index actuel
            int currentIndex = 0;
            if (!aiComponent->getBlackboardValue("current_patrol_index", currentIndex))
            {
                currentIndex = 0;
            }

            // Vérifier si l'index est valide
            if (currentIndex >= patrolPoints.size())
            {
                currentIndex = 0;
            }

            // Obtenir le point actuel
            sf::Vector2f currentPoint = patrolPoints[currentIndex];
            setOutput("target_x", currentPoint.x);
            setOutput("target_y", currentPoint.y);

            // Vérifier si le point est atteint
            auto transform = entityManager->getComponent<Core::TransformComponent>(entityId);
            if (transform)
            {
                sf::Vector2f currentPos = transform->getPosition();
                sf::Vector2f diff = currentPoint - currentPos;
                float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

                if (distance < 10.0f)
                {
                    // Passer au point suivant
                    currentIndex = (currentIndex + 1) % patrolPoints.size();
                    aiComponent->setBlackboardValue("current_patrol_index", currentIndex);
                }
            }

            return BT::NodeStatus::SUCCESS;
        }

        // IsPositionReached
        IsPositionReached::IsPositionReached(const std::string &name, const BT::NodeConfig &config)
            : ConditionNode(name, config)
        {
        }

        BT::PortsList IsPositionReached::providedPorts()
        {
            return {
                BT::InputPort<Core::EntityId>("entity_id"),
                BT::InputPort<float>("target_x"),
                BT::InputPort<float>("target_y"),
                BT::InputPort<float>("threshold")};
        }

        BT::NodeStatus IsPositionReached::tick()
        {
            Core::EntityId entityId;
            if (!getInput("entity_id", entityId))
            {
                return BT::NodeStatus::FAILURE;
            }

            float targetX, targetY;
            if (!getInput("target_x", targetX) || !getInput("target_y", targetY))
            {
                return BT::NodeStatus::FAILURE;
            }

            float threshold = 10.0f;
            getInput("threshold", threshold);

            auto *entityManager = Core::EntityManager::getInstance();
            if (!entityManager->hasEntity(entityId))
            {
                return BT::NodeStatus::FAILURE;
            }

            auto transform = entityManager->getComponent<Core::TransformComponent>(entityId);
            if (!transform)
            {
                return BT::NodeStatus::FAILURE;
            }

            sf::Vector2f currentPos = transform->getPosition();
            sf::Vector2f targetPos(targetX, targetY);
            sf::Vector2f diff = targetPos - currentPos;
            float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

            if (distance <= threshold)
            {
                return BT::NodeStatus::SUCCESS;
            }

            return BT::NodeStatus::FAILURE;
        }

        // Wait
        Wait::Wait(const std::string &name, const BT::NodeConfig &config)
            : BaseAIAction(name, config), m_duration_ms(1000), m_initialized(false)
        {
        }

        BT::PortsList Wait::providedPorts()
        {
            return {
                BT::InputPort<int>("milliseconds")};
        }

        BT::NodeStatus Wait::tick()
        {
            if (!m_initialized)
            {
                getInput("milliseconds", m_duration_ms);
                m_startTime = std::chrono::steady_clock::now();
                m_initialized = true;
            }

            auto elapsed = std::chrono::steady_clock::now() - m_startTime;
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

            if (elapsed_ms >= m_duration_ms)
            {
                m_initialized = false;
                return BT::NodeStatus::SUCCESS;
            }

            return BT::NodeStatus::RUNNING;
        }

    } // namespace AI
} // namespace Orenji