ARMGNU  := aarch64-linux-gnu
BUILD   := build
SRC     := src
CFLAGS  := -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only -MMD -MP
ASFLAGS := -Iinclude
OBJS    := $(patsubst $(SRC)/%.c,$(BUILD)/%_c.o,$(wildcard $(SRC)/*.c)) \
		   $(patsubst $(SRC)/%.S,$(BUILD)/%_s.o,$(wildcard $(SRC)/*.S))

.PHONY: all clean

all: kernel8.img

kernel8.img: $(SRC)/linker.ld $(OBJS)
	$(ARMGNU)-ld -T $(SRC)/linker.ld -o $(BUILD)/kernel8.elf $(OBJS)
	$(ARMGNU)-objcopy $(BUILD)/kernel8.elf -O binary kernel8.img

$(BUILD)/%_c.o: $(SRC)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(BUILD)/%_s.o: $(SRC)/%.S
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(ASFLAGS) -c $< -o $@

-include $(OBJS:.o=.d)

clean:
	rm -rf $(BUILD) *.img
