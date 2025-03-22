#pragma once

#include <random>
#include <cmath>
#include <SFML/System/Vector2.hpp>

namespace Orenji
{
    namespace Utils
    {

        /**
         * @class MathUtils
         * @brief Classe utilitaire pour les fonctions mathématiques
         */
        class MathUtils
        {
        public:
            /**
             * @brief Convertit des degrés en radians
             * @param degrees Angle en degrés
             * @return Angle en radians
             */
            static float toRadians(float degrees);

            /**
             * @brief Convertit des radians en degrés
             * @param radians Angle en radians
             * @return Angle en degrés
             */
            static float toDegrees(float radians);

            /**
             * @brief Génère un nombre aléatoire entier
             * @param min Valeur minimale
             * @param max Valeur maximale
             * @return Nombre aléatoire entre min et max (inclus)
             */
            static int randInt(int min, int max);

            /**
             * @brief Génère un nombre aléatoire flottant
             * @param min Valeur minimale
             * @param max Valeur maximale
             * @return Nombre aléatoire entre min et max
             */
            static float randFloat(float min, float max);

            /**
             * @brief Limite une valeur entre deux bornes
             * @param value Valeur à limiter
             * @param min Borne minimale
             * @param max Borne maximale
             * @return Valeur limitée entre min et max
             */
            template <typename T>
            static T clamp(T value, T min, T max)
            {
                return (value < min) ? min : ((value > max) ? max : value);
            }

            /**
             * @brief Effectue une interpolation linéaire entre deux valeurs
             * @param a Première valeur
             * @param b Seconde valeur
             * @param t Facteur d'interpolation (0 à 1)
             * @return Valeur interpolée
             */
            template <typename T>
            static T lerp(T a, T b, float t)
            {
                return a + (b - a) * t;
            }

            /**
             * @brief Calcule la distance entre deux points
             * @param p1 Premier point
             * @param p2 Second point
             * @return Distance entre les deux points
             */
            static float distance(const sf::Vector2f &p1, const sf::Vector2f &p2);

            /**
             * @brief Calcule la distance au carré entre deux points (plus rapide que distance)
             * @param p1 Premier point
             * @param p2 Second point
             * @return Distance au carré entre les deux points
             */
            static float distanceSquared(const sf::Vector2f &p1, const sf::Vector2f &p2);

            /**
             * @brief Calcule la longueur d'un vecteur
             * @param vector Vecteur
             * @return Longueur du vecteur
             */
            static float length(const sf::Vector2f &vector);

            /**
             * @brief Calcule la longueur au carré d'un vecteur (plus rapide que length)
             * @param vector Vecteur
             * @return Longueur au carré du vecteur
             */
            static float lengthSquared(const sf::Vector2f &vector);

            /**
             * @brief Normalise un vecteur (longueur de 1)
             * @param vector Vecteur à normaliser
             * @return Vecteur normalisé
             */
            static sf::Vector2f normalize(const sf::Vector2f &vector);

            /**
             * @brief Calcule le produit scalaire de deux vecteurs
             * @param v1 Premier vecteur
             * @param v2 Second vecteur
             * @return Produit scalaire
             */
            static float dotProduct(const sf::Vector2f &v1, const sf::Vector2f &v2);

            /**
             * @brief Calcule le produit vectoriel de deux vecteurs
             * @param v1 Premier vecteur
             * @param v2 Second vecteur
             * @return Produit vectoriel
             */
            static float crossProduct(const sf::Vector2f &v1, const sf::Vector2f &v2);

            /**
             * @brief Calcule l'angle entre deux vecteurs
             * @param v1 Premier vecteur
             * @param v2 Second vecteur
             * @return Angle en degrés
             */
            static float angle(const sf::Vector2f &v1, const sf::Vector2f &v2);

            /**
             * @brief Calcule l'angle d'un vecteur
             * @param vector Vecteur
             * @return Angle en degrés
             */
            static float angle(const sf::Vector2f &vector);

            /**
             * @brief Crée un vecteur à partir d'un angle
             * @param angle Angle en degrés
             * @param length Longueur du vecteur
             * @return Vecteur
             */
            static sf::Vector2f vectorFromAngle(float angle, float length = 1.f);

        private:
            static std::random_device s_rd;                      ///< Dispositif aléatoire
            static std::mt19937 s_gen;                           ///< Générateur aléatoire
            static constexpr float PI = 3.14159265358979323846f; ///< Valeur de PI
            static constexpr float DEG_TO_RAD = PI / 180.f;      ///< Facteur de conversion degrés -> radians
            static constexpr float RAD_TO_DEG = 180.f / PI;      ///< Facteur de conversion radians -> degrés
        };

    } // namespace Utils
} // namespace Orenji