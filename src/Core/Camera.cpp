#include "../../include/Core/Camera.hpp"
#include <cmath>
#include <random>

namespace Core
{
    Camera::Camera(sf::RenderWindow &window, const sf::Vector2f &worldSize)
        : m_window(window), m_worldSize(worldSize), m_zoomFactor(1.0f), m_targetPosition(nullptr), m_smoothFollow(true), m_followSpeed(5.0f), m_currentEffect(Effect::None), m_effectDuration(0.0f), m_effectIntensity(0.0f), m_effectTimer(0.0f), m_basePosition(0.0f, 0.0f)
    {
        // Initialiser la vue avec la taille de la fenêtre
        m_view = window.getDefaultView();

        // Définir les limites par défaut comme la taille du monde
        // SFML 3: sf::FloatRect prend maintenant deux sf::Vector2f (position, size)
        m_bounds = sf::FloatRect({0.0f, 0.0f}, {worldSize.x, worldSize.y});
    }

    Camera::~Camera()
    {
    }

    void Camera::update(float deltaTime)
    {
        // Suivre la cible si définie
        if (m_targetPosition)
        {
            if (m_smoothFollow)
            {
                // Suivi fluide avec interpolation
                sf::Vector2f currentCenter = m_view.getCenter();
                sf::Vector2f targetPos = *m_targetPosition;

                // Calculer le vecteur de déplacement
                sf::Vector2f moveVector = targetPos - currentCenter;

                // Appliquer un facteur d'interpolation pour obtenir un mouvement fluide
                m_view.setCenter(currentCenter + moveVector * std::min(1.0f, deltaTime * m_followSpeed));
            }
            else
            {
                // Suivi direct
                m_view.setCenter(*m_targetPosition);
            }
        }

        // Gérer les effets actifs
        if (m_currentEffect != Effect::None)
        {
            m_effectTimer -= deltaTime;

            // Mettre à jour l'effet actuel
            switch (m_currentEffect)
            {
            case Effect::Shake:
                applyShakeEffect();
                break;

            case Effect::Zoom:
                applyZoomEffect(deltaTime);
                break;

            case Effect::Pan:
                applyPanEffect(deltaTime);
                break;

            default:
                break;
            }

            // Vérifier si l'effet est terminé
            if (m_effectTimer <= 0.0f)
            {
                m_currentEffect = Effect::None;
                m_view.setCenter(m_basePosition);
            }
        }

        // S'assurer que la caméra reste dans les limites
        keepInBounds();
    }

    void Camera::apply()
    {
        m_window.setView(m_view);
    }

    void Camera::follow(const sf::Vector2f *targetPosition, bool smoothFollow)
    {
        m_targetPosition = targetPosition;
        m_smoothFollow = smoothFollow;
    }

    void Camera::stopFollowing()
    {
        m_targetPosition = nullptr;
    }

    void Camera::centerOn(const sf::Vector2f &position)
    {
        m_view.setCenter(position);
        keepInBounds();
    }

    void Camera::addEffect(Effect effect, float duration, float intensity)
    {
        // Sauvegarder la position actuelle pour pouvoir y revenir après l'effet
        m_basePosition = m_view.getCenter();

        m_currentEffect = effect;
        m_effectDuration = duration;
        m_effectIntensity = intensity;
        m_effectTimer = duration;
    }

    sf::Vector2f Camera::worldToScreen(const sf::Vector2f &worldPos) const
    {
        // SFML 3: mapCoordsToPixel retourne un sf::Vector2i, on le convertit en sf::Vector2f
        sf::Vector2i pixelPos = m_window.mapCoordsToPixel(worldPos, m_view);
        return sf::Vector2f(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));
    }

    sf::Vector2f Camera::screenToWorld(const sf::Vector2f &screenPos) const
    {
        // SFML 3: Convertir Vector2f en Vector2i pour mapPixelToCoords
        sf::Vector2i pixelPos(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y));
        return m_window.mapPixelToCoords(pixelPos, m_view);
    }

    void Camera::setZoom(float zoomFactor)
    {
        if (zoomFactor > 0.0f)
        {
            m_zoomFactor = zoomFactor;

            // SFML 3: setSize prend un sf::Vector2f
            sf::Vector2f defaultSize = m_window.getDefaultView().getSize();
            m_view.setSize(sf::Vector2f(defaultSize.x / m_zoomFactor, defaultSize.y / m_zoomFactor));
        }
    }

    float Camera::getZoom() const
    {
        return m_zoomFactor;
    }

    void Camera::setViewport(const sf::FloatRect &viewport)
    {
        m_view.setViewport(viewport);
    }

    sf::FloatRect Camera::getViewport() const
    {
        return m_view.getViewport();
    }

    void Camera::setBounds(const sf::FloatRect &bounds)
    {
        m_bounds = bounds;
        keepInBounds();
    }

    sf::FloatRect Camera::getBounds() const
    {
        return m_bounds;
    }

    sf::Vector2f Camera::getCenter() const
    {
        return m_view.getCenter();
    }

    sf::Vector2f Camera::getSize() const
    {
        return m_view.getSize();
    }

    void Camera::applyShakeEffect()
    {
        // Générateur de nombres aléatoires pour l'effet de secousse
        static std::default_random_engine generator(static_cast<unsigned long>(time(nullptr)));
        std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

        // Calculer l'amplitude de la secousse (diminue avec le temps)
        float remainingFactor = m_effectTimer / m_effectDuration;
        float shakeAmount = m_effectIntensity * remainingFactor;

        // Générer un déplacement aléatoire
        sf::Vector2f shakeOffset(
            distribution(generator) * shakeAmount,
            distribution(generator) * shakeAmount);

        // Appliquer à la position de base
        m_view.setCenter(m_basePosition + shakeOffset);
    }

    void Camera::applyZoomEffect(float deltaTime)
    {
        // Calculer le facteur de zoom transitoire
        float remainingFactor = m_effectTimer / m_effectDuration;
        float currentZoomFactor;

        // Si l'intensité est positive, on zoome; sinon on dézoome
        if (m_effectIntensity > 0.0f)
        {
            // Commencer zoomé et revenir progressivement
            currentZoomFactor = 1.0f + (m_effectIntensity * remainingFactor);
        }
        else
        {
            // Commencer dézoomé et revenir progressivement
            currentZoomFactor = 1.0f + (m_effectIntensity * (1.0f - remainingFactor));
        }

        // Appliquer le zoom temporaire à la vue (SFML 3: setSize prend un sf::Vector2f)
        sf::Vector2f defaultSize = m_window.getDefaultView().getSize();
        float factor = m_zoomFactor * currentZoomFactor;
        m_view.setSize(sf::Vector2f(defaultSize.x / factor, defaultSize.y / factor));

        // Centrer sur la position de base
        m_view.setCenter(m_basePosition);
    }

    void Camera::applyPanEffect(float deltaTime)
    {
        // Calculer la progression de l'effet (0.0 -> 1.0)
        float progress = 1.0f - (m_effectTimer / m_effectDuration);

        // Supposons que m_effectIntensity représente la distance maximale de déplacement
        sf::Vector2f targetPos = m_basePosition + sf::Vector2f(m_effectIntensity, 0.0f);

        // Interpolation linéaire entre la position de départ et la cible
        sf::Vector2f newPos = m_basePosition + (targetPos - m_basePosition) * progress;

        // Appliquer la nouvelle position
        m_view.setCenter(newPos);
    }

    void Camera::keepInBounds()
    {
        // Récupérer la taille et le centre actuels de la vue
        sf::Vector2f viewSize = m_view.getSize();
        sf::Vector2f viewCenter = m_view.getCenter();

        // Calculer les limites de la vue en tenant compte de sa taille
        float viewLeft = viewCenter.x - viewSize.x / 2.0f;
        float viewTop = viewCenter.y - viewSize.y / 2.0f;
        float viewRight = viewCenter.x + viewSize.x / 2.0f;
        float viewBottom = viewCenter.y + viewSize.y / 2.0f;

        // SFML 3: les propriétés de FloatRect sont maintenant des vecteurs position et size
        sf::Vector2f boundsPos = m_bounds.position;
        sf::Vector2f boundsSize = m_bounds.size;

        // Ajuster horizontalement si nécessaire
        if (viewSize.x <= boundsSize.x)
        {
            // La vue est plus petite que les limites, on peut la déplacer
            if (viewLeft < boundsPos.x)
            {
                viewCenter.x += boundsPos.x - viewLeft;
            }
            else if (viewRight > boundsPos.x + boundsSize.x)
            {
                viewCenter.x -= viewRight - (boundsPos.x + boundsSize.x);
            }
        }
        else
        {
            // La vue est plus grande que les limites, on centre sur les limites
            viewCenter.x = boundsPos.x + boundsSize.x / 2.0f;
        }

        // Ajuster verticalement si nécessaire
        if (viewSize.y <= boundsSize.y)
        {
            // La vue est plus petite que les limites, on peut la déplacer
            if (viewTop < boundsPos.y)
            {
                viewCenter.y += boundsPos.y - viewTop;
            }
            else if (viewBottom > boundsPos.y + boundsSize.y)
            {
                viewCenter.y -= viewBottom - (boundsPos.y + boundsSize.y);
            }
        }
        else
        {
            // La vue est plus grande que les limites, on centre sur les limites
            viewCenter.y = boundsPos.y + boundsSize.y / 2.0f;
        }

        // Appliquer la nouvelle position
        m_view.setCenter(viewCenter);
    }
} // namespace Core