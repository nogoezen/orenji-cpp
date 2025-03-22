#pragma once

#include "Particles/ParticleEmitter.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <memory>

namespace Orenji
{

    /**
     * @class ParticleSystem
     * @brief Système de particules qui gère plusieurs émetteurs
     *
     * Cette classe utilise le pattern Singleton pour garantir qu'un seul système
     * de particules existe dans l'application.
     */
    class ParticleSystem : public sf::Drawable
    {
    public:
        /**
         * @brief Obtient l'instance unique du système de particules
         * @return Référence vers l'instance unique
         */
        static ParticleSystem &getInstance();

        /**
         * @brief Libère les ressources du système de particules
         */
        static void release();

        /**
         * @brief Crée un nouvel émetteur de particules
         * @param name Nom de l'émetteur
         * @param maxParticles Nombre maximum de particules
         * @return true si l'émetteur a été créé, false sinon
         */
        bool createEmitter(const std::string &name, unsigned int maxParticles = 1000);

        /**
         * @brief Supprime un émetteur de particules
         * @param name Nom de l'émetteur à supprimer
         * @return true si l'émetteur a été supprimé, false sinon
         */
        bool removeEmitter(const std::string &name);

        /**
         * @brief Obtient un émetteur de particules par son nom
         * @param name Nom de l'émetteur
         * @return Pointeur vers l'émetteur, nullptr si non trouvé
         */
        ParticleEmitter *getEmitter(const std::string &name);

        /**
         * @brief Vérifie si un émetteur existe
         * @param name Nom de l'émetteur
         * @return true si l'émetteur existe, false sinon
         */
        bool hasEmitter(const std::string &name) const;

        /**
         * @brief Ajoute un affecteur à un émetteur
         * @param emitterName Nom de l'émetteur
         * @param affector Fonction d'affecteur
         * @return true si l'affecteur a été ajouté, false sinon
         */
        bool addAffector(const std::string &emitterName, const ParticleAffector &affector);

        /**
         * @brief Émet des particules depuis un émetteur
         * @param emitterName Nom de l'émetteur
         * @param count Nombre de particules à émettre
         * @return true si les particules ont été émises, false sinon
         */
        bool emit(const std::string &emitterName, unsigned int count);

        /**
         * @brief Met à jour tous les émetteurs de particules
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        void update(float deltaTime);

        /**
         * @brief Dessine tous les émetteurs de particules
         * @param target Cible de rendu
         * @param states États de rendu
         */
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        /**
         * @brief Supprime tous les émetteurs
         */
        void clear();

        /**
         * @brief Définit la position d'un émetteur
         * @param emitterName Nom de l'émetteur
         * @param position Nouvelle position
         * @return true si la position a été définie, false sinon
         */
        bool setEmitterPosition(const std::string &emitterName, const sf::Vector2f &position);

        /**
         * @brief Définit la texture pour toutes les particules
         * @param texture Pointeur vers la texture
         */
        void setTexture(sf::Texture *texture);

        /**
         * @brief Obtient le nombre total de particules actives
         * @return Nombre de particules actives
         */
        unsigned int getActiveParticleCount() const;

    private:
        /**
         * @brief Constructeur (privé pour le pattern Singleton)
         */
        ParticleSystem();

        /**
         * @brief Destructeur (privé pour le pattern Singleton)
         */
        ~ParticleSystem();

        /**
         * @brief Met à jour les vertices pour le rendu
         */
        void updateVertices() const;

        static ParticleSystem *s_instance;                                  ///< Instance unique du système de particules
        std::map<std::string, std::unique_ptr<ParticleEmitter>> m_emitters; ///< Map des émetteurs par nom
        sf::Texture *m_texture;                                             ///< Texture des particules
        mutable sf::VertexArray m_vertices;                                 ///< Tableau de vertices pour le rendu
        mutable bool m_needVertexUpdate;                                    ///< Indique si les vertices doivent être mis à jour
    };

} // namespace Orenji