V=1
SOURCE_DIR=src
BUILD_DIR=build
include $(N64_INST)/include/n64.mk

src = main.c

all: gacha64.z64

gacha64.z64: N64_ROM_TITLE="Gacha64"
$(BUILD_DIR)/gacha64.elf: $(src:%.c=$(BUILD_DIR)/%.o)

clean:
	rm -f $(BUILD_DIR)/* gacha64.z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
