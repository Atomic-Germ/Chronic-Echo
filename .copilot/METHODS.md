# Chronic Echos Methods & Notes

This file captures useful debugging techniques, design methods, and tools discovered during development of **Chronic Echos**. Add entries here as you go.

## Build Process

- Set PVSNESLIB_HOME environment variable to the pvsneslib installation directory
- Add pvsneslib tools and bin directories to PATH
- Use `make` to build the ROM from source files
- ROM output: ChronicEchos.sfc (262KB LoROM format)

## Toolchain Setup

- pvsneslib 4.3.0 (C library for SNES development)
- 816-tcc compiler for C to assembly conversion
- wla-65816 assembler for 65816 assembly
- wlalink linker for creating SNES ROMs
- SNES9x emulator for testing

## Project Structure

```bash
/Users/caseyjparker/TestNES/
├── pvsneslib/          # pvsneslib library and tools
├── src/main.c          # Main C source file
├── data.asm            # Data includes (fonts, etc.)
├── hdr.asm             # ROM header and memory map
├── Makefile            # Build configuration
├── ChronicEchos.sfc    # Output ROM file
└── .copilot/           # Design documentation
```

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
