# Changements d'API dans SFML 3.0

Ce document détaille les principales modifications de l'API dans SFML 3.0 par rapport à SFML 2.x. Il sert de référence pour mettre à jour votre code.

## 1. Gestion des vecteurs et rectangles

### `sf::Vector2f` et autres vecteurs

**SFML 2.x**:
```cpp
sf::Vector2f position(10.0f, 20.0f);
position.x = 30.0f;
position.y = 40.0f;
```

**SFML 3.0**:
```cpp
sf::Vector2f position({10.0f, 20.0f});
position.x = 30.0f;
position.y = 40.0f;
```

### `sf::FloatRect` et autres rectangles

**SFML 2.x**:
```cpp
sf::FloatRect rect(10.0f, 20.0f, 30.0f, 40.0f);
float left = rect.left;
float top = rect.top;
float width = rect.width;
float height = rect.height;
```

**SFML 3.0**:
```cpp
sf::FloatRect rect({10.0f, 20.0f}, {30.0f, 40.0f});
float left = rect.position.x;
float top = rect.position.y;
float width = rect.size.x;
float height = rect.size.y;
```

### Intersections de rectangles

**SFML 2.x**:
```cpp
sf::FloatRect rect1(0, 0, 10, 10);
sf::FloatRect rect2(5, 5, 10, 10);
sf::FloatRect intersection;
if (rect1.intersects(rect2, intersection)) {
    // utiliser l'intersection
}
```

**SFML 3.0**:
```cpp
sf::FloatRect rect1({0, 0}, {10, 10});
sf::FloatRect rect2({5, 5}, {10, 10});
if (auto intersection = rect1.findIntersection(rect2)) {
    // utiliser intersection.value()
}
```

## 2. Fenêtres et rendu

### Création de fenêtres

**SFML 2.x**:
```cpp
sf::VideoMode mode(800, 600);
sf::RenderWindow window(mode, "SFML Window");
```

**SFML 3.0**:
```cpp
sf::VideoMode mode({800, 600});
sf::RenderWindow window(mode, "SFML Window");
```

### Définition de la vue

**SFML 2.x**:
```cpp
sf::View view(sf::FloatRect(0, 0, 800, 600));
view.setCenter(400, 300);
view.setSize(800, 600);
```

**SFML 3.0**:
```cpp
sf::View view(sf::FloatRect({0, 0}, {800, 600}));
view.setCenter({400, 300});
view.setSize({800, 600});
```

## 3. Événements

### Gestion des événements

**SFML 2.x**:
```cpp
sf::Event event;
while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        window.close();
    }
}
```

**SFML 3.0**:
```cpp
if (auto event = window.pollEvent()) {
    if (event->type == sf::EventType::Closed) {
        window.close();
    }
    if (event->type == sf::EventType::KeyPressed && event->key.code == sf::Keyboard::Key::Escape) {
        window.close();
    }
}
```

### Types d'événements

**SFML 2.x**:
```cpp
if (event.type == sf::Event::MouseButtonPressed) {
    // ...
}
```

**SFML 3.0**:
```cpp
if (event->type == sf::EventType::MouseButtonPressed) {
    // ...
}
```

## 4. Textures et sprites

### Création de textures

**SFML 2.x**:
```cpp
sf::Texture texture;
texture.loadFromFile("texture.png");
```

**SFML 3.0**:
```cpp
sf::Texture texture;
texture.loadFromFile("texture.png");  // Inchangé
```

### Création de sprites

**SFML 2.x**:
```cpp
sf::Sprite sprite;
sprite.setTexture(texture);
sprite.setTextureRect(sf::IntRect(10, 10, 50, 30));
```

**SFML 3.0**:
```cpp
sf::Sprite sprite(texture);  // Texture obligatoire
sprite.setTextureRect(sf::IntRect({10, 10}, {50, 30}));
```

### Modification de la couleur

**SFML 2.x**:
```cpp
sprite.setColor(sf::Color(255, 128, 0, 200));
```

**SFML 3.0**:
```cpp
sprite.setColor(sf::Color(255, 128, 0, 200));  // Inchangé
```

## 5. Texte et police

### Chargement des polices

**SFML 2.x**:
```cpp
sf::Font font;
font.loadFromFile("arial.ttf");
```

**SFML 3.0**:
```cpp
sf::Font font;
font.loadFromFile("arial.ttf");  // Inchangé
```

### Création de texte

**SFML 2.x**:
```cpp
sf::Text text("Hello SFML", font, 24);
text.setPosition(100, 100);
```

**SFML 3.0**:
```cpp
sf::Text text("Hello SFML", font, 24);  // Inchangé
text.setPosition({100, 100});
```

### Récupération des limites du texte

**SFML 2.x**:
```cpp
sf::FloatRect bounds = text.getLocalBounds();
text.setOrigin(bounds.width / 2, bounds.height / 2);
```

**SFML 3.0**:
```cpp
sf::FloatRect bounds = text.getLocalBounds();
text.setOrigin({bounds.size.x / 2, bounds.size.y / 2});
```

## 6. Entrées

### Clavier

**SFML 2.x**:
```cpp
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
    // ...
}
```

**SFML 3.0**:
```cpp
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
    // ...
}
```

### Souris

**SFML 2.x**:
```cpp
if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    sf::Vector2i position = sf::Mouse::getPosition(window);
}
```

**SFML 3.0**:
```cpp
if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    sf::Vector2i position = sf::Mouse::getPosition(window);
}
```

## 7. Audio

### Chargement des sons

**SFML 2.x**:
```cpp
sf::SoundBuffer buffer;
buffer.loadFromFile("sound.wav");

sf::Sound sound;
sound.setBuffer(buffer);
```

**SFML 3.0**:
```cpp
sf::SoundBuffer buffer;
buffer.loadFromFile("sound.wav");

sf::Sound sound(buffer);  // Buffer obligatoire
```

### Musique

**SFML 2.x**:
```cpp
sf::Music music;
music.openFromFile("music.ogg");
music.play();
```

**SFML 3.0**:
```cpp
sf::Music music;
music.openFromFile("music.ogg");  // Inchangé
music.play();
```

## 8. Transformations

### Création de transformations

**SFML 2.x**:
```cpp
sf::Transform transform;
transform.translate(10, 20);
transform.rotate(45);
transform.scale(2, 2);
```

**SFML 3.0**:
```cpp
sf::Transform transform;
transform.translate({10, 20});
transform.rotate(45);
transform.scale({2, 2});
```

## 9. Types de données

### Changements de types

**SFML 2.x**:
```cpp
sf::Uint8 alpha = 255;
sf::Int32 largeNumber = 1000000;
```

**SFML 3.0**:
```cpp
uint8_t alpha = 255;     // Types C++ standard
int32_t largeNumber = 1000000;
```

## 10. Réseau

### Création de sockets

**SFML 2.x**:
```cpp
sf::TcpSocket socket;
socket.connect("localhost", 53000);
```

**SFML 3.0**:
```cpp
sf::TcpSocket socket;
socket.connect("localhost", 53000);  // Inchangé
```

## Conseils de migration

1. Commencez par mettre à jour les structures fondamentales comme `sf::Vector2f` et `sf::FloatRect`
2. Adaptez ensuite la gestion des événements qui a changé significativement
3. Mettez à jour toutes les initialisations de sprites et textures qui nécessitent maintenant des paramètres explicites
4. Ajustez les utilisations de `setPosition`, `setSize` et autres méthodes similaires pour utiliser des `sf::Vector2f` avec la nouvelle syntaxe
5. Modifiez les accès aux propriétés des rectangles en passant par `position` et `size`
6. Remplacez les types `sf::Uint8` et similaires par les équivalents C++ standard

## Notes supplémentaires

- SFML 3.0 est incompatible au niveau binaire avec SFML 2.x
- SFML 3.0 requiert un compilateur supportant C++17 ou supérieur
- Certaines fonctionnalités ont été supprimées, comme le module réseau qui est désormais optionnel 