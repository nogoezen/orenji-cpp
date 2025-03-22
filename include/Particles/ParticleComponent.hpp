#pragma once

#include "Core/Component.hpp"
#include "Particles/ParticleSystem.hpp"
#include <string>
#include <memory>

namespace Orenji
{

    /**
     * @enum ParticleTriggerType
     * @brief Type de déclenchement des particules
     */
    enum class ParticleTriggerType
    {
        Continuous, ///< Émission continue
        Burst,      ///< Émission en rafale
        OnEvent     ///< Émission sur événement
    };

    /**
     * @class ParticleComponent
     * @brief Composant qui gère un système de particules attaché à une entité
     */
    class ParticleComponent : public Component
    {
    public:
        /**
         * @brief Constructeur
         * @param name Nom du composant
         */
        explicit ParticleComponent(const std::string &name = "ParticleComponent");

        /**
         * @brief Destructeur
         */
        virtual ~ParticleComponent();

        /**
         * @brief Initialise le composant
         * @return true si l'initialisation a réussi, false sinon
         */
        virtual bool initialize() override;

        /**
         * @brief Met à jour le composant
         * @param deltaTime Temps écoulé depuis la dernière mise à jour
         */
        virtual void update(float deltaTime) override;

        /**
         * @brief Dessine le composant
         * @param target La cible de rendu SFML
         * @param states Les états de rendu SFML
         */
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        /**
         * @brief Charge un effet de particules à partir d'un fichier JSON
         * @param filename Chemin vers le fichier JSON
         * @return true si le chargement a réussi, false sinon
         */
        bool loadFromFile(const std::string &filename);

        /**
         * @brief Définit la texture des particules
         * @param texture Pointeur vers la texture
         */
        void setTexture(sf::Texture *texture);

        /**
         * @brief Définit le taux d'émission pour l'émission continue
         * @param particlesPerSecond Nombre de particules émises par seconde
         */
        void setEmissionRate(float particlesPerSecond);

        /**
         * @brief Obtient le taux d'émission pour l'émission continue
         * @return Nombre de particules émises par seconde
         */
        float getEmissionRate() const;

        /**
         * @brief Définit la taille de rafale pour l'émission en rafale
         * @param burstSize Nombre de particules émises par rafale
         */
        void setBurstSize(unsigned int burstSize);

        /**
         * @brief Obtient la taille de rafale pour l'émission en rafale
         * @return Nombre de particules émises par rafale
         */
        unsigned int getBurstSize() const;

        /**
         * @brief Définit le type de déclenchement des particules
         * @param triggerType Type de déclenchement
         */
        void setTriggerType(ParticleTriggerType triggerType);

        /**
         * @brief Obtient le type de déclenchement des particules
         * @return Type de déclenchement
         */
        ParticleTriggerType getTriggerType() const;

        /**
         * @brief Déclenche l'émission de particules (pour OnEvent)
         */
        void trigger();

        /**
         * @brief Vérifie si le système de particules est actif
         * @return true si le système est actif, false sinon
         */
        bool isEmitting() const;

        /**
         * @brief Démarre l'émission de particules
         */
        void start();

        /**
         * @brief Arrête l'émission de particules
         */
        void stop();

        /**
         * @brief Obtient l'émetteur de particules
         * @return Pointeur vers l'émetteur
         */
        ParticleEmitter *getEmitter();

        /**
         * @brief Ajoute un affecteur de particules
         * @param affector Fonction d'affecteur
         */
        void addAffector(const ParticleAffector &affector);

    private:
        std::string m_emitterName;           ///< Nom unique de l'émetteur
        ParticleEmitter *m_emitter;          ///< Pointeur vers l'émetteur
        ParticleTriggerType m_triggerType;   ///< Type de déclenchement
        float m_emissionRate;                ///< Taux d'émission (particules/seconde)
        unsigned int m_burstSize;            ///< Taille de rafale
        float m_emissionAccumulator;         ///< Accumulateur pour l'émission continue
        bool m_emitting;                     ///< Indique si l'émission est active
        sf::Texture *m_texture;              ///< Texture des particules
        static unsigned int s_nextEmitterId; ///< ID pour le prochain émetteur
    };

} // namespace Orenji