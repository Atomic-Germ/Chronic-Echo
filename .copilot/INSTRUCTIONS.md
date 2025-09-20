# Chronic Echos Implementation Instructions

This document provides an actionable step-by-step guide for implementing the **Chronic Echos** SNES RPG, based on the design in `.copilot/CHRONIC_ECHOS.md`. It is intended for developers or AI agents responsible for creating the game engine, features, and assets.

---

## 1. Prerequisites

1. Install and configure an SNES development toolchain:
   - pvsneslib (via git submodule from <https://github.com/alekmaul/pvsneslib.git>)
   - Build tools from source (816-tcc, wla-65816, wla-spc700, wlalink)
   - Graphics/audio conversion tools (gfx2snes, gfx4snes, smconv, etc.)
2. Graphics and tile editing tools:
   - YY-CHR or Tile Layer Pro for CHR and tilemap design
   - A palette editor (e.g., Tile Molester)
3. Audio tools:
   - EliteSNESSound or NSRT for SPC music and SFX
   - Audio-to-SPC conversion scripts
4. Emulator for testing on PC:
   - bsnes-plus, higan, or ZMZ Emulator
5. Version control:
   - Git with GitHub repository and submodule support
6. Scripting environment:
   - Python 3.x or Bash/Zsh for build automation

---

## 2. Project Structure

```text
 / (repo root)
├── .copilot/
│   ├── CHRONIC_ECHOS.md         # Original design document
│   ├── INSTRUCTIONS.md          # This file
│   ├── LISTS.md                # Checklists and flowcharts
│   ├── METHODS.md              # Technical methods and tools
│   ├── NOTES.md                # Development progress notes
│   └── QUESTIONS_AND_ANSWERS.md # Q&A for development guidance
├── .github/workflows/
│   └── ci.yml                  # GitHub Actions CI with submodule support
├── pvsneslib/                  # Git submodule for pvsneslib library
│   ├── devkitsnes/             # Compiler and tools (built from source)
│   ├── pvsneslib/              # Library headers and binaries
│   ├── tools/                  # Graphics/audio conversion tools
│   └── compiler/               # Source for building 816-tcc, wla tools
├── src/                        # 65816 assembly and C source
│   └── main.c                  # Main game logic
├── data.asm                    # Data includes (fonts, palettes)
├── hdr.asm                     # ROM header and memory map
├── Makefile                    # Build configuration
├── build/                      # Build output directory
│   └── ChronicEchos.sfc        # Output ROM file
├── scripts/                    # Build and automation scripts
│   ├── build.sh                # Build automation
│   └── validate_rom.sh         # ROM validation
├── README.md                   # Project overview
└── pvsneslibfont.png           # Default font asset
```

---

## 3. Development Phases

### Phase 1: Engine Core

- [ ] Set up and validate the build pipeline to produce a working blank ROM
- [ ] Implement reset vector, CPU init, simple PPU clear and palette load
- [ ] Display a tilemap and allow basic controller movement on screen

### Phase 2: Time Manipulation System

- [ ] Design in-memory buffers to record player state each frame
- [ ] Implement rewind and fast-forward logic with input sampling
- [ ] Create echo playback routines for recorded frames
- [ ] Hook time abilities into the engine (preview, freeze, accelerate)

### Phase 3: Overworld & Puzzles

- [ ] Build era-specific maps and palette sets (Past, Present, Future)
- [ ] Load and unload CHR banks dynamically per era
- [ ] Implement interactive objects that respond to time-shifts (bridges, doors)
- [ ] Create sample environmental puzzles using time echoes

### Phase 4: Combat System

- [ ] Implement turn queue, menu navigation, and basic attack actions
- [ ] Integrate time rewind and echo combo within battles
- [ ] Develop status effects influenced by time manipulation
- [ ] Create a boss battle template requiring advanced time strategies

### Phase 5: Narrative & Dialogue

- [ ] Implement text engine for displaying NPC dialogue and choices
- [ ] Design branching quests that influence future/past states
- [ ] Integrate memory system to persist player choices across eras

### Phase 6: Audio & UI

- [ ] Integrate SPC music tracks and SFX triggers
- [ ] Create HUD for HP, MP, time ability meters, menus
- [ ] Implement save/load state via battery-backed SRAM

### Phase 7: Polish & Testing

- [ ] Playtest each era and combat module for balance
- [ ] Optimize performance and memory usage for real SNES hardware
- [ ] Document asset formats and build steps in `docs/`
- [ ] Prepare final ROM, packaging, and documentation

---

## 4. Technical Guidelines

- **Modular Design**: Keep each system (engine, time, combat) in separate modules and clearly documented.
- **Comments & Docs**: Write assembly comments for all routines; maintain YAML or JSON metadata for assets.
- **Version Control**: Commit early and often; use feature branches per phase/task.
- **Testing**: Use emulator breakpoints and hex editors to verify VRAM writes and game logic.
- **Performance**: Aim for 60 FPS on SNES; minimize CPU cycles per frame.

---

## 5. Deliverables

1. **Working ROM**: `ChronicEchos.sfc`, fully playable with core features.
2. **Source Code**: Well-structured `src/` directory with commented assembly.
3. **Assets**: Organized `assets/` folder with CHR, maps, audio, and fonts.
4. **Build Scripts**: Automated scripts to assemble and package the ROM.
5. **Documentation**: Updated `.copilot/CHRONIC_ECHOS.md`, detailed `docs/` guides, and this `INSTRUCTIONS.md`.

---

Refer back to `.copilot/CHRONIC_ECHOS.md` regularly to ensure fidelity to the original design. Good luck, and may your echoes transcend time!
