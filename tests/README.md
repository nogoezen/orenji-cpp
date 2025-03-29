# Test Instructions

## SimplePhysicsTest

This test demonstrates the Box2D physics engine with collision filtering using the new `b2BodyId` system. It creates several bodies with different collision categories and shows how they interact:

- A static ground platform
- A dynamic player that collides with platforms and enemies
- A dynamic enemy that collides with platforms and the player
- A static sensor that detects overlaps without generating physical collisions
- A projectile that only collides with enemies

### Prerequisites

Before running the test, you need to build the Box2D library:

1. Navigate to the lib/box2d directory
2. Run CMake to configure and build the library:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
   ```
3. Copy the built library files to lib/box2d/lib:
   ```bash
   mkdir -p ../lib
   cp bin/Release/box2d.* ../lib/
   ```

### Compiling and Running

To compile the test:

```bash
g++ -o build/bin/physics-test.exe tests/SimplePhysicsTest.cpp -I./include -I./lib/sfml/include -I./lib/box2d/include -L./lib/sfml/lib -L./lib/box2d/lib -lsfml-graphics -lsfml-window -lsfml-system -lbox2d
```

Run the test:

```bash
./build/bin/physics-test.exe
```

### Controls

- Left Arrow: Apply force to the player to the left
- Right Arrow: Apply force to the player to the right
- Up Arrow: Apply upward force to the player
- Escape: Close the application

This demo shows:
1. How to set up a Box2D physics simulation using the new modern Box2D API
2. How to implement collision filtering with categories and masks
3. How to create different types of bodies (static, dynamic)
4. How to visualize collision shapes 

## GameplayEntitiesTest

This test demonstrates the Player and Enemy classes with physics and AI integration. It sets up a simple scene with:

- A player character that can move left/right and jump
- An enemy that uses a behavior tree for patrolling and player detection
- A ground platform for character movement

### Prerequisites

This test requires the same Box2D library setup as the SimplePhysicsTest (see above).
It also requires the BehaviorTree.CPP library to be properly built and linked.

### Compiling and Running

To compile the test:

```bash
g++ -o build/bin/gameplay-test.exe tests/GameplayEntitiesTest.cpp src/Gameplay/Player.cpp src/Gameplay/Enemy.cpp -I./include -I./lib/sfml/include -I./lib/box2d/include -I./lib/behaviortree_cpp/include -L./lib/sfml/lib -L./lib/box2d/lib -L./lib/behaviortree_cpp/lib -lsfml-graphics -lsfml-window -lsfml-system -lbox2d -lbehaviortree_cpp
```

Run the test:

```bash
./build/bin/gameplay-test.exe
```

### Controls

- A/Left Arrow: Move player left
- D/Right Arrow: Move player right
- Space/W/Up Arrow: Jump
- Escape: Close the application

This demo shows:
1. How to create and use the Player and Enemy classes
2. Integration of Box2D physics with game entities
3. Behavior tree-based AI for enemy movement
4. Entity component system with physics and graphics components 

## TGUITest

This test demonstrates the TGUI integration in the engine, showcasing a complete UI system with multiple screens and interactive elements:

- Main menu with buttons for navigation
- Pause menu that can be toggled with ESC key during gameplay
- Property editor interface for modifying game object properties
- Theme system for applying consistent styling to UI elements

### Prerequisites

This test requires the TGUI library to be properly built and linked. Make sure that:

- TGUI is installed in the `lib/tgui` directory
- SFML is installed and working (TGUI depends on SFML)

### Compiling and Running

To compile the test:

```bash
g++ -o build/bin/tgui-test.exe tests/TGUITest.cpp src/UI/UIManager.cpp src/UI/PauseMenu.cpp src/UI/EditorForm.cpp -I./include -I./lib/sfml/include -I./lib/tgui/include -L./lib/sfml/lib -L./lib/tgui/lib -lsfml-graphics -lsfml-window -lsfml-system -ltgui
```

Run the test:

```bash
./build/bin/tgui-test.exe
```

### Features Demonstrated

1. **UIManager**: Central manager for all UI forms and themes
2. **Forms and Widgets**: Management of different UI screens (main menu, pause menu, editor)
3. **Event Handling**: Processing SFML events and dispatching them to the UI
4. **Properties Editor**: Dynamic property editing interface for game objects
5. **Theming**: Applying TGUI themes to style the interface consistently
6. **Callbacks**: Event-driven programming through widget callbacks

### Controls

- Left click: Interact with UI elements
- ESC: Toggle the pause menu (during gameplay)
- Various buttons for navigating between different UI screens 

## ResourceManagerTest

This test demonstrates the enhanced ResourceManager functionality for SFML 3, including:

### Prerequisites
- SFML 3 library (Graphics and Audio modules)
- Resource files (textures, fonts, sounds, music)

### Compiling and Running
```bash
g++ -std=c++17 -o ResourceManagerTest tests/ResourceManagerTest.cpp src/Resources/ResourceManager.cpp -I./include -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
```

### Features Demonstrated
- Resource path management with configurable base directories
- Texture loading with smoothing and repeating options
- Sprite sheet creation and animation
- Font loading and text rendering
- Sound and music loading and playback
- Resource availability checking
- Exception handling for missing resources

### Controls
- **Space**: Play jump sound effect
- **Escape**: Exit the application

### Resource Structure
The test expects resources to be organized in the following structure:
```
resources/
  ├── textures/
  │     ├── characters/
  │     │     └── player.png
  │     └── tilesets/
  │           └── tileset.png
  ├── fonts/
  │     └── main.ttf
  ├── sounds/
  │     └── jump.wav
  └── music/
        └── background.ogg
```

If the resources aren't found, the test will display error messages indicating which files are missing and where it was looking for them. 