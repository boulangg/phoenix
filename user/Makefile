# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

### Import utils ###
include ../build/utils.mk

### Compilation sources ###
SRC_DIRS 		:= test1 glue
INCLUDE_DIRS 	:= ../lib/libc/include
LIBRARY_DIRS	:= ../lib/libc
SRC 			:= $(call get-all-files-with-ext, $(SRC_DIRS), cpp) \
	               $(call get-all-files-with-ext, $(SRC_DIRS), c) \
	               $(call get-all-files-with-ext, $(SRC_DIRS), S)
INCLUDES 		:= $(addprefix -I, $(INCLUDE_DIRS))
LIBRARIES		:= $(addprefix -L, $(LIBRARY_DIRS))

CRT_DIR		:= init/x86_64
CRT_SRC		:= init/x86_64/crt0.S init/x86_64/crti.S init/x86_64/crtn.S

### Output directory ###
OUTPUT=out
APPS=test1.out

### Tool flags ###
CXXFLAGS	:= -Wall -Wextra \
		  	   -g -std=c++11 \
		  	   -fno-exceptions -fno-rtti \
		  	   -Wno-packed-bitfield-compat \
		  	   -fno-stack-protector
LDFLAGS 	:= -nodefaultlibs -g -specs=init/x86_64/specs
LIBFLAGS	:= $(LIBRARIES)  -lc

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

$(info $$CRT_OBJ is [${CRT_OBJ}])

.PHONY:all
all: $(APPS)

### Generate application ###
$(APPS): $(OBJS) | $(CRT_OBJ)
	$(LD) $(LDFLAGS) $^ $(LIBFLAGS) -o $@

### Generic rules ###
$(OUTPUT)/%.d: % | $(OUTPUT)
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