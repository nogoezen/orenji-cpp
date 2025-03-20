# Orenji-CPP Examples

This directory contains examples demonstrating how to use the Orenji-CPP game engine.

## TiledMap Example

This example demonstrates how to load and display a Tiled map using the `TiledMap` and `TiledMapLoader` classes.

### Building the Example

```bash
mkdir build
cd build
cmake ..
make
```

### Running the Example

```bash
./TiledMapExample
```

### What the Example Demonstrates

- Setting up the TiledMapLoader with a root directory
- Loading a TMX map file
- Accessing map properties (width, height, custom properties)
- Accessing tile layers and rendering them
- Simple camera controls (arrow keys to move, mouse wheel to zoom)

### Requirements

- SFML 2.5 or newer
- TMXLite library (optional - stub headers are provided if not installed)

### Note About the Example Map

The example requires a tileset image named `example_tileset.png` in the `assets/maps` directory. You can create a simple 320x320 image with different colored tiles, or download a free tileset from sources like [OpenGameArt.org](https://opengameart.org/).

If you're using the provided example.tmx file, it expects tiles to be arranged in a 10x10 grid, with each tile being 32x32 pixels. 