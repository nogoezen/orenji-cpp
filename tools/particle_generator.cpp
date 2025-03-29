#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include <filesystem>

/**
 * Outil de génération de textures de particules pour le système de particules
 * Cet outil génère plusieurs textures de particules basiques et les sauvegarde
 * dans le dossier resources/textures/
 */
int main()
{
    // S'assurer que le dossier de destination existe
    std::filesystem::create_directories("../resources/textures");

    // Taille de la texture (doit être une puissance de 2 pour de meilleures performances)
    const int size = 64;

    // Créer une image pour notre texture
    sf::Image image;
    image.create(size, size, sf::Color::Transparent);

    // Générer une particule circulaire avec dégradé
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            // Calculer la distance du centre
            float dx = x - size / 2.0f;
            float dy = y - size / 2.0f;
            float distance = std::sqrt(dx * dx + dy * dy);

            // Normaliser la distance (0 au centre, 1 au bord)
            float normalizedDistance = distance / (size / 2.0f);

            // Appliquer un dégradé radial (plus vif au centre, transparent aux bords)
            if (normalizedDistance < 1.0f)
            {
                // Calculer l'alpha basé sur la distance (dégradé exponentiel pour des bords plus doux)
                float alpha = 255.0f * (1.0f - std::pow(normalizedDistance, 2.0f));

                // Fixer le pixel avec la couleur blanche et l'alpha calculé
                image.setPixel(x, y, sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
            }
        }
    }

    // Sauvegarder l'image comme texture de base pour les particules
    std::string filePath = "../resources/textures/particle.png";
    if (image.saveToFile(filePath))
    {
        std::cout << "Texture de particule générée avec succès: " << filePath << std::endl;
    }
    else
    {
        std::cerr << "Erreur lors de la sauvegarde de la texture: " << filePath << std::endl;
        return 1;
    }

    // Générer une particule étoilée
    sf::Image starImage;
    starImage.create(size, size, sf::Color::Transparent);

    // Nombre de branches de l'étoile
    const int numSpikes = 5;
    const float innerRadius = size / 6.0f;
    const float outerRadius = size / 2.5f;

    // Centre de l'image
    const float centerX = size / 2.0f;
    const float centerY = size / 2.0f;

    // Dessiner d'abord un cercle plein au centre
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            // Calculer la distance du centre
            float dx = x - centerX;
            float dy = y - centerY;
            float distance = std::sqrt(dx * dx + dy * dy);

            // Créer un cercle au centre
            if (distance < innerRadius)
            {
                float alpha = 255.0f * (1.0f - distance / innerRadius);
                starImage.setPixel(x, y, sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
            }
        }
    }

    // Dessiner les branches de l'étoile
    for (int i = 0; i < numSpikes; ++i)
    {
        // Angle pour cette branche
        float angle = i * 2.0f * 3.14159f / numSpikes;

        // Coordonnées de la pointe
        float tipX = centerX + outerRadius * std::cos(angle);
        float tipY = centerY + outerRadius * std::sin(angle);

        // Dessiner une ligne du centre à la pointe
        for (float t = 0.0f; t <= 1.0f; t += 0.01f)
        {
            float x = centerX + t * (tipX - centerX);
            float y = centerY + t * (tipY - centerY);

            // Calculer l'épaisseur de la ligne en fonction de la distance
            float thickness = 3.0f * (1.0f - t);

            // Dessiner un petit cercle à cette position
            for (int dy = -static_cast<int>(thickness); dy <= static_cast<int>(thickness); ++dy)
            {
                for (int dx = -static_cast<int>(thickness); dx <= static_cast<int>(thickness); ++dx)
                {
                    int pixelX = static_cast<int>(x) + dx;
                    int pixelY = static_cast<int>(y) + dy;

                    // Vérifier que le pixel est dans l'image
                    if (pixelX >= 0 && pixelX < size && pixelY >= 0 && pixelY < size)
                    {
                        // Calculer la distance du centre de la ligne
                        float distFromLine = std::sqrt(dx * dx + dy * dy);

                        if (distFromLine <= thickness)
                        {
                            // Calculer l'alpha en fonction de la distance de la ligne et de t
                            float alpha = 255.0f * (1.0f - distFromLine / thickness) * (1.0f - t);

                            // Ne pas écraser les pixels déjà plus opaques
                            sf::Color currentColor = starImage.getPixel(pixelX, pixelY);
                            if (currentColor.a < static_cast<sf::Uint8>(alpha))
                            {
                                starImage.setPixel(pixelX, pixelY, sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
                            }
                        }
                    }
                }
            }
        }
    }

    // Sauvegarder l'image étoile
    std::string starFilePath = "../resources/textures/particle_star.png";
    if (starImage.saveToFile(starFilePath))
    {
        std::cout << "Texture de particule étoile générée avec succès: " << starFilePath << std::endl;
    }
    else
    {
        std::cerr << "Erreur lors de la sauvegarde de la texture étoile: " << starFilePath << std::endl;
        return 1;
    }

    // Générer une texture de flamme
    sf::Image flameImage;
    flameImage.create(size, size, sf::Color::Transparent);

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            // Distance horizontale du centre
            float dx = x - size / 2.0f;
            // Distance verticale du bas
            float dy = size - y;

            // Calculer une forme de flamme
            float distFromCenter = std::abs(dx) / (size / 2.0f);

            // Plus proche du centre = plus haut
            float height = 1.0f - distFromCenter * distFromCenter;

            // Limiter la hauteur de la flamme
            float flameHeight = size * 0.9f * height;

            if (dy < flameHeight)
            {
                // Plus haut = plus transparent
                float verticalFade = 1.0f - dy / flameHeight;
                // Plus au bord = plus transparent
                float horizontalFade = 1.0f - distFromCenter;

                float alpha = 255.0f * verticalFade * horizontalFade;
                flameImage.setPixel(x, y, sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
            }
        }
    }

    // Sauvegarder l'image flamme
    std::string flameFilePath = "../resources/textures/particle_flame.png";
    if (flameImage.saveToFile(flameFilePath))
    {
        std::cout << "Texture de flamme générée avec succès: " << flameFilePath << std::endl;
    }
    else
    {
        std::cerr << "Erreur lors de la sauvegarde de la texture flamme: " << flameFilePath << std::endl;
        return 1;
    }

    return 0;
}