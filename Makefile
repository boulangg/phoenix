# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa 
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

### import tools ###
include build/tools.mk

### Define quiet tools ###
ifneq "$(VERBOSE)" "1"
    include build/quiet.mk
endif

BIN := phoenix.bin
KERNEL_DIR := kernel

KERNEL := $(KERNEL_DIR)/bin/$(BIN)

GDB := gdb

DISK := disk.iso
DISK_DIR := iso

DRIVE := drive.img
DRIVE_DIR := drive
DRIVE_SIZE := 16M

QEMU := qemu-system-x86_64
QEMU_OPTS := -boot d -m 256 -cdrom $(DISK) -hda $(DRIVE) -d int,cpu_reset 
QEMU_OPTS_DEBUG := $(QEMU_OPTS) -s -S

### Basic rules ###
.PHONY: all launch debug drive clean clean_disk clean-user clean-all kernel_target
kernel_target: all
	@$(ECHO) "\033[0;32m  Kernel succesfully built\033[0m\n"

all:
	@$(ECHO) "\033[0;33m  Build kernel\033[0m\n"
	@$(MAKE) all --no-print-directory -C $(KERNEL_DIR)/ VERBOSE=$(VERBOSE)

$(DISK): kernel_target
	@$(ECHO) "  CP      $(KERNEL) -> $(DISK_DIR)\n"; cp $(KERNEL) $(DISK_DIR)/boot/
	@$(ECHO) "  GRUBMK\n"; grub-mkrescue -o $(DISK) $(DISK_DIR)

drive:
	@$(ECHO) "\033[0;33m  Create ext2 drive\033[0m\n"
	@test -s $(DRIVE) || ./scripts/disk.sh CREATE $(DRIVE) $(DRIVE_SIZE)
	@$(ECHO) "\033[0;32m  Ext2 drive created\033[0m\n"

launch: $(DISK) drive
	@$(ECHO) "  SH      $(QEMU)\n"; scripts/terminal.sh -e "$(QEMU) $(QEMU_OPTS)" &

debug: $(DISK) drive
	@$(ECHO) "  SH      $(QEMU)\n"; scripts/terminal.sh -e "$(QEMU) $(QEMU_OPTS_DEBUG)" &
	@$(ECHO) "  SLEEP   1\n"; sleep 1
	@$(ECHO) "  SH      $(GDB)\n"; scripts/terminal.sh -e "$(GDB) $(KERNEL)" &

clean:
	@$(ECHO) "\033[0;32m  Clean disk.iso \033[0m\n"; rm -rf $(DISK) $(DISK_DIR)/boot/$(BIN) $(DRIVE)
	
clean-libs:
	@$(MAKE) --no-print-directory clean -C lib/libc/
	@$(MAKE) --no-print-directory clean -C lib/libstdcpp/
	
clean-user:
	@$(MAKE) --no-print-directory clean -C user/
	
clean-kernel:
	@$(MAKE) --no-print-directory clean -C kernel/
	
clean-all: clean clean-libs clean-user clean-kernel
