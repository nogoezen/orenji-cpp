#pragma once

#include <SFML/Graphics.hpp>
#include "Particles/ParticleTypes.hpp"
#include "Core/Component.hpp"
#include <vector>
#include <string>
#include <memory>

namespace Orenji
{
    /**
     * @brief Composant de particules simplifié
     *
     * Cette implémentation utilise uniquement SFML pour un système
     * de particules simple mais suffisant pour la plupart des effets.
     */
    class ParticleComponent : public Component
    {
    public:
        /**
         * @brief Constructeur
         *
         * @param id Identifiant du composant
         */
        ParticleComponent(const std::string &id);

        /**
         * @brief Destructeur
         */
        ~ParticleComponent();

        /**
         * @brief Initialise le composant
         */
        void initialize() override;

        /**
         * @brief Met à jour le système de particules
         *
         * @param dt Temps écoulé depuis la dernière mise à jour (en secondes)
         */
        void update(float dt) override;

        /**
         * @brief Dessine les particules
         *
         * @param target Cible de rendu
         */
        void draw(sf::RenderTarget &target) override;

        /**
         * @brief Définit le taux d'émission
         *
         * @param rate Particules par seconde
         */
        void setEmissionRate(float rate);

        /**
         * @brief Définit le type de déclenchement
         *
         * @param triggerType Type de déclenchement
         */
        void setTriggerType(ParticleTriggerType triggerType);

        /**
         * @brief Émet un nombre spécifique de particules
         *
         * @param count Nombre de particules à émettre
         */
        void emit(unsigned int count);

        /**
         * @brief Active ou désactive le système
         *
         * @param enabled État d'activation
         */
        void setEnabled(bool enabled);

        /**
         * @brief Vérifie si le système est actif
         *
         * @return true si actif, false sinon
         */
        bool isEnabled() const { return m_enabled; }

        /**
         * @brief Vérifie si le système est en train d'émettre
         *
         * @return true si émission en cours, false sinon
         */
        bool isEmitting() const { return m_isActive; }

        /**
         * @brief Définit la texture pour les particules
         *
         * @param texture Pointeur vers la texture
         * @param textureRect Rectangle définissant la partie de la texture à utiliser
         */
        void setTexture(const sf::Texture *texture, const sf::IntRect &textureRect = sf::IntRect());

        /**
         * @brief Définit la position du système
         *
         * @param position Nouvelle position
         */
        void setPosition(const sf::Vector2f &position);

        /**
         * @brief Définit le nombre de particules à émettre en mode OneShot
         *
         * @param size Nombre de particules
         */
        void setBurstSize(unsigned int size) { m_burstSize = size; }

        /**
         * @brief Définit les paramètres d'émission
         *
         * @param params Paramètres d'émission
         */
        void setEmissionParameters(const EmissionParameters &params);

        /**
         * @brief Ajoute un affecteur au système
         *
         * @param type Type d'affecteur
         * @param strength Force de l'effet
         */
        void addAffector(AffectorType type, float strength = 1.0f);

    private:
        /**
         * @brief Mettre à jour les vertices pour le rendu
         */
        void updateVertices();

        /**
         * @brief Crée une nouvelle particule
         *
         * @return Particule créée
         */
        SimpleParticle createParticle();

        /**
         * @brief Interpole entre deux couleurs
         *
         * @param start Couleur de départ
         * @param end Couleur d'arrivée
         * @param alpha Facteur d'interpolation (0-1)
         * @return Couleur interpolée
         */
        sf::Color lerpColor(const sf::Color &start, const sf::Color &end, float alpha);

        float m_emissionRate;              ///< Taux d'émission (particules/sec)
        float m_emissionTime;              ///< Temps accumulé pour l'émission
        ParticleTriggerType m_triggerType; ///< Type de déclenchement
        bool m_enabled;                    ///< État d'activation
        bool m_isActive;                   ///< État d'émission
        bool m_continuousEmitting;         ///< Émission continue
        sf::Vector2f m_position;           ///< Position du système
        float m_distanceTraveled;          ///< Distance parcourue pour l'émission par distance
        unsigned int m_burstSize;          ///< Taille du burst pour mode OneShot

        std::vector<SimpleParticle> m_particles;                 ///< Liste des particules
        EmissionParameters m_emissionParams;                     ///< Paramètres d'émission
        std::vector<std::pair<AffectorType, float>> m_affectors; ///< Affecteurs

        sf::VertexArray m_vertices;   ///< Vertex array pour le rendu
        const sf::Texture *m_texture; ///< Texture des particules
        sf::IntRect m_textureRect;    ///< Rectangle de texture
    };
}