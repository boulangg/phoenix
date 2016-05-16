# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

### Import utils ###
include ../build/utils.mk

### Compilation sources ###
SRC_DIRS 		:= boot core lib/libc/src mm lib/libcpp/src lib/libc/src/stdio
INCLUDE_DIRS 	:= lib/libc/include lib/libcpp/include
SRC 			:= $(call get-all-files-with-ext, $(SRC_DIRS), cpp) \
	               $(call get-all-files-with-ext, $(SRC_DIRS), c) \
	               $(call get-all-files-with-ext, $(SRC_DIRS), S)
INCLUDES 		:= $(addprefix -I, $(INCLUDE_DIRS))

### Output directory ###
OUTPUT=out
BIN=phoenix.bin

### Tool flags ###
CXXFLAGS	:= -ffreestanding \
			   -Wall -Wextra \
		  	   -g -std=c++11 \
		  	   -fno-exceptions -fno-rtti \
		  	   -mcmodel=kernel -m64 \
		  	   -Wno-packed-bitfield-compat
LDFLAGS 	:= -ffreestanding \
		   	   -g -nostdlib \
		  	   -mcmodel=large
#CFLAGS 		:= -ffreestanding \
#		  	   -Wall -Wextra \
#		  	   -g -std=c11
#ASFLAGS 	:= 

### Inmport tools ###
include ../build/tools.mk

### Define quiet tools ###
ifneq "$(VERBOSE)" "1"
    include ../build/quiet.mk
endif


### Output files ###
DEPS		:= $(addprefix $(OUTPUT)/, $(call get-deps, $(SRC)))
OBJS		:= $(addprefix $(OUTPUT)/, $(call get-objs, $(SRC)))
OUTPUT_DIRS	:= $(addprefix $(OUTPUT)/, $(SRC_DIRS))

### Generate kernel ###
$(BIN): build/linker.ld $(OBJS)
	$(LD) $(LDFLAGS) -T$< $(filter-out %.ld, $^) -o $@

### Generic rules ###
$(OUTPUT)/%.d: % | $(OUTPUT)
	$(DEP) $(INCLUDES) -MM $< -MT $@ > $@

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