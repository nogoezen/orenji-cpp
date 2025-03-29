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