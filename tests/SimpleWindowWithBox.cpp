#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    // Create an SFML window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Moving Box Test");
    window.setFramerateLimit(60);

    // Create a box shape
    sf::RectangleShape box({50, 50});
    box.setFillColor(sf::Color::Red);
    box.setPosition({375, 275}); // Center of the window

    // Box velocity
    sf::Vector2f velocity(100.0f, 80.0f); // pixels per second

    // Clock for delta time calculation
    sf::Clock clock;

    // Main loop
    while (window.isOpen())
    {
        // Calculate delta time
        float deltaTime = clock.restart().asSeconds();

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
                // Reset box position when Space is pressed
                else if (keyEvent->code == sf::Keyboard::Key::Space)
                {
                    box.setPosition({375, 275});
                }
            }
        }

        // Update box position
        sf::Vector2f position = box.getPosition();
        position += velocity * deltaTime;

        // Simple collision detection with window boundaries
        if (position.x < 0 || position.x + 50 > 800)
        {
            velocity.x = -velocity.x;
            position.x = (position.x < 0) ? 0 : 800 - 50;
        }
        if (position.y < 0 || position.y + 50 > 600)
        {
            velocity.y = -velocity.y;
            position.y = (position.y < 0) ? 0 : 600 - 50;
        }

        // Update the box position
        box.setPosition(position);

        // Clear the window
        window.clear(sf::Color(50, 50, 100));

        // Draw the box
        window.draw(box);

        // Display the window contents
        window.display();
    }

    return 0;
}