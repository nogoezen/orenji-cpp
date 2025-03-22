#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <string>

namespace Orenji
{
    /**
     * @class SceneNode
     * @brief Classe de base pour les nœuds dans le graphe de scène
     *
     * Cette classe implémente une structure en arbre où chaque nœud peut avoir
     * des enfants et un parent. Elle permet l'organisation hiérarchique des objets
     * dans la scène.
     */
    class SceneNode : public sf::Drawable, public sf::Transformable
    {
    public:
        using Ptr = std::unique_ptr<SceneNode>;
        using NodeList = std::vector<Ptr>;

        /**
         * @brief Constructeur
         * @param name Nom du nœud
         */
        explicit SceneNode(const std::string &name = "Node");

        /**
         * @brief Destructeur virtuel
         */
        virtual ~SceneNode() = default;

        /**
         * @brief Attache un nœud enfant à ce nœud
         * @param child Nœud enfant à attacher
         * @return Pointeur vers le nœud enfant (pour chaîner les opérations)
         */
        SceneNode *attachChild(Ptr child);

        /**
         * @brief Détache un nœud enfant
         * @param node Nœud enfant à détacher
         * @return Le nœud détaché (ownership transféré)
         */
        Ptr detachChild(const SceneNode &node);

        /**
         * @brief Détache un nœud enfant par son index
         * @param index Index du nœud à détacher
         * @return Le nœud détaché (ownership transféré)
         */
        Ptr detachChild(size_t index);

        /**
         * @brief Met à jour ce nœud et tous ses enfants
         * @param dt Delta time (temps écoulé depuis la dernière mise à jour)
         */
        void update(float dt);

        /**
         * @brief Récupère la transformation globale de ce nœud
         * @return Transformation globale (combinaison de toutes les transformations parentes)
         */
        sf::Transform getWorldTransform() const;

        /**
         * @brief Récupère la position globale de ce nœud
         * @return Position globale dans le monde
         */
        sf::Vector2f getWorldPosition() const;

        /**
         * @brief Définit le parent de ce nœud
         * @param parent Pointeur vers le nœud parent
         */
        void setParent(SceneNode *parent);

        /**
         * @brief Récupère le parent de ce nœud
         * @return Pointeur vers le nœud parent, nullptr si pas de parent
         */
        SceneNode *getParent() const;

        /**
         * @brief Récupère le nombre d'enfants de ce nœud
         * @return Nombre d'enfants
         */
        size_t getChildCount() const;

        /**
         * @brief Récupère un enfant par son index
         * @param index Index de l'enfant
         * @return Pointeur vers l'enfant, nullptr si index invalide
         */
        SceneNode *getChild(size_t index);

        /**
         * @brief Récupère un enfant par son nom
         * @param name Nom de l'enfant à rechercher
         * @return Pointeur vers l'enfant, nullptr si non trouvé
         */
        SceneNode *getChildByName(const std::string &name);

        /**
         * @brief Récupère le nom de ce nœud
         * @return Le nom du nœud
         */
        const std::string &getName() const;

        /**
         * @brief Définit le nom de ce nœud
         * @param name Nouveau nom
         */
        void setName(const std::string &name);

        /**
         * @brief Définit la visibilité de ce nœud
         * @param visible true pour rendre le nœud visible, false sinon
         */
        void setVisible(bool visible);

        /**
         * @brief Vérifie si ce nœud est visible
         * @return true si le nœud est visible, false sinon
         */
        bool isVisible() const;

    protected:
        /**
         * @brief Méthode de mise à jour spécifique à la classe dérivée
         * @param dt Delta time (temps écoulé depuis la dernière mise à jour)
         */
        virtual void updateCurrent(float dt);

        /**
         * @brief Méthode de dessin spécifique à la classe dérivée
         * @param target Cible de rendu
         * @param states États de rendu SFML
         */
        virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

        /**
         * @brief Fonction pour dessiner le nœud et ses enfants
         * @param target Cible de rendu
         * @param states États de rendu SFML
         */
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        NodeList m_children; ///< Liste des nœuds enfants
        SceneNode *m_parent; ///< Pointeur vers le nœud parent
        std::string m_name;  ///< Nom du nœud
        bool m_visible;      ///< Visibilité du nœud
    };

} // namespace Orenji