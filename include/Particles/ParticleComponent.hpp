#pragma once

#include "../Core/Component.hpp"
#include <SFML/Graphics.hpp>
#include <Thor/Particles.hpp>
#include <memory>
#include <string>

namespace Orenji
{
    /**
     * @brief Type de déclencheur de particules
     */
    enum class ParticleTriggerType
    {
        Continuous, // Émission continue
        OneShot,    // Émission unique (un burst)
        OnCommand,  // Émission sur commande
        OnDistance  // Émission en fonction de la distance parcourue
    };

    /**
     * @brief Composant pour gérer un système de particules.
     * Utilise la bibliothèque Thor pour le système de particules.
     */
    class ParticleComponent : public Component, public sf::Drawable
    {
    public:
        ParticleComponent();
        virtual ~ParticleComponent();

        // Méthodes du Component
        virtual void initialize() override;
        virtual void update(float deltaTime) override;

        // Méthodes spécifiques aux particules

        /**
         * @brief Charge un système de particules depuis un fichier
         * @param filename Chemin vers le fichier de configuration
         * @return true si le chargement a réussi
         */
        bool loadFromFile(const std::string &filename);

        /**
         * @brief Définit la texture utilisée par les particules
         * @param texturePath Chemin vers la texture
         */
        void setTexture(const std::string &texturePath);

        /**
         * @brief Définit le nombre de particules à émettre
         * @param rate Nombre de particules à émettre par seconde (pour émission continue)
         */
        void setEmissionRate(float rate);

        /**
         * @brief Définit le nombre de particules à émettre dans un burst
         * @param count Nombre de particules
         */
        void setBurstSize(unsigned int count);

        /**
         * @brief Définit le type de déclencheur
         * @param type Type de déclencheur
         */
        void setTriggerType(ParticleTriggerType type);

        /**
         * @brief Déclenche une émission manuelle de particules
         * @param count Nombre de particules à émettre (0 = utiliser la taille de burst par défaut)
         */
        void emit(unsigned int count = 0);

        /**
         * @brief Démarre ou arrête l'émission continue
         * @param enabled Active ou désactive l'émission
         */
        void setEnabled(bool enabled);

        /**
         * @brief Vérifie si le système est actif
         * @return true si le système est actif
         */
        bool isEnabled() const;

        /**
         * @brief Récupère le nombre de particules actives
         * @return Nombre de particules
         */
        unsigned int getParticleCount() const;

        /**
         * @brief Efface toutes les particules
         */
        void clearParticles();

    private:
        // Méthode pour dessiner les particules
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        // Système de particules Thor
        thor::ParticleSystem m_particleSystem;

        // Émetteur de particules
        thor::UniversalEmitter m_emitter;

        // Type de déclencheur
        ParticleTriggerType m_triggerType;

        // Connexion pour l'émission continue
        thor::Connection m_connection;

        // Paramètres de particules
        float m_emissionRate;
        unsigned int m_burstSize;
        bool m_enabled;

        // Pour le déclencheur basé sur la distance
        sf::Vector2f m_lastPosition;
        float m_distanceTrigger;
        float m_distanceAccumulator;

        // Texture
        sf::Texture m_texture;
    };

} // namespace Orenji