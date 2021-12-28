V = 1
SOURCE_DIR = src
BUILD_DIR = build
MKSPRITEPATH= $(N64_ROOTDIR)/bin/mksprite
include $(N64_INST)/include/n64.mk

N64_CFLAGS += -Ilibs/libdragon-extensions/include
N64_CFLAGS += -Wno-error=format-contains-nul -Wno-error=format-truncation

N64_ROM_TITLE = "Gacha64"
N64_ROM_SAVETYPE = # Supported savetypes: none eeprom4k eeprom16 sram256k sram768k sram1m flashram
N64_ROM_REGIONFREE = true

C_ROOT_FILES := $(wildcard src/*.c)
C_SCENE_FILES := $(wildcard src/scenes/*.c)
C_ONLINE_FILES := $(wildcard src/online/*.c)
C_LIB_EXTENSIONS_FILES := $(wildcard libs/libdragon-extensions/src/*.c)

SRC = $(C_ROOT_FILES) $(C_SCENE_FILES) $(C_ONLINE_FILES) $(C_LIB_EXTENSIONS_FILES)
OBJS = $(SRC:%.c=%.o)
DEPS = $(SRC:%.c=%.d)

all: gacha64.z64

gacha64.z64: N64_ROM_TITLE="Gacha64"
gacha64.z64: $(BUILD_DIR)/gacha64.dfs

$(BUILD_DIR)/gacha64.dfs: $(wildcard build/filesystem/*)
	mkdir -p build/filesystem
	mkdir -p build/filesystem/gfx

	$(MKSPRITEPATH) 16 16 1 assets/gfx/game_ui.png build/filesystem/gfx/game_ui.sprite
	$(MKSPRITEPATH) 16 9 8 assets/gfx/stickers.png build/filesystem/gfx/stickers.sprite
	$(MKSPRITEPATH) 16 1 1 assets/gfx/background-stickers.png build/filesystem/gfx/background-stickers.sprite

	$(N64_MKDFS) $@ $(<D)

$(BUILD_DIR)/gacha64.elf: $(OBJS)

clean:
	find . -name '*.v64' -delete
	find . -name '*.z64' -delete
	find . -name '*.elf' -delete
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	find . -name '*.bin' -delete
	find . -name '*.plan_bak*' -delete
	find ./src -name '*.sprite' -delete
	find . -name '*.dfs' -delete
	find . -name '*.raw' -delete
	find . -name '*.z64' -delete
	find . -name '*.n64' -delete

-include $(DEPS)

.PHONY: all clean
