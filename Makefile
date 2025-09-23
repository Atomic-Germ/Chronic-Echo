# Always use local pvsneslib installation
PVSNESLIB_HOME = $(CURDIR)/pvsneslib

# Export PATH to include PVSnesLib tools if dependencies are installed
ifneq ($(wildcard ${PVSNESLIB_HOME}/devkitsnes/snes_rules),)
export PATH := ${PVSNESLIB_HOME}/devkitsnes/bin:${PVSNESLIB_HOME}/devkitsnes/tools:$(PATH)
export PVSNESLIB_DEBUG=1
endif

# Only include snes_rules if it exists (after deps are installed)
ifneq ($(wildcard ${PVSNESLIB_HOME}/devkitsnes/snes_rules),)
include ${PVSNESLIB_HOME}/devkitsnes/snes_rules
endif

# Override assembler rule to include pvsneslib headers
%.obj: %.asm
	@echo Doing obj files ... $(notdir $<)
	$(AS) -I$(PVSNESLIB_HOME)/devkitsnes/include -d -s -x -o $@ $<

.PHONY: bitmaps all run clean deps check-deps

#---------------------------------------------------------------------------------
# Check if dependencies are installed
check-deps:
	@if [ ! -d "pvsneslib" ]; then \
		echo "Error: PVSnesLib not found. Run 'make deps' first."; \
		exit 1; \
	fi
	@if [ ! -f "pvsneslib/devkitsnes/snes_rules" ]; then \
		echo "Error: PVSnesLib installation incomplete. Run 'make deps' first."; \
		exit 1; \
	fi

#---------------------------------------------------------------------------------
# Build directory setup
export BUILD_DIR := build
export ROMNAME := ChronicEchos

#---------------------------------------------------------------------------------
# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

#---------------------------------------------------------------------------------
# ROMNAME is used in snes_rules file
all: check-deps bitmaps $(BUILD_DIR) $(BUILD_DIR)/$(ROMNAME).sfc

validate: check-deps $(BUILD_DIR)/$(ROMNAME).sfc
	@echo "Validating $(ROMNAME).sfc..."
	./scripts/validate_rom.sh $(BUILD_DIR)/$(ROMNAME).sfc

deps: 
	@echo "Setting up dependencies..."
	@if [ ! -d "pvsneslib" ]; then \
		echo "Downloading PVSnesLib..."; \
		wget https://github.com/alekmaul/pvsneslib/releases/download/4.4.0/pvsneslib_440_64b_darwin.zip -O pvsneslib.zip || curl -L https://github.com/alekmaul/pvsneslib/releases/download/4.4.0/pvsneslib_440_64b_darwin.zip -o pvsneslib.zip; \
		unzip pvsneslib.zip; \
		chmod +x pvsneslib/devkitsnes/bin/*; \
		chmod +x pvsneslib/devkitsnes/tools/*; \
		rm pvsneslib.zip; \
		echo "PVSnesLib installed successfully!"; \
	else \
		echo "PVSnesLib already installed."; \
	fi
	@if [ ! -f "snes_test" ]; then \
		OS=$$(uname -s); \
		ARCH=$$(uname -m); \
		if [ "$$OS" = "Darwin" ]; then \
			OS_NAME="macos"; \
		elif [ "$$OS" = "Linux" ]; then \
			OS_NAME="linux"; \
		else \
			echo "Unsupported OS: $$OS"; \
			exit 1; \
		fi; \
		if [ "$$ARCH" = "x86_64" ] || [ "$$ARCH" = "amd64" ]; then \
			ARCH_NAME="x64"; \
		elif [ "$$ARCH" = "arm64" ] || [ "$$ARCH" = "aarch64" ]; then \
			ARCH_NAME="arm64"; \
		else \
			echo "Unsupported arch: $$ARCH"; \
			exit 1; \
		fi; \
		URL="https://github.com/Atomic-Germ/Mesen2-SDL3/releases/download/v1.0.1-dev/snes-test-suite-$${OS_NAME}-$${ARCH_NAME}.tar.gz"; \
		echo "Downloading snes_test for $${OS_NAME}-$${ARCH_NAME}..."; \
		wget $$URL -O snes_test.tar.gz || curl -L $$URL -o snes_test.tar.gz; \
		tar -xzf snes_test.tar.gz; \
		chmod +x snes_test; \
		rm snes_test.tar.gz; \
		echo "snes_test installed successfully!"; \
	else \
		echo "snes_test already installed."; \
	fi
	@echo "Setting up PATH for PVSnesLib tools..."
	sh ./setup_path.sh
	@echo "Dependencies setup complete!"

run: $(BUILD_DIR)/$(ROMNAME).sfc
	@echo "Running $(ROMNAME).sfc in snes_test..."
	./snes_test $(BUILD_DIR)/$(ROMNAME).sfc

test: $(BUILD_DIR)/$(ROMNAME).sfc
	@echo "Running unit tests headlessly with snes_test..."
	for i in tests/*.lua; do ./snes_test lua build/ChronicEchos.sfc $$i; done

clean: cleanBuildRes cleanRomTemp cleanGfx
	@echo clean intermediate files preserving ROM and tools
	@rm -rf src/*.ps src/*.obj src/*.asp linkfile hdr.obj src/hdr.obj src/data.obj
	@rm -f src/main.asm src/sprites.asm

# New target for full clean including ROM
cleanAll: cleanBuildRes cleanRom cleanGfx
	@echo clean ALL build files including ROM and tools
	@rm -rf $(BUILD_DIR)
	@rm -rf pvsneslib
	@rm -f snes_test
#---------------------------------------------------------------------------------
# Override cleanGfx to preserve essential converted graphics files
cleanGfx:
	@echo clean graphics data but preserve essentials
	@rm -f *.map *.pc7 *.mp7 *.til *.m16 *.b16 *.o16 *.t16
	@echo preserved pvsneslibfont.pic and pvsneslibfont.pal for build

# New target to clean ROM from build directory
cleanRomTemp:
	@echo clean temporary rom files
	@rm -f $(ROMNAME).sfc $(ROMNAME).sym

# New target to clean build resources
cleanBuildRes:
	@echo clean build resources
	@rm -rf src/*.ps src/*.obj src/*.asp linkfile hdr.obj src/hdr.obj src/data.obj
	@rm -f src/main.asm src/sprites.asm

# New target to clean ROM
cleanRom:
	@echo clean ROM files
	@rm -f $(BUILD_DIR)/$(ROMNAME).sfc $(BUILD_DIR)/$(ROMNAME).sym

#---------------------------------------------------------------------------------
pvsneslibfont.pic: assets/graphics/fonts/pvsneslibfont.png
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -p -e 0 -i $<
	@mv assets/graphics/fonts/pvsneslibfont.pic .
	@mv assets/graphics/fonts/pvsneslibfont.pal .

bitmaps : check-deps pvsneslibfont.pic

#---------------------------------------------------------------------------------
# Graphics conversion targets
#---------------------------------------------------------------------------------

# Convert tileset PNG to SNES format
assets/graphics/backgrounds/tileset.pic assets/graphics/backgrounds/tileset.pal: assets/graphics/backgrounds/tileset.png
	@echo "Converting tileset $(notdir $<) to SNES format..."
	$(GFXCONV) -i $< -p

# Convert sprites PNG to SNES format  
assets/graphics/sprites/sprites_simple.pic assets/graphics/sprites/sprites_simple.pal: assets/graphics/sprites/sprites_simple.png
	@echo "Converting sprites $(notdir $<) to SNES format..."
	$(GFXCONV) -s 32 -o 16 -u 16 -t png -p -i $<

# Convert any PNG to SNES format (usage: make convert PNG=image.png)
convert:
	@if [ -z "$(PNG)" ]; then \
		echo "Usage: make convert PNG=image.png"; \
		exit 1; \
	fi
	@echo "Converting $(PNG) to SNES format..."
	$(GFXCONV) -i $(PNG) -p

# Clean graphics files
clean-gfx:
	@echo "Cleaning generated graphics files..."
	@rm -f assets/graphics/backgrounds/*.pic assets/graphics/backgrounds/*.pal
	@rm -f assets/graphics/sprites/*.pic assets/graphics/sprites/*.pal
	@rm -f assets/graphics/fonts/*.pic assets/graphics/fonts/*.pal

# Rebuild all graphics and ROM
rebuild-graphics: clean-gfx assets/graphics/backgrounds/tileset.pic assets/graphics/sprites/sprites_simple.pic
	@echo "Graphics rebuilt successfully!"

# Full rebuild including graphics
full-rebuild: clean rebuild-graphics all
	@echo "Full rebuild completed!"

#---------------------------------------------------------------------------------
# Override the default .sfc rule to output to build directory
$(BUILD_DIR)/$(ROMNAME).sfc: $(BUILD_DIR) $(OFILES)
	@echo Creating linkfile ...
	@echo [objects] > linkfile
	@for i in $(OFILES); do \
		echo $$i >> linkfile; \
	done

	@for i in $(shell ls $(LIBDIRSOBJSW)); do \
		echo $(LIBDIRSOBJSW)/$$i >> linkfile; \
	done

	@echo Linking ... $(notdir $@)
	@rm -f $(BUILD_DIR)/$(ROMNAME).sym
# -c should be removed ASAP ! It allow duplicate labels and definitions
	$(LD) -d -s -v -A -c -L ${LIBDIRSOBJS} linkfile $(BUILD_DIR)/$(ROMNAME).sfc

	@gsed -i 's/://' $(BUILD_DIR)/$(ROMNAME).sym

	@echo
	@echo Build finished successfully !
	@echo