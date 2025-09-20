# Chronic Echos - SNES Game Development

A time manipulation RPG for the Super Nintendo Entertainment System, built using pvsneslib.



## Quick Start

### Prerequisites
- macOS with Homebrew
- SNES9x emulator (installed via `brew install --cask snes9x`)

### Build the ROM
```bash
# Clone or navigate to the project directory
cd ./TestNES

# Run the build script
./scripts/build.sh

# Or build manually
export PVSNESLIB_HOME="$(pwd)/pvsneslib"
export PATH="$PATH:$PVSNESLIB_HOME/devkitsnes/tools:$PVSNESLIB_HOME/devkitsnes/bin"
make
```

### Test the ROM
```bash
# Quick test with make run (requires SNES9x)
make run

# Or open manually
open -a Snes9x build/ChronicEchos.sfc
```

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

### Local CI Validation

```bash
# Run the same validation as CI
./scripts/validate_rom.sh build/ChronicEchos.sfc
```

### CI Status

[![CI](https://github.com/Atomic-Germ/Echos/actions/workflows/ci.yml/badge.svg)](https://github.com/Atomic-Germ/Echos/actions/workflows/ci.yml)

## Project Structure

```
TestNES/
├── pvsneslib/          # pvsneslib library and tools (v4.3.0)
├── src/main.c          # Main C source file
├── data.asm            # Data includes (fonts, graphics)
├── hdr.asm             # ROM header and memory configuration
├── Makefile            # Build configuration
├── scripts/
│   └── build.sh        # Automated build script
├── assets/             # Graphics, audio, and other assets
├── docs/               # Documentation
├── .copilot/           # Design documents and development notes
└── ChronicEchos.sfc    # Output ROM file (262KB LoROM)
```

## Development Workflow

1. **Setup Environment**: Set PVSNESLIB_HOME and PATH variables
2. **Edit Source**: Modify `src/main.c` and other source files
3. **Build**: Run `make` to compile C to assembly to ROM
4. **Test**: Open ROM in SNES9x emulator
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
