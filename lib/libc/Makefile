# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

### Import utils ###
include ../../build/utils.mk

### Compilation sources ###
SRC_DIRS 		:= src/stdlib src/math src/posix \
				   src/stdio src/errno src/ctype \
				   src/string glue src/gcc
INCLUDE_DIRS 	:= include
SRC 			:= $(call get-all-files-with-ext, $(SRC_DIRS), c) \
	               $(call get-all-files-with-ext, $(SRC_DIRS), S)
INCLUDES 		:= $(addprefix -I, $(INCLUDE_DIRS))

### Output directory ###
OUTPUT=out
STATIC_LIB=libc.a

### Tool flags ###
CFLAGS	:= -Wall -Wextra -g -std=c11 -mcmodel=kernel -m64 
ARFLAGS	:= rcs

### Inmport tools ###
include ../../build/tools.mk

### Define quiet tools ###
ifneq "$(VERBOSE)" "1"
    include ../../build/quiet.mk
endif

.PHONY:all static_lib
static_lib: all
	@echo "\033[0;32m  libc.a succesfully built\033[0m"
	
all: $(STATIC_LIB)

### Output files ###
DEPS		:= $(addprefix $(OUTPUT)/, $(call get-deps, $(SRC)))
OBJS		:= $(addprefix $(OUTPUT)/, $(call get-objs, $(SRC)))
OUTPUT_DIRS	:= $(addprefix $(OUTPUT)/, $(SRC_DIRS))

### Generate library ###
$(STATIC_LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

### Generic rules ###
$(OUTPUT)/%.d: % | $(OUTPUT)
	$(DEP) $(INCLUDES) -MM $< -MT $@ > $@

$(OUTPUT)/%.o : % $(OUTPUT)/%.d | $(OUTPUT)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

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
	rm -rf $(OUTPUT) $(STATIC_LIB)