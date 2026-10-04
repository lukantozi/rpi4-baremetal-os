ARMGNU := aarch64-linux-gnu
KERNEL := kernel8.img

BUILD := build
SRC := src

CFLAGS := -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only -MMD -MP
ASFLAGS := -Iinclude
UART_FLAG = $(if $(strip $(UART_SWITCH)),-DUART_SWITCH=$(UART_SWITCH))
OBJS := $(patsubst $(SRC)/%.c,$(BUILD)/%_c.o,$(wildcard $(SRC)/*.c)) \
		$(patsubst $(SRC)/%.S,$(BUILD)/%_s.o,$(wildcard $(SRC)/*.S))

QEMU	:= qemu-system-aarch64
QFLAGS	:= -machine raspi4b -cpu cortex-a72 -dtb bcm2711-rpi-4-b.dtb -m 2G -kernel $(KERNEL) \
		   -nographic -drive format=raw,file=rpi4.img

.PHONY: all qemu clean

all: $(KERNEL)

$(KERNEL): $(SRC)/linker.ld $(OBJS)
	$(ARMGNU)-ld -T $(SRC)/linker.ld -o $(BUILD)/kernel8.elf $(OBJS)
	$(ARMGNU)-objcopy $(BUILD)/kernel8.elf -O binary $(KERNEL)

$(BUILD)/%_c.o: $(SRC)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(CFLAGS) $(UART_FLAG) -c $< -o $@

$(BUILD)/%_s.o: $(SRC)/%.S
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(ASFLAGS) -c $< -o $@

-include $(OBJS:.o=.d)

qemu: UART_SWITCH ?= 0
qemu: $(KERNEL) rpi4.img
	$(QEMU) $(QFLAGS) $(DFLAGS)

rpi4.img:
	qemu-img create -f raw rpi4.img 8G

clean:
	rm -rf $(BUILD) *.img
