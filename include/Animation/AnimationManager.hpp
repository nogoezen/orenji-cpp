#pragma once

#include <SFML/Graphics.hpp>
#include <Thor/Animations.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <utility>
#include <tuple>

namespace Orenji
{
    /**
     * @brief Animation management class based on Thor
     *
     * This class allows creating, storing and playing animations from
     * spritesheets or image sequences.
     */
    class AnimationManager
    {
    public:
        /**
         * @brief Animation type
         */
        enum class AnimationType
        {
            LOOP,      ///< Looping animation
            PLAY_ONCE, ///< Animation played once
            PING_PONG  ///< Back and forth animation
        };

        /**
         * @brief Get the singleton instance
         * @return Pointer to the AnimationManager instance
         */
        static AnimationManager *getInstance();

        /**
         * @brief Destructor
         */
        ~AnimationManager();

        /**
         * @brief Create an animation
         * @param id Unique identifier for the animation
         * @param type Animation type (loop, once, ping-pong)
         */
        void createAnimation(const std::string &id, AnimationType type = AnimationType::LOOP);

        /**
         * @brief Add a frame to an existing animation
         * @param id Animation identifier
         * @param rect Rectangle defining the position of the sprite in the texture
         * @param duration Duration of this frame in seconds
         */
        void addFrame(const std::string &id, const sf::IntRect &rect, float duration);

        /**
         * @brief Play an animation
         * @param id Animation identifier
         * @param target Sprite to apply the animation to
         */
        void playAnimation(const std::string &id, sf::Sprite &target);

        /**
         * @brief Stop an animation for a specific sprite
         * @param target Sprite to stop animation for
         */
        void stopAnimation(sf::Sprite &target);

        /**
         * @brief Stop all animations
         */
        void stopAllAnimations();

        /**
         * @brief Update animation for a specific sprite
         * @param target Sprite to update animation for
         * @param dt Time elapsed since the last update in seconds
         */
        void updateAnimation(sf::Sprite &target, float dt);

    private:
        /**
         * @brief Constructor (private for singleton)
         */
        AnimationManager();

        static AnimationManager *instance;

        // Mapping of animation ID to (animation, type) pair
        std::unordered_map<std::string, std::pair<thor::FrameAnimation, AnimationType>> m_animations;

        // Mapping of sprite pointers to their animators
        std::unordered_map<sf::Sprite *, thor::Animator<sf::Sprite, std::string>> m_animators;

        // Store frame data for ping-pong animations (needed to create reversed animations)
        std::unordered_map<std::string, std::vector<std::pair<sf::IntRect, float>>> m_animationFrames;

        // Track ping-pong animation states (sprite pointer to [animation id, is forward direction, duration])
        std::unordered_map<sf::Sprite *, std::tuple<std::string, bool, float>> m_pingPongState;

        // Track timers for ping-pong animations
        std::unordered_map<sf::Sprite *, float> m_pingPongTimers;
    };

} // namespace Orenji