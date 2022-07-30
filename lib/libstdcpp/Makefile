# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

### Import utils ###
include ../../build/utils.mk

### Compilation sources ###
SRC_DIRS 		:= src
INCLUDE_DIRS 	:= include ../libc/include

STATIC_LIB_NAME := libstdcpp.a

SRC 			:= $(call get-all-files-with-ext, $(SRC_DIRS), cpp) \
	               $(call get-all-files-with-ext, $(SRC_DIRS), S)
INCLUDES 		:= $(addprefix -I, $(INCLUDE_DIRS))

### Output directory ###
BUILD_DIR	:= build
OUTPUT_DIR	:= bin
STATIC_LIB	:= $(OUTPUT_DIR)/$(STATIC_LIB_NAME)

### Tool flags ###
CXXFLAGS    := -Wall -Wextra -g -std=c++11 -m64 \
               -fno-exceptions -fno-rtti \
               -mcmodel=kernel \
               -fno-pie \
               -Wno-packed-bitfield-compat \
               -fno-stack-protector \
               -nostdinc -nostdinc++
ARFLAGS		:= rcs

### Inmport tools ###
include ../../build/tools.mk

### Define quiet tools ###
ifneq "$(VERBOSE)" "1"
    include ../../build/quiet.mk
endif

ifndef $(QUIET_PREFIX)
	QUIET_PREFIX := "\\r"
endif

### Main rules ###
.PHONY:all static_lib static_lib_build
static_lib: 
	@$(ECHO) "\033[0;33m$(QUIET_PREFIX)  Building libstdcpp.a \033[0m\n"
	@$(MAKE) static_lib_build --no-print-directory VERBOSE=$(VERBOSE) QUIET_PREFIX="$(QUIET_PREFIX)  "
	@$(ECHO) "\033[0;32m$(QUIET_PREFIX)  Building libstdcpp.a succesful\033[0m\n"

static_lib_build: $(STATIC_LIB)
	@:

all: static_lib

### Output files ###
DEPS		:= $(addprefix $(BUILD_DIR)/, $(call get-deps, $(SRC)))
OBJS		:= $(addprefix $(BUILD_DIR)/, $(call get-objs, $(SRC)))

### Generate library ###
$(STATIC_LIB): $(OBJS) | $(DEPS)
	$(dir_guard)
	$(AR) $(ARFLAGS) -o $@ $^

### Generic rules ###
$(eval $(call create-cpp-targets,$(CXXFLAGS),$(INCLUDES),$(BUILD_DIR)))

### Include auto-generated dependencies
ifneq ($(MAKECMDGOALS),clean)
    -include $(DEPS)
endif

### Clean ###
.PHONY: clean
clean:
	@$(ECHO) "\033[0;32m  Clean libstdcpp \033[0m\n"; rm -rf $(BUILD_DIR) $(OUTPUT_DIR)
