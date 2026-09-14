ARMGNU = aarch64-linux-gnu

CFLAGS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
ASFLAGS = -Iinclude

BUILD = build
SRC = src

all: kernel8.img

clean:
	rm -rf $(BUILD)/*.img

$(BUILD)/%_c.o: $(SRC)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(BUILD)/%_s.o: $(SRC)/%.S
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(ASFLAGS) -c $< -o $@

C_FILES   = $(wildcard $(SRC)/*.c)
ASM_FILES = $(wildcard $(SRC)/*.S)
OBJ_FILES = $(C_FILES:$(SRC)/%.c=$(BUILD)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC)/%.S=$(BUILD)/%_s.o)

kernel8.img: $(SRC)/linker.ld $(OBJ_FILES)
	$(ARMGNU)-ld -T $(SRC)/linker.ld -o $(BUILD)/kernel8.elf $(OBJ_FILES)
	$(ARMGNU)-objcopy $(BUILD)/kernel8.elf -O binary kernel8.img
