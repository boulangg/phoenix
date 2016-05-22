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
.PHONY: all launch debug clean clean_disk kernel_target
kernel_target: all
	@echo "\033[0;32m  Kernel succesfully build\033[0m"

all:
	@echo "\033[0;33m  Generate kernel\033[0m"
	@$(MAKE) $(BIN) --no-print-directory -C $(KERNEL_DIR)/ VERBOSE=$(VERBOSE)

$(DISK): kernel_target
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
	
clean-libs:
	@$(MAKE) --no-print-directory clean -C lib/libc/
	@$(MAKE) --no-print-directory clean -C lib/libstdcpp/


### Targets to cross compile gcc ###

GCC_MIRROR=ftp://ftp.gwdg.de/pub/misc/gcc/releases
GCC_VERSION=gcc-5.3.0
CROSS_TARGET=x86_64-elf
CROSS_DIR=cross

GCC_COMPRESS_FILE=$(addsuffix .tar.gz, $(GCC_VERSION))
GCC_MIRROR_DOWNLOAD=$(GCC_MIRROR)/$(GCC_VERSION)/$(GCC_COMPRESS_FILE)
GCC_EXTRACT_TARGET=$(GCC_VERSION)/_extract_file
GCC_EXTRACT_DIR=$(GCC_VERSION)
GCC_DOWNLOAD_PREREQ=$(GCC_EXTRACT_DIR)/contrib/download_prerequisites
GCC_DOWNLOAD_PREREQ_TARGET=$(GCC_EXTRACT_DIR)/_download_prereq

CROSS_GCC=$(CROSS_DIR)/bin/$(CROSS_TARGET)-gcc
CROSS_BUILD_DIR=$(GCC_EXTRACT_DIR)/build

.PHONY:cross-gcc
cross-gcc: $(CROSS_GCC)

$(GCC_COMPRESS_FILE):
	wget $(GCC_MIRROR_DOWNLOAD)
	
$(GCC_EXTRACT_TARGET): $(GCC_COMPRESS_FILE)
	tar xzf $(GCC_COMPRESS_FILE)
	touch $@
	
$(GCC_DOWNLOAD_PREREQ): $(GCC_EXTRACT_TARGET)

$(GCC_DOWNLOAD_PREREQ_TARGET): $(GCC_DOWNLOAD_PREREQ)
	cd $(GCC_EXTRACT_DIR); $(PWD)/$(GCC_EXTRACT_DIR)/contrib/download_prerequisites
	touch $@

$(CROSS_BUILD_DIR): $(GCC_EXTRACT_TARGET)
	mkdir -p $@

$(CROSS_GCC): $(CROSS_DIR) $(GCC_EXTRACT_TARGET) $(GCC_DOWNLOAD_PREREQ_TARGET) $(CROSS_BUILD_DIR)
	mkdir -p $(CROSS_BUILD_DIR)
	cd $(CROSS_BUILD_DIR) && \
	$(PWD)/$(GCC_EXTRACT_DIR)/configure --target=$(CROSS_TARGET) --prefix=$(PWD)/$(CROSS_DIR) --disable-nls --enable-languages=c,c++ --without-headers&& \
	make -j all-gcc &&\
	make -j all-libgcc &&\
	make install-gcc &&\
	make install-target-libgcc
	@echo "\033[0;32m  Cross compiler succesfully build\033[0m"
	

$(CROSS_DIR):
	$(MKDIR) -p $@