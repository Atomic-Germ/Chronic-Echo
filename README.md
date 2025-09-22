# Chronic Echos - SNES Game Development

A time manipulation RPG for the Super Nintendo Entertainment System, built using pvsneslib.

[![CI Workflow](https://github.com/Atomic-Germ/Chronic-Echo/actions/workflows/ci.yml/badge.svg)](https://github.com/Atomic-Germ/Chronic-Echo/actions/workflows/ci.yml)



## Quick Start

### Prerequisites
- macOS with Homebrew
- SNES emulator (SNES9x: `brew install --cask snes9x` or Mesen)

### Build the ROM
```bash
# Clone or navigate to the project directory
cd Chronic-Echo

# Install dependencies and build
make deps && make
```

### Test the ROM

```bash
# Test with Mesen emulator (installed by make deps)
make run

# Or open manually with SNES9x
open -a Snes9x build/ChronicEchos.sfc

# Or open manually with Mesen
open -a Mesen build/ChronicEchos.sfc

# Validate ROM integrity
./validate_rom.sh build/ChronicEchos.sfc
```

### Build Testing

The project includes comprehensive automated build tests to ensure build system reliability:

```bash
# Run the full build test suite (cleans, builds, and validates)
make test-build

# Or run the test script directly
./test_build.sh
```

The test suite validates:

- Clean build environment
- Dependency installation
- Successful compilation and linking
- ROM size and content validation
- Build reproducibility
- Clean artifact removal
- Incremental build functionality
- Debug build support

## Continuous Integration

This project uses GitHub Actions for automated building and testing.

### CI Pipeline

- **Triggers**: Push and pull requests to `main` branch
- **Environment**: Ubuntu Linux with build tools
- **Build Process**:
  - Sets up pvsneslib environment
  - Compiles C code to 65816 assembly
  - Links and generates SNES ROM
  - Validates ROM integrity and size
- **Testing**: Attempts headless ROM testing with available emulators
- **Artifacts**: Built ROM and symbols uploaded for download

### CI Status

[![CI](https://github.com/Atomic-Germ/Echos/actions/workflows/ci.yml/badge.svg)](https://github.com/Atomic-Germ/Echos/actions/workflows/ci.yml)

## Project Structure

```text
Chronic-Echo/
├── pvsneslib/          # pvsneslib library and tools (v4.3.0)
├── src/
│   ├── main.c          # Main C source file
│   └── sprites.c       # Sprite management system
├── data.asm            # Data includes (fonts, graphics)
├── hdr.asm             # ROM header and memory configuration
├── Makefile            # Build configuration
├── assets/             # Graphics, audio, and other assets
├── build/              # Build output directory
├── docs/               # Documentation
├── .copilot/           # Design documents and development notes
└── ChronicEchos.sfc    # Output ROM file (262KB LoROM)
```

## Troubleshooting

### Build Issues

**Problem**: `make: *** No rule to make target 'cleanBuildRes'. Stop.`
**Solution**: This occurs when `PVSNESLIB_HOME` is not set correctly. Run `make deps` first to install pvsneslib, then `make`.

**Problem**: Build works in CI but fails locally
**Solution**: The Makefile was updated to use local paths instead of environment variables. Ensure you're using the latest Makefile from the repository.

**Problem**: Duplicate label warnings during linking
**Solution**: These warnings are expected with pvsneslib and harmless. The build will complete successfully.

### Emulator Issues

**Problem**: ROM doesn't load in emulator
**Solution**: Ensure you're using a compatible SNES emulator like SNES9x or Mesen. The ROM is built as LoROM format.

**Problem**: No sound or graphics
**Solution**: Check that your emulator settings are correct for SNES games. Try different video filters if graphics appear corrupted.

### Development Setup

**Problem**: Tools not found in PATH
**Solution**: Run `./setup_path.sh` after `make deps` to add pvsneslib tools to your PATH, or add the following to your `~/.zshrc`:

```bash
export PATH="/path/to/Chronic-Echo/pvsneslib/devkitsnes/bin:$PATH"
export PATH="/path/to/Chronic-Echo/pvsneslib/devkitsnes/tools:$PATH"
```

## Development Workflow

1. **Setup Environment**: Run `make deps` to install pvsneslib and tools
2. **Edit Source**: Modify `src/main.c`, `src/sprites.c` and other source files
3. **Build**: Run `make` to compile C to assembly to ROM
4. **Test**: Run `make run` to test in emulator
5. **Iterate**: Repeat steps 2-4

## Toolchain

- **pvsneslib 4.3.0**: C library for SNES development
- **816-tcc**: C compiler targeting 65816 assembly
- **wla-65816**: 65816 assembler
- **wlalink**: ROM linker
- **SNES9x**: Emulator for testing

## Current Status

✅ **Phase 1: Engine Core** - Basic title screen working

- Build pipeline established
- PPU initialization and text rendering
- Simple title screen with "CHRONIC ECHOS"

🔄 **Next Steps**

- Add input handling for navigation
- Implement game state management
- Begin time manipulation mechanics

## Documentation

See `.copilot/` directory for detailed design documents:

- `CHRONIC_ECHOS.md` - Game design and features
- `INSTRUCTIONS.md` - Development guidelines
- `LISTS.md` - Checklists and flowcharts
- `METHODS.md` - Tools and techniques
- `NOTES.md` - Development progress
- `QUESTIONS_AND_ANSWERS.md` - Q&A for development decisions

## License

This project is part of the Chronic Echos game development effort.
