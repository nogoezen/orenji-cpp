#include "AI/BehaviorTree.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace AI
{
    //--------------------------------------------------
    // BehaviorTree
    //--------------------------------------------------

    BehaviorTree::BehaviorTree(const std::string &name)
        : m_name(name), m_blackboard(BT::Blackboard::create())
    {
        // Créer le blackboard par défaut
        m_blackboard->set("name", name);
    }

    BehaviorTree::~BehaviorTree()
    {
    }

    std::string BehaviorTree::getName() const
    {
        return m_name;
    }

    bool BehaviorTree::createFromFile(const std::string &xmlFile)
    {
        try
        {
            // Vérifier si le fichier existe
            std::ifstream file(xmlFile);
            if (!file.good())
            {
                std::cerr << "Cannot open file: " << xmlFile << std::endl;
                return false;
            }

            // Créer l'arbre depuis le fichier XML
            m_tree = std::make_unique<BT::Tree>(m_factory.createTreeFromFile(xmlFile));

            // Associer le blackboard
            m_tree->blackboard_stack.push_back(m_blackboard);

            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error creating behavior tree from file: " << e.what() << std::endl;
            return false;
        }
    }

    bool BehaviorTree::createFromText(const std::string &xmlContent)
    {
        try
        {
            // Créer l'arbre depuis le texte XML
            m_tree = std::make_unique<BT::Tree>(m_factory.createTreeFromText(xmlContent));

            // Associer le blackboard
            m_tree->blackboard_stack.push_back(m_blackboard);

            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error creating behavior tree from text: " << e.what() << std::endl;
            return false;
        }
    }

    void BehaviorTree::setBlackboard(const BT::Blackboard::Ptr &blackboard)
    {
        if (blackboard)
        {
            m_blackboard = blackboard;
            if (m_tree)
            {
                // Mettre à jour le blackboard dans l'arbre existant
                if (!m_tree->blackboard_stack.empty())
                {
                    m_tree->blackboard_stack[0] = m_blackboard;
                }
                else
                {
                    m_tree->blackboard_stack.push_back(m_blackboard);
                }
            }
        }
    }

    BT::Blackboard::Ptr BehaviorTree::getBlackboard() const
    {
        return m_blackboard;
    }

    BT::NodeStatus BehaviorTree::update(float deltaTime)
    {
        if (m_tree)
        {
            // Ajouter deltaTime au blackboard pour que les nœuds puissent l'utiliser
            m_blackboard->set("deltaTime", deltaTime);

            // Exécuter un tick
            return m_tree->tickOnce();
        }

        return BT::NodeStatus::FAILURE;
    }

    void BehaviorTree::reset()
    {
        if (m_tree)
        {
            m_tree->haltTree();
        }
    }

    BT::BehaviorTreeFactory &BehaviorTree::getFactory()
    {
        return m_factory;
    }

    bool BehaviorTree::isValid() const
    {
        return m_tree != nullptr;
    }

    //--------------------------------------------------
    // GameEntityNode - Nœud de base pour tous les nœuds liés à une entité
    //--------------------------------------------------

    GameEntityNode::GameEntityNode(const std::string &name, const BT::NodeConfiguration &config, Core::EntityManager &entityManager)
        : BT::SyncActionNode(name, config), m_entityManager(entityManager), m_entityId(0) // ID invalide par défaut
    {
    }

    BT::PortsList GameEntityNode::providedPorts()
    {
        // Ports communs à tous les nœuds d'entités
        return {BT::InputPort<int>("entity_id")};
    }

    Core::EntityId GameEntityNode::getEntityId() const
    {
        return m_entityId;
    }

    void GameEntityNode::setEntityId(Core::EntityId entityId)
    {
        m_entityId = entityId;
    }

    BT::NodeStatus GameEntityNode::tick()
    {
        // Récupérer l'ID d'entité depuis le blackboard ou les ports
        int entityId = 0;
        if (getInput<int>("entity_id", entityId))
        {
            m_entityId = static_cast<Core::EntityId>(entityId);
        }

        // Vérifier si l'entité existe
        if (m_entityId == 0 || !m_entityManager.entityExists(m_entityId))
        {
            return BT::NodeStatus::FAILURE;
        }

        return BT::NodeStatus::SUCCESS;
    }

    //--------------------------------------------------
    // MoveToTargetNode - Déplace une entité vers une cible
    //--------------------------------------------------

    MoveToTargetNode::MoveToTargetNode(const std::string &name, const BT::NodeConfiguration &config, Core::EntityManager &entityManager)
        : GameEntityNode(name, config, entityManager)
    {
    }

    BT::PortsList MoveToTargetNode::providedPorts()
    {
        // Hériter des ports du parent et ajouter les ports spécifiques
        auto ports = GameEntityNode::providedPorts();
        ports.insert(BT::InputPort<float>("x"));
        ports.insert(BT::InputPort<float>("y"));
        ports.insert(BT::InputPort<float>("speed"));
        ports.insert(BT::OutputPort<bool>("reached"));
        return ports;
    }

    BT::NodeStatus MoveToTargetNode::tick()
    {
        // Appeler la méthode du parent pour valider l'entité
        BT::NodeStatus status = GameEntityNode::tick();
        if (status != BT::NodeStatus::SUCCESS)
        {
            return status;
        }

        // Récupérer la position cible
        float targetX, targetY, speed;
        if (!getInput<float>("x", targetX) || !getInput<float>("y", targetY))
        {
            // Position cible manquante
            return BT::NodeStatus::FAILURE;
        }

        // Vitesse par défaut si non spécifiée
        if (!getInput<float>("speed", speed))
        {
            speed = 100.0f; // Valeur par défaut
        }

        // Récupérer le temps écoulé depuis le dernier tick
        float deltaTime = 0.016f; // 60 FPS par défaut
        auto bb = getBlackboard();
        if (bb && bb->get("deltaTime", deltaTime))
        {
            // Deltatime récupéré du blackboard
        }

        // Pour cet exemple, nous simulons simplement l'atteinte de la cible
        // Dans une implémentation réelle, nous utiliserions le system de physique
        // pour déplacer l'entité vers la cible

        // Simuler l'atteinte de la cible (75% de chance d'y arriver)
        bool reached = (rand() % 100) < 75;

        // Écrire le résultat dans le blackboard
        setOutput("reached", reached);

        if (reached)
        {
            return BT::NodeStatus::SUCCESS;
        }
        else
        {
            return BT::NodeStatus::RUNNING; // Toujours en cours de déplacement
        }
    }

    //--------------------------------------------------
    // IsTargetVisibleNode - Détecte si une cible est visible
    //--------------------------------------------------

    IsTargetVisibleNode::IsTargetVisibleNode(const std::string &name, const BT::NodeConfiguration &config, Core::EntityManager &entityManager)
        : GameEntityNode(name, config, entityManager)
    {
    }

    BT::PortsList IsTargetVisibleNode::providedPorts()
    {
        auto ports = GameEntityNode::providedPorts();
        ports.insert(BT::InputPort<int>("target_id"));
        ports.insert(BT::InputPort<float>("view_distance"));
        ports.insert(BT::InputPort<float>("view_angle"));
        ports.insert(BT::OutputPort<float>("distance"));
        return ports;
    }

    BT::NodeStatus IsTargetVisibleNode::tick()
    {
        // Valider l'entité observer
        BT::NodeStatus status = GameEntityNode::tick();
        if (status != BT::NodeStatus::SUCCESS)
        {
            return status;
        }

        // Récupérer l'ID de la cible
        int targetId;
        if (!getInput<int>("target_id", targetId))
        {
            return BT::NodeStatus::FAILURE;
        }

        // Vérifier si la cible existe
        if (targetId <= 0 || !m_entityManager.entityExists(static_cast<Core::EntityId>(targetId)))
        {
            return BT::NodeStatus::FAILURE;
        }

        // Récupérer la distance de vue et l'angle
        float viewDistance = 300.0f; // Valeur par défaut
        getInput<float>("view_distance", viewDistance);

        float viewAngle = 60.0f; // Valeur par défaut en degrés
        getInput<float>("view_angle", viewAngle);

        // Dans une implémentation réelle, nous vérifierions si la cible
        // est visible en fonction de la position, de la distance et de
        // l'angle de vue.

        // Ici, simulons une visibilité aléatoire (50% de chance)
        bool isVisible = (rand() % 100) < 50;

        // Si visible, définir une distance aléatoire
        if (isVisible)
        {
            float distance = static_cast<float>(rand() % static_cast<int>(viewDistance));
            setOutput("distance", distance);
            return BT::NodeStatus::SUCCESS;
        }

        return BT::NodeStatus::FAILURE;
    }

    //--------------------------------------------------
    // FindPathNode - Calcule un chemin vers une cible
    //--------------------------------------------------

    FindPathNode::FindPathNode(const std::string &name, const BT::NodeConfiguration &config, Core::EntityManager &entityManager)
        : GameEntityNode(name, config, entityManager)
    {
    }

    BT::PortsList FindPathNode::providedPorts()
    {
        auto ports = GameEntityNode::providedPorts();
        ports.insert(BT::InputPort<float>("start_x"));
        ports.insert(BT::InputPort<float>("start_y"));
        ports.insert(BT::InputPort<float>("target_x"));
        ports.insert(BT::InputPort<float>("target_y"));
        ports.insert(BT::OutputPort<std::string>("path"));
        return ports;
    }

    BT::NodeStatus FindPathNode::tick()
    {
        // Valider l'entité
        BT::NodeStatus status = GameEntityNode::tick();
        if (status != BT::NodeStatus::SUCCESS)
        {
            return status;
        }

        // Récupérer les coordonnées de départ et d'arrivée
        float startX, startY, targetX, targetY;
        if (!getInput<float>("start_x", startX) || !getInput<float>("start_y", startY) ||
            !getInput<float>("target_x", targetX) || !getInput<float>("target_y", targetY))
        {
            return BT::NodeStatus::FAILURE;
        }

        // Dans une implémentation réelle, nous utiliserions notre système de pathfinding
        // pour calculer un chemin entre les deux points.

        // Ici, simulons un chemin trouvé (80% de chance)
        bool pathFound = (rand() % 100) < 80;

        if (pathFound)
        {
            // Simuler un chemin sous forme de chaîne
            std::stringstream pathStr;
            pathStr << startX << "," << startY << ";"
                    << (startX + targetX) / 2 << "," << (startY + targetY) / 2 << ";"
                    << targetX << "," << targetY;

            setOutput("path", pathStr.str());
            return BT::NodeStatus::SUCCESS;
        }

        return BT::NodeStatus::FAILURE;
    }

    //--------------------------------------------------
    // PatrolNode - Fait patrouiller une entité
    //--------------------------------------------------

    PatrolNode::PatrolNode(const std::string &name, const BT::NodeConfiguration &config, Core::EntityManager &entityManager)
        : GameEntityNode(name, config, entityManager)
    {
    }

    BT::PortsList PatrolNode::providedPorts()
    {
        auto ports = GameEntityNode::providedPorts();
        ports.insert(BT::InputPort<std::string>("patrol_points"));
        ports.insert(BT::InputPort<float>("wait_time"));
        ports.insert(BT::InputPort<bool>("loop"));
        ports.insert(BT::OutputPort<int>("current_point"));
        return ports;
    }

    BT::NodeStatus PatrolNode::tick()
    {
        // Valider l'entité
        BT::NodeStatus status = GameEntityNode::tick();
        if (status != BT::NodeStatus::SUCCESS)
        {
            return status;
        }

        // Récupérer les points de patrouille
        std::string patrolPointsStr;
        if (!getInput<std::string>("patrol_points", patrolPointsStr))
        {
            return BT::NodeStatus::FAILURE;
        }

        // Récupérer le temps d'attente à chaque point
        float waitTime = 1.0f; // Valeur par défaut
        getInput<float>("wait_time", waitTime);

        // Récupérer si la patrouille doit boucler
        bool loop = true; // Valeur par défaut
        getInput<bool>("loop", loop);

        // Récupérer l'index du point actuel depuis le blackboard
        auto bb = getBlackboard();
        int currentPoint = 0;
        std::string pointKey = "patrol_current_point_" + std::to_string(m_entityId);

        // Si l'index existe, le récupérer
        if (bb && bb->get(pointKey, currentPoint))
        {
            // Point récupéré
        }

        // Simuler le passage au point suivant
        currentPoint = (currentPoint + 1) % 4; // Simuler 4 points

        // Stocker le nouvel index
        if (bb)
        {
            bb->set(pointKey, currentPoint);
        }

        // Retourner l'index actuel
        setOutput("current_point", currentPoint);

        // Simuler une patrouille en cours (toujours RUNNING)
        return BT::NodeStatus::RUNNING;
    }

    //--------------------------------------------------
    // BehaviorTreeNodeRegistry - Registre des nœuds personnalisés
    //--------------------------------------------------

    void BehaviorTreeNodeRegistry::registerNodes(BT::BehaviorTreeFactory &factory, Core::EntityManager &entityManager)
    {
        // Enregistrer les nœuds personnalisés dans la factory

        // Nœud MoveToTarget
        factory.registerBuilder<MoveToTargetNode>("MoveToTarget", [&entityManager](const std::string &name, const BT::NodeConfiguration &config)
                                                  { return std::make_unique<MoveToTargetNode>(name, config, entityManager); }, MoveToTargetNode::providedPorts());

        // Nœud IsTargetVisible
        factory.registerBuilder<IsTargetVisibleNode>("IsTargetVisible", [&entityManager](const std::string &name, const BT::NodeConfiguration &config)
                                                     { return std::make_unique<IsTargetVisibleNode>(name, config, entityManager); }, IsTargetVisibleNode::providedPorts());

        // Nœud FindPath
        factory.registerBuilder<FindPathNode>("FindPath", [&entityManager](const std::string &name, const BT::NodeConfiguration &config)
                                              { return std::make_unique<FindPathNode>(name, config, entityManager); }, FindPathNode::providedPorts());

        // Nœud Patrol
        factory.registerBuilder<PatrolNode>("Patrol", [&entityManager](const std::string &name, const BT::NodeConfiguration &config)
                                            { return std::make_unique<PatrolNode>(name, config, entityManager); }, PatrolNode::providedPorts());
    }

} // namespace AI