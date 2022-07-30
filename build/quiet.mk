# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa 
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

### QUIET TOOLS ###
QCC 		:= $(CC)
QCXX 		:= $(CXX)
QAS 		:= $(AS) 
QLD 		:= $(LD)
QAR 		:= $(AR)
QOBJCOPY 	:= $(OBJCOPY)
QNM 		:= $(NM)
QDEP 		:= $(DEP)
QMKDIR 		:= $(MKDIR)
QCP 		:= $(CP)

CC          = @$(ECHO) "$(QUIET_PREFIX)  CC      $@\n"; $(QCC)
CXX         = @$(ECHO) "$(QUIET_PREFIX)  CXX     $@\n"; $(QCXX)
AS          = @$(ECHO) "$(QUIET_PREFIX)  AS      $@\n"; $(QAS)
LD          = @$(ECHO) "$(QUIET_PREFIX)  LD      $@\n"; $(QLD)
AR          = @$(ECHO) "$(QUIET_PREFIX)  AR      $@\n"; $(QAR)
OBJCOPY     = @$(ECHO) "$(QUIET_PREFIX)  OBJCOPY $@\n"; $(QOBJCOPY)
NM          = @$(ECHO) "$(QUIET_PREFIX)  NM      $@\n"; $(QNM)
DEP         = @$(ECHO) "$(QUIET_PREFIX)  DEP     $@\n"; $(QDEP)
MKDIR       = @if [ ! -d $(@D) ]; then $(ECHO) "$(QUIET_PREFIX)  MKDIR   $(@D)\n"; fi; $(QMKDIR)
CP          = @$(ECHO) "$(QUIET_PREFIX)  CP      $< -> $@\n"; $(QCP)
