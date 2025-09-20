# Development Notes

## Initial Setup (September 19, 2025)

- Successfully set up pvsneslib 4.3.0 toolchain for macOS
- Created basic project structure with src/, assets/, scripts/ directories
- Implemented simple title screen displaying "CHRONIC ECHOS" and subtitle
- Build pipeline working: C source -> assembly -> ROM (ChronicEchos.sfc)
- Tested with SNES9x emulator - title screen displays correctly
- ROM size: 262KB LoROM format

## Git Submodule Conversion (September 20, 2025)

- Successfully converted pvsneslib from direct repository inclusion to git submodule
- Added pvsneslib as submodule from <https://github.com/alekmaul/pvsneslib.git>
- Updated GitHub Actions CI workflow to handle submodules recursively
- Modified Makefile to correctly set PVSNESLIB_HOME for submodule structure
- Built pvsneslib tools from source (816-tcc, wla-65816, wla-spc700, wlalink, etc.)
- Compiled pvsneslib library with all necessary components
- Fixed console function calls (consoleSetTextVramBGAdr/consoleSetTextVramAdr → consoleSetTextMapPtr/consoleSetTextGfxPtr)
- ROM validation passed with 99% content density
- Build system now fully functional with proper dependency management

## Build Commands

```bash
# Set environment (run in each terminal session)
export PVSNESLIB_HOME=/Users/caseyjparker/Echos/pvsneslib
export PATH=$PATH:$PVSNESLIB_HOME/devkitsnes/tools:$PVSNESLIB_HOME/devkitsnes/bin

# Build the ROM
make

# Clean build files
make clean

# Test in emulator
open -a Snes9x build/ChronicEchos.sfc
```

## Current Project Status

- ✅ Git submodule setup complete
- ✅ CI workflow updated for submodules
- ✅ Build pipeline working
- ✅ Basic title screen implemented
- ✅ ROM generation and validation working
- 🔄 Ready for Phase 2: Time Manipulation System development

## Next Steps

- Add input handling for title screen navigation
- Implement basic game loop structure
- Begin time manipulation system development (record/rewind/echo mechanics)
- Add more graphics and assets
- Implement overworld exploration and era transitions
