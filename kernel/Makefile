# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

### Import utils ###
include ../build/utils.mk

SRC_DIRS 			:= src
LIBC_DIR 			:= ../lib/libc
LIBSTDCPP_DIR 		:= ../lib/libstdcpp
USER_APPS_DIR 		:= ../user
USER_APPS_OUT_DIR 	:= $(USER_APPS_DIR)/bin
#USER_APPS_NAMES		:= $(notdir $(call get-all-files, $(USER_APPS_OUT_DIR)))
USER_APPS_NAMES		:= ls init

### Output directory ###
OUTPUT		:= out
BUILD_DIR	:= build
OUTPUT_DIR	:= bin

### Generated file ###
USER_APPS_MV_DIR	:= $(BUILD_DIR)/user_apps
USER_APPS_MV_FILES	:= $(addprefix $(USER_APPS_MV_DIR)/, $(USER_APPS_NAMES))
USER_APPS_GEN_DIR 	:= $(BUILD_DIR)/gen
USER_APPS_GEN_FILE 	:= $(USER_APPS_GEN_DIR)/user_apps.S
	
### Compilation sources ###
LIB_INCLUDE_DIRS 	:= $(LIBC_DIR)/include $(LIBSTDCPP_DIR)/include
INCLUDE_DIRS 		:= $(LIB_INCLUDE_DIRS) src/arch/x64 $(SRC_DIRS)
LIBRARY_DIRS		:= $(LIBC_DIR)/bin $(LIBSTDCPP_DIR)/bin
SRC 				:= $(call get-all-files-with-ext,$(SRC_DIRS), cpp) \
					   $(call get-all-files-with-ext,$(SRC_DIRS), c) \
	               	   $(call get-all-files-with-ext,$(SRC_DIRS), S)
DEPS				:= $(addprefix $(BUILD_DIR)/, $(call get-deps,$(SRC)))
OBJS				:= $(addprefix $(BUILD_DIR)/, $(call get-objs,$(SRC)))
INCLUDES 			:= $(addprefix -I, $(INCLUDE_DIRS))
LIBRARIES			:= $(addprefix -L, $(LIBRARY_DIRS))
LINKER 				:= src/arch/x64/build/linker.gen

LIBC		:= $(LIBC_DIR)/bin/libc_weak.a
LIBSTDCPP 	:= $(LIBSTDCPP_DIR)/bin/libstdcpp.a


### Kernel file ###
BIN=$(OUTPUT_DIR)/phoenix.bin

### Tool flags ###
CXXFLAGS	:= -Wall -Wextra \
		  	   -g -std=c++11 \
		  	   -fno-exceptions -fno-rtti \
		  	   -mcmodel=kernel \
			   -fno-pie \
		  	   -Wno-packed-bitfield-compat \
		  	   -fno-stack-protector -nostdinc -nostdinc++ \
		  	   -fno-use-cxa-atexit
LDFLAGS 	:= -nostdlib -g \
			   -mcmodel=kernel -no-pie
LIBFLAGS	:= $(LIBRARIES) -lstdcpp -lc_weak 

### Import tools ###
include ../build/tools.mk

### Define quiet tools ###
ifneq "$(VERBOSE)" "1"
    include ../build/quiet.mk
endif

ifndef $(QUIET_PREFIX)
	QUIET_PREFIX := "\\r"
endif

.PHONY:all kernel kernel_build static_libc static_libstdcpp user_apps initrd_fs initrd_fs_build
kernel:
	@$(ECHO) "\033[0;33m$(QUIET_PREFIX)  Building $(STATIC_LIB_NAME) \033[0m\n"
	@$(MAKE) kernel_build --no-print-directory VERBOSE=$(VERBOSE) QUIET_PREFIX="$(QUIET_PREFIX)  "
	@$(ECHO) "\033[0;32m$(QUIET_PREFIX)  Building $(STATIC_LIB_NAME) succesful\033[0m\n"

kernel_build: $(BIN)
	@:

all: kernel

### INITRD ###
INITRD_DISK := $(OUTPUT)/initrd/initrd.iso

$(OUTPUT)/initrd/initrd.iso: ./script/initrd.sh
	$(dir_guard)
	@./script/initrd.sh $(INITRD_DISK)
	
out/initrd/initrd.S.o: $(OUTPUT)/initrd/initrd.iso

### Generate C librairy ###
static_libc:
	@$(MAKE) static_lib --no-print-directory -C $(LIBC_DIR) WEAK_SYSCALL=1 VERBOSE=$(VERBOSE) QUIET_PREFIX="$(QUIET_PREFIX)"

### Generate CPP librairy ###
static_libstdcpp:
	@$(MAKE) static_lib --no-print-directory -C $(LIBSTDCPP_DIR) VERBOSE=$(VERBOSE) QUIET_PREFIX="$(QUIET_PREFIX)"
	
### Generate user applications ###
user_apps: 
	@$(MAKE) all --no-print-directory -C $(USER_APPS_DIR) VERBOSE=$(VERBOSE) QUIET_PREFIX="$(QUIET_PREFIX)"
	
### Generate initrd filesystem ###
initrd_fs:
	@$(ECHO) "\033[0;33m$(QUIET_PREFIX)  Loading user apps in kernel\033[0m\n"
	@$(MAKE) initrd_fs_build --no-print-directory VERBOSE=$(VERBOSE) QUIET_PREFIX="$(QUIET_PREFIX)  "
	@$(ECHO) "\033[0;32m$(QUIET_PREFIX)  Loading user apps succesful\033[0m\n"

initrd_fs_build: $(BUILD_DIR)/initrd/initrd.S.o
	@:

### User application sub-target and generation of user_apps.S
$(USER_APPS_MV_DIR)/%: $(USER_APPS_OUT_DIR)/%
	$(dir_guard)
	$(CP) $< $@

.PHONY: update_gen_file
update_gen_file: user_apps
	@$(ECHO) "$(QUIET_PREFIX)  UPDATE  $(USER_APPS_GEN_FILE)\n"; $(MAKE) -s $(USER_APPS_GEN_FILE)

$(USER_APPS_GEN_FILE): $(USER_APPS_MV_FILES) script/user_apps.sh
	$(dir_guard)
	@$(ECHO) "user app files $(USER_APP_FILES)"
	@$(ECHO) "$(QUIET_PREFIX)  GEN     $@\n"; script/user_apps.sh $(USER_APPS_MV_DIR) $(USER_APPS_GEN_FILE)

$(eval $(call create-cpp-targets,$(CXXFLAGS),$(INCLUDES),$(BUILD_DIR)))

$(USER_APPS_GEN_DIR)/user_apps.S.o: | update_gen_file
	$(dir_guard)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $(USER_APPS_GEN_DIR)/user_apps.S -o $@
$(USER_APPS_GEN_DIR)/user_apps.S.d: | update_gen_file
	$(dir_guard)
	$(DEP) $(CXXFLAGS) $(INCLUDES) -MM $(USER_APPS_GEN_DIR)/user_apps.S -MT $@ > $@

### INITRD ###
INITRD_DISK := $(BUILD_DIR)/initrd/initrd.iso

$(BUILD_DIR)/initrd/initrd.iso: ./script/initrd.sh
	$(dir_guard)
	@./script/initrd.sh $(INITRD_DISK)
	
$(BUILD_DIR)/initrd/initrd.S.o: $(BUILD_DIR)/initrd/initrd.iso $(BUILD_DIR)/initrd/initrd.S.d

$(BUILD_DIR)/initrd/initrd.S.d: $(BUILD_DIR)/initrd/initrd.iso

### Generate linker script ###
$(BUILD_DIR)/linker.ld: $(LINKER) $(BUILD_DIR)/linker.ld.d
	$(dir_guard)
	$(CXX) -P -E $(INCLUDES) -x c $< -o $@
	
$(BUILD_DIR)/linker.ld.d: $(LINKER)
	$(dir_guard)
	$(DEP) $(INCLUDES) -x c -MM $< -MT $@ > $@

### Generate kernel ###
$(BIN): $(BUILD_DIR)/linker.ld $(OBJS) $(USER_APPS_GEN_DIR)/user_apps.S.o | static_libc static_libstdcpp initrd_fs
	$(dir_guard)
	$(LD) $(LDFLAGS) -T$(BUILD_DIR)/linker.ld $(OBJS) $(USER_APPS_GEN_DIR)/user_apps.S.o $(BUILD_DIR)/initrd/initrd.S.o $(LIBFLAGS) -o $@

### Include auto-generated dependencies
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),clean-all)
    -include $(DEPS)
endif
endif

### Clean ###
.PHONY: clean clean-all
clean:
	@$(ECHO) "\033[0;32m  Clean kernel \033[0m\n"; rm -rf $(BUILD_DIR) $(OUTPUT_DIR)
	
clean-all: clean
	@$(MAKE) clean --no-print-directory -C $(LIBC_DIR) VERBOSE=$(VERBOSE)
	@$(MAKE) clean --no-print-directory -C $(LIBSTDCPP_DIR) VERBOSE=$(VERBOSE)
	@$(MAKE) clean --no-print-directory -C $(USER_APPS_DIR) VERBOSE=$(VERBOSE)
