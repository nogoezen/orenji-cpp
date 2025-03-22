#include "Scene/TitleScene.hpp"
#include <iostream>

namespace Orenji
{
    TitleScene::TitleScene(const std::string &name)
        : Scene(name),
          m_backgroundSprite(nullptr),
          m_titleNode(nullptr),
          m_subtitleNode(nullptr),
          m_selectedItemIndex(0),
          m_textAnimationTimer(0.f),
          m_showInstructions(true)
    {
    }

    TitleScene::~TitleScene()
    {
        // Arrête la musique si elle est en cours de lecture
        if (m_music.getStatus() == sf::Music::Playing)
        {
            m_music.stop();
        }
    }

    bool TitleScene::initialize()
    {
        // Appelle l'initialisation de base
        if (!Scene::initialize())
            return false;

        // Charge la police
        if (!m_font.loadFromFile("resources/fonts/VeniceClassic.ttf"))
        {
            std::cerr << "Erreur: Impossible de charger la police VeniceClassic.ttf" << std::endl;
            return false;
        }

        // Crée le fond
        createBackground("resources/textures/title-bg.png");

        // Crée les textes du menu
        createTexts();

        // Charge la musique
        if (!m_music.openFromFile("resources/sounds/012-Theme02.mid"))
        {
            std::cerr << "Erreur: Impossible de charger la musique 012-Theme02.mid" << std::endl;
            // On continue même si la musique ne charge pas
        }

        // Configure la musique
        m_music.setLoop(true);
        m_music.setVolume(50.f);

        return true;
    }

    void TitleScene::update(float dt)
    {
        // Animation du texte (pulsation)
        m_textAnimationTimer += dt;
        float pulse = (std::sin(m_textAnimationTimer * 2.f) + 1.f) * 0.25f + 0.5f;

        // Applique l'animation au titre
        if (m_titleNode)
        {
            m_titleNode->setScale(1.f, 1.f);
            sf::Color titleColor = m_titleNode->getText().getFillColor();
            titleColor.a = static_cast<sf::Uint8>(255 * pulse);
            m_titleNode->setFillColor(titleColor);
        }

        // Anime l'option sélectionnée
        for (size_t i = 0; i < m_menuItems.size(); ++i)
        {
            if (i == m_selectedItemIndex)
            {
                m_menuItems[i].textNode->setScale(1.05f + pulse * 0.1f, 1.05f + pulse * 0.1f);
                sf::Color color = sf::Color(255, 255, 0); // Jaune
                color.a = 255;
                m_menuItems[i].textNode->setFillColor(color);
                m_menuItems[i].textNode->setOutlineThickness(2.f);
                m_menuItems[i].textNode->setOutlineColor(sf::Color(128, 0, 0, 192)); // Rouge foncé
            }
            else
            {
                m_menuItems[i].textNode->setScale(1.f, 1.f);
                sf::Color color = sf::Color::White;
                color.a = 192;
                m_menuItems[i].textNode->setFillColor(color);
                m_menuItems[i].textNode->setOutlineThickness(0.f);
            }
        }

        // Appelle la mise à jour de base
        Scene::update(dt);
    }

    void TitleScene::onActivate()
    {
        Scene::onActivate();

        // Démarre la musique
        m_music.play();
    }

    void TitleScene::onDeactivate()
    {
        // Arrête la musique
        m_music.stop();

        Scene::onDeactivate();
    }

    void TitleScene::handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                selectPreviousItem();
                break;

            case sf::Keyboard::Down:
                selectNextItem();
                break;

            case sf::Keyboard::Return:
            case sf::Keyboard::Space:
                activateSelectedItem();
                break;

            default:
                break;
            }
        }
    }

    void TitleScene::addMenuItem(const std::string &text, std::function<void()> callback)
    {
        // Crée un nouveau nœud de texte pour l'option
        auto textNode = new TextNode(m_font, text, 30, "MenuItem_" + std::to_string(m_menuItems.size()));
        textNode->setFillColor(sf::Color::White);
        textNode->centerOrigin();

        // Ajoute le nœud à la scène
        addNode(std::unique_ptr<SceneNode>(textNode), "root");

        // Ajoute l'option à la liste
        MenuItem item = {textNode, callback};
        m_menuItems.push_back(item);

        // Met à jour la disposition
        updateLayout();
    }

    void TitleScene::setMainTitle(const std::string &title)
    {
        if (m_titleNode)
        {
            m_titleNode->setText(title);
            m_titleNode->centerOrigin();
            updateLayout();
        }
    }

    void TitleScene::setSubtitle(const std::string &subtitle)
    {
        if (m_subtitleNode)
        {
            m_subtitleNode->setText(subtitle);
            m_subtitleNode->centerOrigin();
            updateLayout();
        }
    }

    void TitleScene::createBackground(const std::string &texturePath)
    {
        // Charge la texture de fond
        if (!m_backgroundTexture.loadFromFile(texturePath))
        {
            std::cerr << "Erreur: Impossible de charger l'image " << texturePath << std::endl;
            return;
        }

        // Crée le sprite
        m_backgroundSprite = new SpriteNode(m_backgroundTexture, "Background");

        // Ajoute le sprite à la scène (en premier pour qu'il soit dessiné en arrière-plan)
        addNode(std::unique_ptr<SceneNode>(m_backgroundSprite), "root");
    }

    void TitleScene::createTexts()
    {
        // Crée le titre principal
        m_titleNode = new TextNode(m_font, "ORENJI", 72, "MainTitle");
        m_titleNode->setFillColor(sf::Color(255, 165, 0));    // Orange
        m_titleNode->setOutlineColor(sf::Color(139, 69, 19)); // Marron
        m_titleNode->setOutlineThickness(3.f);
        m_titleNode->centerOrigin();

        // Crée le sous-titre
        m_subtitleNode = new TextNode(m_font, "Un Jeu Magique", 36, "Subtitle");
        m_subtitleNode->setFillColor(sf::Color(255, 255, 255, 200));
        m_subtitleNode->centerOrigin();

        // Ajoute les nœuds à la scène
        addNode(std::unique_ptr<SceneNode>(m_titleNode), "root");
        addNode(std::unique_ptr<SceneNode>(m_subtitleNode), "root");

        // Crée des options par défaut si aucune n'a été ajoutée
        if (m_menuItems.empty())
        {
            addMenuItem("Nouvelle Partie", []()
                        { std::cout << "Nouvelle partie" << std::endl; });
            addMenuItem("Charger Partie", []()
                        { std::cout << "Charger partie" << std::endl; });
            addMenuItem("Options", []()
                        { std::cout << "Options" << std::endl; });
            addMenuItem("Quitter", []()
                        { std::cout << "Quitter" << std::endl; });
        }

        // Met à jour la disposition
        updateLayout();
    }

    void TitleScene::updateLayout()
    {
        // Position du titre et sous-titre
        sf::Vector2f windowCenter(800.f / 2.f, 600.f / 2.f);

        if (m_titleNode)
        {
            m_titleNode->setPosition(windowCenter.x, windowCenter.y - 150.f);
        }

        if (m_subtitleNode)
        {
            m_subtitleNode->setPosition(windowCenter.x, windowCenter.y - 90.f);
        }

        // Position des options du menu
        float startY = windowCenter.y;
        float itemSpacing = 40.f;

        for (size_t i = 0; i < m_menuItems.size(); ++i)
        {
            m_menuItems[i].textNode->setPosition(windowCenter.x, startY + i * itemSpacing);
        }
    }

    void TitleScene::selectNextItem()
    {
        if (m_menuItems.empty())
            return;

        // Passe à l'option suivante
        m_selectedItemIndex = (m_selectedItemIndex + 1) % m_menuItems.size();
    }

    void TitleScene::selectPreviousItem()
    {
        if (m_menuItems.empty())
            return;

        // Passe à l'option précédente
        m_selectedItemIndex = (m_selectedItemIndex + m_menuItems.size() - 1) % m_menuItems.size();
    }

    void TitleScene::activateSelectedItem()
    {
        if (m_menuItems.empty() || m_selectedItemIndex >= m_menuItems.size())
            return;

        // Exécute la fonction associée à l'option sélectionnée
        if (m_menuItems[m_selectedItemIndex].callback)
        {
            m_menuItems[m_selectedItemIndex].callback();
        }
    }

} // namespace Orenji