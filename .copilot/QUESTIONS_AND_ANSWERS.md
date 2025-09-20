# Developers Questions and Answers

## Technical Setup & Environment

Q: 
- Toolchain Installation: Do you have the SNES development toolchain (WLA-DX assembler, Spred/SDASM linker, etc.) already installed and configured on your system? If not, should I set it up first?
A: 
- You will have to set up the toolchains first.

Q: 
- Emulator Preference: Which emulator should I use for testing (bsnes-plus, higan, or ZMZ)? Do you have a preference based on debugging features?
A:
- There is not a specific correct emulator, but we tend to use the one that offers the most actionable debugging output when possible.

Q:
- Python Environment: For build scripts and asset conversion, should I use a virtual environment, and are there specific Python packages (e.g., for audio conversion) already listed in a requirements.txt?
A:
- For any python command, always use a venv. There is not requirements.txt to begin with, but you will write one and maintain it as you go.

Q:
- Git Repository: Is the repo already initialized with the basic structure from INSTRUCTIONS.md, or should I create it from scratch?
A:
- There is no structure in the repo yet, it's just blank aside from your own files.

## Project Scope & Priorities

Q:
- Starting Phase: Which development phase should I begin with (e.g., Phase 1: Engine Core, or jump to Phase 2: Time Manipulation)? Should I implement everything sequentially or focus on a specific feature first?
A:
- You should take on tasks from the bottom up, and make sure one piece is completed and has full tests before moving to the next. If there are two equally low level tasks, the easier one should be prioritized to make debugging simpler in early stages.

Q:
- Full Implementation vs. Prototype: Are we aiming for a complete, playable ROM or a proof-of-concept prototype of the core time manipulation mechanics?
A:
- The full impementation is our goal. The first step in doing that will be by prototyping pieces at a time. The most complex mechanics will require proofs before we commit to them.

Q:
- Target Hardware: Since the design mentions running on actual SNES hardware, do you have access to a flash cart or dev kit for testing, or should we focus on emulator-only development for now?
A:
- We aren't going to worry too much about perfect compatibility, so we can do things that might break on a cartridge if we must. Ideally, the rom will work everywhere, but we know it will live in an emulator realistically.

Q:
- Single Developer Constraints: As noted in the design, this is scoped for a single developer—should I prioritize modular, maintainable code to allow for future collaboration?
A:
- While you are to be the sole developer, the code needs to be easily maintainable to you; or to another version of you.

## Assets & Resources

Q: 
- Existing Assets: Do any graphics (CHR banks, sprites, tilemaps), audio (SPC files, SFX), or fonts already exist in the assets/ folder, or do I need to create placeholder assets first?
A:
- We have nothing at all! Please, use your creativity. Arti will be the last work to happen fully, so placeholders will be necessary all the time.

Q:
- Asset Creation Tools: For graphics/audio, do you have preferred tools (e.g., YY-CHR for tiles, EliteSNESSound for audio), or should I use the ones mentioned in the instructions?
A:
- The best tools in this case are the ones you can most easily and completely use from a commandline and programmatically. It's not important exactly which tools you choose.

Q:
- Sample Content: Should I create minimal sample assets (e.g., a basic tilemap, one music track) to get the engine running, or do you have specific assets ready?
A:
- If there is a library of sample assets known to you, they can be used. Otherwise, generated assets will do for the development phases.

## Gameplay & Design Clarifications

Q:
- Core Mechanics Details: For time manipulation (e.g., rewind buffer size, echo entity limits), are there specific technical constraints or balance numbers I should follow from the design doc?
A:
- We assume the balance and actual playability will be all over the map at first; this is all very experimental. The only real technical constraints are that it needs to run in an SNES emulator; I often test with Mednafen and I usually end up playing on retroarch.

Q:
- Story/Narrative Scope: How much of the branching quests and multiple endings should I implement initially—core dialogue system only, or full narrative integration?
A:
- The narrative will be very tightly tied to the way quests branch, but we should initially work out the "canon" storyline and create a system for adding sidequests and alternate timelines, etc.

Q:
- Combat Balance: For turn-based combat and echo combos, do you have specific stat formulas, damage calculations, or boss battle templates I should reference?
A:
- This is something that we're going to have to figure out by trying to play it. Use numbers and formulas that don't seem to actually _break_ anything, and then we can tune it for actual fun once it functions.

Q:
- Memory/Performance Limits: Given SNES hardware constraints (e.g., 128KB WRAM), should I prioritize memory-efficient implementations for time recording?
A:
- You're limited to SNES, including enhancement chips. Everything that we can do more efficiently definitely should be done more efficiently. Often there are situations when we might have much higher degrees of accuracy than we need to, or other optimization challenges.

## Development Workflow & Testing

Q:
- Build Automation: Should I create the build scripts (build_rom.sh, convert_audio.py) immediately, or start with manual assembly first?
A:
- Create the build scripts early and update them often

Q:
- Testing Strategy: How should I handle testing—automated scripts for ROM validation, manual playtesting in emulator, or both? Any specific test cases for time mechanics?
A:
- We'll be pushing the code to github and running the CI. Because of this, we need to write scripts for unit and coverage tests. We'd like to use CodeCov, which might take some creative solutions for making the lcov files. You should be using a `Makefile` so that you can run `make test` for automated tests. Actual play testing in emulators will be done at your request.

Q:
- Version Control: Should I use feature branches for each phase (as suggested), and do you want GitHub PRs for each major change?
A:
- You should use feature branches as suggested, and PRs for big changes. Make sure that when steps of a checklist are completed, the commits are grouped together the best you can.

Q:
- Documentation Updates: As I implement, should I update the .copilot files (e.g., add notes to NOTES.md or check off items in LISTS.md)?
A:
- Yes! You should maintain all of the files in .copilot, adding notes to NOTES.md, check things off (and add things to sometimes) LISTS.md, place repeatedly useful snippits in METHODS.md, add information for tools and toolkits in TOOLS.md, and even add questions to this file.

## Timeline & Milestones
Q:
- Deadlines: Is there a target completion date or milestone (e.g., working ROM by end of Phase 1)?
A:
- We don't actually have any!

Q:
- Iterative Feedback: How often should I check in for feedback—after each phase, or more frequently for core features?
A:
- Look for feedback whenever your next decision would be changed by having it. I will also be supervising, so you can ask questions.

Q:
- Potential Roadblocks: Are there any known challenges (e.g., complex 65816 assembly optimizations) I should prepare for?
A:
- This is a very ambitious project, and I can't foresee anything _but_ roadblocks. Be prepared for all.