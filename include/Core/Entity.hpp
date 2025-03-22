#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <vector>

namespace Orenji
{

    // Déclarations avancées
    class Component;
    using ComponentPtr = std::shared_ptr<Component>;

    /**
     * @class Entity
     * @brief Représente une entité de jeu avec des composants
     */
    class Entity : public sf::Transformable, public sf::Drawable
    {
    public:
        /**
         * @brief Constructeur
         * @param id ID unique de l'entité
         * @param name Nom de l'entité
         */
        Entity(unsigned int id, const std::string &name = "Entity");

        /**
         * @brief Destructeur virtuel
         */
        virtual ~Entity();

        /**
         * @brief Initialise l'entité
         * @return true si l'initialisation a réussi, false sinon
         */
        virtual bool initialize();

        /**
         * @brief Met à jour l'entité et ses composants
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        virtual void update(float deltaTime);

        /**
         * @brief Dessine l'entité et ses composants
         * @param target La cible de rendu SFML
         * @param states Les états de rendu SFML
         */
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        /**
         * @brief Ajoute un composant à l'entité
         * @tparam T Type du composant à ajouter
         * @param component Le composant à ajouter
         * @return Pointeur vers le composant ajouté
         */
        template <typename T>
        std::shared_ptr<T> addComponent(std::shared_ptr<T> component)
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            auto typeIndex = std::type_index(typeid(T));
            m_components[typeIndex] = component;
            m_componentList.push_back(component);
            component->setOwner(this);
            return component;
        }

        /**
         * @brief Crée et ajoute un composant à l'entité
         * @tparam T Type du composant à créer
         * @tparam Args Types des arguments du constructeur de T
         * @param args Arguments à passer au constructeur de T
         * @return Pointeur vers le composant créé
         */
        template <typename T, typename... Args>
        std::shared_ptr<T> createComponent(Args &&...args)
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            auto component = std::make_shared<T>(std::forward<Args>(args)...);
            return addComponent<T>(component);
        }

        /**
         * @brief Récupère un composant de l'entité
         * @tparam T Type du composant à récupérer
         * @return Pointeur vers le composant, nullptr si non trouvé
         */
        template <typename T>
        std::shared_ptr<T> getComponent() const
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            auto typeIndex = std::type_index(typeid(T));
            auto it = m_components.find(typeIndex);
            if (it != m_components.end())
            {
                return std::static_pointer_cast<T>(it->second);
            }
            return nullptr;
        }

        /**
         * @brief Vérifie si l'entité possède un composant
         * @tparam T Type du composant à vérifier
         * @return true si l'entité possède le composant, false sinon
         */
        template <typename T>
        bool hasComponent() const
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            auto typeIndex = std::type_index(typeid(T));
            return m_components.find(typeIndex) != m_components.end();
        }

        /**
         * @brief Supprime un composant de l'entité
         * @tparam T Type du composant à supprimer
         * @return true si le composant a été supprimé, false sinon
         */
        template <typename T>
        bool removeComponent()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            auto typeIndex = std::type_index(typeid(T));
            auto it = m_components.find(typeIndex);
            if (it != m_components.end())
            {
                auto component = it->second;
                m_components.erase(it);

                auto listIt = std::find(m_componentList.begin(), m_componentList.end(), component);
                if (listIt != m_componentList.end())
                {
                    m_componentList.erase(listIt);
                }

                return true;
            }
            return false;
        }

        /**
         * @brief Obtient l'ID de l'entité
         * @return ID de l'entité
         */
        unsigned int getId() const { return m_id; }

        /**
         * @brief Obtient le nom de l'entité
         * @return Nom de l'entité
         */
        const std::string &getName() const { return m_name; }

        /**
         * @brief Définit le nom de l'entité
         * @param name Nouveau nom de l'entité
         */
        void setName(const std::string &name) { m_name = name; }

        /**
         * @brief Obtient l'état d'activité de l'entité
         * @return true si l'entité est active, false sinon
         */
        bool isActive() const { return m_active; }

        /**
         * @brief Définit l'état d'activité de l'entité
         * @param active Nouvel état d'activité
         */
        void setActive(bool active) { m_active = active; }

    private:
        unsigned int m_id;                                              ///< ID unique de l'entité
        std::string m_name;                                             ///< Nom de l'entité
        bool m_active;                                                  ///< État d'activité de l'entité
        std::unordered_map<std::type_index, ComponentPtr> m_components; ///< Map des composants par type
        std::vector<ComponentPtr> m_componentList;                      ///< Liste ordonnée des composants
    };

    /**
     * @typedef EntityPtr
     * @brief Pointeur vers une entité
     */
    using EntityPtr = std::shared_ptr<Entity>;

} // namespace Orenji