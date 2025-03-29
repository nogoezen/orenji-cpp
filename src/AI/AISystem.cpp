#include "../../include/AI/AISystem.hpp"
#include "../../include/Core/EntityManager.hpp"
#include "../../include/AI/Pathfinding.hpp"
#include "../../include/AI/BehaviorTree.hpp"
#include "../../include/AI/Components/AIComponent.hpp"
#include "../../include/AI/Behaviors/CustomBehaviorNodes.hpp"
#include <iostream>
#include <filesystem>

namespace AI
{
    // Implémentation temporaire de BehaviorTree
    class BehaviorTree
    {
    public:
        BehaviorTree(const std::string &name) : m_name(name) {}
        std::string getName() const { return m_name; }
        // À développer avec BehaviorTree.CPP dans une future implémentation
    private:
        std::string m_name;
    };

    AISystem::AISystem(Core::EntityManager &entityManager)
        : Core::System(entityManager)
    {
        // Créer et initialiser le système de pathfinding
        m_pathfinder = std::make_unique<Pathfinding::AStar>();

        // Par défaut, utiliser une grille de taille modeste
        m_pathfinder->setGridSize(100, 100);

        std::cout << "AISystem created" << std::endl;
    }

    AISystem::~AISystem()
    {
        std::cout << "AISystem destroyed" << std::endl;

        // Nettoyer les arbres de comportement
        m_behaviorTrees.clear();
    }

    bool AISystem::init()
    {
        if (m_initialized)
        {
            return true;
        }

        // Créer la factory
        m_factory = std::make_shared<BT::BehaviorTreeFactory>();

        // Enregistrer les nœuds personnalisés
        RegisterCustomNodes(*m_factory);

        // Charger les arbres de comportement depuis les fichiers XML
        std::filesystem::path behaviorsDir = "resources/behaviors/";

        if (!std::filesystem::exists(behaviorsDir))
        {
            std::cerr << "Behaviors directory not found: " << behaviorsDir << std::endl;
            return false;
        }

        try
        {
            // Créer les arbres de comportement à partir des fichiers XML
            createBehaviorTreeFromFile("patrol", "resources/behaviors/patrol.xml");
            createBehaviorTreeFromFile("chase", "resources/behaviors/chase.xml");
            createBehaviorTreeFromFile("attack", "resources/behaviors/attack.xml");
            createBehaviorTreeFromFile("flee", "resources/behaviors/flee.xml");
            createBehaviorTreeFromFile("enemy_ai", "resources/behaviors/enemy_ai.xml");
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error loading behavior trees: " << e.what() << std::endl;
            return false;
        }

        m_initialized = true;
        return true;
    }

    void AISystem::update(float deltaTime)
    {
        if (!m_initialized)
        {
            return;
        }

        // Mettre à jour tous les composants AI
        auto entityManager = Core::EntityManager::getInstance();
        auto entities = entityManager->getEntitiesWithComponent<AIComponent>();

        for (auto entity : entities)
        {
            auto aiComponent = entityManager->getComponent<AIComponent>(entity);
            if (!aiComponent || !aiComponent->isEnabled())
            {
                continue;
            }

            auto behaviorTree = aiComponent->getBehaviorTree();
            if (!behaviorTree)
            {
                continue;
            }

            // Mettre à jour le blackboard avec le deltaTime et l'entityId
            auto blackboard = behaviorTree->blackboard;
            blackboard->set("deltaTime", deltaTime);
            blackboard->set("entity_id", entity);

            // Mettre à jour d'autres paramètres du blackboard
            updateEntityBlackboard(entity);

            // Exécuter le behavior tree
            BT::NodeStatus status = behaviorTree->tickRoot();

            // Traiter le statut du behavior tree
            switch (status)
            {
            case BT::NodeStatus::SUCCESS:
                // Comportement terminé avec succès
                break;
            case BT::NodeStatus::FAILURE:
                // Comportement a échoué
                break;
            case BT::NodeStatus::RUNNING:
                // Comportement en cours d'exécution
                break;
            default:
                break;
            }
        }
    }

    Pathfinding::AStar &AISystem::getPathfinder()
    {
        return *m_pathfinder;
    }

    std::shared_ptr<BehaviorTree> AISystem::createBehaviorTree(const std::string &name)
    {
        // Vérifier si un arbre avec ce nom existe déjà
        auto it = m_behaviorTrees.find(name);
        if (it != m_behaviorTrees.end())
        {
            return it->second;
        }

        // Créer un nouvel arbre
        auto tree = std::make_shared<BehaviorTree>(name);

        // Enregistrer les nœuds personnalisés
        BehaviorTreeNodeRegistry::registerNodes(tree->getFactory(), m_entityManager);

        // Ajouter l'arbre à la map
        m_behaviorTrees[name] = tree;

        return tree;
    }

    std::shared_ptr<BehaviorTree> AISystem::getBehaviorTree(const std::string &name)
    {
        auto it = m_behaviorTrees.find(name);
        if (it != m_behaviorTrees.end())
        {
            return it->second;
        }

        return nullptr;
    }

    bool AISystem::createBehaviorTreeFromFile(const std::string &name, const std::string &filePath)
    {
        try
        {
            auto tree = m_factory->createTreeFromFile(filePath);
            m_behaviorTrees[name] = tree;
            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error creating behavior tree from file: " << e.what() << std::endl;
            return false;
        }
    }

    bool AISystem::assignBehaviorTree(Core::EntityId entityId, const std::string &behaviorTreeName)
    {
        auto entityManager = Core::EntityManager::getInstance();
        if (!entityManager->hasEntity(entityId))
        {
            return false;
        }

        // Vérifier si le behavior tree existe
        auto it = m_behaviorTrees.find(behaviorTreeName);
        if (it == m_behaviorTrees.end())
        {
            return false;
        }

        // Récupérer ou créer un AIComponent pour l'entité
        auto aiComponent = entityManager->getComponent<AIComponent>(entityId);
        if (!aiComponent)
        {
            aiComponent = entityManager->addComponent<AIComponent>(entityId);
        }

        // Créer une copie du behavior tree et l'assigner au composant
        auto behaviorTree = m_factory->createTree(it->second.rootNode->name(), it->second.blackboard);
        aiComponent->setBehaviorTree(behaviorTree);

        return true;
    }

    void AISystem::updateEntityBlackboard(Core::EntityId entityId)
    {
        auto entityManager = Core::EntityManager::getInstance();
        if (!entityManager->hasEntity(entityId))
        {
            return;
        }

        auto aiComponent = entityManager->getComponent<AIComponent>(entityId);
        if (!aiComponent)
        {
            return;
        }

        auto transform = entityManager->getComponent<Core::TransformComponent>(entityId);
        if (!transform)
        {
            return;
        }

        // Mise à jour du blackboard avec les informations de l'entité
        aiComponent->setBlackboardValue("entity_id", entityId);

        // Position de l'entité
        sf::Vector2f position = transform->getPosition();
        aiComponent->setBlackboardValue("entity_x", position.x);
        aiComponent->setBlackboardValue("entity_y", position.y);

        // Rotation de l'entité
        aiComponent->setBlackboardValue("entity_rotation", transform->getRotation());

        // Temps actuel (pour mesurer les délais)
        static float currentTime = 0.0f;
        currentTime += 0.016f; // Approximation de deltaTime
        aiComponent->setBlackboardValue("current_time", currentTime);

        // Autres mises à jour spécifiques pourraient être ajoutées ici
    }
} // namespace AI