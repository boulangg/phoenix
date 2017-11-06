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

CC          = @$(ECHO) "  CC      $@\n"; $(QCC)
CXX         = @$(ECHO) "  CXX     $@\n"; $(QCXX)
AS          = @$(ECHO) "  AS      $@\n"; $(QAS)
LD          = @$(ECHO) "  LD      $@\n"; $(QLD)
AR          = @$(ECHO) "  AR      $@\n"; $(QAR)
OBJCOPY     = @$(ECHO) "  OBJCOPY $@\n"; $(QOBJCOPY)
NM          = @$(ECHO) "  NM      $@\n"; $(QNM)
DEP         = @$(ECHO) "  DEP     $@\n"; $(QDEP)
MKDIR       = @$(ECHO) "  MKDIR   $@\n"; $(QMKDIR)
CP          = @$(ECHO) "  CP      $< -> $@\n"; $(QCP)
