# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

### Import utils ###
include ../build/utils.mk

LIBC_DIR := ../lib/libc
LIBSTDCPP_DIR := ../lib/libstdcpp

### Compilation sources ###
SRC_DIRS 			:= glue
APPS_NAMES			:= $(notdir $(call clean-dir-names, $(sort $(dir $(wildcard apps/*/)))))
LIB_INCLUDE_DIRS 	:= $(LIBC_DIR)/include $(LIBSTDCPP_DIR)/include
INCLUDE_DIRS 		:= $(LIB_INCLUDE_DIRS)
LIBRARY_DIRS		:= ../lib/libc ../lib/libstdcpp
SRC 				:= $(call get-all-files-with-ext, $(SRC_DIRS), c) \
	               	   $(call get-all-files-with-ext, $(SRC_DIRS), cpp) \
	                   $(call get-all-files-with-ext, $(SRC_DIRS), S)
INCLUDES 			:= $(addprefix -I, $(INCLUDE_DIRS))
LIBRARIES			:= $(addprefix -L, $(LIBRARY_DIRS))

LIBC		:= $(LIBC_DIR)/libc.a
LIBSTDCPP 	:= $(LIBSTDCPP_DIR)/libstdcpp.a

CRT_DIR		:= init/x86_64
CRT_SRC		:= init/x86_64/crt0.S init/x86_64/crti.S init/x86_64/crtn.S

### Output directory ###
OUTPUT=out

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


### Output files ###
DEPS		:= $(addprefix $(OUTPUT)/, $(call get-deps, $(SRC)))
OBJS		:= $(addprefix $(OUTPUT)/, $(call get-objs, $(SRC)))
CRT_OBJ		:= $(addprefix $(OUTPUT)/, $(call get-objs, $(CRT_SRC)))
CRT_DEP		:= $(addprefix $(OUTPUT)/, $(call get-deps, $(CRT_SRC)))
OUTPUT_DIRS	:= $(addprefix $(OUTPUT)/, $(SRC_DIRS))  $(addprefix $(OUTPUT)/, $(CRT_DIR))

.PHONY:all user_apps static_libc static_libstdcpp
user_apps: all static_libc static_libstdcpp
	@$(ECHO) "\033[0;32m  User apps succesfully built\033[0m\n"

### Generate C librairy ###
static_libc:
	@$(ECHO) "\033[0;33m  Build libc.a\033[0m\n"
	@$(MAKE) static_lib --no-print-directory -C $(LIBC_DIR) VERBOSE=$(VERBOSE)

### Generate CPP librairy ###
static_libstdcpp:
	@$(ECHO) "\033[0;33m  Build libstdcpp.a\033[0m\n"
	@$(MAKE) static_lib --no-print-directory -C $(LIBSTDCPP_DIR) VERBOSE=$(VERBOSE)

### Import targets for apps ###
include build/apps.mk

all: $(APPS_TARGETS) 

### Generic rules ###
$(OUTPUT)/%.S.d: %.S | $(OUTPUT)
	$(DEP) $(INCLUDES) -MM $< -MT $@ > $@
$(OUTPUT)/%.c.d: %.c | $(OUTPUT)
	$(DEP) $(INCLUDES) -MM $< -MT $@ > $@
$(OUTPUT)/%.cpp.d: %.cpp | $(OUTPUT)
	$(DEP) $(INCLUDES) -MM $< -MT $@ > $@

$(OUTPUT)/%.o : % $(OUTPUT)/%.d | $(OUTPUT)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

### Add dependency between object file and the directory containing it ###
$(call create-dir-files, $(OBJS))
$(call create-dir-files, $(CRT_OBJ))
$(call create-dir-files, $(DEPS))
$(call create-dir-files, $(CRT_DEP))

### Create a target for output dirs ###
$(call create-dir-targets, $(OUTPUT_DIRS))
	
### Output directory ###
$(CRT_OBJ): | $(OUTPUT)
$(CRT_DEP): | $(OUTPUT)
$(OBJS): | $(OUTPUT)
$(DEPS): | $(OUTPUT)
$(OUTPUT):
	$(MKDIR) -p $@
	
### Include auto-generated dependencies
ifneq ($(MAKECMDGOALS),clean)
    -include $(DEPS) $(CRT_DEP)
endif

### Clean ###
.PHONY: clean
clean:
	rm -rf $(OUTPUT) $(BIN)
