#pragma once

#include "Core/ResourceCache.hpp"
#include <SFML/Graphics.hpp>
#include "ParticleTypes.hpp"

#ifndef DISABLE_THOR
#include <Thor/Particles.hpp>
#include <Thor/Math/Distributions.hpp>
#include <Thor/Animations.hpp>
#endif

namespace Orenji
{
    class ParticleComponent;

    // Énumération pour les types de particules prédéfinis
    enum class ParticleType
    {
        EXPLOSION, // Explosion
        SMOKE,     // Fumée
        SPARK,     // Étincelle
        FIRE,      // Feu
        WATER,     // Eau
        CUSTOM     // Personnalisé
    };

    // Structure pour stocker les données d'un système de particules
    struct ParticleData
    {
        bool active = false;
        sf::Vector2f position;
        EmissionParameters parameters;
        std::vector<std::pair<AffectorType, float>> affectors;
        const sf::Texture *texture = nullptr;
        sf::IntRect textureRect;
    };

    /**
     * @brief Système de gestion des particules, utilisant ou non Thor selon la configuration.
     *
     * Cette classe gère les différents systèmes de particules du jeu.
     * Elle permet d'ajouter, retirer et mettre à jour les systèmes de particules.
     * En mode singleton, elle s'occupe de centraliser tous les systèmes.
     */
    class ParticleSystem : public sf::Drawable
    {
    public:
        /**
         * @brief Obtient l'instance unique du système de particules.
         *
         * @return Référence à l'instance unique.
         */
        static ParticleSystem &getInstance();

        // Destructeur
        virtual ~ParticleSystem();

        /**
         * @brief Configure un système de particules existant.
         *
         * @param component Composant de particules à configurer.
         * @param name Nom unique pour identifier le système.
         * @return true si la configuration a réussi, false sinon.
         */
        bool setupParticleSystem(ParticleComponent &component, const std::string &name);

        /**
         * @brief Crée un nouveau système de particules.
         *
         * @param name Nom unique pour identifier le système.
         * @param texturePath Chemin vers la texture à utiliser.
         * @return Pointeur vers le composant créé, nullptr en cas d'échec.
         */
        ParticleComponent *createParticleSystem(const std::string &name, const std::string &texturePath);

        /**
         * @brief Définit la texture d'un système existant.
         *
         * @param name Nom du système à modifier.
         * @param texturePath Chemin vers la texture.
         * @return true si la texture a été définie, false sinon.
         */
        bool setTexture(const std::string &name, const std::string &texturePath);

        /**
         * @brief Définit la texture d'un système existant.
         *
         * @param name Nom du système à modifier.
         * @param texture Texture à utiliser.
         * @param textureRect Rectangle de texture à utiliser.
         * @return true si la texture a été définie, false sinon.
         */
        bool setTexture(const std::string &name, const sf::Texture &texture, const sf::IntRect &textureRect = sf::IntRect());

        /**
         * @brief Crée un système de particules prédéfini.
         *
         * @param id Identifiant unique pour le système.
         * @param type Type prédéfini de particules.
         * @param position Position initiale du système.
         * @param color Couleur des particules.
         * @return true si la création a réussi, false sinon.
         */
        bool createSystem(const std::string &id, ParticleType type,
                          const sf::Vector2f &position,
                          const sf::Color &color = sf::Color::White);

        /**
         * @brief Crée un système de particules personnalisé.
         *
         * @param id Identifiant unique pour le système.
         * @param position Position initiale du système.
         * @param params Paramètres d'émission personnalisés.
         * @return true si la création a réussi, false sinon.
         */
        bool createCustomSystem(const std::string &id, const sf::Vector2f &position,
                                const EmissionParameters &params);

        /**
         * @brief Met à jour tous les systèmes de particules.
         *
         * @param dt Temps écoulé depuis la dernière mise à jour.
         */
        void update(float dt);

        /**
         * @brief Dessine tous les systèmes de particules.
         *
         * @param target Cible de rendu.
         * @param states États de rendu.
         */
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        /**
         * @brief Obtient un composant de particules par son nom.
         *
         * @param name Nom du composant recherché.
         * @return Pointeur vers le composant, nullptr s'il n'existe pas.
         */
        ParticleComponent *getParticleComponent(const std::string &name);

        /**
         * @brief Supprime un système de particules.
         *
         * @param name Nom du système à supprimer.
         */
        void removeParticleSystem(const std::string &name);

        /**
         * @brief Vérifie si un système de particules existe.
         *
         * @param name Nom du système recherché.
         * @return true si le système existe, false sinon.
         */
        bool hasParticleSystem(const std::string &name) const;

        /**
         * @brief Active ou désactive un système de particules.
         *
         * @param id Identifiant du système.
         * @param active État d'activation.
         * @return true si l'opération a réussi, false sinon.
         */
        bool setActive(const std::string &id, bool active);

        /**
         * @brief Définit la position d'un système.
         *
         * @param id Identifiant du système.
         * @param position Nouvelle position.
         * @return true si l'opération a réussi, false sinon.
         */
        bool setPosition(const std::string &id, const sf::Vector2f &position);

        /**
         * @brief Émet des particules sur un système existant.
         *
         * @param id Identifiant du système.
         * @param count Nombre de particules à émettre.
         * @return true si l'opération a réussi, false sinon.
         */
        bool emitParticles(const std::string &id, unsigned int count);

        /**
         * @brief Supprime un système de particules.
         *
         * @param id Identifiant du système à supprimer.
         * @return true si l'opération a réussi, false sinon.
         */
        bool removeSystem(const std::string &id);

        /**
         * @brief Ajoute un affecteur à un système existant.
         *
         * @param id Identifiant du système.
         * @param type Type d'affecteur à ajouter.
         * @param strength Force de l'affecteur.
         * @return true si l'opération a réussi, false sinon.
         */
        bool addAffector(const std::string &id, AffectorType type, float strength = 1.0f);

    private:
        // Constructeur privé (singleton)
        ParticleSystem();

        // Interdit la copie et l'assignation
        ParticleSystem(const ParticleSystem &) = delete;
        ParticleSystem &operator=(const ParticleSystem &) = delete;

        /**
         * @brief Applique des affecteurs à une particule.
         *
         * @param particle Particule à affecter.
         * @param affectors Liste des affecteurs à appliquer.
         * @param dt Temps écoulé depuis la dernière mise à jour.
         */
        void applyAffectors(SimpleParticle &particle,
                            const std::vector<std::pair<AffectorType, float>> &affectors,
                            float dt);

        // Instance unique
        static std::unique_ptr<ParticleSystem> s_instance;

        // Stockage des systèmes de particules
        std::unordered_map<std::string, std::unique_ptr<ParticleData>> m_particleSystems;
        std::unordered_map<std::string, std::unique_ptr<ParticleComponent>> m_particleComponents;

        // Modèles de systèmes de particules prédéfinis
        std::unordered_map<std::string, EmissionParameters> m_templates;

        // Gestionnaire de ressources
        ResourceCache<sf::Texture> m_textureCache;
    };

} // namespace Orenji