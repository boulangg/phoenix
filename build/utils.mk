# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa 
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

dir_guard=@$(MKDIR) -p $(@D)

# clean-file-names(FILE): ./FILE -> FILE
define clean-file-names
$(strip $(patsubst ./%, %, $(strip $(1))))
endef

# clean-dir-names(DIR): DIR/ -> DIR
define clean-dir-names
$(patsubst %/, %, $(strip $(1)))
endef

# get-all-files -> recursive file search in DIR
define get-all-files
$(call clean-file-names,				\
	$(shell find $(strip $(1)) -name "*" -type f)\
)
endef

# get-all-files-with-ext(DIR, EXT) -> DIR/*.EXT
define get-all-files-with-ext
$(call clean-file-names, 		\
	$(shell find $(strip $(1)) -name "*.$(strip $(2))" -type f) \
)
endef

# get-objs(FILE): FILE -> FILE.o
define get-objs
$(call clean-file-names,\
	$(patsubst %, %.o, $(strip $(1)))
)
endef

# get-deps(FILE): FILE -> FILE.d
define get-deps
$(call clean-file-names,\
	$(patsubst %, %.d, $(strip $(1)))\
)
endef

# create-cpp-targets(CXXFLAGS, INCLUDES, OUTPUT_ROOT_DIR)
define create-cpp-targets
dir_guard=@$$(MKDIR) -p $$(@D)

### Generic rules ###
$(3)/%.S.d: %.S
	$$(dir_guard)
	$$(DEP) $(1) $(2) -MM $$< -MT $$@ > $$@
$(3)/%.c.d: %.c
	$$(dir_guard)
	$$(DEP) $(1) $(2) -MM $$< -MT $$@ > $$@
$(3)/%.cpp.d: %.cpp
	$$(dir_guard)
	$$(DEP) $(1) $(2) -MM $$< -MT $$@ > $$@

$(3)/%.o : % $(3)/%.d
	$$(CXX) $(1) $(2) -c $$< -o $$@

endef


# create-c-targets(CXXFLAGS, INCLUDES, OUTPUT_ROOT_DIR)
define create-c-targets
### Output files ###
dir_guard=@$$(MKDIR) -p $$(@D)

### Generic rules ###
$(3)/%.S.d: %.S
	$$(dir_guard)
	$$(DEP) $(1) $(2) -MM $$< -MT $$@ > $$@
$(3)/%.c.d: %.c
	$$(dir_guard)
	$$(DEP) $(1) $(2) -MM $$< -MT $$@ > $$@
$(3)/%.cpp.d: %.cpp
	$$(dir_guard)
	$$(DEP) $(1) $(2) -MM $$< -MT $$@ > $$@

$(3)/%.o : % $(3)/%.d
	$$(CC) $(1) $(2) -c $$< -o $$@

endef

