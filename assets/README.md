# Assets Directory

This directory contains all the graphics assets for the Chronic Echos SNES game.

## Directory Structure

```
assets/
├── graphics/
│   ├── backgrounds/     # Background tilesets and tilemaps
│   ├── sprites/         # Character and object sprites
│   └── fonts/           # Font graphics for text display
└── palettes/            # (Reserved for future palette files)
```

## File Naming Convention

- **Source files**: `filename.png` - Original PNG images created in graphics editors
- **SNES graphics**: `filename.pic` - Converted graphics data for SNES
- **SNES palettes**: `filename.pal` - Converted palette data for SNES

## Asset Types

### Backgrounds (`graphics/backgrounds/`)

- `tileset.png/pic/pal` - Main tileset with 16 colored 8x8 tiles arranged in a 4x4 grid

### Sprites (`graphics/sprites/`)

- `sprites.png` - Original directional sprite sheet (64x64 with 4 frames)
- `sprites_simple.png/pic/pal` - Processed compass sprite with directional arrows

### Fonts (`graphics/fonts/`)

- `pvsneslibfont.png/pic/pal` - Font graphics for text console display

## Usage

### Converting PNG to SNES Format

Use the Makefile targets to convert PNG files to SNES format:

```bash
# Convert a specific PNG file
make convert PNG=path/to/image.png

# Rebuild all graphics
make rebuild-graphics

# Clean and rebuild everything
make full-rebuild
```

### Adding New Assets

1. Place your PNG file in the appropriate subdirectory
2. Add conversion rules to the Makefile if needed
3. Update `data.asm` to include the new assets
4. Update your C code to reference the new assets

## Notes

- All PNG files should be optimized for SNES limitations (256 colors max, appropriate sizes)
- Generated `.pic` and `.pal` files are automatically created from PNG sources
- Intermediate files (`crop*.png`, `*fixed.png`) are ignored by git
