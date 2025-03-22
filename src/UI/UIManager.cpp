#include "../../include/UI/UIManager.hpp"
#include <iostream>
#include <variant>

namespace UI
{

    UIManager::UIManager(sf::RenderWindow &window)
        : m_window(window)
    {
        std::cout << "UIManager created" << std::endl;
    }

    UIManager::~UIManager()
    {
        std::cout << "UIManager destroyed" << std::endl;
    }

    void UIManager::init()
    {
        std::cout << "UIManager initialized" << std::endl;

        // Dans une vraie implémentation, nous initialiserions TGUI ici
        // et chargerions les thèmes et widgets
    }

    void UIManager::update(float deltaTime)
    {
        // Mise à jour des éléments d'interface utilisateur
        // Animations, effets, etc.
    }

    void UIManager::render()
    {
        // Dessin des éléments d'interface utilisateur
        // Dans une vraie implémentation avec TGUI :
        // m_gui.draw();
    }

    bool UIManager::handleEvent(const sf::Event &event)
    {
        // Gestion des événements de l'interface utilisateur
        // Retourne true si l'événement a été traité

        // Exemple simple - détecte si un clic est sur une zone d'UI
        if (event.is<sf::Event::MouseButtonPressed>())
        {
            // Vérifier si le clic est sur un élément d'UI
            // Si oui, traiter et retourner true
            return false;
        }

        return false;
    }

} // namespace UI