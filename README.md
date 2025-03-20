# Orenji Game Engine

Orenji is a lightweight C++ game engine built on top of SFML, designed for creating 2D games with a clean entity-component-system architecture.

## Features

- **Entity-Component-System Architecture**: Flexible architecture for game objects and behaviors
- **State Management**: Game state machine for managing different game screens and modes
- **Resource Management**: Centralized resource loading and caching for textures, fonts, and sounds
- **Input Handling**: Comprehensive input management with key/mouse binding support
- **Rendering System**: Component-based rendering with proper layering

## Requirements

- C++17 compatible compiler (GCC, Clang, MSVC)
- SFML 2.5 or newer
- CMake 3.10 or newer (for CMake build)

## Building the Project

### Using CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Using build.bat (Windows)

On Windows, you can simply run the included build.bat script:

```
build.bat
```

## Project Structure

- **include/**: Header files
  - **Core/**: Core systems (Entity, Component, System)
  - **States/**: Game state management
  - **Utilities/**: Helper utilities (Resource management, Input handling)
- **src/**: Implementation files
- **resources/**: Game resources
  - **fonts/**: Font files
  - **textures/**: Texture files
  - **sounds/**: Sound files

## Architecture Overview

### Entity-Component-System

The engine follows the Entity-Component-System pattern:
- **Entity**: A game object with a unique ID
- **Component**: Data containers attached to entities
- **System**: Logic processors that operate on entities with specific components

### State Machine

Game flow is managed through a state machine:
- **State**: Base class for game states (MainMenu, Game, Pause, etc.)
- **StateMachine**: Manages state transitions and updates

## License

This project is provided under the MIT License. See LICENSE file for details.

## Credits

Orenji Game Engine was created by [Your Name] and uses SFML, a free multimedia C++ API.
