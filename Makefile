ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME by following this guide: https://github.com/alekmaul/pvsneslib/wiki/Installation")
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

run: $(BUILD_DIR)/$(ROMNAME).sfc
	@echo "Running $(ROMNAME).sfc in Mednafen..."
	mednafen $(BUILD_DIR)/$(ROMNAME).sfc

clean: cleanBuildRes cleanRom cleanGfx
	@echo clean build directory
	@rm -rf $(BUILD_DIR)

#---------------------------------------------------------------------------------
# Override cleanRom to clean from build directory
cleanRom:
	@echo clean rom
	@rm -f $(BUILD_DIR)/$(ROMNAME).sfc $(BUILD_DIR)/$(ROMNAME).sym

#---------------------------------------------------------------------------------
pvsneslibfont.pic: pvsneslibfont.png
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -p -e 0 -i $<

bitmaps : pvsneslibfont.pic

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