#include "Utils/MathUtils.hpp"

namespace Orenji
{
    namespace Utils
    {

        // Initialisation des membres statiques
        std::random_device MathUtils::s_rd;
        std::mt19937 MathUtils::s_gen(s_rd());

        float MathUtils::toRadians(float degrees)
        {
            return degrees * DEG_TO_RAD;
        }

        float MathUtils::toDegrees(float radians)
        {
            return radians * RAD_TO_DEG;
        }

        int MathUtils::randInt(int min, int max)
        {
            std::uniform_int_distribution<int> dist(min, max);
            return dist(s_gen);
        }

        float MathUtils::randFloat(float min, float max)
        {
            std::uniform_real_distribution<float> dist(min, max);
            return dist(s_gen);
        }

        float MathUtils::distance(const sf::Vector2f &p1, const sf::Vector2f &p2)
        {
            return std::sqrt(distanceSquared(p1, p2));
        }

        float MathUtils::distanceSquared(const sf::Vector2f &p1, const sf::Vector2f &p2)
        {
            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            return dx * dx + dy * dy;
        }

        float MathUtils::length(const sf::Vector2f &vector)
        {
            return std::sqrt(lengthSquared(vector));
        }

        float MathUtils::lengthSquared(const sf::Vector2f &vector)
        {
            return vector.x * vector.x + vector.y * vector.y;
        }

        sf::Vector2f MathUtils::normalize(const sf::Vector2f &vector)
        {
            float len = length(vector);
            if (len != 0.f)
            {
                return sf::Vector2f(vector.x / len, vector.y / len);
            }
            return vector;
        }

        float MathUtils::dotProduct(const sf::Vector2f &v1, const sf::Vector2f &v2)
        {
            return v1.x * v2.x + v1.y * v2.y;
        }

        float MathUtils::crossProduct(const sf::Vector2f &v1, const sf::Vector2f &v2)
        {
            return v1.x * v2.y - v1.y * v2.x;
        }

        float MathUtils::angle(const sf::Vector2f &v1, const sf::Vector2f &v2)
        {
            float dot = dotProduct(normalize(v1), normalize(v2));
            return toDegrees(std::acos(clamp(dot, -1.f, 1.f)));
        }

        float MathUtils::angle(const sf::Vector2f &vector)
        {
            return toDegrees(std::atan2(vector.y, vector.x));
        }

        sf::Vector2f MathUtils::vectorFromAngle(float angle, float length)
        {
            float rad = toRadians(angle);
            return sf::Vector2f(std::cos(rad) * length, std::sin(rad) * length);
        }

    } // namespace Utils
} // namespace Orenji