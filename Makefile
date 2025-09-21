ifeq ($(strip $(PVSNESLIB_HOME)),)
PVSNESLIB_HOME = $(CURDIR)/pvsneslib
endif

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: bitmaps all run clean

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
all: bitmaps $(BUILD_DIR) $(BUILD_DIR)/$(ROMNAME).sfc

validate: $(BUILD_DIR)/$(ROMNAME).sfc
	@echo "Validating $(ROMNAME).sfc..."
	./scripts/validate_rom.sh $(BUILD_DIR)/$(ROMNAME).sfc

run: $(BUILD_DIR)/$(ROMNAME).sfc
	@echo "Running $(ROMNAME).sfc in Mednafen..."
	mednafen $(BUILD_DIR)/$(ROMNAME).sfc

test: $(BUILD_DIR)/$(ROMNAME).sfc
	@echo "Running unit tests headlessly with Mesen..."
	mesen --testRunner tests/test.lua $<

clean: cleanBuildRes cleanRomTemp cleanGfx
	@echo clean intermediate files preserving ROM
	@rm -rf src/*.ps src/*.obj src/*.asp linkfile hdr.obj src/hdr.obj src/data.obj
	@rm -f src/main.asm src/sprites.asm

# New target for full clean including ROM
cleanAll: cleanBuildRes cleanRom cleanGfx
	@echo clean ALL build files including ROM
	@rm -rf $(BUILD_DIR)

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

#---------------------------------------------------------------------------------
pvsneslibfont.pic: assets/graphics/fonts/pvsneslibfont.png
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -p -e 0 -i $<
	@mv assets/graphics/fonts/pvsneslibfont.pic .
	@mv assets/graphics/fonts/pvsneslibfont.pal .

bitmaps : pvsneslibfont.pic

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

	@sed -i 's/://' $(BUILD_DIR)/$(ROMNAME).sym

	@echo
	@echo Build finished successfully !
	@echo