# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

define APP_BUILD

$(1)_DIR=$$(addprefix apps/, $(1))
$(1)_SRC=$$(strip $$(call get-all-files-with-ext, $$($(1)_DIR), cpp) \
	        	  $$(call get-all-files-with-ext, $$($(1)_DIR), c) \
	        	  $$(call get-all-files-with-ext, $$($(1)_DIR), S))
	        
$(1)_OUT=$$(addprefix $(OUTPUT)/, $$($(1)_DIR))
$(1)_DEPS=$$(addprefix $(OUTPUT)/, $(call get-deps, $$($(1)_SRC)))
$(1)_OBJS=$$(addprefix $(OUTPUT)/, $(call get-objs, $$($(1)_SRC)))

$(1)_TARGET=$$(addprefix $(OUTPUT)/, $$(addsuffix .out, $$($(1)_DIR)))

### Include auto-generated dependencies
ifneq ($$(MAKECMDGOALS),clean)
    -include $$($(1)_DEPS)
endif

### Add dependency between object file and the directory containing it ###
$$($(1)_OBJS): | $$($(1)_OUT)
$$($(1)_DEPS): | $$($(1)_OUT)

### Create a target for output dirs ###
$$($(1)_OUT):
	$$(MKDIR) -p $$@

### Generate application ###
$$($(1)_TARGET): $$($(1)_OBJS) | $$(CRT_OBJ)
	$$(LD) $$(LDFLAGS) $$^ $$(LIBFLAGS) -o $$@

endef

APPS_TARGETS := $(addprefix $(OUTPUT)/apps/, $(addsuffix .out, $(APPS_NAMES)))

#$(info $$APPS_TARGETS is [${APPS_TARGETS}])

$(foreach app, $(APPS_NAMES), $(eval $(call APP_BUILD,$(app))))
