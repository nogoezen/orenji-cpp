# Orenji-CPP

A 2D game engine written in C++ using SFML 3 and Box2D.

## Prerequisites

- MinGW C++ compiler (g++)
- CMake 3.10 or higher
- SFML 3.0 (included in lib/sfml)
- Box2D 3.0 (must be downloaded separately)

## Setting up Box2D

Before building the project, you need to set up Box2D:

1. Download Box2D 3.0 from [the official GitHub repository](https://github.com/erincatto/box2d/releases)
2. Extract it to the `lib/box2d` directory
3. The directory structure should look like:
   ```
   lib/
   ├── box2d/
   │   ├── include/
   │   │   └── box2d/
   │   │       └── box2d.h
   │   ├── src/
   │   └── CMakeLists.txt
   ```

## Building the Project

You can build the project using the provided build script:

```
.\build.bat
```

The script will:
1. Check for required tools (CMake, g++)
2. Build Box2D if it's available
3. Configure the project with CMake
4. Build the main project
5. Copy required resources
6. Build example applications

If you have issues:

- Make sure CMake and g++ are in your PATH
- Make sure Box2D is correctly set up in the lib/box2d directory
- Check that SFML libraries are in the lib/sfml directory

## Running Examples

After a successful build, you can run the examples from the build directory:

```
cd build/bin/examples
.\CameraExample.exe
.\ParticleDemo.exe
.\GameMapExample.exe  # Requires Box2D
```

## Project Structure

- `include/`: Header files
- `src/`: Source files
- `examples/`: Example applications
- `lib/`: External libraries
  - `sfml/`: SFML 3.0 libraries
  - `box2d/`: Box2D physics library
- `resources/`: Game resources (textures, sounds, etc.)
- `build/`: Build outputs

## Features

- Entity-Component System
- Physics simulation using Box2D
- Tiled map loading and rendering
- Particle effects system
- Camera controls with effects
- Resource management

## Contributing

Feel free to contribute to the project by submitting pull requests or reporting issues.

## License

This project is licensed under the MIT License - see the LICENSE file for details. 