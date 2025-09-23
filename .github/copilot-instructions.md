# Copilot Instructions for Chronic Echos SNES Game

## Project Overview
- This is a time manipulation RPG for SNES, built with pvsneslib (v4.3.0).
- Main code is in `src/` (C), with assets in `assets/` and build configuration in `Makefile`.
- Output is a LoROM SNES ROM (`ChronicEchos.sfc`).

## Key Workflows
- **Build:**
  - Run `make deps && make` to install dependencies and build the ROM.
  - Use `make run` to launch the ROM in snes_test.
  - Manual emulator launch: `open -a Snes9x build/ChronicEchos.sfc` or `./snes_test build/ChronicEchos.sfc`.
- **Validate ROM:**
  - Run `./validate_rom.sh build/ChronicEchos.sfc` to check ROM integrity.
- **Graphics Conversion:**
  - Convert PNG assets: `make convert PNG=path/to/image.png`.
  - Rebuild all graphics: `make rebuild-graphics`.
  - Add new assets: place PNG in correct subdir, update Makefile, `data.asm`, and C code references.

## Architecture & Patterns
- **Main C files:**
  - `src/main.c`: Game logic entry point.
  - `src/sprites.c/h`: Sprite management system.
- **Data includes:**
  - `data.asm`: Fonts and graphics data for SNES.
  - `hdr.asm`: ROM header and memory config.
- **Assets:**
  - `assets/graphics/backgrounds/`: Tilesets.
  - `assets/graphics/sprites/`: Sprite sheets.
  - `assets/graphics/fonts/`: Font graphics.
  - `.pic` and `.pal` files are generated from PNGs via Makefile rules.

## Conventions
- Asset filenames: `filename.png` (source), `filename.pic` (SNES graphics), `filename.pal` (palette).
- All PNGs must be SNES-compatible (max 256 colors, correct sizes).
- Intermediate files (`crop*.png`, `*fixed.png`) are ignored by git.
- Update `data.asm` and C code when adding new assets.

## Troubleshooting
- If build fails with `cleanBuildRes` error, run `make deps` first.
- If tools are missing, run `./setup_path.sh` after `make deps` or update your PATH.
- Duplicate label warnings during linking are harmless.
- Use compatible emulators (SNES9x, snes_test) with LoROM settings.

## CI/CD
- GitHub Actions build and test on push/PR to `main`.
- CI sets up pvsneslib, builds, validates ROM, and uploads artifacts.

---
For unclear workflows or missing conventions, consult `README.md`, `assets/README.md`, or ask maintainers for guidance.

## AI Agent Tool Usage

**Memory Management:**
Always use the "Remember" tool to store, update, and recall user/project preferences, standards, and facts throughout every coding session. Persist relevant context and decisions for future work.

**Reflective Problem Solving:**
Continuously use the "Sequential Thinking" tool for step-by-step analysis, planning, and solution generation. Apply chain-of-thought reasoning for complex or ambiguous tasks, and document the reasoning process in code comments or commit messages when appropriate.

> These tools should be used as a default part of your workflow—activate them for every non-trivial request, design decision, or when context may be reused or revisited.
