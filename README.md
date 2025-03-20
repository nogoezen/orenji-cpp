# Orenji-CPP Game Engine

A 2D game engine built with C++ and SFML.

## Dependencies

### SFML
The Simple and Fast Multimedia Library (SFML) is used for graphics, window management, and input handling.

### Box2D
Box2D is used for physics simulation.

### Thor
Thor extends SFML with higher-level functionality like particle systems, animations, and resources.

### TMXLite
TMXLite is used for loading and parsing Tiled Map Editor files (.tmx).

## Installation

### Install Dependencies

#### Windows with vcpkg
```bash
vcpkg install sfml:x64-windows
vcpkg install box2d:x64-windows
vcpkg install thor:x64-windows
vcpkg install tmxlite:x64-windows
```

#### Linux
```bash
# SFML
sudo apt-get install libsfml-dev

# Box2D
sudo apt-get install libbox2d-dev

# Thor and TMXLite need to be built from source
git clone https://github.com/Bromeon/Thor.git
cd Thor
cmake -DCMAKE_BUILD_TYPE=Release .
make
sudo make install

git clone https://github.com/fallahn/tmxlite.git
cd tmxlite/tmxlite
cmake -DCMAKE_BUILD_TYPE=Release .
make
sudo make install
```

### Build the Engine

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

### TiledMap Module

The TiledMap module provides functionality for loading and rendering maps created with the Tiled Map Editor.

#### TiledMapLoader

The `TiledMapLoader` class provides static methods for loading and managing Tiled maps:

```cpp
// Load a map
auto map = Orenji::TiledMapLoader::load("assets/maps/level1.tmx");

// Check if a map is loaded
bool isLoaded = Orenji::TiledMapLoader::isLoaded("assets/maps/level1.tmx");

// Unload a map to free memory
Orenji::TiledMapLoader::unload("assets/maps/level1.tmx");

// Set the root directory for relative paths
Orenji::TiledMapLoader::setRootDirectory("assets/maps/");
```

#### TiledMap

The `TiledMap` class represents a loaded Tiled map and provides methods for accessing its data:

```cpp
// Access map properties
int width = map->getWidth();
int height = map->getHeight();
int tileWidth = map->getTileWidth();
int tileHeight = map->getTileHeight();

// Get custom properties
std::string prop = map->getProperty("author");
bool isOutdoor = map->getPropertyBool("outdoor");
int difficulty = map->getPropertyInt("difficulty");

// Access layers
const auto& tileLayers = map->getTileLayers();
const auto& objectLayers = map->getObjectLayers();
const auto& imageLayers = map->getImageLayers();
```

## License

[MIT License](LICENSE)
