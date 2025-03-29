#include "Graphics/Components/AnimationComponent.hpp"

namespace Graphics
{
    namespace Components
    {

        AnimationComponent::AnimationComponent()
            : m_currentFrame(0), m_currentTime(0.0f), m_playing(false), m_finished(false), m_speed(1.0f)
        {
        }

        void AnimationComponent::addAnimation(const Animation &animation)
        {
            m_animations[animation.name] = animation;
        }

        bool AnimationComponent::play(const std::string &name, bool restart)
        {
            auto it = m_animations.find(name);
            if (it == m_animations.end())
            {
                return false;
            }

            if (m_currentAnimation == name && !restart)
            {
                if (!m_playing)
                {
                    m_playing = true;
                    m_finished = false;
                }
                return true;
            }

            m_currentAnimation = name;
            m_currentFrame = 0;
            m_currentTime = 0.0f;
            m_playing = true;
            m_finished = false;

            return true;
        }

        void AnimationComponent::stop()
        {
            m_playing = false;
            m_currentFrame = 0;
            m_currentTime = 0.0f;
            m_finished = false;
        }

        void AnimationComponent::pause()
        {
            m_playing = false;
        }

        void AnimationComponent::resume()
        {
            if (!m_finished)
            {
                m_playing = true;
            }
        }

        sf::IntRect AnimationComponent::update(float deltaTime)
        {
            if (!m_playing || m_currentAnimation.empty())
            {
                // Si aucune animation n'est en cours, on retourne un rectangle vide
                return sf::IntRect();
            }

            const Animation &anim = m_animations[m_currentAnimation];

            if (anim.frames.empty())
            {
                return sf::IntRect();
            }

            // Mettre à jour le temps écoulé
            m_currentTime += deltaTime * m_speed * anim.speed;

            // Vérifier si nous devons passer au frame suivant
            while (m_currentTime >= anim.frames[m_currentFrame].duration && m_playing)
            {
                m_currentTime -= anim.frames[m_currentFrame].duration;
                m_currentFrame++;

                // Si l'animation est terminée
                if (m_currentFrame >= anim.frames.size())
                {
                    if (anim.loop)
                    {
                        // Boucler au début
                        m_currentFrame = 0;
                    }
                    else
                    {
                        // Arrêter à la dernière frame
                        m_currentFrame = anim.frames.size() - 1;
                        m_playing = false;
                        m_finished = true;
                        break;
                    }
                }
            }

            // Retourner le rectangle de la frame courante
            return anim.frames[m_currentFrame].textureRect;
        }

        bool AnimationComponent::isPlaying() const
        {
            return m_playing;
        }

        std::string AnimationComponent::getCurrentAnimationName() const
        {
            return m_currentAnimation;
        }

        bool AnimationComponent::isFinished() const
        {
            return m_finished;
        }

        void AnimationComponent::setSpeed(float speed)
        {
            m_speed = speed;
        }

        float AnimationComponent::getSpeed() const
        {
            return m_speed;
        }

    }
} // namespace Graphics::Components