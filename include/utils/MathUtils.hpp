#pragma once

#include <cmath>
#include <random>
#include <algorithm>

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
    }
}