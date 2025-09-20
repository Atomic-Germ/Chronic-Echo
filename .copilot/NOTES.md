# Development Notes

## Initial Setup (September 19, 2025)

- Successfully set up pvsneslib 4.3.0 toolchain for macOS
- Created basic project structure with src/, assets/, scripts/ directories
- Implemented simple title screen displaying "CHRONIC ECHOS" and subtitle
- Build pipeline working: C source -> assembly -> ROM (ChronicEchos.sfc)
- Tested with SNES9x emulator - title screen displays correctly
- ROM size: 262KB LoROM format

## Build Commands

```bash
# Set environment (run in each terminal session)
export PVSNESLIB_HOME=/Users/caseyjparker/TestNES/pvsneslib
export PATH=$PATH:$PVSNESLIB_HOME/devkitsnes/tools:$PVSNESLIB_HOME/devkitsnes/bin

# Build the ROM
make

# Clean build files
make clean

# Test in emulator
open -a Snes9x ChronicEchos.sfc
```

## Next Steps

- Add input handling for title screen navigation
- Implement basic game loop structure
- Add more graphics and assets
- Begin time manipulation system development
