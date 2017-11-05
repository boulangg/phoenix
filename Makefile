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
.PHONY: all launch debug clean clean_disk clean-user clean-all kernel_target
kernel_target: all
	@echo -e "\033[0;32m  Kernel succesfully built\033[0m"

all:
	@echo -e "\033[0;33m  Build kernel\033[0m"
	@$(MAKE) $(BIN) --no-print-directory -C $(KERNEL_DIR)/ VERBOSE=$(VERBOSE)

$(DISK): kernel_target
	cp $(KERNEL) $(DISK_DIR)/boot/
	grub-mkrescue -o $(DISK) $(DISK_DIR)

launch: $(DISK)
	scripts/terminal.sh -e "$(QEMU) $(QEMU_OPTS)" &

debug: $(DISK)
	scripts/terminal.sh -e "$(QEMU) $(QEMU_OPTS_DEBUG)" &
	sleep 1
	scripts/terminal.sh -e "$(GDB) $(KERNEL)" &

clean:
	@$(MAKE) --no-print-directory clean -C kernel/
	rm -rf $(DISK) $(DISK_DIR)/boot/$(BIN)
	
clean-libs:
	@$(MAKE) --no-print-directory clean -C lib/libc/
	@$(MAKE) --no-print-directory clean -C lib/libstdcpp/
	
clean-user:
	@$(MAKE) --no-print-directory clean -C user/
	
clean-all: clean clean-libs clean-user
