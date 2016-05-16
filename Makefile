# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa 
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

BIN := phoenix.bin
KERNEL_DIR := kernel

KERNEL := $(KERNEL_DIR)/$(BIN)

GDB := gdb

DISK := disk.iso
DISK_DIR := iso

QEMU := qemu-system-x86_64
QEMU_OPTS := -m 256 -hda $(DISK) -d int,cpu_reset
QEMU_OPTS_DEBUG := $(QEMU_OPTS) -s -S

### Basic rules ###
.PHONY: all launch debug clean clean_disk
$(KERNEL): all
	@echo "\033[0;32m  Kernel succesfully build\033[0m"

all:
	@$(MAKE) $(BIN) --no-print-directory -C $(KERNEL_DIR)/ VERBOSE=$(VERBOSE)

$(DISK): $(KERNEL)
	cp $(KERNEL) $(DISK_DIR)/boot/
	grub-mkrescue -o $(DISK) $(DISK_DIR)

launch: $(DISK)
	gnome-terminal -e "$(QEMU) $(QEMU_OPTS)" &

debug: $(DISK)
	gnome-terminal -e "$(QEMU) $(QEMU_OPTS_DEBUG)" &
	sleep 2
	gnome-terminal -e "$(GDB) $(KERNEL)"

clean:
	@$(MAKE) --no-print-directory clean -C kernel/
	rm -rf $(DISK) $(DISK_DIR)/boot/$(BIN)

