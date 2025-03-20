#include "../include/Animation/AnimationManager.hpp"
#include <Thor/Animations/FrameAnimation.hpp>
#include <vector>

namespace Orenji
{
    // Singleton instance
    AnimationManager *AnimationManager::instance = nullptr;

    AnimationManager *AnimationManager::getInstance()
    {
        if (instance == nullptr)
        {
            instance = new AnimationManager();
        }
        return instance;
    }

    AnimationManager::AnimationManager()
    {
        // Constructor initialization
    }

    AnimationManager::~AnimationManager()
    {
        // Clean up resources
        m_animations.clear();
        m_animators.clear();
    }

    void AnimationManager::createAnimation(const std::string &id, AnimationType type)
    {
        // Check if animation already exists
        if (m_animations.find(id) != m_animations.end())
        {
            return;
        }

        // Create a new animation
        thor::FrameAnimation animation;

        // Store the animation with its type
        m_animations[id] = std::make_pair(animation, type);

        // For ping-pong animations, we'll also create a reversed version
        if (type == AnimationType::PING_PONG)
        {
            std::string reversedId = id + "_reversed";
            thor::FrameAnimation reversedAnimation;
            m_animations[reversedId] = std::make_pair(reversedAnimation, AnimationType::PLAY_ONCE);
        }
    }

    void AnimationManager::addFrame(const std::string &id, const sf::IntRect &rect, float duration)
    {
        // Check if animation exists
        auto it = m_animations.find(id);
        if (it == m_animations.end())
        {
            return;
        }

        // Add frame to the animation
        it->second.first.addFrame(duration, rect);

        // If this animation is part of a ping-pong, update the reversed version too
        if (it->second.second == AnimationType::PING_PONG)
        {
            // Store this frame so we can add them in reverse order later
            if (m_animationFrames.find(id) == m_animationFrames.end())
            {
                m_animationFrames[id] = std::vector<std::pair<sf::IntRect, float>>();
            }
            m_animationFrames[id].push_back(std::make_pair(rect, duration));

            // Update the reversed animation with all frames in reverse order
            std::string reversedId = id + "_reversed";
            auto revIt = m_animations.find(reversedId);
            if (revIt != m_animations.end())
            {
                // Clear existing frames and rebuild in reverse
                thor::FrameAnimation newReversedAnim;
                for (auto frameIt = m_animationFrames[id].rbegin(); frameIt != m_animationFrames[id].rend(); ++frameIt)
                {
                    newReversedAnim.addFrame(frameIt->second, frameIt->first);
                }
                revIt->second.first = newReversedAnim;
            }
        }
    }

    void AnimationManager::playAnimation(const std::string &id, sf::Sprite &target)
    {
        // Check if animation exists
        auto it = m_animations.find(id);
        if (it == m_animations.end())
        {
            return;
        }

        // Create an animation map for this specific animation
        thor::AnimationMap<sf::Sprite, std::string> animMap;

        // Get animation and type
        thor::FrameAnimation &animation = it->second.first;
        AnimationType type = it->second.second;

        // Add the animation to the map
        animMap.addAnimation(id, animation, sf::seconds(1.f));

        // For ping-pong, add the reversed animation too
        std::string reversedId = id + "_reversed";
        if (type == AnimationType::PING_PONG)
        {
            auto revIt = m_animations.find(reversedId);
            if (revIt != m_animations.end())
            {
                animMap.addAnimation(reversedId, revIt->second.first, sf::seconds(1.f));
            }
        }

        // Create animator with the animation map
        thor::Animator<sf::Sprite, std::string> animator(animMap);

        // Calculate total animation duration based on frames
        float totalDuration = 0.f;
        if (type == AnimationType::PING_PONG && m_animationFrames.find(id) != m_animationFrames.end())
        {
            for (const auto &frame : m_animationFrames[id])
            {
                totalDuration += frame.second;
            }
        }

        // Configure playback based on animation type
        switch (type)
        {
        case AnimationType::LOOP:
            animator.play() << thor::Playback::loop(id);
            break;
        case AnimationType::PLAY_ONCE:
            animator.play() << id;
            break;
        case AnimationType::PING_PONG:
            // For ping-pong in Thor 2.0, we play the forward animation
            // then we'll switch to the reversed animation when it's done
            animator.play() << id;

            // Set up animation timers for ping-pong
            m_pingPongState[&target] = {
                id,           // animation ID
                true,         // is forward direction
                totalDuration // total duration
            };
            m_pingPongTimers[&target] = 0.f; // Reset timer
            break;
        default:
            animator.play() << id;
            break;
        }

        // Store the animator (replacing any existing one)
        if (m_animators.find(&target) != m_animators.end())
        {
            m_animators.erase(&target);
        }
        m_animators.insert(std::make_pair(&target, animator));
    }

    void AnimationManager::stopAnimation(sf::Sprite &target)
    {
        // Find the animator for the target
        auto it = m_animators.find(&target);
        if (it != m_animators.end())
        {
            it->second.stop();
        }

        // Remove from ping-pong tracking if present
        auto pingPongIt = m_pingPongState.find(&target);
        if (pingPongIt != m_pingPongState.end())
        {
            m_pingPongState.erase(pingPongIt);
        }

        // Remove from ping-pong timers if present
        auto timerIt = m_pingPongTimers.find(&target);
        if (timerIt != m_pingPongTimers.end())
        {
            m_pingPongTimers.erase(timerIt);
        }
    }

    void AnimationManager::stopAllAnimations()
    {
        // Stop all animators
        for (auto &animator : m_animators)
        {
            animator.second.stop();
        }

        // Clear ping-pong tracking
        m_pingPongState.clear();
        m_pingPongTimers.clear();
    }

    void AnimationManager::updateAnimation(sf::Sprite &target, float dt)
    {
        // Find the animator for the target
        auto it = m_animators.find(&target);
        if (it != m_animators.end())
        {
            // Update the animator with elapsed time
            it->second.update(sf::seconds(dt));

            // Apply the animation to the target
            it->second.animate(target);

            // Handle ping-pong animations using timers
            auto pingPongIt = m_pingPongState.find(&target);
            auto timerIt = m_pingPongTimers.find(&target);

            if (pingPongIt != m_pingPongState.end() && timerIt != m_pingPongTimers.end())
            {
                std::string animId = std::get<0>(pingPongIt->second);
                bool isForward = std::get<1>(pingPongIt->second);
                float duration = std::get<2>(pingPongIt->second);

                // Update timer
                timerIt->second += dt;

                // Check if we need to switch animation
                if (timerIt->second >= duration)
                {
                    timerIt->second = 0.f; // Reset timer

                    // Switch animation direction
                    if (isForward)
                    {
                        // Forward animation done, play reversed
                        std::string reversedId = animId + "_reversed";
                        it->second.play() << reversedId;
                        std::get<1>(pingPongIt->second) = false;
                    }
                    else
                    {
                        // Reversed animation done, play forward again
                        it->second.play() << animId;
                        std::get<1>(pingPongIt->second) = true;
                    }
                }
            }
        }
    }
}