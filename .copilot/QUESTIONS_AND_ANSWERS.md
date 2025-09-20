# Development Knowledge Base

This document serves as a living knowledge base for Chronic Echos development decisions, guidelines, and frequently asked questions. It's organized by topic for easy reference during development.

---

## 🚀 Quick Reference

### Current Status

- **Phase**: 1 (Engine Core) ✅ Complete
- **Build System**: pvsneslib 4.4.0 via git submodule
- **Target**: SNES emulator (Mednafen/RetroArch preferred)
- **Output**: 256KB LoROM format ROM

### Key Decisions Made

- ✅ Git submodule approach for pvsneslib dependency management
- ✅ Build-from-source toolchain for consistency
- ✅ Bottom-up development approach (foundations first)
- ✅ Emulator-first development (cartridge compatibility secondary)

---

## 🛠️ Technical Setup & Environment

### Toolchain & Dependencies

#### Toolchain Installation

- **Answer**: pvsneslib via git submodule with build-from-source tools (816-tcc, wla-65816, etc.)

#### Emulator Selection

- **Answer**: Mednafen/RetroArch preferred for debugging. No strict requirements - use what provides best debugging output.

#### Python Environment

- **Answer**: Always use virtual environment. Create and maintain `requirements.txt` as needed.

#### Git Repository Structure

- **Answer**: Repository initialized with current structure. Use feature branches and PRs for major changes.

---

## 🎯 Project Scope & Development Strategy

### Development Approach

#### Implementation Strategy

- **Answer**: Bottom-up approach: complete foundational pieces with full tests before moving up. Prioritize easier tasks for simpler debugging.

#### Scope & Goals

- **Answer**: Full implementation is the target. Prototype complex mechanics first to validate feasibility.

#### Hardware Compatibility

- **Answer**: Emulator-first development. Cartridge compatibility is nice-to-have, not required.

#### Code Quality

- **Answer**: Modular, maintainable code. Single developer but prepare for future collaboration or your future self.

---

## 🎨 Assets & Resources

### Asset Strategy

#### Existing Assets

- **Answer**: None exist. Use creativity and placeholders. Art assets will be developed last.

#### Asset Creation Tools

- **Answer**: Prioritize command-line and programmable tools for automation and consistency.

#### Sample Content

- **Answer**: Use known sample asset libraries when available, otherwise generate procedural assets for development phases.

---

## ⚔️ Gameplay & Design Implementation

### Core Mechanics

#### Time Manipulation Constraints

- **Answer**: Experimental phase - no strict limits. SNES emulator compatibility is the only hard constraint.

#### Narrative Scope

- **Answer**: Start with "canon" storyline, build system for branching quests and alternate timelines.

#### Combat Balance

- **Answer**: Use functional numbers initially, tune for fun once mechanics work.

#### Memory/Performance

- **Answer**: Optimize for SNES limitations. Prioritize efficiency but don't over-optimize prematurely.

---

## 🔄 Development Workflow & Testing

### Build & Automation

#### Build Scripts

- **Answer**: Create early, update often. Current: `make` for ROM, `scripts/validate_rom.sh` for testing.

#### Testing Strategy

- **Answer**: GitHub CI with automated tests. Use `make test` for unit tests. Manual playtesting on request.

#### Version Control

- **Answer**: Feature branches + PRs for major changes. Group checklist completions in commits.

#### Documentation

- **Answer**: Maintain all `.copilot/` files actively. Update NOTES.md, check off LISTS.md items, add to METHODS.md.

---

## 📅 Timeline & Milestones

### Project Timeline

#### Deadlines

- **Answer**: No strict deadlines. Focus on quality over speed.

#### Feedback Cycle

- **Answer**: Request feedback when next decisions would change based on input.

---

## 🔧 Git Submodule Management

### Submodule Workflow

#### Updates & Conflicts

- **Answer**: `git submodule update --remote --merge` for updates. Resolve conflicts in submodule directory, commit reference in main repo.

#### CI Performance

- **Answer**: Recursive checkout clones full pvsneslib + submodules. Consider caching for production builds.

#### Build Optimization

- **Answer**: Build-from-source ensures consistency. Cache tools for CI optimization if needed.

#### Version Pinning

- **Answer**: Track master during development for bug fixes. Pin to commits for production stability.

#### Environment Consistency

- **Answer**: Submodule commit hash ensures version consistency. Document build process and consider version validation scripts.

---

## 📝 Adding New Entries

When adding questions/answers to this file:

1. **Categorize**: Place under appropriate section header
2. **Format**: Use proper headings and bullet points for clarity
3. **Context**: Include date or phase when decision was made
4. **References**: Link to related files or sections when relevant

**Template:**

```markdown
#### [Question Title]

- **Answer**: [Clear, actionable answer with context]
```
