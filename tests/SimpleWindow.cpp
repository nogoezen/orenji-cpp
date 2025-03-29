#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    // Create an SFML window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML 3 Window Test");
    window.setFramerateLimit(60);

    // Main loop
    while (window.isOpen())
    {
        // Handle events
        while (const auto event = window.pollEvent())
        {
            // Close window when closed button is clicked
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Close window when Escape key is pressed
            if (const auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
        }

        // Clear the window
        window.clear(sf::Color(50, 50, 100));

        // Display the window contents
        window.display();
    }

    return 0;
}