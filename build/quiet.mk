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

CC          = @$(ECHO) "  CC      $@"; $(QCC)
CXX			= @$(ECHO) "  CXX     $@"; $(QCXX)
AS          = @$(ECHO) "  AS      $@"; $(QAS)
LD          = @$(ECHO) "  LD      $@"; $(QLD)
AR          = @$(ECHO) "  AR      $@"; $(QAR)
OBJCOPY     = @$(ECHO) "  OBJCOPY $@"; $(QOBJCOPY)
NM          = @$(ECHO) "  NM      $@"; $(QNM)
DEP         = @$(ECHO) "  DEP     $@"; $(QDEP)
MKDIR       = @$(ECHO) "  MKDIR   $@"; $(QMKDIR)
CP          = @$(ECHO) "  CP      $< -> $@"; $(QCP)
