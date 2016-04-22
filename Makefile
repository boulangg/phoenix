

BIN := phoenix.bin
KERNEL_DIR := kernel

KERNEL := $(KERNEL_DIR)/$(BIN)

GDB := gdb

DISK := disk.iso
DISK_DIR := iso

QEMU := qemu-system-x86_64
QEMU_OPTS := -m 256 -hda $(DISK)
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
	gnome-terminal -e "$(GDB) $(KERNEL)"

clean:
	@$(MAKE) --no-print-directory clean -C kernel/
	rm -rf $(DISK) $(DISK_DIR)/boot/$(BIN)

