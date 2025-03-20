#pragma once

#include "../Core/ResourceCache.hpp"
#include <SFML/Graphics.hpp>
#include <Thor/Particles.hpp>
#include <string>
#include <unordered_map>
#include <memory>

namespace Orenji
{
    /**
     * @brief Classe pour gérer et charger des systèmes de particules prédéfinis.
     *
     * Permet de créer, charger et stocker des configurations de particules qui peuvent
     * être réutilisées dans le jeu. Fonctionne comme un singleton pour partager
     * les configurations entre différentes parties du jeu.
     */
    class ParticleSystem
    {
    public:
        /**
         * @brief Structure pour stocker une configuration de particules
         */
        struct ParticleTemplate
        {
            // Paramètres d'émission
            float emissionRate;      // Taux d'émission (particules/seconde)
            unsigned int burstSize;  // Nombre de particules dans un burst
            float lifetime;          // Durée de vie des particules (secondes)
            float lifetimeDeviation; // Variation aléatoire de la durée de vie

            // Paramètres visuels
            sf::Color color;    // Couleur de base des particules
            sf::Color colorEnd; // Couleur finale des particules
            float size;         // Taille des particules
            float sizeEnd;      // Taille finale des particules
            float rotation;     // Rotation des particules
            float rotationEnd;  // Rotation finale des particules

            // Paramètres de mouvement
            sf::Vector2f velocity;          // Vitesse initiale
            float velocityDeviation;        // Variation aléatoire de la vitesse
            sf::Vector2f acceleration;      // Accélération
            float angularVelocity;          // Vitesse de rotation
            float angularVelocityDeviation; // Variation aléatoire de la vitesse de rotation

            // Texture
            std::string texturePath; // Chemin vers la texture

            // Constructeur par défaut avec valeurs raisonnables
            ParticleTemplate()
                : emissionRate(30.f), burstSize(10), lifetime(3.f), lifetimeDeviation(1.f), color(sf::Color::White), colorEnd(sf::Color(255, 255, 255, 0)), size(10.f), sizeEnd(2.f), rotation(0.f), rotationEnd(0.f), velocity(0.f, -50.f), velocityDeviation(10.f), acceleration(0.f, 0.f), angularVelocity(0.f), angularVelocityDeviation(0.f), texturePath("")
            {
            }
        };

        /**
         * @brief Obtient l'instance singleton du système de particules
         * @return Référence à l'instance unique
         */
        static ParticleSystem &getInstance();

        /**
         * @brief Charge un template de particules depuis un fichier JSON
         * @param name Nom pour identifier le template
         * @param filename Chemin vers le fichier JSON
         * @return true si le chargement a réussi
         */
        bool loadFromFile(const std::string &name, const std::string &filename);

        /**
         * @brief Enregistre un template de particules
         * @param name Nom pour identifier le template
         * @param templ Configuration du template
         */
        void registerTemplate(const std::string &name, const ParticleTemplate &templ);

        /**
         * @brief Récupère un template de particules par son nom
         * @param name Nom du template
         * @return Pointeur vers le template ou nullptr si non trouvé
         */
        const ParticleTemplate *getTemplate(const std::string &name) const;

        /**
         * @brief Configure un émetteur Thor avec les paramètres d'un template
         * @param emitter Émetteur à configurer
         * @param templateName Nom du template à appliquer
         * @return true si le template a été trouvé et appliqué
         */
        bool applyTemplate(thor::UniversalEmitter &emitter, const std::string &templateName);

        /**
         * @brief Configure un système de particules Thor avec les paramètres d'un template
         * @param system Système de particules à configurer
         * @param emitter Émetteur à configurer
         * @param templateName Nom du template à appliquer
         * @return true si le template a été trouvé et appliqué
         */
        bool setupParticleSystem(thor::ParticleSystem &system, thor::UniversalEmitter &emitter,
                                 const std::string &templateName);

        /**
         * @brief Charge toutes les textures des templates dans le cache
         */
        void preloadTextures();

    private:
        // Constructeur privé (singleton)
        ParticleSystem();

        // Pointeur unique vers l'instance singleton
        static std::unique_ptr<ParticleSystem> s_instance;

        // Map des templates de particules
        std::unordered_map<std::string, ParticleTemplate> m_templates;

        // Cache des textures de particules
        ResourceCache<sf::Texture> m_textureCache;
    };

} // namespace Orenji