# Chronic Echos Methods & Notes

This file captures useful debugging techniques, design methods, and tools discovered during development of **Chronic Echos**. Add entries here as you go.

## Build Process

- Set PVSNESLIB_HOME environment variable to the pvsneslib submodule directory
- Add pvsneslib tools and bin directories to PATH
- Use `make` to build the ROM from source files
- ROM output: build/ChronicEchos.sfc (262KB LoROM format)
- Build process: C → 816-tcc → assembly → wla-65816 → wlalink → ROM

## Toolchain Setup

- pvsneslib 4.4.0 (C library for SNES development, git submodule)
- 816-tcc compiler for C to assembly conversion (built from source)
- wla-65816 assembler for 65816 assembly (built from source)
- wla-spc700 assembler for SPC700 audio (built from source)
- wlalink linker for creating SNES ROMs (built from source)
- Additional tools: gfx2snes, gfx4snes, smconv, bin2txt, constify (all built from source)
- SNES9x emulator for testing

## Project Structure

```bash
/Users/caseyjparker/Echos/
├── pvsneslib/              # pvsneslib git submodule
│   ├── devkitsnes/         # Compiler and tools
│   ├── pvsneslib/          # Library headers and binaries
│   └── tools/              # Graphics/audio conversion tools
├── src/main.c              # Main C source file
├── data.asm                # Data includes (fonts, etc.)
├── hdr.asm                 # ROM header and memory map
├── Makefile                # Build configuration
├── build/ChronicEchos.sfc  # Output ROM file
├── scripts/                # Build and validation scripts
│   ├── build.sh
│   └── validate_rom.sh
└── .copilot/               # Design documentation
    ├── CHRONIC_ECHOS.md    # Game design document
    ├── INSTRUCTIONS.md     # Implementation guide
    ├── LISTS.md           # Checklists and flowcharts
    ├── METHODS.md         # Technical methods and tools
    ├── NOTES.md           # Development progress
    └── QUESTIONS_AND_ANSWERS.md
```

## Git Submodule Workflow

- Initialize submodules: `git submodule update --init --recursive`
- Update submodules: `git submodule update --remote --merge`
- CI integration: Use `submodules: recursive` in GitHub Actions checkout
- Build tools from source when submodule is updated

## Debugging Techniques

- Monitor VRAM writes using emulator debugger (bsnes-plus/higan).
- Use breakpoints on DMA transfers to track tile loading.
- Inspect CPU cycle counts with performance timer routines.
- Log PPU scroll and palette changes to SRAM for offline analysis.

## Design Patterns

- Modular assembly includes for engine, time, combat, and UI.
- Double-buffered state recording for time rewind stability.
- Metadata sections in asset headers for automated import scripts.

## Tools & Utilities

- `nsrt` for validating SPC audio files.
- `md5` checksums for ROM integrity verification.
- Automated QA script (`scripts/qa_checks.sh`) for static asset tests.

## Performance Profiling

- Frame cycle budget measurement using custom `cyclecounter` routine.
- Optimize tight loops to be page-boundary friendly on the 65816.

## Future Notes

- Add any future debugging or design notes here.
