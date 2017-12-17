# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

### Import utils ###
include ../build/utils.mk

LIBC_DIR := ../lib/libc
LIBSTDCPP_DIR := ../lib/libstdcpp
USER_APPS_DIR := ../user
USER_APPS_OUT_DIR := $(USER_APPS_DIR)/out/apps
USER_APPS_MV_DIR := user

USER_APPS_GEN_DIR := gen
USER_APPS_GEN_FILE := $(USER_APPS_GEN_DIR)/user_apps.S

### Output directory ###
OUTPUT=out
	
### Compilation sources ###
SRC_DIRS 		:= boot core core/interrupt mm fs \
				   fs/vfs fs/kernelfs driver/input \
				   proc driver driver/PCI driver/PCI/IDE \
				   fs/ext2fs
LIB_INCLUDE_DIRS := $(LIBC_DIR)/include $(LIBSTDCPP_DIR)/include
INCLUDE_DIRS 	:= $(LIB_INCLUDE_DIRS) arch/x64 .
LIBRARY_DIRS	:= $(LIBC_DIR) $(LIBSTDCPP_DIR)
SRC 			:= $(call get-all-files-with-ext, $(SRC_DIRS), cpp) \
	               $(call get-all-files-with-ext, $(SRC_DIRS), c) \
	               $(call get-all-files-with-ext, $(SRC_DIRS), S)
INCLUDES 		:= $(addprefix -I, $(INCLUDE_DIRS))
LIBRARIES		:= $(addprefix -L, $(LIBRARY_DIRS))
LINKER 			:= arch/x64/build/linker.gen

LIBC		:= $(LIBC_DIR)/libc.a
LIBSTDCPP 	:= $(LIBSTDCPP_DIR)/libstdcpp.a

USER_APPS := $(call get-all-files-with-ext, $(USER_APPS_OUT_DIR), out)
USER_APPS_MV := $(addprefix $(USER_APPS_MV_DIR)/, $(notdir  $(USER_APPS)))

### Kernel file ###
BIN=phoenix.bin

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
LIBFLAGS	:= $(LIBRARIES) -lstdcpp -lc 

### Import tools ###
include ../build/tools.mk

### Define quiet tools ###
ifneq "$(VERBOSE)" "1"
    include ../build/quiet.mk
endif

.PHONY:all static_libc static_libstdcpp user_apps initrd_fs initrd_fs_build
all:
	@$(ECHO) "\033[0;33m  Build kernel\033[0m\n"
	@$(MAKE) $(BIN) --no-print-directory VERBOSE=$(VERBOSE)
	@$(ECHO) "\033[0;32m  Kernel succesfully built\033[0m\n"


### INITRD ###
INITRD_DISK := $(OUTPUT)/initrd/initrd.iso

$(OUTPUT)/initrd/initrd.iso: ./script/initrd.sh |  $(OUTPUT) $(OUTPUT)/initrd
	@./script/initrd.sh $(INITRD_DISK)
	
out/initrd/initrd.S.o: $(OUTPUT)/initrd/initrd.iso

$(call create-dir-files, $(OUTPUT)/initrd/initrd.S.o)
$(call create-dir-files, $(OUTPUT)/initrd/initrd.S.d)
$(call create-dir-targets, $(OUTPUT)/initrd)


###out/initrd/mkird.S.o: | $(INITRD_DISK)
	

### Output files ###
DEPS		:= $(addprefix $(OUTPUT)/, $(call get-deps, $(SRC)))
OBJS		:= $(addprefix $(OUTPUT)/, $(call get-objs, $(SRC)))
OUTPUT_DIRS	:= $(addprefix $(OUTPUT)/, $(SRC_DIRS))

### Generate C librairy ###
static_libc:
	@$(ECHO) "\033[0;33m  Build libc.a\033[0m\n"
	@$(MAKE) static_lib --no-print-directory -C $(LIBC_DIR) VERBOSE=$(VERBOSE)

### Generate CPP librairy ###
static_libstdcpp:
	@$(ECHO) "\033[0;33m  Build libstdcpp.a\033[0m\n"
	@$(MAKE) static_lib --no-print-directory -C $(LIBSTDCPP_DIR) VERBOSE=$(VERBOSE)
	
### Generate user applications ###
user_apps: 
	@$(ECHO) "\033[0;33m  Build user applications\033[0m\n"
	@$(MAKE) user_apps --no-print-directory -C $(USER_APPS_DIR) VERBOSE=$(VERBOSE)
	
### Generate initrd filesystem ###
initrd_fs:
	@$(ECHO) "\033[0;33m  Build initrd filesystem\033[0m\n"
	@$(MAKE) initrd_fs_build --no-print-directory VERBOSE=$(VERBOSE)
	@$(ECHO) "\033[0;32m  initrd succesfully built\033[0m\n"

initrd_fs_build: out/initrd/initrd.S.o
	@:

### User application sub-target and generation of user_apps.S
$(USER_APPS_MV_DIR):
	$(MKDIR) -p $@	

USER_APP_FILES := $(call get-all-files-with-ext, $(USER_APPS_OUT_DIR), cpp)

$(USER_APPS_MV_DIR)/%.out: $(USER_APPS_OUT_DIR)/%.out | $(USER_APPS_MV_DIR)
	$(CP) $< $@

update_gen_file: user_apps
	@$(ECHO) "  UPDATE  $(USER_APPS_GEN_FILE)\n"; $(MAKE) -s $(USER_APPS_GEN_FILE)

$(USER_APPS_GEN_FILE): $(USER_APPS_MV) script/user_apps.sh | $(USER_APPS_GEN_DIR)
	@$(ECHO) "  GEN     $@\n"; script/user_apps.sh $(USER_APPS_MV_DIR) $(USER_APPS_GEN_FILE)

$(USER_APPS_GEN_DIR):
	$(MKDIR) -p $@

out/gen/user_apps.S.o: $(USER_APP_FILES) update_gen_file | out/gen
out/gen/user_apps.S.d: $(USER_APP_FILES) update_gen_file | out/gen

out/gen:
	$(MKDIR) -p $@

### Generate linker script ###
$(OUTPUT)/build/linker.ld: $(LINKER) $(OUTPUT)/build/linker.ld.d | $(OUTPUT)
	$(CXX) -P -E $(INCLUDES) -x c $< -o $@
	
$(OUTPUT)/build/linker.ld.d: $(LINKER) | $(OUTPUT)
	$(MKDIR) -p $(OUTPUT)/build
	$(DEP) $(INCLUDES) -x c -MM $< -MT $@ > $@

### Generate kernel ###
$(BIN): $(OUTPUT)/build/linker.ld $(OBJS) out/gen/user_apps.S.o static_libc static_libstdcpp initrd_fs
	$(LD) $(LDFLAGS) -T$(OUTPUT)/build/linker.ld $(OBJS) out/gen/user_apps.S.o out/initrd/initrd.S.o $(LIBFLAGS) -o $@

### Generic rules ###
$(OUTPUT)/%.S.d: %.S | $(OUTPUT)
	$(DEP) $(CXXFLAGS) $(INCLUDES) -MM $< -MT $@ > $@
$(OUTPUT)/%.c.d: %.c | $(OUTPUT)
	$(DEP) $(CXXFLAGS) $(INCLUDES) -MM $< -MT $@ > $@
$(OUTPUT)/%.cpp.d: %.cpp | $(OUTPUT)
	$(DEP) $(CXXFLAGS) $(INCLUDES) -MM $< -MT $@ > $@

$(OUTPUT)/%.o : % $(OUTPUT)/%.d | $(OUTPUT)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

### Add dependency between object file and the directory containing it ###
$(call create-dir-files, $(OBJS))
$(call create-dir-files, $(DEPS))

### Create a target for output dirs ###
$(call create-dir-targets, $(OUTPUT_DIRS))

	
### Output directory ###
$(OBJS): | $(OUTPUT)
$(DEPS): | $(OUTPUT)
$(OUTPUT):
	$(MKDIR) -p $@
	
### Include auto-generated dependencies
ifneq ($(MAKECMDGOALS),clean)
    -include $(DEPS)
endif

### Clean ###
.PHONY: clean
clean:
	rm -rf $(OUTPUT) $(BIN) $(USER_APPS_GEN_DIR) $(USER_APPS_MV_DIR)
