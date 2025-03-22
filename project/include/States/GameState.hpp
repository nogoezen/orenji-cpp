#pragma once

#include "States/State.hpp"
#include "Core/EntityManager.hpp"
#include "Core/System.hpp"
#include <vector>
#include <memory>
#include <Box2D/Box2D.h>

namespace Orenji
{
    /**
     * @brief État de jeu principal
     *
     * Cet état gère le déroulement du jeu lui-même.
     */
    class GameState : public State
    {
    public:
        /**
         * @brief Constructeur
         *
         * @param machine Référence vers la machine à états
         */
        GameState(StateMachine &machine);

        /**
         * @brief Destructeur
         */
        ~GameState();

        /**
         * @brief Initialise l'état de jeu
         */
        void onEnter() override;

        /**
         * @brief Nettoie l'état de jeu
         */
        void onExit() override;

        /**
         * @brief Met à jour l'état de jeu
         *
         * @param dt Temps écoulé depuis la dernière mise à jour (en secondes)
         */
        void update(float dt) override;

        /**
         * @brief Gère les événements de jeu
         *
         * @param event Événement SFML à traiter
         */
        void handleEvent(const sf::Event &event) override;

        /**
         * @brief Dessine l'état de jeu
         *
         * @param target Cible de rendu
         */
        void draw(sf::RenderTarget &target) override;

        /**
         * @brief Récupère l'identifiant de l'état
         *
         * @return Identifiant de l'état
         */
        std::string getStateID() const override { return "Game"; }

    private:
        /**
         * @brief Initialise les systèmes ECS
         */
        void initSystems();

        /**
         * @brief Initialise le monde physique Box2D
         */
        void initPhysics();

        /**
         * @brief Charge le niveau à partir d'un fichier Tiled
         *
         * @param levelPath Chemin vers le fichier de niveau
         * @return true si le chargement a réussi, false sinon
         */
        bool loadLevel(const std::string &levelPath);

    private:
        EntityManager m_entityManager;           ///< Gestionnaire d'entités
        std::vector<SystemPtr> m_systems;        ///< Systèmes de jeu
        std::unique_ptr<b2World> m_physicsWorld; ///< Monde physique Box2D

        sf::View m_gameView; ///< Vue de jeu
        sf::View m_uiView;   ///< Vue de l'interface utilisateur

        bool m_paused; ///< État de pause du jeu

        // Ressources de l'état
        sf::Texture m_backgroundTexture; ///< Texture de fond
        sf::Sprite m_backgroundSprite;   ///< Sprite de fond
    };

} // namespace Orenji