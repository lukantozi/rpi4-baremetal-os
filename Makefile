ARMGNU := aarch64-linux-gnu
KERNEL := kernel8.img

BUILD := build
SRC := src
DATA := bcm2711-rpi-4-b.dtb cmdline.txt  config.txt  fixup4.dat  start4.elf

CFLAGS := -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only -MMD -MP
ASFLAGS := -Iinclude
UART_FLAG = $(if $(strip $(UART_SWITCH)),-DUART_SWITCH=$(UART_SWITCH))
OBJS := $(patsubst $(SRC)/%.c,$(BUILD)/%_c.o,$(wildcard $(SRC)/*.c)) \
		$(patsubst $(SRC)/%.S,$(BUILD)/%_s.o,$(wildcard $(SRC)/*.S))
GENERIC ?= 0

QEMU	:= qemu-system-aarch64
QFLAGS	:= -machine raspi4b -cpu cortex-a72 -dtb bcm2711-rpi-4-b.dtb -m 2G -kernel $(KERNEL) \
		   -nographic -drive format=raw,file=rpi4.img

.PHONY: all qemu flash run clean

all: $(KERNEL)

$(KERNEL): $(SRC)/linker.ld $(OBJS)
	$(ARMGNU)-ld -T $(SRC)/linker.ld -o $(BUILD)/kernel8.elf $(OBJS)
	$(ARMGNU)-objcopy $(BUILD)/kernel8.elf -O binary $(KERNEL)

$(BUILD)/%_c.o: $(SRC)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(CFLAGS) $(UART_FLAG) -DGENERIC=$(GENERIC) -c $< -o $@

$(BUILD)/%_s.o: $(SRC)/%.S
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(ASFLAGS) -c $< -o $@

.PHONY: FORCE
FORCE:

$(BUILD)/kernel_c.o: $(SRC)/kernel.c FORCE
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(CFLAGS) $(UART_FLAG) -DGENERIC=$(GENERIC) -c $< -o $@

-include $(OBJS:.o=.d)

qemu: UART_SWITCH ?= 0
qemu: GENERIC = 1
qemu: $(KERNEL) rpi4.img
	$(QEMU) $(QFLAGS) $(DFLAGS)

rpi4.img:
	qemu-img create -f raw rpi4.img 8G

flash: $(KERNEL) $(DATA)
	udisksctl mount -b /dev/sdb1
	cp $(KERNEL) $(DATA) /run/media/$(USER)/RPIBOOT
	sync
	udisksctl unmount -b /dev/sdb1

run:
	picocom -b 115200 /dev/ttyUSB0

clean:
	rm -rf $(BUILD) *.img
