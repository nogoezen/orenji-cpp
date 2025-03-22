#pragma once

#include "Scene/SceneNode.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace Orenji
{
    /**
     * @class Scene
     * @brief Représente une scène complète dans le jeu
     *
     * Une scène contient un graphe de nœuds (SceneNode) et gère leur cycle de vie.
     * Elle permet d'organiser et manipuler les objets du jeu de manière hiérarchique.
     */
    class Scene
    {
    public:
        /**
         * @brief Constructeur
         * @param name Nom de la scène
         */
        explicit Scene(const std::string &name = "Scene");

        /**
         * @brief Destructeur
         */
        virtual ~Scene();

        /**
         * @brief Initialise la scène
         * @return true si l'initialisation a réussi, false sinon
         */
        virtual bool initialize();

        /**
         * @brief Met à jour la scène
         * @param dt Delta time (temps écoulé depuis la dernière mise à jour)
         */
        virtual void update(float dt);

        /**
         * @brief Dessine la scène
         * @param target Cible de rendu
         */
        virtual void render(sf::RenderTarget &target);

        /**
         * @brief Récupère le nœud racine de la scène
         * @return Pointeur vers le nœud racine
         */
        SceneNode *getRootNode();

        /**
         * @brief Récupère le nom de la scène
         * @return Nom de la scène
         */
        const std::string &getName() const;

        /**
         * @brief Définit le nom de la scène
         * @param name Nouveau nom
         */
        void setName(const std::string &name);

        /**
         * @brief Récupère un nœud par son nom
         * @param name Nom du nœud à rechercher
         * @return Pointeur vers le nœud, nullptr si non trouvé
         */
        SceneNode *findNodeByName(const std::string &name);

        /**
         * @brief Ajoute un nœud à la scène
         * @param node Nœud à ajouter
         * @param parent Nom du parent (utilise la racine si vide)
         * @return Pointeur vers le nœud ajouté
         */
        SceneNode *addNode(SceneNode::Ptr node, const std::string &parent = "");

        /**
         * @brief Crée et ajoute un nouveau nœud à la scène
         * @param name Nom du nouveau nœud
         * @param parent Nom du parent (utilise la racine si vide)
         * @return Pointeur vers le nœud créé
         */
        SceneNode *createNode(const std::string &name, const std::string &parent = "");

        /**
         * @brief Supprime un nœud de la scène
         * @param name Nom du nœud à supprimer
         * @return true si le nœud a été supprimé, false si non trouvé
         */
        bool removeNode(const std::string &name);

        /**
         * @brief Charge une scène depuis un fichier JSON
         * @param filename Chemin vers le fichier JSON
         * @return true si le chargement a réussi, false sinon
         */
        bool loadFromFile(const std::string &filename);

        /**
         * @brief Sauvegarde la scène dans un fichier JSON
         * @param filename Chemin vers le fichier JSON
         * @return true si la sauvegarde a réussi, false sinon
         */
        bool saveToFile(const std::string &filename);

        /**
         * @brief Efface tous les nœuds de la scène
         */
        void clear();

        /**
         * @brief Fonction appelée lorsque la scène est activée
         */
        virtual void onActivate();

        /**
         * @brief Fonction appelée lorsque la scène est désactivée
         */
        virtual void onDeactivate();

    protected:
        /**
         * @brief Enregistre un nœud dans le registre de nœuds
         * @param node Nœud à enregistrer
         */
        void registerNode(SceneNode *node);

        /**
         * @brief Supprime un nœud du registre de nœuds
         * @param node Nœud à supprimer
         */
        void unregisterNode(SceneNode *node);

    private:
        std::string m_name;                                   ///< Nom de la scène
        std::unique_ptr<SceneNode> m_rootNode;                ///< Nœud racine de la scène
        std::unordered_map<std::string, SceneNode *> m_nodes; ///< Registre des nœuds par nom
        bool m_isActive;                                      ///< Indique si la scène est active
    };

} // namespace Orenji