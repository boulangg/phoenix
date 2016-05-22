# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

### Import utils ###
include ../build/utils.mk

LIBC_DIR := ../lib/libc
LIBSTDCPP_DIR := ../lib/libstdcpp

### Compilation sources ###
SRC_DIRS 		:= boot core mm 
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

### Output directory ###
OUTPUT=out
BIN=phoenix.bin

### Tool flags ###
CXXFLAGS	:= -Wall -Wextra \
		  	   -g -std=c++11 \
		  	   -fno-exceptions -fno-rtti \
		  	   -mcmodel=kernel \
		  	   -Wno-packed-bitfield-compat \
		  	   -fno-stack-protector -nostdinc -nostdinc++
LDFLAGS 	:= -nostdlib -g \
		  	   -mcmodel=kernel
LIBFLAGS	:= $(LIBRARIES) -lstdcpp -lc 

### Inmport tools ###
include ../build/tools.mk

### Define quiet tools ###
ifneq "$(VERBOSE)" "1"
    include ../build/quiet.mk
endif

.PHONY:all static_libc static_libcpp
all: phoenix.bin
	@echo "\033[0;32m  Kernel succesfully build\033[0m"

### Output files ###
DEPS		:= $(addprefix $(OUTPUT)/, $(call get-deps, $(SRC)))
OBJS		:= $(addprefix $(OUTPUT)/, $(call get-objs, $(SRC)))
OUTPUT_DIRS	:= $(addprefix $(OUTPUT)/, $(SRC_DIRS))

### Generate C librairy ###
static_libc:
	@echo "\033[0;33m  Generate libc.a\033[0m"
	@$(MAKE) static_lib --no-print-directory -C $(LIBC_DIR) VERBOSE=$(VERBOSE)

### Generate CPP librairy ###
static_libcpp:
	@echo "\033[0;33m  Generate libstdcpp.a\033[0m"
	@$(MAKE) static_lib --no-print-directory -C $(LIBSTDCPP_DIR) VERBOSE=$(VERBOSE)

### Generate linker script ###
$(OUTPUT)/build/linker.ld: $(LINKER) $(OUTPUT)/build/linker.ld.d | $(OUTPUT)
	$(CXX) -P -E $(INCLUDES) -x c $< -o $@
	
$(OUTPUT)/build/linker.ld.d: $(LINKER) | $(OUTPUT)
	$(MKDIR) -p $(OUTPUT)/build
	$(DEP) $(INCLUDES) -x c -MM $< -MT $@ > $@

### Generate kernel ###
$(BIN): $(OUTPUT)/build/linker.ld $(OBJS) static_libc static_libcpp
	$(LD) $(LDFLAGS) -T$(OUTPUT)/build/linker.ld $(OBJS) $(LIBFLAGS) -o $@

### Generic rules ###
$(OUTPUT)/%.d: % | $(OUTPUT)
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
	rm -rf $(OUTPUT) $(BIN)