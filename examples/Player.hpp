#pragma once

#include "../include/Core/Entity.hpp"
#include "../include/Core/Component.hpp"
#include "../include/TiledMap/TiledMapCollision.hpp"
#include <SFML/Graphics.hpp>

namespace Orenji
{
    // Composant de joueur
    class PlayerComponent : public Component
    {
    public:
        PlayerComponent() : m_speed(200.0f), m_canJump(true), m_velocity(0.0f, 0.0f) {}

        void initialize() override
        {
            // Chercher le composant de collision de la carte
            auto *scene = getEntity()->getScene();
            if (!scene)
                return;

            auto *entities = scene->getEntityManager();
            if (!entities)
                return;

            for (auto *entity : entities->getEntities())
            {
                m_mapCollision = entity->getComponent<TiledMapCollision>();
                if (m_mapCollision)
                    break;
            }
        }

        void update(float deltaTime) override
        {
            if (!getEntity())
                return;

            // Vecteur de mouvement
            sf::Vector2f movement(0.0f, 0.0f);

            // Gestion des entrées
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                movement.x -= m_speed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                movement.x += m_speed;
            }

            // Saut
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_canJump)
            {
                m_velocity.y = -400.0f;
                m_canJump = false;
            }

            // Gravité
            m_velocity.y += 980.0f * deltaTime;

            // Appliquer la vitesse horizontale
            m_velocity.x = movement.x;

            // Calculer le déplacement
            sf::Vector2f displacement = m_velocity * deltaTime;

            // Gérer les collisions si disponible
            if (m_mapCollision)
            {
                // Position actuelle
                sf::Vector2f position = getEntity()->getPosition();

                // Boîte de collision du joueur (simple rectangle 32x64)
                sf::FloatRect playerRect(position.x - 16, position.y - 32, 32, 64);

                // Déplacer horizontalement et vérifier les collisions
                playerRect.left += displacement.x;
                if (m_mapCollision->checkCollision(playerRect))
                {
                    // Collision horizontale, arrêter le mouvement
                    displacement.x = 0.0f;
                    m_velocity.x = 0.0f;
                }

                // Déplacer verticalement et vérifier les collisions
                playerRect.left -= displacement.x; // Remettre à la position horizontale originale
                playerRect.top += displacement.y;
                if (m_mapCollision->checkCollision(playerRect))
                {
                    // Collision verticale
                    if (m_velocity.y > 0)
                    {
                        // Collision avec le sol
                        m_canJump = true;
                    }
                    displacement.y = 0.0f;
                    m_velocity.y = 0.0f;
                }
                else
                {
                    // Pas de collision avec le sol
                    m_canJump = false;
                }
            }

            // Appliquer le déplacement
            getEntity()->move(displacement);
        }

    private:
        float m_speed;
        bool m_canJump;
        sf::Vector2f m_velocity;
        TiledMapCollision *m_mapCollision = nullptr;
    };

} // namespace Orenji