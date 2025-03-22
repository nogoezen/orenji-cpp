#pragma once

#include "../Core/Component.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Orenji
{
    // Déclaration anticipée
    struct SimpleParticle;
    struct EmissionParameters;

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
     * @brief Structure représentant une particule simple
     */
    struct SimpleParticle
    {
        sf::Vector2f position; // Position
        sf::Vector2f velocity; // Vitesse
        sf::Color color;       // Couleur
        float lifetime;        // Durée de vie maximale
        float elapsed;         // Temps écoulé
        float size;            // Taille
        float rotation;        // Rotation
        float rotationSpeed;   // Vitesse de rotation
    };

    /**
     * @brief Composant pour gérer un système de particules simple
     * Implémentation sans dépendance à Thor
     */
    class ParticleComponent : public Component, public sf::Drawable
    {
    public:
        ParticleComponent();
        virtual ~ParticleComponent() = default;

        // Méthodes du Component
        virtual void initialize() override;
        virtual void update(float deltaTime) override;

        // Méthodes spécifiques aux particules

        /**
         * @brief Charge un système de particules depuis un fichier ou un template
         * @param templateName Nom du template ou chemin vers le fichier de configuration
         * @return true si le chargement a réussi
         */
        bool loadFromFile(const std::string &templateName);

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

        /**
         * @brief Définit les paramètres d'émission de particules
         * @param params Paramètres d'émission
         */
        void setEmissionParameters(const EmissionParameters &params);

    private:
        // Méthode pour dessiner les particules
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        // Génère une nouvelle particule
        SimpleParticle generateParticle() const;

        // Applique les affecteurs aux particules
        void applyAffectors(SimpleParticle &particle, float deltaTime);

        // Liste des particules actives
        std::vector<SimpleParticle> m_particles;

        // Type de déclencheur
        ParticleTriggerType m_triggerType;

        // Paramètres de particules
        float m_emissionRate;
        unsigned int m_burstSize;
        bool m_enabled;
        float m_emissionAccumulator;

        // Pour le déclencheur basé sur la distance
        sf::Vector2f m_lastPosition;
        float m_distanceTrigger;
        float m_distanceAccumulator;

        // Paramètres d'émission
        EmissionParameters *m_parameters;

        // Texture
        sf::Texture m_texture;
        sf::VertexArray m_vertices;
        sf::Color m_startColor;
        sf::Color m_endColor;
        float m_minSize;
        float m_maxSize;
        float m_minLifetime;
        float m_maxLifetime;
    };

} // namespace Orenji