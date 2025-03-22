#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

namespace Core
{
    // Cette classe gère la caméra du jeu, avec suivi d'entité et effets
    class Camera
    {
    public:
        // Types d'effets de caméra
        enum class Effect
        {
            None,
            Shake,
            Zoom,
            Pan
        };

        Camera(sf::RenderWindow &window, const sf::Vector2f &worldSize);
        ~Camera();

        // Mettre à jour la caméra (position, effets, etc.)
        void update(float deltaTime);

        // Appliquer la vue à la fenêtre
        void apply();

        // Définir une entité à suivre
        void follow(const sf::Vector2f *targetPosition, bool smoothFollow = true);

        // Arrêter de suivre
        void stopFollowing();

        // Centrer la caméra sur une position précise
        void centerOn(const sf::Vector2f &position);

        // Ajouter un effet temporaire à la caméra
        void addEffect(Effect effect, float duration, float intensity);

        // Transformations de coordonnées
        sf::Vector2f worldToScreen(const sf::Vector2f &worldPos) const;
        sf::Vector2f screenToWorld(const sf::Vector2f &screenPos) const;

        // Accesseurs et modificateurs
        void setZoom(float zoomFactor);
        float getZoom() const;

        void setViewport(const sf::FloatRect &viewport);
        sf::FloatRect getViewport() const;

        void setBounds(const sf::FloatRect &bounds);
        sf::FloatRect getBounds() const;

        sf::Vector2f getCenter() const;
        sf::Vector2f getSize() const;

    private:
        // Référence à la fenêtre SFML
        sf::RenderWindow &m_window;

        // La vue SFML
        sf::View m_view;

        // Les limites de la caméra dans le monde
        sf::FloatRect m_bounds;

        // La taille du monde
        sf::Vector2f m_worldSize;

        // Facteur de zoom actuel
        float m_zoomFactor;

        // Position cible (pour le suivi)
        const sf::Vector2f *m_targetPosition;

        // Utiliser un suivi fluide ou non
        bool m_smoothFollow;

        // Vitesse de déplacement pour le suivi fluide
        float m_followSpeed;

        // Effet actuel
        Effect m_currentEffect;

        // Durée et intensité de l'effet actuel
        float m_effectDuration;
        float m_effectIntensity;
        float m_effectTimer;

        // Position de la caméra avant effet
        sf::Vector2f m_basePosition;

        // Méthodes pour les effets
        void applyShakeEffect();
        void applyZoomEffect(float deltaTime);
        void applyPanEffect(float deltaTime);

        // Méthode pour maintenir la caméra dans les limites
        void keepInBounds();
    };

} // namespace Core