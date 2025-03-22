#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <SFML/Graphics.hpp>
#include "Core/Component.hpp"

namespace Orenji
{
    class EntityManager;

    /**
     * @brief Classe de base pour toutes les entités
     *
     * Cette classe représente une entité dans le système ECS,
     * capable de contenir et gérer plusieurs composants.
     */
    class Entity : public sf::Transformable, public sf::Drawable
    {
    public:
        /**
         * @brief Constructeur
         *
         * @param id Identifiant unique de l'entité
         */
        Entity(const std::string &id);

        /**
         * @brief Destructeur
         */
        virtual ~Entity();

        /**
         * @brief Identifiant de l'entité
         *
         * @return Identifiant de l'entité
         */
        const std::string &getId() const { return m_id; }

        /**
         * @brief Met à jour l'entité et tous ses composants
         *
         * @param dt Temps écoulé depuis la dernière mise à jour (en secondes)
         */
        virtual void update(float dt);

        /**
         * @brief Initialise l'entité
         */
        virtual void initialize();

        /**
         * @brief Dessine l'entité et tous ses composants
         *
         * @param target Cible de rendu
         * @param states États de rendu SFML
         */
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        /**
         * @brief Ajoute un composant à l'entité
         *
         * @param component Composant à ajouter
         * @return Référence vers l'entité (pour chaînage)
         */
        Entity &addComponent(ComponentPtr component);

        /**
         * @brief Récupère un composant par son identifiant
         *
         * @param id Identifiant du composant
         * @return Pointeur vers le composant, nullptr si non trouvé
         */
        ComponentPtr getComponent(const std::string &id);

        /**
         * @brief Récupère un composant par son type
         *
         * @tparam T Type du composant à récupérer
         * @return Pointeur vers le composant, nullptr si non trouvé
         */
        template <typename T>
        std::shared_ptr<T> getComponentByType()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
            auto it = m_componentsByType.find(std::type_index(typeid(T)));
            if (it != m_componentsByType.end() && it->second.size() > 0)
            {
                return std::dynamic_pointer_cast<T>(it->second[0]);
            }
            return nullptr;
        }

        /**
         * @brief Récupère tous les composants d'un type spécifique
         *
         * @tparam T Type des composants à récupérer
         * @return Vecteur de composants du type spécifié
         */
        template <typename T>
        std::vector<std::shared_ptr<T>> getComponentsByType()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
            std::vector<std::shared_ptr<T>> result;
            auto it = m_componentsByType.find(std::type_index(typeid(T)));
            if (it != m_componentsByType.end())
            {
                result.reserve(it->second.size());
                for (auto &comp : it->second)
                {
                    auto castedComp = std::dynamic_pointer_cast<T>(comp);
                    if (castedComp)
                    {
                        result.push_back(castedComp);
                    }
                }
            }
            return result;
        }

        /**
         * @brief Supprime un composant par son identifiant
         *
         * @param id Identifiant du composant à supprimer
         * @return true si le composant a été supprimé, false sinon
         */
        bool removeComponent(const std::string &id);

        /**
         * @brief Supprime tous les composants d'un type spécifique
         *
         * @tparam T Type des composants à supprimer
         * @return Nombre de composants supprimés
         */
        template <typename T>
        int removeComponentsByType()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
            auto typeIndex = std::type_index(typeid(T));
            auto it = m_componentsByType.find(typeIndex);
            if (it != m_componentsByType.end())
            {
                int count = it->second.size();
                for (auto &comp : it->second)
                {
                    auto idIt = m_componentsById.begin();
                    while (idIt != m_componentsById.end())
                    {
                        if (idIt->second == comp)
                        {
                            idIt = m_componentsById.erase(idIt);
                        }
                        else
                        {
                            ++idIt;
                        }
                    }
                }
                m_componentsByType.erase(it);
                return count;
            }
            return 0;
        }

        /**
         * @brief Vérifie si l'entité est active
         *
         * @return true si l'entité est active, false sinon
         */
        bool isActive() const { return m_active; }

        /**
         * @brief Active ou désactive l'entité
         *
         * @param active Nouvel état d'activation
         */
        void setActive(bool active) { m_active = active; }

        /**
         * @brief Adapté pour SFML 3: Définit la position de l'entité
         *
         * @param x Coordonnée X
         * @param y Coordonnée Y
         */
        void setPosition(float x, float y);

        /**
         * @brief Adapté pour SFML 3: Définit la position de l'entité
         *
         * @param position Nouvelle position
         */
        void setPosition(const sf::Vector2f &position);

        /**
         * @brief Adapté pour SFML 3: Déplace l'entité
         *
         * @param offsetX Déplacement en X
         * @param offsetY Déplacement en Y
         */
        void move(float offsetX, float offsetY);

        /**
         * @brief Adapté pour SFML 3: Déplace l'entité
         *
         * @param offset Vecteur de déplacement
         */
        void move(const sf::Vector2f &offset);

        /**
         * @brief Adapté pour SFML 3: Récupère la rotation de l'entité
         *
         * @return Rotation en degrés
         */
        float getRotation() const;

        /**
         * @brief Adapté pour SFML 3: Définit la rotation de l'entité
         *
         * @param angle Angle en degrés
         */
        void setRotation(float angle);

        /**
         * @brief Adapté pour SFML 3: Applique une rotation à l'entité
         *
         * @param angle Angle de rotation en degrés
         */
        void rotate(float angle);

    private:
        std::string m_id;                                                                  ///< Identifiant unique de l'entité
        bool m_active;                                                                     ///< État d'activation de l'entité
        std::unordered_map<std::string, ComponentPtr> m_componentsById;                    ///< Composants indexés par identifiant
        std::unordered_map<std::type_index, std::vector<ComponentPtr>> m_componentsByType; ///< Composants indexés par type
    };

    /**
     * @brief Alias pour un pointeur intelligent vers une Entity
     */
    using EntityPtr = std::shared_ptr<Entity>;

} // namespace Orenji