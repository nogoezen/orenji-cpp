#pragma once

#include <SFML/Graphics.hpp>
#include "Particles/ParticleTypes.hpp"
#include "Core/Component.hpp"
#include <vector>
#include <string>
#include <memory>

#ifndef DISABLE_THOR
#include <Thor/Particles.hpp>
#include <Thor/Math.hpp>
#include <Thor/Animations.hpp>
#endif

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

#ifndef DISABLE_THOR
    /**
     * @brief Composant de particules utilisant Thor
     */
    class ParticleComponent : public Component
    {
    public:
        /**
         * @brief Constructeur de la classe ParticleComponent
         * @param id Identifiant du composant
         */
        ParticleComponent(const std::string &id = "particle");

        /**
         * @brief Destructeur de la classe ParticleComponent
         */
        ~ParticleComponent();

        /**
         * @brief Initialise le composant
         */
        void initialize() override;

        /**
         * @brief Met à jour le composant
         * @param dt Delta time
         */
        void update(float dt) override;

        /**
         * @brief Dessine le composant
         * @param target Cible de rendu
         */
        void draw(sf::RenderTarget &target) override;

        /**
         * @brief Charge un fichier de particules
         * @param filename Nom du fichier
         * @return true si le chargement a réussi
         */
        bool loadFromFile(const std::string &filename);

        /**
         * @brief Définit la texture du système de particules
         * @param texture Pointeur vers la texture
         * @param textureRect Rectangle de texture
         */
        void setTexture(const sf::Texture *texture, const sf::IntRect &textureRect = sf::IntRect());

        /**
         * @brief Définit le taux d'émission
         * @param rate Taux d'émission
         */
        void setEmissionRate(float rate);

        /**
         * @brief Définit le type de déclencheur
         * @param triggerType Type de déclencheur
         */
        void setTriggerType(ParticleTriggerType triggerType);

        /**
         * @brief Emets des particules
         * @param count Nombre de particules
         */
        void emit(unsigned int count = 1);

        /**
         * @brief Vérifie si le système est actif
         * @return true si le système est actif
         */
        bool isSystemActive() const;

        /**
         * @brief Vérifie si le système continue d'émettre
         * @return true si le système continue d'émettre
         */
        bool isContinuousEmitting() const;

        /**
         * @brief Définit si le système est activé
         * @param enabled true si le système est activé
         */
        void setEnabled(bool enabled);

        /**
         * @brief Retourne si le système est activé
         * @return true si le système est activé
         */
        bool isEnabled() const;

        /**
         * @brief Définit la position du système
         * @param position Position du système
         */
        void setPosition(const sf::Vector2f &position);

    private:
        float m_emissionRate;              ///< Taux d'émission
        float m_emissionTime;              ///< Temps d'émission
        ParticleTriggerType m_triggerType; ///< Type de déclencheur
        bool m_enabled;                    ///< Si le système est activé
        bool m_isActive;                   ///< Si le système est actif
        bool m_continuousEmitting;         ///< Si le système continue d'émettre
        sf::Vector2f m_position;           ///< Position du système

        thor::ParticleSystem m_system;                     ///< Système de particules
        thor::Connection m_connection;                     ///< Connexion de l'émetteur
        std::unique_ptr<thor::UniversalEmitter> m_emitter; ///< Émetteur
    };
#else
    /**
     * @brief Composant de particules simple sans Thor
     */
    class ParticleComponent : public Component
    {
    public:
        /**
         * @brief Constructeur de la classe ParticleComponent
         * @param id Identifiant du composant
         */
        ParticleComponent(const std::string &id = "particle");

        /**
         * @brief Destructeur de la classe ParticleComponent
         */
        ~ParticleComponent();

        /**
         * @brief Initialise le composant
         */
        void initialize() override;

        /**
         * @brief Met à jour le composant
         * @param dt Delta time
         */
        void update(float dt) override;

        /**
         * @brief Dessine le composant
         * @param target Cible de rendu
         */
        void draw(sf::RenderTarget &target) override;

        /**
         * @brief Charge un fichier de particules
         * @param filename Nom du fichier
         * @return true si le chargement a réussi
         */
        bool loadFromFile(const std::string &filename);

        /**
         * @brief Définit la texture du système de particules
         * @param texture Pointeur vers la texture
         * @param textureRect Rectangle de texture
         */
        void setTexture(const sf::Texture *texture, const sf::IntRect &textureRect = sf::IntRect());

        /**
         * @brief Définit le taux d'émission
         * @param rate Taux d'émission
         */
        void setEmissionRate(float rate);

        /**
         * @brief Définit le type de déclencheur
         * @param triggerType Type de déclencheur
         */
        void setTriggerType(ParticleTriggerType triggerType);

        /**
         * @brief Emets des particules
         * @param count Nombre de particules
         */
        void emit(unsigned int count = 1);

        /**
         * @brief Vérifie si le système est actif
         * @return true si le système est actif
         */
        bool isSystemActive() const;

        /**
         * @brief Vérifie si le système continue d'émettre
         * @return true si le système continue d'émettre
         */
        bool isContinuousEmitting() const;

        /**
         * @brief Définit si le système est activé
         * @param enabled true si le système est activé
         */
        void setEnabled(bool enabled);

        /**
         * @brief Retourne si le système est activé
         * @return true si le système est activé
         */
        bool isEnabled() const;

        /**
         * @brief Définit la position du système
         * @param position Position du système
         */
        void setPosition(const sf::Vector2f &position);

        /**
         * @brief Définit les paramètres d'émission
         * @param params Paramètres d'émission
         */
        void setEmissionParameters(const EmissionParameters &params);

        /**
         * @brief Obtient les paramètres d'émission
         * @return Paramètres d'émission
         */
        const EmissionParameters &getEmissionParameters() const;

    private:
        /**
         * @brief Crée une nouvelle particule
         * @return Nouvelle particule
         */
        SimpleParticle createParticle();

        /**
         * @brief Mélange une couleur entre début et fin
         * @param start Couleur de début
         * @param end Couleur de fin
         * @param alpha Ratio (0.0 à 1.0)
         * @return Couleur mélangée
         */
        sf::Color lerpColor(const sf::Color &start, const sf::Color &end, float alpha);

        /**
         * @brief Met à jour les vertices pour le rendu
         */
        void updateVertices();

        float m_emissionRate;              ///< Taux d'émission
        float m_emissionTime;              ///< Temps d'émission
        ParticleTriggerType m_triggerType; ///< Type de déclencheur
        bool m_enabled;                    ///< Si le système est activé
        bool m_isActive;                   ///< Si le système est actif
        bool m_continuousEmitting;         ///< Si le système continue d'émettre
        sf::Vector2f m_position;           ///< Position du système
        float m_distanceTraveled;          ///< Distance parcourue depuis la dernière émission

        EmissionParameters m_emissionParams;     ///< Paramètres d'émission
        std::vector<SimpleParticle> m_particles; ///< Liste des particules
        sf::VertexArray m_vertices;              ///< Vertex array pour le rendu
        const sf::Texture *m_texture;            ///< Texture des particules
        sf::IntRect m_textureRect;               ///< Rectangle de texture
    };
#endif
}