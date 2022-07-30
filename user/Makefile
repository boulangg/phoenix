# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

### Import utils ###
include ../build/utils.mk

LIBC_DIR := ../lib/libc
LIBSTDCPP_DIR := ../lib/libstdcpp

### Compilation sources ###
APPS_NAMES			:= $(notdir $(call clean-dir-names, $(sort $(dir $(wildcard apps/*/)))))
LIB_INCLUDE_DIRS 	:= $(LIBC_DIR)/include $(LIBSTDCPP_DIR)/include
INCLUDE_DIRS 		:= $(LIB_INCLUDE_DIRS)
LIBRARY_DIRS		:= $(LIBC_DIR)/bin $(LIBSTDCPP_DIR)/bin
INCLUDES 			:= $(addprefix -I, $(INCLUDE_DIRS))
LIBRARIES			:= $(addprefix -L, $(LIBRARY_DIRS))

### CRT files ###
CRT_DIR		:= init/x86_64
CRT_SRC		:= init/x86_64/crt0.S init/x86_64/crti.S init/x86_64/crtn.S

### Output directory ###
BUILD_DIR	:= build
OUTPUT_DIR	:= bin

### Tool flags ###
CXXFLAGS    := -Wall -Wextra -nostdinc -nostdinc++ \
               -g -std=c++11 \
               -fno-exceptions -fno-rtti \
               -Wno-packed-bitfield-compat \
               -fno-stack-protector -fno-builtin-fprintf -fno-builtin-printf \
               -fno-pie
LDFLAGS 	:= -nodefaultlibs -g -specs=init/x86_64/specs -no-pie
LIBFLAGS	:= $(LIBRARIES) -lc -lstdcpp

### Inmport tools ###
include ../build/tools.mk

### Define quiet tools ###
ifneq "$(VERBOSE)" "1"
    include ../build/quiet.mk
endif

ifndef $(QUIET_PREFIX)
	QUIET_PREFIX := "\\r"
endif

### Output files ###
CRT_OBJS	:= $(addprefix $(BUILD_DIR)/, $(call get-objs, $(CRT_SRC)))
CRT_DEPS	:= $(addprefix $(BUILD_DIR)/, $(call get-deps, $(CRT_SRC)))

.PHONY:all user_apps user_apps_build static_libc static_libstdcpp
user_apps: 
	@$(ECHO) "\033[0;33m$(QUIET_PREFIX)  Building user apps \033[0m\n"
	@$(MAKE) user_apps_build --no-print-directory VERBOSE=$(VERBOSE) QUIET_PREFIX="$(QUIET_PREFIX)  "
	@$(ECHO) "\033[0;32m$(QUIET_PREFIX)  Building user apps succesful\033[0m\n"

all: user_apps

### Generate C librairy ###
static_libc:
	@$(MAKE) static_lib --no-print-directory -C $(LIBC_DIR) VERBOSE=$(VERBOSE) QUIET_PREFIX="$(QUIET_PREFIX)"

### Generate CPP librairy ###
static_libstdcpp:
	@$(MAKE) static_lib --no-print-directory -C $(LIBSTDCPP_DIR) VERBOSE=$(VERBOSE) QUIET_PREFIX="$(QUIET_PREFIX)"

### Targets for apps ###
define APP_BUILD
dir_guard=@$$(MKDIR) -p $$(@D)

#### Compilation sources ####
$(1)_DIR=$$(addprefix apps/, $(1))
$(1)_SRC=$$(strip $$(call get-all-files-with-ext, $$($(1)_DIR), cpp) \
	        	  $$(call get-all-files-with-ext, $$($(1)_DIR), c) \
	        	  $$(call get-all-files-with-ext, $$($(1)_DIR), S))
	        
$(1)_OUT=$$(addprefix $(BUILD_DIR)/, $$($(1)_DIR))
$(1)_DEPS=$$(addprefix $(BUILD_DIR)/, $(call get-deps, $$($(1)_SRC)))
$(1)_OBJS=$$(addprefix $(BUILD_DIR)/, $(call get-objs, $$($(1)_SRC)))

$(1)_TARGET=$$(addprefix $(OUTPUT_DIR)/, $(1))

#### Include auto-generated dependencies ####
ifneq ($$(MAKECMDGOALS),clean)
ifneq ($$(MAKECMDGOALS),clean-all)
    -include $$($(1)_DEPS)
endif
endif

.PHONY: $(1)_APP
$(1)_APP:
	@$(ECHO) "\033[0;33m$(QUIET_PREFIX)  Building $(1) app \033[0m\n"
	@$(MAKE) $$($(1)_TARGET) --no-print-directory VERBOSE=$(VERBOSE) QUIET_PREFIX="$(QUIET_PREFIX)  "
	@$(ECHO) "\033[0;32m$(QUIET_PREFIX)  Building $(1) app succesful\033[0m\n"

#### Generate application ####
$$($(1)_TARGET): $$($(1)_OBJS) | $$(CRT_OBJS) static_libc static_libstdcpp
	$$(dir_guard)
	$$(LD) $$(LDFLAGS) $$^ $$(LIBFLAGS) -o $$@
endef

$(foreach app, $(APPS_NAMES), $(eval $(call APP_BUILD,$(app))))

APPS_TARGETS_MAIN := $(foreach app, $(APPS_NAMES), $(app)_APP)

user_apps_build: $(APPS_TARGETS_MAIN)

### Generic rules ###
$(eval $(call create-cpp-targets,$(CXXFLAGS),$(INCLUDES),$(BUILD_DIR)))
	
### Include auto-generated dependencies
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),clean-all)
    -include $(CRT_DEPS)
endif
endif

### Clean ###
.PHONY: clean clean-all
clean:
	@$(ECHO) "\033[0;32m  Clean user apps \033[0m\n"; rm -rf $(BUILD_DIR) $(OUTPUT_DIR)
	
clean-all: clean
	@$(MAKE) clean --no-print-directory -C $(LIBC_DIR) VERBOSE=$(VERBOSE)
	@$(MAKE) clean --no-print-directory -C $(LIBSTDCPP_DIR) VERBOSE=$(VERBOSE)
