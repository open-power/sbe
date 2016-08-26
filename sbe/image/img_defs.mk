# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: sbe/image/img_defs.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2016
# [+] International Business Machines Corp.
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied. See the License for the specific language governing
# permissions and limitations under the License.
#
# IBM_PROLOG_END_TAG
#  Make header for SBE image build
#
# Settings to for SBE image file build
#
# IMG_INCLUDES       : Aplication-specific header search paths
#
# DEFS               : A string of -D<symbol>[=<value>] to control compilation
#
# PK                : Default ..; The path to the PK source code.
#                      The default is set for building the PK
#                      subdirectories.
#
# PK_THREAD_SUPPORT : (0/1, default 1); Compile PK thread and
#                      semaphore suppprt
#
# PK_TIMER_SUPPORT  : (0/1, default 1); Compile PK timer suppprt
#
# GCC-O-LEVEL        : The optimization level passed to GCC (default -Os). May
#                      also be defined empty (GCC-O-LEVEL=) to disable
#                      optimization. This variable can also be used to pass
#                      any other non-default setting to GCC, e.g.
#                      make GCC-O-LEVEL="-Os -fno-branch-count-reg"
#
# GCC-TOOL-PREFIX    : The full path (including executable file prefixes) to
#                      the GCC cross-development tools to use.  The default is
#                      "ppcnf-mcp5-"
#
# CTEPATH            : This variable defaults to the afs/awd CTE tool
#                      installation - The PORE binutils are stored there. If
#                      you are not in Austin be sure to define CTEPATH in
#                      your .profile.
#
# OBJDIR             : target directory for all generated files

IMAGE_SEEPROM_NAME := sbe_seeprom
IMAGE_SBE_NAME := sbe_pibmem
IMAGE_LOADER_NAME := sbe_loader
IMAGE_OTPROM_NAME := sbe_otprom

SBE_SYMBOLS_NAME := sbe.syms

ifndef PPE_TYPE
PPE_TYPE := std
endif

ifndef SBE_ROOT_DIR
export SBE_ROOT_DIR = $(abspath ../..)
endif

ifndef SBE_SRC_DIR
export SBE_SRC_DIR = $(SBE_ROOT_DIR)/sbe
endif

ifndef SBE_FW_DIR
export SBE_FW_DIR = $(SBE_SRC_DIR)/sbefw
endif

ifndef IMAGE_SRCDIR
export IMAGE_SRCDIR = $(SBE_SRC_DIR)/image
endif

ifndef BOOT_SRCDIR
export BOOT_SRCDIR = $(SBE_SRC_DIR)/boot
endif

ifndef IMPORT_SRCDIR
export IMPORT_SRCDIR = $(SBE_ROOT_DIR)/import
endif

ifndef TOOLS_SRCDIR
export TOOLS_SRCDIR = $(SBE_ROOT_DIR)/tools
endif

ifndef UTILS_SRCDIR
export UTILS_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/utils
endif

ifndef CACHE_SRCDIR
export CACHE_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/hwp/cache
endif

ifndef CORE_SRCDIR
export CORE_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/hwp/core
endif

ifndef PERV_SRCDIR
export PERV_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/hwp/perv
endif

ifndef HWPERR_SRCDIR
export HWPERR_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/xml/error_info
endif

ifndef NEST_SRCDIR
export NEST_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/hwp/nest
endif

ifndef PM_SRCDIR
export PM_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/hwp/pm
endif

ifndef INITFILES_SRCDIR
export INITFILES_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/hwp/initfiles
endif

ifndef HWPLIB_SRCDIR
export HWPLIB_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/hwp/lib
endif

ifndef IMAGEPROCS_SRCDIR
export IMAGEPROCS_SRCDIR = $(IMPORT_SRCDIR)/tools/imageProcs
endif

ifndef BASE_OBJDIR
export BASE_OBJDIR = $(SBE_SRC_DIR)/obj
endif

ifndef P9_XIP_SRCDIR
export P9_XIP_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/xip
endif

ifndef P9_XIP_BINDIR
export P9_XIP_BINDIR = $(BASE_OBJDIR)/xip
endif

ifndef IMG_INCLUDES
export IMG_INCLUDES = -I$(IMAGEPROCS_SRCDIR) -I$(P9_XIP_SRCDIR) -I$(IMAGE_SRCDIR) -I$(CACHE_SRCDIR) -I$(UTILS_SRCDIR) -I$(CORE_SRCDIR) -I$(PERV_SRCDIR) -I$(NEST_SRCDIR) -I$(PM_SRCDIR) -I$(INITFILES_SRCDIR)
endif

ifndef BASE_OBJDIR
export BASE_OBJDIR = $(SBE_SRC_DIR)/obj
endif

ifndef BOOT_OBJDIR
export BOOT_OBJDIR = $(BASE_OBJDIR)/boot
endif

export IMG_OBJDIR = $(BASE_OBJDIR)/$(IMAGE_SEEPROM_NAME)

ifndef PK_SRCDIR
export PK_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/ppe/pk
endif


ifndef TOOLS_ATTR_DIR
export TOOLS_ATTR_DIR = $(TOOLS_SRCDIR)/scripts
endif

ifndef TOOLS_IMAGE_DIR
export TOOLS_IMAGE_DIR = $(TOOLS_SRCDIR)/image
endif

ifndef IMPORT_XML_DIR
export IMPORT_XML_DIR = $(IMPORT_SRCDIR)/chips/p9/procedures/xml
endif

ifndef IMPORT_UTILS_DIR
export IMPORT_UTILS_DIR = $(IMPORT_SRCDIR)/chips/p9/utils
endif

ifndef IMPORT_COMMON_DIR
export IMPORT_COMMON_DIR = $(IMPORT_SRCDIR)/chips/p9/common
endif

ifndef PPETRACEPP_DIR
export PPETRACEPP_DIR = $(SBE_ROOT_DIR)/tools/ppetracepp
endif

ifndef PPETRACEPP_BIN_DIR
export PPETRACEPP_BIN_DIR = $(BASE_OBJDIR)
endif

ifndef PLAT_FAPI2_DIR
export PLAT_FAPI2_DIR = $(SBE_SRC_DIR)/hwpf
endif

ifndef BASE_FAPI2_DIR
export BASE_FAPI2_DIR = $(IMPORT_SRCDIR)/hwpf/fapi2
endif

ifndef FAPI2_TOOLS_DIR
export FAPI2_TOOLS_DIR = $(IMPORT_SRCDIR)/hwpf/fapi2/tools/
endif

ifdef P2P_ENABLE
ifndef CC_ROOT
export CC_ROOT = ${CTEPATH}/tools/gcc405lin/prod
endif
endif

GCC-TOOL-PATH = $(CTEPATH)/tools/ppetools/prod


ifndef GCC-TOOL-PREFIX
GCC-TOOL-PREFIX = $(GCC-TOOL-PATH)/bin/powerpc-eabi-
endif

ifndef BINUTILS-TOOL-PREFIX
BINUTILS-TOOL-PREFIX = $(CTEPATH)/tools/ppetools/prod/powerpc-eabi/bin/
endif

ifndef FAPI_RC
FAPI_RC = hwp_return_codes.H
#FAPI_RC =
endif

OBJDIR = $(BASE_OBJDIR)$(SUB_OBJDIR)

CC_ASM  = $(GCC-TOOL-PREFIX)gcc
#TCC     = $(PPETRACEPP_DIR)/ppetracepp $(GCC-TOOL-PREFIX)g++
TCC     = $(PPETRACEPP_BIN_DIR)/ppetracepp $(GCC-TOOL-PREFIX)gcc
CC      = $(PPETRACEPP_BIN_DIR)/ppetracepp $(GCC-TOOL-PREFIX)gcc
AS      = $(BINUTILS-TOOL-PREFIX)as
AR      = $(BINUTILS-TOOL-PREFIX)ar
LD      = $(BINUTILS-TOOL-PREFIX)ld
NM      = $(BINUTILS-TOOL-PREFIX)nm -S
OBJDUMP = $(BINUTILS-TOOL-PREFIX)objdump
OBJCOPY = $(BINUTILS-TOOL-PREFIX)objcopy
TCPP    = $(PPETRACEPP_BIN_DIR)/ppetracepp $(GCC-TOOL-PREFIX)gcc
THASH	= $(PPETRACEPP_DIR)/tracehash.pl
CPP     = $(GCC-TOOL-PREFIX)gcc

ifdef P2P_ENABLE
PCP     = $(P2P_SRCDIR)/ppc-ppe-pcp.py
endif


ifndef CTEPATH
$(warning The CTEPATH variable is not defined; Defaulting to /afs/awd)
export CTEPATH = /afs/awd/projects/cte
endif

ifeq "$(PK_TIMER_SUPPORT)" ""
PK_TIMER_SUPPORT  = 1
endif

ifeq "$(PK_THREAD_SUPPORT)" ""
PK_THREAD_SUPPORT = 1
endif

ifeq "$(PK_TRACE_SUPPORT)" ""
PK_TRACE_SUPPORT = 1
endif

ifdef SBE_ISTEP_STUBBED
GCC-DEFS += -DSBE_ISTEP_STUBBED=1
endif

ifdef SIM_ONLY_OSC_SWC_CHK
GCC-DEFS += -DSIM_ONLY_OSC_SWC_CHK=1
endif

ifdef __FAPI_DELAY_SIM__
GCC-DEFS += -D__FAPI_DELAY_SIM__=1
endif

# Generate a 16bit trace string hash prefix value based on the name of this image.  This will form
# the upper 16 bits of the 32 bit trace hash values.
ifndef PK_TRACE_HASH_PREFIX
PK_TRACE_HASH_PREFIX := $(shell echo $(IMAGE_SEEPROM_NAME) | md5sum | cut -c1-4 | xargs -i printf "%d" 0x{})
endif

ifndef GCC-O-LEVEL
ifdef P2P_ENABLE
GCC-O-LEVEL = -O
#GCC-O-LEVEL = -O -g
else
GCC-O-LEVEL = -Os
#GCC-O-LEVEL = -Os -g
endif
endif

FAPI_TRACE_LEVEL_DEF = 4
ifdef FAPI_TRACE_LEVEL_ENV
FAPI_TRACE_LEVEL_DEF = $(FAPI_TRACE_LEVEL_ENV)
endif

# Levels of SBE logging
# 0 - No tracing
# 1 - Error
# 2 - Error, info
# 3 - Error, info, entry/exit
# 4 - Error, info, entry/exit, debug
SBE_TRACE_LEVEL_DEF = 3

GCC-DEFS += -DIMAGE_NAME=$(IMAGE_SEEPROM_NAME)
GCC-DEFS += -DPK_TIMER_SUPPORT=$(PK_TIMER_SUPPORT)
GCC-DEFS += -DPK_THREAD_SUPPORT=$(PK_THREAD_SUPPORT)
GCC-DEFS += -DPK_TRACE_SUPPORT=$(PK_TRACE_SUPPORT)
GCC-DEFS += -DPK_TRACE_HASH_PREFIX=$(PK_TRACE_HASH_PREFIX)
GCC-DEFS += -DUSE_PK_APP_CFG_H=1
GCC-DEFS += -D__PK__=1
GCC-DEFS += -D__SBE__=1
GCC-DEFS += -D__PPE__=1
GCC-DEFS += -DMINIMUM_FFDC=1
GCC-DEFS += -DFAPI_TRACE_LEVEL=$(FAPI_TRACE_LEVEL_DEF)
GCC-DEFS += -DSBE_TRACE_LEVEL=$(SBE_TRACE_LEVEL_DEF)
# use the default settings in the code unless a size is defined
ifdef PK_TRACE_SZ
GCC-DEFS += -DPK_TRACE_SZ=$(PK_TRACE_SZ)
endif

DEFS += $(GCC-DEFS)
export LD_LIBRARY_PATH+=:$(GCC-TOOL-PATH)/lib
############################################################################


INCLUDES += $(IMG_INCLUDES)
INCLUDES += -I$(IMAGE_SRCDIR)/../../include
INCLUDES += -I$(HWPLIB_SRCDIR)
INCLUDES += -I$(PLAT_FAPI2_DIR)/include/plat
INCLUDES += -I$(PLAT_FAPI2_DIR)/include
INCLUDES += -I$(BASE_FAPI2_DIR)/include
INCLUDES += -I$(PK_SRCDIR)/../include
INCLUDES += -I$(PK_SRCDIR)/$(PPE_TYPE)
INCLUDES += -I$(PK_SRCDIR)/../include
INCLUDES += -I$(PK_SRCDIR)/../include/std
INCLUDES += -I$(PK_SRCDIR)/kernel
INCLUDES += -I$(PK_SRCDIR)/ppe
INCLUDES += -I$(PK_SRCDIR)/ppe42
INCLUDES += -I$(SBE_FW_DIR)
INCLUDES += -I$(BOOT_SRCDIR)
INCLUDES += -I$(PK_SRCDIR)/trace
INCLUDES += -I$(SBE_ROOT_DIR)/tools/ppetracepp
INCLUDES += -I$(IMPORT_COMMON_DIR)/include
INCLUDES += -I$(IMPORT_UTILS_DIR)/imageProcs
INCLUDES += -I$(IMPORT_UTILS_DIR)/

GCC-CFLAGS += -Wall -Werror -Wno-unused-label
GCC-CFLAGS += -msoft-float
GCC-CFLAGS += -meabi -msdata=eabi
GCC-CFLAGS += -gpubnames -gdwarf-3
GCC-CFLAGS += -ffreestanding
GCC-CFLAGS += -fno-common
GCC-CFLAGS += -fno-exceptions
GCC-CFLAGS += -fsigned-char
GCC-CFLAGS += -fno-inline-functions-called-once

ifdef P2P_ENABLE
## Flags specific to 405 compiler with PowerPc to PPE backend
PIPE-CFLAGS = -pipe -Wa,-m405

GCC-CFLAGS +=  -mcpu=405  -mmulhw
GCC-CFLAGS += -ffixed-r11
GCC-CFLAGS += -ffixed-r12
GCC-CFLAGS += -ffixed-r14
GCC-CFLAGS += -ffixed-r15
GCC-CFLAGS += -ffixed-r16
GCC-CFLAGS += -ffixed-r17
GCC-CFLAGS += -ffixed-r18
GCC-CFLAGS += -ffixed-r19
GCC-CFLAGS += -ffixed-r20
GCC-CFLAGS += -ffixed-r21
GCC-CFLAGS += -ffixed-r22
GCC-CFLAGS += -ffixed-r23
GCC-CFLAGS += -ffixed-r24
GCC-CFLAGS += -ffixed-r25
GCC-CFLAGS += -ffixed-r26
GCC-CFLAGS += -ffixed-r27
GCC-CFLAGS += -ffixed-cr1
GCC-CFLAGS += -ffixed-cr2
GCC-CFLAGS += -ffixed-cr3
GCC-CFLAGS += -ffixed-cr4
GCC-CFLAGS += -ffixed-cr5
GCC-CFLAGS += -ffixed-cr6
GCC-CFLAGS += -ffixed-cr7
else
## Flags specific to ppe42 compiler
PIPE-CFLAGS = -pipe

GCC-CFLAGS += -mcpu=ppe42
GCC-CFLAGS += -ffunction-sections
GCC-CFLAGS += -fdata-sections
endif

CFLAGS =
PPE-CFLAGS = $(CFLAGS) -c $(GCC-CFLAGS) $(PIPE-CFLAGS) $(GCC-O-LEVEL) $(INCLUDES)

CXXFLAGS        = -std=c++11 -nostdinc++ -fno-rtti -fno-threadsafe-statics -fno-strict-aliasing
CPPFLAGS    	= -E

ASFLAGS		= -mppe42

ifdef P2P_ENABLE
#use this to disable optimizations (fused compare/branch etc.)
PCP-FLAG    =  -b
endif

############################################################################

#override the GNU Make implicit rule for going from a .C to a .o
%.o: %.C

# -Wno-conversion-null is necesary to allow mapping of NULL to TARGET_TYPE_SYSTEM
#   for attribute accesses
$(OBJDIR)/%.s: %.C
	$(TCC) $(PPE-CFLAGS) $(DEFS) -Wno-conversion-null -S $(CXXFLAGS) -o $@ $<


#override the GNU Make implicit rule for going from a .c to a .o
%.o: %.c

$(OBJDIR)/%.s: %.c
	$(CC) $(PPE-CFLAGS) $(DEFS) -S -o $@ $<

#override the GNU Make implicit rule for going from a .S to a .o
%.o: %.S

$(OBJDIR)/%.s: %.S
	$(TCPP) $(PPE-CFLAGS) $(DEFS) $(CPPFLAGS) -o $@ $<
.PRECIOUS: $(OBJDIR)/%.s

ifndef P2P_ENABLE

$(OBJDIR)/%.o: $(OBJDIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

else

$(OBJDIR)/%.es: $(OBJDIR)/%.s
	$(PCP) $(PCP-FLAG) -f $<
.PRECIOUS: $(OBJDIR)/%.es

$(OBJDIR)/%.o: $(OBJDIR)/%.es
	$(AS) $(ASFLAGS) -o $@ $<

endif

# From the GNU 'Make' manual - these scripts uses the preprocessor to
# create dependency files (*.d), then mungs them slightly to make them
# work as Make targets. The *.d files are include-ed in the
# subdirectory Makefiles.

$(OBJDIR)/%.d: %.C $(FAPI_RC)
	@set -e; rm -f $@; \
	echo -n "$(OBJDIR)/" > $@.$$$$; \
	$(CC_ASM) -MM $(INCLUDES) $(CPPFLAGS) $(DEFS) $< >> $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJDIR)/%.d: %.c $(FAPI_RC)
	@set -e; rm -f $@; \
	echo -n "$(OBJDIR)/" > $@.$$$$; \
	echo "$(INCLUDES)"; \
	$(CC_ASM) -MM $(INCLUDES) $(CPPFLAGS) $(DEFS) $< >> $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJDIR)/%.d: %.S $(FAPI_RC)
	@set -e; rm -f $@; \
	echo -n "$(OBJDIR)/" > $@.$$$$; \
	$(CC_ASM) -MM $(INCLUDES) $(CPPFLAGS) $(DEFS) $< >> $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

