#include "../../include/Particles/ParticleSystem.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace Orenji
{
    // Initialisation du membre statique
    std::unique_ptr<ParticleSystem> ParticleSystem::s_instance = nullptr;

    ParticleSystem::ParticleSystem()
    {
        // Initialiser quelques templates prédéfinis

        // Feu
        ParticleTemplate fire;
        fire.emissionRate = 40.f;
        fire.burstSize = 15;
        fire.lifetime = 2.0f;
        fire.lifetimeDeviation = 0.5f;
        fire.color = sf::Color(255, 100, 0, 200);
        fire.colorEnd = sf::Color(100, 0, 0, 0);
        fire.size = 12.f;
        fire.sizeEnd = 3.f;
        fire.velocity = sf::Vector2f(0.f, -40.f);
        fire.velocityDeviation = 10.f;
        fire.acceleration = sf::Vector2f(0.f, -5.f);
        fire.texturePath = "assets/textures/particles/fire.png";
        registerTemplate("fire", fire);

        // Fumée
        ParticleTemplate smoke;
        smoke.emissionRate = 10.f;
        smoke.burstSize = 5;
        smoke.lifetime = 4.0f;
        smoke.lifetimeDeviation = 1.f;
        smoke.color = sf::Color(200, 200, 200, 100);
        smoke.colorEnd = sf::Color(100, 100, 100, 0);
        smoke.size = 15.f;
        smoke.sizeEnd = 30.f;
        smoke.velocity = sf::Vector2f(0.f, -20.f);
        smoke.velocityDeviation = 5.f;
        smoke.acceleration = sf::Vector2f(0.f, -2.f);
        smoke.angularVelocity = 2.f;
        smoke.angularVelocityDeviation = 1.f;
        smoke.texturePath = "assets/textures/particles/smoke.png";
        registerTemplate("smoke", smoke);

        // Étincelles
        ParticleTemplate sparks;
        sparks.emissionRate = 30.f;
        sparks.burstSize = 20;
        sparks.lifetime = 1.0f;
        sparks.lifetimeDeviation = 0.4f;
        sparks.color = sf::Color(255, 255, 0, 255);
        sparks.colorEnd = sf::Color(255, 0, 0, 0);
        sparks.size = 3.f;
        sparks.sizeEnd = 1.f;
        sparks.velocity = sf::Vector2f(0.f, -70.f);
        sparks.velocityDeviation = 40.f;
        sparks.acceleration = sf::Vector2f(0.f, 40.f);
        sparks.texturePath = "assets/textures/particles/spark.png";
        registerTemplate("sparks", sparks);
    }

    ParticleSystem &ParticleSystem::getInstance()
    {
        if (!s_instance)
        {
            s_instance = std::make_unique<ParticleSystem>();
        }
        return *s_instance;
    }

    bool ParticleSystem::loadFromFile(const std::string &name, const std::string &filename)
    {
        try
        {
            // Ouvrir le fichier
            std::ifstream file(filename);
            if (!file.is_open())
            {
                std::cerr << "Erreur: Impossible d'ouvrir le fichier de particules: " << filename << std::endl;
                return false;
            }

            // Charger le JSON
            nlohmann::json json;
            file >> json;

            // Créer un template
            ParticleTemplate templ;

            // Paramètres d'émission
            if (json.contains("emissionRate"))
                templ.emissionRate = json["emissionRate"].get<float>();
            if (json.contains("burstSize"))
                templ.burstSize = json["burstSize"].get<unsigned int>();
            if (json.contains("lifetime"))
                templ.lifetime = json["lifetime"].get<float>();
            if (json.contains("lifetimeDeviation"))
                templ.lifetimeDeviation = json["lifetimeDeviation"].get<float>();

            // Paramètres visuels
            if (json.contains("color"))
            {
                auto &c = json["color"];
                templ.color = sf::Color(
                    c[0].get<int>(),
                    c[1].get<int>(),
                    c[2].get<int>(),
                    c.size() > 3 ? c[3].get<int>() : 255);
            }

            if (json.contains("colorEnd"))
            {
                auto &c = json["colorEnd"];
                templ.colorEnd = sf::Color(
                    c[0].get<int>(),
                    c[1].get<int>(),
                    c[2].get<int>(),
                    c.size() > 3 ? c[3].get<int>() : 255);
            }

            if (json.contains("size"))
                templ.size = json["size"].get<float>();
            if (json.contains("sizeEnd"))
                templ.sizeEnd = json["sizeEnd"].get<float>();
            if (json.contains("rotation"))
                templ.rotation = json["rotation"].get<float>();
            if (json.contains("rotationEnd"))
                templ.rotationEnd = json["rotationEnd"].get<float>();

            // Paramètres de mouvement
            if (json.contains("velocity"))
            {
                auto &v = json["velocity"];
                templ.velocity = sf::Vector2f(v[0].get<float>(), v[1].get<float>());
            }

            if (json.contains("velocityDeviation"))
                templ.velocityDeviation = json["velocityDeviation"].get<float>();

            if (json.contains("acceleration"))
            {
                auto &a = json["acceleration"];
                templ.acceleration = sf::Vector2f(a[0].get<float>(), a[1].get<float>());
            }

            if (json.contains("angularVelocity"))
                templ.angularVelocity = json["angularVelocity"].get<float>();
            if (json.contains("angularVelocityDeviation"))
                templ.angularVelocityDeviation = json["angularVelocityDeviation"].get<float>();

            // Texture
            if (json.contains("texturePath"))
                templ.texturePath = json["texturePath"].get<std::string>();

            // Enregistrer le template
            registerTemplate(name, templ);

            return true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Erreur lors du chargement du template de particules: " << e.what() << std::endl;
            return false;
        }
    }

    void ParticleSystem::registerTemplate(const std::string &name, const ParticleTemplate &templ)
    {
        m_templates[name] = templ;

        // Précharger la texture si un chemin est spécifié
        if (!templ.texturePath.empty())
        {
            auto texture = std::make_shared<sf::Texture>();
            if (texture->loadFromFile(templ.texturePath))
            {
                m_textureCache.add(templ.texturePath, texture);
            }
            else
            {
                std::cerr << "Erreur: Impossible de charger la texture: " << templ.texturePath << std::endl;
            }
        }
    }

    const ParticleSystem::ParticleTemplate *ParticleSystem::getTemplate(const std::string &name) const
    {
        auto it = m_templates.find(name);
        if (it != m_templates.end())
        {
            return &(it->second);
        }
        return nullptr;
    }

    bool ParticleSystem::applyTemplate(thor::UniversalEmitter &emitter, const std::string &templateName)
    {
        const ParticleTemplate *templ = getTemplate(templateName);
        if (!templ)
        {
            std::cerr << "Erreur: Template de particules non trouvé: " << templateName << std::endl;
            return false;
        }

        // Configurer l'émetteur
        emitter.setEmissionRate(templ->emissionRate);
        emitter.setParticleLifetime(thor::Distributions::uniform(
            templ->lifetime - templ->lifetimeDeviation,
            templ->lifetime + templ->lifetimeDeviation));

        // Configurer la vitesse
        emitter.setParticleVelocity(thor::Distributions::deflect(
            templ->velocity,
            templ->velocityDeviation));

        // Configurer la position (0, 0 par défaut, sera ajustée par le composant)
        emitter.setParticlePosition(thor::Distributions::circle(sf::Vector2f(0.f, 0.f), 2.f));

        // Configurer la rotation
        emitter.setParticleRotation(thor::Distributions::uniform(
            templ->rotation - 10.f,
            templ->rotation + 10.f));

        // Configurer la vitesse angulaire
        emitter.setParticleRotationSpeed(thor::Distributions::uniform(
            templ->angularVelocity - templ->angularVelocityDeviation,
            templ->angularVelocity + templ->angularVelocityDeviation));

        // Configurer la taille des particules
        emitter.setParticleScale(sf::Vector2f(templ->size, templ->size));

        // Configurer la couleur
        emitter.setParticleColor(templ->color);

        return true;
    }

    bool ParticleSystem::setupParticleSystem(thor::ParticleSystem &system, thor::UniversalEmitter &emitter, const std::string &templateName)
    {
        const ParticleTemplate *templ = getTemplate(templateName);
        if (!templ)
        {
            std::cerr << "Erreur: Template de particules non trouvé: " << templateName << std::endl;
            return false;
        }

        // Appliquer le template à l'émetteur
        if (!applyTemplate(emitter, templateName))
            return false;

        // Configurer les affecteurs

        // Couleur qui change avec le temps
        system.addAffector(thor::ColorAffector(templ->colorEnd));

        // Taille qui change avec le temps
        system.addAffector(thor::ScaleAffector(sf::Vector2f(
            templ->sizeEnd / templ->size,
            templ->sizeEnd / templ->size)));

        // Accélération constante
        system.addAffector(thor::ForceAffector(templ->acceleration));

        // Texture
        if (!templ->texturePath.empty())
        {
            // Vérifier si la texture est dans le cache
            auto texture = m_textureCache.get(templ->texturePath);
            if (!texture)
            {
                // Si non, essayer de la charger
                texture = std::make_shared<sf::Texture>();
                if (texture->loadFromFile(templ->texturePath))
                {
                    m_textureCache.add(templ->texturePath, texture);
                }
                else
                {
                    std::cerr << "Erreur: Impossible de charger la texture: " << templ->texturePath << std::endl;
                }
            }

            if (texture)
            {
                system.setTexture(*texture);
            }
        }

        return true;
    }

    void ParticleSystem::preloadTextures()
    {
        for (const auto &pair : m_templates)
        {
            const auto &templ = pair.second;
            if (!templ.texturePath.empty() && !m_textureCache.contains(templ.texturePath))
            {
                auto texture = std::make_shared<sf::Texture>();
                if (texture->loadFromFile(templ.texturePath))
                {
                    m_textureCache.add(templ.texturePath, texture);
                }
                else
                {
                    std::cerr << "Erreur: Impossible de précharger la texture: " << templ.texturePath << std::endl;
                }
            }
        }
    }

} // namespace Orenji