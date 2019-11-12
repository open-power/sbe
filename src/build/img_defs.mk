# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/img_defs.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2019
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
# CROSS_COMPILER_PATH : Cross-development tool chain path.
#                       From the SBE package, make must be called like this:
#                       make LD_LIBRARY_PATH=$(HOST_DIR)/usr/lib CROSS_COMPILER_PATH=$(PPE42_GCC_BIN)
#                       Defaults to IBM CTE tools path.
#
# OBJDIR             : target directory for all generated files
ifndef PPE_TYPE
PPE_TYPE := std
endif

ifndef SBE_ROOT_DIR
export SBE_ROOT_DIR = $(abspath ../..)
endif

ifndef SBE_SRC_DIR
export SBE_SRC_DIR = $(SBE_ROOT_DIR)/src
endif

ifndef SBE_FW_DIR
export SBE_FW_DIR = $(SBE_SRC_DIR)/sbefw
endif

ifndef BUILD_DIR
export BUILD_DIR = $(SBE_SRC_DIR)/build
endif

# Required for pk/std/makefile
ifndef IMAGE_SRCDIR
export IMAGE_SRCDIR = $(BUILD_DIR)
endif

ifndef BUILDDATA_SRCDIR
export BUILDDATA_SRCDIR = $(BUILD_DIR)/utils
endif

ifndef LINKER_DIR
export LINKER_DIR = $(BUILD_DIR)/linkerscripts/$(project)
endif

ifndef IMPORT_HWP_MK_DIR
export IMPORT_HWP_MK_DIR = $(BUILD_DIR)/import_hwp_mk/$(project)
endif

ifndef MEASUREMENT_SRCDIR
export MEASUREMENT_SRCDIR = $(SBE_SRC_DIR)/sbefw/measurement
endif

ifndef BOOT_SRCDIR
export BOOT_SRCDIR = $(SBE_SRC_DIR)/boot
endif

ifndef IMPORT_SRCDIR
export IMPORT_SRCDIR = $(SBE_SRC_DIR)/import
endif

ifndef TOOLS_SRCDIR
export TOOLS_SRCDIR = $(SBE_SRC_DIR)/tools
endif

ifndef TEST_SRCDIR
export TEST_SRCDIR = $(SBE_SRC_DIR)/test/testcases
endif

ifndef DEBUGTOOLS_SRCDIR
export DEBUGTOOLS_SRCDIR = $(TOOLS_SRCDIR)/debug
endif

ifndef SIMICSTOOLS_SRCDIR
export SIMICSTOOLS_SRCDIR = $(DEBUGTOOLS_SRCDIR)/simics
endif

ifndef CACHE_SRCDIR
export CACHE_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/hwp/cache
endif

ifndef MEMHWP_SRCDIR
export MEMHWP_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/hwp/memory/lib/workarounds
endif

ifndef CORE_SRCDIR
export CORE_SRCDIR = $(IMPORT_SRCDIR)/chips/p10/procedures/hwp/core
endif

ifndef PERV_SRCDIR
export PERV_SRCDIR = $(IMPORT_SRCDIR)/chips/p10/procedures/hwp/perv
endif

ifndef P9_HWPERR_SRCDIR
export P9_HWPERR_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/xml/error_info
endif

ifndef HWPERR_SRCDIR
export HWPERR_SRCDIR = $(IMPORT_SRCDIR)/chips/p10/procedures/xml/error_info
endif

ifndef P9_NEST_SRCDIR
export P9_NEST_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/hwp/nest
endif

ifndef NEST_SRCDIR
export NEST_SRCDIR = $(IMPORT_SRCDIR)/chips/p10/procedures/hwp/nest
endif

ifndef CORECACHE_SRCDIR
export CORECACHE_SRCDIR = $(IMPORT_SRCDIR)/chips/p10/procedures/hwp/corecache
endif

ifndef IO_SRCDIR
export IO_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/hwp/io
endif

ifndef PM_SRCDIR
export PM_SRCDIR = $(IMPORT_SRCDIR)/chips/p10/procedures/hwp/pm
endif

ifndef INITFILES_SRCDIR
export INITFILES_SRCDIR = $(IMPORT_SRCDIR)/chips/p10/procedures/hwp/initfiles
endif

ifndef HWPLIB_SRCDIR
export HWPLIB_SRCDIR = $(IMPORT_SRCDIR)/chips/p10/procedures/hwp/lib
endif

ifndef P9_HWPLIB_SRCDIR
export P9_HWPLIB_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/hwp/lib
endif

ifndef HWPFFDC_SRCDIR
export HWPFFDC_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/procedures/hwp/ffdc
endif

ifndef IMAGEPROCS_TOOL_DIR
export IMAGEPROCS_TOOL_DIR = $(IMPORT_SRCDIR)/tools/imageProcs
endif

ifndef IMAGEPROCS_COMMON_SRCDIR
export IMAGEPROCS_COMMON_SRCDIR = $(IMPORT_SRCDIR)/chips/common/utils/imageProcs
endif

ifndef IMAGEPROCS_COMMON_SCOM_SRCDIR
export IMAGEPROCS_COMMON_SCOM_SRCDIR = $(IMPORT_SRCDIR)/chips/common/utils/scomt
endif

ifndef IMAGEPROCS_P9_SRCDIR
export IMAGEPROCS_P9_SRCDIR = $(IMPORT_SRCDIR)/chips/p9/utils/imageProcs
endif

ifndef IMAGEPROCS_P10_SRCDIR
export IMAGEPROCS_P10_SRCDIR = $(IMPORT_SRCDIR)/chips/p10/utils/imageProcs
endif

ifndef IMAGEPROCS_COMMON_INCDIR
export IMAGEPROCS_COMMON_INCDIR = $(IMPORT_SRCDIR)/chips/p10/common/include/
endif

ifndef BASE_OBJDIR
export BASE_OBJDIR = $(SBE_ROOT_DIR)/obj/$(project)
endif

ifndef TAR_OBJDIR
export TAR_OBJDIR = $(SBE_ROOT_DIR)/obj/simics-tar-dir
endif

ifndef P10_IMAGE_TOOL_BINDIR
export P10_IMAGE_TOOL_BINDIR = $(BASE_OBJDIR)/image_tool
endif

ifndef IMG_INCLUDES
export IMG_INCLUDES = -I$(IMAGEPROCS_COMMON_SRCDIR) -I$(IMAGEPROCS_P9_SRCDIR) -I$(IMAGEPROCS_P10_SRCDIR) -I$(IMAGEPROCS_CEN_SRCDIR) -I$(IMAGEPROCS_TOOL_DIR) -I$(BUILD_DIR) -I$(CACHE_SRCDIR) -I$(CORE_SRCDIR) -I$(PERV_SRCDIR) -I$(NEST_SRCDIR) -I$(P9_NEST_SRCDIR) -I$(IO_SRCDIR) -I$(PM_SRCDIR) -I$(INITFILES_SRCDIR) -I$(HWPLIB_SRCDIR) -I$(P9_HWPLIB_SRCDIR) -I$(HWPFFDC_SRCDIR) -I$(MEMHWP_SRCDIR) -I$(IMAGEPROCS_COMMON_SCOM_SRCDIR) -I$(IMAGEPROCS_COMMON_INCDIR) -I$(CORECACHE_SRCDIR)
endif

ifndef BOOT_OBJDIR
export BOOT_OBJDIR = $(BASE_OBJDIR)/boot
endif

ifndef BUILDDATA_OBJDIR
export BUILDDATA_OBJDIR = $(BASE_OBJDIR)/build/utils
endif

ifndef IMG_ROOT_DIR
export IMG_ROOT_DIR = $(SBE_ROOT_DIR)/images
endif

ifndef IMG_DIR
export IMG_DIR = $(IMG_ROOT_DIR)
endif

ifndef PK_BASELIB_SRCDIR
export PK_BASELIB_SRCDIR = $(IMPORT_SRCDIR)/chips/p10/common/ppe/baselib
endif

ifndef PK_SRCDIR
export PK_SRCDIR = $(IMPORT_SRCDIR)/chips/p10/common/ppe/pk
endif


ifndef TOOLS_ATTR_DIR
export TOOLS_ATTR_DIR = $(TOOLS_SRCDIR)/scripts
endif

ifndef IMPORT_TOOLS_DIR
export IMPORT_TOOLS_DIR = $(IMPORT_SRCDIR)/hwpf/fapi2/tools
endif

ifndef TOOLS_IMAGE_DIR
export TOOLS_IMAGE_DIR = $(TOOLS_SRCDIR)/image
endif

ifndef IPL_BUILD_DIR
export IPL_BUILD_DIR = $(TOOLS_SRCDIR)/iplbuild
endif

ifndef IPL_IMAGE_BUILD_DIR
export IPL_IMAGE_BUILD_DIR = $(TOOLS_SRCDIR)/imagetool
endif

ifndef IMPORT_XML_DIR
export IMPORT_XML_DIR = $(IMPORT_SRCDIR)/chips/p10/procedures/xml
endif

ifndef IMPORT_UTILS_DIR
export IMPORT_UTILS_DIR = $(IMPORT_SRCDIR)/chips/p9/utils
endif

ifndef IMPORT_COMMON_DIRS
export IMPORT_COMMON_DIRS = $(IMPORT_SRCDIR)/chips/p9/common $(IMPORT_SRCDIR)/chips/p10/common
endif

ifndef PPETRACEPP_DIR
export PPETRACEPP_DIR = $(TOOLS_SRCDIR)/trace
endif

ifndef PPETRACEPP_BIN_DIR
export PPETRACEPP_BIN_DIR = $(BASE_OBJDIR)/genfiles
endif

ifndef GENFILES_DIR
export GENFILES_DIR = $(BASE_OBJDIR)/genfiles
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

ifndef IMPORT_OBJDIR
export IMPORT_OBJDIR= import_hwp_mk
endif

ifndef SECURITY_SRC_DIR
export SECURITY_SRC_DIR = $(BUILD_DIR)/security
endif

ifndef SECURITY_LIST
export SECURITY_LIST = $(IMPORT_SRCDIR)/chips/p9/security/p9_security_white_black_list.csv
endif

ifndef CROSS_COMPILER_PATH
$(warning The CROSS_COMPILER_PATH variable is not defined; Defaulting to IBM CTE tools path)
export CROSS_COMPILER_PATH = /afs/awd/projects/cte/tools/ppetools/prod
endif

ifndef GCC-TOOL-PREFIX
GCC-TOOL-PREFIX = $(CROSS_COMPILER_PATH)/bin/powerpc-eabi-
endif

ifndef BINUTILS-TOOL-PREFIX
BINUTILS-TOOL-PREFIX = $(CROSS_COMPILER_PATH)/powerpc-eabi/bin/
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

ifdef __AWAN_SIM_ENV__
GCC-DEFS += -D__FAPI_DELAY_SIM__=1
# allow invalid scoms in AWAN run to let regression test cases run
GCC-DEFS += -D__ALLOW_INVALID_SCOMS__=1
GCC-DEFS += -DSIM_DPLL_LOCK_CHK=1
endif

ifeq ($(AWAN_NMZ_MODEL), 1)
GCC-DEFS += -DPFET_SENSE_POLL_DISABLE
endif

# Generate a 16bit trace string hash prefix value based on the name of this image.  This will form
# the upper 16 bits of the 32 bit trace hash values.
ifndef PK_TRACE_HASH_PREFIX
PK_TRACE_HASH_PREFIX := $(shell echo "SBE_MAIN" | md5sum | cut -c1-4 | xargs -i printf "%d" 0x{})
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

ifdef FAPI_TRACE_LEVEL_ENV
FAPI_TRACE_LEVEL_DEF = $(FAPI_TRACE_LEVEL_ENV)
endif


GCC-DEFS += -DIMAGE_NAME=SBE_TRACE
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
GCC-DEFS += -DPLAT_NO_THREAD_LOCAL_STORAGE=1
# disable assert
GCC-DEFS += -DNDEBUG
GCC-DEFS += -DHOST_INTERFACE_AVAILABLE=$(HOST_INTERFACE_AVAILABLE)
GCC-DEFS += -DPERIODIC_IO_TOGGLE_SUPPORTED=$(PERIODIC_IO_TOGGLE_SUPPORTED)

# use the default settings in the code unless a size is defined
ifdef PK_TRACE_SZ
GCC-DEFS += -DPK_TRACE_SZ=$(PK_TRACE_SZ)
endif

export LD_LIBRARY_PATH+=:$(CROSS_COMPILER_PATH)/lib
############################################################################


INCLUDES += $(IMG_INCLUDES)
INCLUDES += -I$(HWPLIB_SRCDIR)
INCLUDES += -I$(GENFILES_DIR)
INCLUDES += -I$(PLAT_FAPI2_DIR)
INCLUDES += -I$(BASE_FAPI2_DIR)/include
INCLUDES += -I$(PK_SRCDIR)/$(PPE_TYPE)
INCLUDES += -I$(PK_SRCDIR)/kernel
INCLUDES += -I$(PK_SRCDIR)/../baselib
INCLUDES += -I$(PK_SRCDIR)/../boltonlib
INCLUDES += -I$(PK_SRCDIR)/../../../procedures/ppe/include/std
INCLUDES += -I$(PK_SRCDIR)/../boltonlib/standard
INCLUDES += -I$(PK_SRCDIR)/ppe42
INCLUDES += -I$(PK_SRCDIR)/../ppetrace
INCLUDES += -I$(SBE_FW_DIR)
INCLUDES += -I$(SBE_FW_DIR)/core
INCLUDES += -I$(SBE_FW_DIR)/app
INCLUDES += -I$(SBE_FW_DIR)/app/common
INCLUDES += -I$(SBE_FW_DIR)/app/power
INCLUDES += -I$(BOOT_SRCDIR)
INCLUDES += -I$(BUILDDATA_SRCDIR)
INCLUDES += -I$(BUILDDATA_SRCDIR)/$(project)
INCLUDES += -I$(PPETRACEPP_DIR)
INCLUDES += $(patsubst %,-I%/include,$(IMPORT_COMMON_DIRS))
INCLUDES += -I$(IMPORT_UTILS_DIR)/
INCLUDES += -I$(IMPORT_SRCDIR)/tools/imageProcs
INCLUDES += -I$(IMPORT_SRCDIR)/chips/p10/utils/imageProcs

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

GCC-CFLAGS += -mcpu=ppe42x
GCC-CFLAGS += -ffunction-sections
GCC-CFLAGS += -fdata-sections
GCC-CFLAGS += -fstack-usage
GCC-CFLAGS += -mno-ppe42x-stack
endif

# Since pibmem is bigger in P10, we will build everything for pibmem image.
GCC-DEFS += -DSEEPROM_IMAGE

ifeq ($(img), pibmem)
GCC-DEFS += -DPIBMEM_ONLY_IMAGE
endif

#By default MEASUREMENT_IMAGE_SUPPORTED is set to ZERO 
ifeq ($(MEASUREMENT_IMAGE_SUPPORTED), 1)
# OTPROM and Seeprom L1 loader will use this variable to do additional
# functionalities if the SBE image is being built without measurement image
GCC-DEFS += -DSBE_MEASUREMENT_SUPPORT
endif

ifeq ($(SBE_S0_SUPPORT), 1)
GCC-DEFS += -D_S0_=$(SBE_S0_SUPPORT)
endif

############################################################################
CFLAGS =
PPE-CFLAGS = $(CFLAGS) -c $(GCC-CFLAGS) $(PIPE-CFLAGS) $(GCC-O-LEVEL) $(INCLUDES)

CXXFLAGS        = -std=c++11 -nostdinc++ -fno-rtti -fno-threadsafe-statics -fno-strict-aliasing
CPPFLAGS    	= -E

ASFLAGS		= -mppe42x

ifdef P2P_ENABLE
#use this to disable optimizations (fused compare/branch etc.)
PCP-FLAG    =  -b
endif
###########################################################################
# project specific include
ifeq ($(project),power)
include power_defs.mk
endif

ifeq ($(project),z)
include z_defs.mk
endif

############################################################################
ifdef SBE_CONSOLE_SUPPORT
GCC-DEFS += -DSBE_CONSOLE_SUPPORT
endif

DEFS += $(GCC-DEFS)
###########################################################################

ifdef BUILD_VERBOSE
C1=
C2=@true || echo
else
C1=@
C2=@echo
MAKE+= --no-print-directory
endif

# -Wno-conversion-null is necesary to allow mapping of NULL to TARGET_TYPE_SYSTEM
#   for attribute accesses
$(OBJDIR)/%.o: %.C
	$(C2) "    CC         $(notdir $<)"
	$(C1)$(TCC) $(PPE-CFLAGS) $(DEFS) -Wno-conversion-null  $(CXXFLAGS) -o $@ $<


$(OBJDIR)/%.o: %.c
	$(C2) "    C          $(notdir $<)"
	$(C1)$(CC) $(PPE-CFLAGS) $(DEFS) -o $@ $<

#override the GNU Make implicit rule for going from a .S to a .o
%.o: %.S

$(OBJDIR)/%.s: %.S
	$(C2) "    ASM        $(notdir $<)"
	$(C1)$(TCPP) $(PPE-CFLAGS) $(DEFS) $(CPPFLAGS) -o $@ $<
.PRECIOUS: $(OBJDIR)/%.s

ifndef P2P_ENABLE

$(OBJDIR)/%.o: $(OBJDIR)/%.s
	$(C2) "    ASM        $(notdir $<)"
	$(C1)$(AS) $(ASFLAGS) -o $@ $<

else

$(OBJDIR)/%.es: $(OBJDIR)/%.s
	$(PCP) $(PCP-FLAG) -f $<
.PRECIOUS: $(OBJDIR)/%.es

$(OBJDIR)/%.o: $(OBJDIR)/%.es
	$(AS) $(ASFLAGS) -o $@ $<

endif
