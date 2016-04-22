
# clean-file-names(FILE): ./FILE -> FILE
define clean-file-names
$(strip $(patsubst ./%, %, $(strip $(1))))
endef

# clean-dir-names(DIR): DIR/ -> DIR
define clean-dir-names
$(patsubst %/, %, $(strip $(1)))
endef

# get-all-files-with-ext(DIR, EXT) -> DIR/*.EXT
define get-all-files-with-ext
$(call clean-file-names, \
	$(wildcard $(addsuffix /*.$(strip $(2)), $(call clean-dir-names, $(1)))) \
)
endef

# get-objs(FILE): FILE -> FILE.o
define get-objs
$(call clean-file-names, \
	$(patsubst %, %.o, $(strip $(1)))
)
endef

# get-deps(FILE): FILE -> FILE.d
define get-deps
$(call clean-file-names, \
	$(patsubst %, %.d, $(strip $(1)))
)
endef

# create-dir-file(FILE): DIR/FILE -> FILE: DIR
define create-dir-file
$(1): | $(patsubst %/, %, $(dir $(1)))
endef

# create-dir-files(FILES)
define create-dir-files
$(foreach file, $(1), $(eval 			\
	$(call create-dir-file, $(file)) \
))
endef

# create-dir-target(DIR): mkdir -p DIR
define create-dir-target
$(1):
	$$(MKDIR) -p $$@
endef

# create-dir-targets(DIRS)
define create-dir-targets
$(foreach file, $(1), $(eval 			\
	$(call create-dir-target, $(file)) \
))
endef
