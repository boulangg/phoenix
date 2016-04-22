# Copyright (c) 2016 Boulanger Guillaume, Chathura Namalgamuwa 
# The file is distributed under the MIT license
# The license is available in the LICENSE file or at https://github.com/boulangg/phoenix/blob/master/LICENSE

### TOOLS ###
TOOLCHAIN_PREFIX := $(HOME)/opt/cross/bin/x86_64-elf-
CC      := $(TOOLCHAIN_PREFIX)gcc
CXX		:= $(TOOLCHAIN_PREFIX)g++
AS      := $(TOOLCHAIN_PREFIX)gcc
LD      := $(TOOLCHAIN_PREFIX)g++
AR      := $(TOOLCHAIN_PREFIX)ar
OBJCOPY := $(TOOLCHAIN_PREFIX)objcopy
NM      := $(TOOLCHAIN_PREFIX)nm
DEP     := $(CC)
MKDIR   := mkdir
CP      := cp
ECHO    := echo
RM		:= rm

