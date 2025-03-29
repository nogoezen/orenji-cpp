#pragma once

#include <SFML/Graphics.hpp>
#include "../../Core/Component.hpp"
#include <map>
#include <string>
#include <vector>
#include <memory>

namespace Graphics
{
    namespace Components
    {

        /**
         * @brief Structure representing an animation frame
         */
        struct AnimationFrame
        {
            sf::IntRect textureRect; // Rectangle in the texture
            float duration;          // Duration of the frame in seconds
        };

        /**
         * @brief Structure representing an animation sequence
         */
        struct Animation
        {
            std::string name;                   // Name of the animation
            std::vector<AnimationFrame> frames; // Frames of the animation
            bool loop;                          // Whether the animation should loop
            float speed;                        // Speed multiplier for the animation

            /**
             * @brief Constructor
             * @param name Name of the animation
             * @param loop Whether it should loop
             * @param speed Speed multiplier (1.0 = normal)
             */
            Animation(const std::string &name, bool loop = true, float speed = 1.0f)
                : name(name), loop(loop), speed(speed) {}

            /**
             * @brief Add a frame to the animation
             * @param rect Rectangle in the texture
             * @param duration Duration of the frame in seconds
             */
            void addFrame(const sf::IntRect &rect, float duration)
            {
                frames.push_back({rect, duration});
            }

            /**
             * @brief Get total duration of the animation
             * @return Duration in seconds
             */
            float getTotalDuration() const
            {
                float total = 0.0f;
                for (const auto &frame : frames)
                {
                    total += frame.duration;
                }
                return total;
            }
        };

        /**
         * @brief Component for sprite animations
         */
        class AnimationComponent : public Core::Component
        {
        public:
            /**
             * @brief Constructor
             */
            AnimationComponent();

            /**
             * @brief Destructor
             */
            virtual ~AnimationComponent() = default;

            /**
             * @brief Add an animation
             * @param animation Animation to add
             */
            void addAnimation(const Animation &animation);

            /**
             * @brief Play an animation
             * @param name Name of the animation
             * @param restart Whether to restart if already playing
             * @return True if the animation was found and started
             */
            bool play(const std::string &name, bool restart = false);

            /**
             * @brief Stop the current animation
             */
            void stop();

            /**
             * @brief Pause the current animation
             */
            void pause();

            /**
             * @brief Resume the current animation
             */
            void resume();

            /**
             * @brief Update the animation
             * @param deltaTime Time since last frame in seconds
             * @return Current texture rectangle, or empty if no animation
             */
            sf::IntRect update(float deltaTime);

            /**
             * @brief Check if an animation is playing
             * @return True if playing
             */
            bool isPlaying() const;

            /**
             * @brief Get current animation name
             * @return Name of current animation, or empty string if none
             */
            std::string getCurrentAnimationName() const;

            /**
             * @brief Check if current animation has finished
             * @return True if finished
             */
            bool isFinished() const;

            /**
             * @brief Set animation speed
             * @param speed Speed multiplier (1.0 = normal)
             */
            void setSpeed(float speed);

            /**
             * @brief Get animation speed
             * @return Speed multiplier
             */
            float getSpeed() const;

        private:
            std::map<std::string, Animation> m_animations;
            std::string m_currentAnimation;
            size_t m_currentFrame;
            float m_currentTime;
            bool m_playing;
            bool m_finished;
            float m_speed;
        };

    }
} // namespace Graphics::Components