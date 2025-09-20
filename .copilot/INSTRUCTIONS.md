# Chronic Echos Implementation Instructions

This document provides an actionable step-by-step guide for implementing the **Chronic Echos** SNES RPG, based on the design in `.copilot/CHRONIC_ECHOS.md`. It is intended for developers or AI agents responsible for creating the game engine, features, and assets.

---

## 1. Prerequisites

1. Install and configure an SNES development toolchain:
   - WLA-DX (WLA-65816 assembler)
   - Spred or SDASM for linking and ROM building
2. Graphics and tile editing tools:
   - YY-CHR or Tile Layer Pro for CHR and tilemap design
   - A palette editor (e.g., Tile Molester)
3. Audio tools:
   - EliteSNESSound or NSRT for SPC music and SFX
   - Audio-to-SPC conversion scripts
4. Emulator for testing on PC:
   - bsnes-plus, higan, or ZMZ Emulator
5. Version control:
   - Git with a GitHub or GitLab repository
6. Scripting environment (optional, for build automation):
   - Python 3.x or Bash/Zsh

---

## 2. Project Structure

```text
 / (repo root)
├── .copilot/
│   ├── CHRONIC_ECHOS.md         # Original design document
│   └── INSTRUCTIONS.md          # This file
├── src/                        # 65816 assembly source
│   ├── reset.asm               # Reset vector and startup
│   ├── ppu_init.asm            # PPU and VRAM routines
│   ├── engine/                 # Core engine modules
│   │   ├── input.asm
│   │   ├── rendering.asm
│   │   └── memory.asm
│   ├── time/                   # Time manipulation system
│   │   ├── record.asm
│   │   ├── rewind.asm
│   │   └── echo.asm
│   ├── combat/                 # Turn-based and echo combos
│   │   ├── battle_flow.asm
│   │   └── turn_menu.asm
│   ├── exploration/            # Overworld, puzzles, era transitions
│   └── dialog.asm              # NPC and story dialogues
├── assets/
│   ├── graphics/
│   │   ├── tiles/              # CHR banks and tilemaps
│   │   └── sprites/            # Character and echo sprites
│   ├── audio/
│   │   ├── music/              # SPC files
│   │   └── sfx/                # Sound effects
│   └── fonts/                  # Bitmap font data
├── scripts/                    # Build and automation scripts
│   ├── build_rom.sh            # Assemble/link and produce .sfc
│   └── convert_audio.py        # SPC conversion helper
├── docs/                       # Design docs, asset guides
│   └── art_guidelines.md
└── README.md                   # High-level project info
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
