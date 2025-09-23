---
applyTo: '**'
description: Workspace-specific AI memory for this project
lastOptimized: '2025-09-23T19:10:03.134221+00:00'
entryCount: 2
optimizationVersion: 1
autoOptimize: true
sizeThreshold: 50000
entryThreshold: 20
timeThreshold: 7
---
# Workspace AI Memory
This file contains workspace-specific information for AI conversations.

## Personal Context
*(No entries yet)*

## Professional Context
*(No entries yet)*

## Technical Preferences
- **2025-09-23 12:09:** Chronic-Echo is a SNES time manipulation RPG built with pvsneslib v4.3.0, outputting a LoROM ChronicEchos.sfc. Main code in src/ (C), assets in assets/, build config in Makefile. Key components: main.c (screen state machine), sprites.c/h (player sprite system), data.asm (graphics includes).

## Communication Preferences
*(No entries yet)*

## Universal Laws
*(No entries yet)*

## Policies
*(No entries yet)*

## Suggestions/Hints
*(No entries yet)*

## Memories/Facts
- **2025-09-23 12:09:** Chronic-Echo project structure and technical details recorded.- **2025-09-23 12:10:** Build workflow: 'make deps && make' to install pvsneslib and build ROM. Run with 'make run' (snes_test emulator) or 'open -a Snes9x build/ChronicEchos.sfc'. Graphics: 'make convert PNG=path/to/image.png' for single conversion, 'make rebuild-graphics' for all. Validate ROM with './validate_rom.sh build/ChronicEchos.sfc'.
- **2025-09-23 12:10:** Asset conventions: Source PNGs in assets/graphics/subdirs, converted to .pic (graphics) and .pal (palette) files. PNGs must be SNES-compatible (≤256 colors, correct dimensions). Intermediate files (*.crop.png, *fixed.png) ignored by git. When adding assets: place PNG, update Makefile rules, data.asm includes, and C code references.
- **2025-09-23 12:10:** Testing: Manual/visual with Mesen2 emulator, automated Lua scripts in tests/ using Mesen2-SDL3 API for memory, graphics, and input validation. CI: GitHub Actions on push/PR to main/develop branches, builds on macOS, runs tests, uploads ChronicEchos.sfc artifact.
- **2025-09-23 12:10:** Development guidelines: TDD with unit tests first, red-green-refactor cycle. High unit test coverage (>80%) for critical components. Modular design, clear naming, documentation. Reusable code with DRY principle. Follow established coding conventions. Code reviews required. Incremental development.
- **2025-09-23 12:12:** Mesen2-SDL3 API enhancements: Hardware-specific assertions (assertVramEquals, assertOamEquals, assertInputState, assertInputPressed), bulk memory operations (readVramBlock, readOamBlock, readWramBlock), enhanced input polling (pollInput), frame-based testing with event callbacks. Provides comprehensive SNES testing tools with direct hardware access, inspired by pvsneslib patterns.
- **2025-09-23 12:12:** Mesen2-SDL3 API reference: Core functions (read/write 8/16/32-bit, screen buffer access, input control), hardware-specific functions (VRAM/OAM/WRAM bulk operations, input assertions), testing framework (logTest, assertEqual, assertTrue, frame-based callbacks), event system (frameEnd/frameStart/reset events), execution control (reset/stop/step). Memory types: cpu, vram, oam, cgram, spcr, spram.
- **2025-09-23 12:12:** Mesen2-SDL3 testing best practices: Frame-based testing with event callbacks for precise timing, modular test organization with setup/test/cleanup phases, comprehensive memory testing (WRAM integrity, bulk validation), graphics validation (VRAM tiles, screen capture), input testing (polling, state assertions), performance benchmarking, debugging helpers (memory dumps, profiling), automated test runners with result aggregation.
- **2025-09-23 12:17:** PVSnesLib v4.4.0 APIs: Background management (bgInitTileSet, bgInitMapSet, bgSetScroll, bgSetGfxPtr), Sprite system (oamInit, oamSet, oamUpdate, oamDynamic* for VRAM management), Input handling (padsCurrent, padsDown, padsUp, KEYPAD_BITS enum), Console text (consoleInitText, consoleDrawText, consoleSetText*), Video control (setMode, setBrightness), Audio (spcLoad, spcPlay), DMA transfers (dmaCopyVram), System functions. Comprehensive SNES SDK used in Chronic-Echo.
- **2025-09-23 12:17:** Key PVSnesLib functions used in Chronic-Echo: bgInitTileSet (load BG graphics), bgInitMapSet (load tilemaps), bgSetScroll (BG positioning), oamInitGfxSet (load sprite graphics), oamSet (position sprites), oamUpdate (transfer to OAM), consoleInitText (initialize text system), consoleDrawText (print text), padsCurrent (read controller input), setMode/setBrightness (video setup), dmaCopyVram (VRAM transfers).
- **2025-09-23 12:36:** Refined TDD roadmap incorporating developer feedback: Phase 1 (Player & Time Systems) with accessibility (high contrast, audio cues), Phase 2 (Battle with Time Mechanics) isolating logic from rendering, Phase 3 (World & Progression) with thorough serialization testing, Phase 4 (Integration) with hardware testing. Emphasize logic isolation, frame-based testing, and accessibility for SNES limitations.
- **2025-09-23 13:49:** Chronic Echo test execution: Use 'make test' to run all Lua test scripts in tests/ directory using Mesen2-SDL3 emulator. Individual tests can be run with './snes_test lua build/ChronicEchos.sfc tests/test_name.lua'. Tests validate ROM loading, memory operations, game mechanics, and system integration.
