#pragma once

#include <cmath>
#include <random>
#include <algorithm>
#include <SFML/System/Vector2.hpp>

namespace Utils
{
    namespace Math
    {
        // Constantes
        constexpr float PI = 3.14159265358979323846f;
        constexpr float TWO_PI = 2.0f * PI;
        constexpr float HALF_PI = PI / 2.0f;
        constexpr float EPSILON = 0.000001f;

        // Conversion entre degrés et radians
        inline float toRadians(float degrees)
        {
            return degrees * (PI / 180.0f);
        }

        inline float toDegrees(float radians)
        {
            return radians * (180.0f / PI);
        }

        // Limite une valeur entre un minimum et un maximum
        template <typename T>
        inline T clamp(T value, T min, T max)
        {
            return std::max(min, std::min(value, max));
        }

        // Génère un nombre aléatoire entre min et max
        inline float random(float min, float max)
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(min, max);
            return dis(gen);
        }

        inline int randomInt(int min, int max)
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(min, max);
            return dis(gen);
        }

        // Fonction d'interpolation linéaire
        template <typename T>
        inline T lerp(T a, T b, float t)
        {
            return a + (b - a) * clamp(t, 0.0f, 1.0f);
        }

        // Calcule la distance entre deux points
        template <typename T>
        inline float distance(T x1, T y1, T x2, T y2)
        {
            float dx = static_cast<float>(x2 - x1);
            float dy = static_cast<float>(y2 - y1);
            return std::sqrt(dx * dx + dy * dy);
        }

        // Vérifie si deux valeurs flottantes sont approximativement égales
        inline bool approximately(float a, float b, float epsilon = EPSILON)
        {
            return std::abs(a - b) <= epsilon;
        }

        // Calcule la direction (angle) entre deux points
        inline float direction(float x1, float y1, float x2, float y2)
        {
            return std::atan2(y2 - y1, x2 - x1);
        }

        // Génère un nombre aléatoire entier entre min et max inclus
        inline int randInt(int min, int max)
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(min, max);
            return dist(gen);
        }

        // Génère un nombre aléatoire flottant entre min et max
        inline float randFloat(float min, float max)
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dist(min, max);
            return dist(gen);
        }

        // Calcule la distance entre deux points
        inline float distance(const sf::Vector2f &a, const sf::Vector2f &b)
        {
            float dx = b.x - a.x;
            float dy = b.y - a.y;
            return std::sqrt(dx * dx + dy * dy);
        }

        // Calcule l'angle entre deux points en degrés
        inline float angleBetween(const sf::Vector2f &a, const sf::Vector2f &b)
        {
            float dx = b.x - a.x;
            float dy = b.y - a.y;
            float angle = std::atan2(dy, dx) * 180.0f / PI;
            return angle < 0 ? angle + 360.0f : angle;
        }

        // Normalise un vecteur (le ramène à une longueur de 1)
        inline sf::Vector2f normalize(const sf::Vector2f &vector)
        {
            float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
            if (length > 0)
                return sf::Vector2f(vector.x / length, vector.y / length);
            return vector;
        }
    }
}

namespace Orenji
{
    /**
     * @brief Classe utilitaire pour les opérations mathématiques,
     * qui expose les fonctions de Utils::Math
     */
    class MathUtils
    {
    public:
        // Constantes
        static constexpr float PI = Utils::Math::PI;
        static constexpr float TWO_PI = Utils::Math::TWO_PI;
        static constexpr float HALF_PI = Utils::Math::HALF_PI;
        static constexpr float EPSILON = Utils::Math::EPSILON;

        // Conversion entre degrés et radians
        static float toRadians(float degrees)
        {
            return Utils::Math::toRadians(degrees);
        }

        static float toDegrees(float radians)
        {
            return Utils::Math::toDegrees(radians);
        }

        // Limite une valeur entre un minimum et un maximum
        template <typename T>
        static T clamp(T value, T min, T max)
        {
            return Utils::Math::clamp(value, min, max);
        }

        // Génère un nombre aléatoire entier entre min et max inclus
        static int randInt(int min, int max)
        {
            return Utils::Math::randInt(min, max);
        }

        // Génère un nombre aléatoire flottant entre min et max
        static float randFloat(float min, float max)
        {
            return Utils::Math::randFloat(min, max);
        }

        // Fonction d'interpolation linéaire
        template <typename T>
        static T lerp(T a, T b, float t)
        {
            return Utils::Math::lerp(a, b, t);
        }

        // Calcule la distance entre deux points
        template <typename T>
        static float distance(T x1, T y1, T x2, T y2)
        {
            return Utils::Math::distance(x1, y1, x2, y2);
        }

        // Calcule la distance entre deux points
        static float distance(const sf::Vector2f &a, const sf::Vector2f &b)
        {
            return Utils::Math::distance(a, b);
        }

        // Vérifie si deux valeurs flottantes sont approximativement égales
        static bool approximately(float a, float b, float epsilon = EPSILON)
        {
            return Utils::Math::approximately(a, b, epsilon);
        }

        // Calcule la direction (angle) entre deux points
        static float direction(float x1, float y1, float x2, float y2)
        {
            return Utils::Math::direction(x1, y1, x2, y2);
        }

        // Calcule l'angle entre deux points en degrés
        static float angleBetween(const sf::Vector2f &a, const sf::Vector2f &b)
        {
            return Utils::Math::angleBetween(a, b);
        }

        // Normalise un vecteur (le ramène à une longueur de 1)
        static sf::Vector2f normalize(const sf::Vector2f &vector)
        {
            return Utils::Math::normalize(vector);
        }
    };
}