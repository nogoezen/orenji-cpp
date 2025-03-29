#pragma once

#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/behavior_tree.h>
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include "../Core/EntityManager.hpp"

namespace AI
{
    /**
     * @brief Classe de wrapper pour BehaviorTree.CPP
     */
    class BehaviorTree
    {
    public:
        /**
         * @brief Constructeur
         * @param name Nom de l'arbre
         */
        BehaviorTree(const std::string &name);

        /**
         * @brief Destructeur
         */
        ~BehaviorTree();

        /**
         * @brief Obtenir le nom de l'arbre
         * @return Nom de l'arbre
         */
        std::string getName() const;

        /**
         * @brief Créer l'arbre à partir d'un fichier XML
         * @param xmlFile Chemin vers le fichier XML
         * @return true si le chargement a réussi
         */
        bool createFromFile(const std::string &xmlFile);

        /**
         * @brief Créer l'arbre à partir d'une chaîne XML
         * @param xmlContent Contenu XML de l'arbre
         * @return true si le chargement a réussi
         */
        bool createFromText(const std::string &xmlContent);

        /**
         * @brief Définir le blackboard pour l'arbre
         * @param blackboard Blackboard partagé
         */
        void setBlackboard(const BT::Blackboard::Ptr &blackboard);

        /**
         * @brief Obtenir le blackboard de l'arbre
         * @return Pointeur vers le blackboard
         */
        BT::Blackboard::Ptr getBlackboard() const;

        /**
         * @brief Mettre à jour l'arbre
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         * @return Statut retourné par l'arbre (SUCCESS, FAILURE, RUNNING)
         */
        BT::NodeStatus update(float deltaTime);

        /**
         * @brief Réinitialiser l'arbre
         */
        void reset();

        /**
         * @brief Obtenir la Factory BT
         * @return Référence vers la Factory BT
         */
        BT::BehaviorTreeFactory &getFactory();

        /**
         * @brief Vérifier si l'arbre est valide
         * @return true si l'arbre est valide
         */
        bool isValid() const;

    private:
        std::string m_name;
        BT::BehaviorTreeFactory m_factory;
        std::unique_ptr<BT::Tree> m_tree;
        BT::Blackboard::Ptr m_blackboard;
    };

    /**
     * @brief Nœud GameEntity pour lier entités du jeu aux arbres de comportement
     */
    class GameEntityNode : public BT::SyncActionNode
    {
    public:
        /**
         * @brief Constructeur
         * @param name Nom du nœud
         * @param config Configuration du nœud
         * @param entityManager Gestionnaire d'entités
         */
        GameEntityNode(const std::string &name, const BT::NodeConfiguration &config, Core::EntityManager &entityManager);

        /**
         * @brief Configuration des ports (entrées/sorties) du nœud
         * @return Configuration statique des ports
         */
        static BT::PortsList providedPorts();

        /**
         * @brief Obtenir l'ID de l'entité associée
         * @return ID de l'entité
         */
        Core::EntityId getEntityId() const;

        /**
         * @brief Définir l'ID de l'entité
         * @param entityId ID de l'entité
         */
        void setEntityId(Core::EntityId entityId);

    protected:
        /**
         * @brief Méthode appelée à l'exécution du nœud
         * @return Statut du nœud
         */
        virtual BT::NodeStatus tick() override;

        Core::EntityManager &m_entityManager;
        Core::EntityId m_entityId;
    };

    /**
     * @brief Nœud pour déplacer une entité vers une cible
     */
    class MoveToTargetNode : public GameEntityNode
    {
    public:
        MoveToTargetNode(const std::string &name, const BT::NodeConfiguration &config, Core::EntityManager &entityManager);

        static BT::PortsList providedPorts();

    protected:
        virtual BT::NodeStatus tick() override;
    };

    /**
     * @brief Nœud pour détecter si une cible est visible
     */
    class IsTargetVisibleNode : public GameEntityNode
    {
    public:
        IsTargetVisibleNode(const std::string &name, const BT::NodeConfiguration &config, Core::EntityManager &entityManager);

        static BT::PortsList providedPorts();

    protected:
        virtual BT::NodeStatus tick() override;
    };

    /**
     * @brief Nœud pour calculer un chemin vers une cible
     */
    class FindPathNode : public GameEntityNode
    {
    public:
        FindPathNode(const std::string &name, const BT::NodeConfiguration &config, Core::EntityManager &entityManager);

        static BT::PortsList providedPorts();

    protected:
        virtual BT::NodeStatus tick() override;
    };

    /**
     * @brief Nœud pour faire patrouiller une entité
     */
    class PatrolNode : public GameEntityNode
    {
    public:
        PatrolNode(const std::string &name, const BT::NodeConfiguration &config, Core::EntityManager &entityManager);

        static BT::PortsList providedPorts();

    protected:
        virtual BT::NodeStatus tick() override;
    };

    /**
     * @brief Registre des nœuds personnalisés pour BehaviorTree.CPP
     */
    class BehaviorTreeNodeRegistry
    {
    public:
        /**
         * @brief Enregistrer tous les nœuds dans une factory
         * @param factory Factory à utiliser
         * @param entityManager Gestionnaire d'entités
         */
        static void registerNodes(BT::BehaviorTreeFactory &factory, Core::EntityManager &entityManager);
    };

} // namespace AI