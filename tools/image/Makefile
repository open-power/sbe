# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: tools/image/Makefile $
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
############################################################################

# Makefile for image tools
# works on X86 Linux hosts.

# Make targets:

# all          :
#
# utilities    : Build utility programs and procedures
#
# clean        : Removes the bin/ directory and all symbolic links
#

############################################################################


ifeq ($(CTEPATH),)
$(warning CTEPATH not defined; defaulting to awd)
CTEPATH = /afs/awd/projects/cte
endif

P9_XIP_SRCDIR = $(abspath ../../import/chips/p9/xip)
P9_XIP_BINDIR = $(P9_XIP_SRCDIR)/bin

#  Locations of required headers.
INCLUDES += -I. -I../../ -I../../utils
INCLUDES += -I ../../sbe/image/
INCLUDES += -I ../../sbe/sbefw/
INCLUDES += -I ../../sbe/hwpf/include/plat
INCLUDES += -I ../../sbe/hwpf/include
INCLUDES += -I ../../hwpf/include/plat
INCLUDES += -I ../../hwpf/include/
INCLUDES += -I ../../import/chips/p9/procedures/ppe/pk/ppe42/
INCLUDES += -I ../../import/chips/p9/procedures/ppe/pk/kernel/
INCLUDES += -I ../../import/chips/p9/procedures/ppe/pk/std/
INCLUDES += -I ../../import/chips/p9/procedures/ppe/pk/trace/
INCLUDES += -I ../../tools/ppetracepp/
INCLUDES += -I ../../import/hwpf/fapi2/include/
INCLUDES += -I ../../import/chips/p9/xip/
INCLUDES += -I ../../import/tools/imageProcs/
INCLUDES += -I ../../import/chips/p9/utils/
INCLUDES += -I ../../import/chips/p9/utils/imageProcs

# Under Linux the scheme is to use a common compiler to create procedures.
# However, the common compiler can be VERY slow, so if the system compiler is
# also 4.1.2 we're using that one instead.  Also, the Linux FAPI libraries we
# link with are 32-bit only so we need to force 32-bit mode.

ifeq ($(wildcard /etc/ldap.conf), )
  GSACELL     = ausgsa
else
  GSACELL     = $(shell cat /etc/ldap.conf | grep "host " | \
        cut -d" " -f2 | cut -d. -f1)
endif

GCC-RELEASE = 4.8.2
GCC-VERSION = $(shell gcc -v 2>&1 | grep "$(GCC-RELEASE)")

ifeq ($(GCC-VERSION),)
$(error wrong compiler version. Use $(GCC-RELEASE) compiler. Try: "scl enable devtoolset-2 bash")
else
CC   = gcc
CXX  = g++
endif

UTILITIES-SOURCES = sbe_default_tool.c

UTILITIES = sbe_default_tool

#  Utility targets
UTILITIES-OBJc         = $(patsubst %.c,bin/%.o,$(UTILITIES-SOURCES))
UTILITIES-OBJECTS      += $(patsubst %.C,bin/%.o,$(UTILITIES-OBJc))
UTILITIES-DEPENDENCIES = $(patsubst %.o,%.d,$(UTILITIES-OBJECTS))
UTILITIES-EXECUTABLES  = $(patsubst %,bin/%,$(UTILITIES))


.PHONY : utilities buildBinDir
utilities: buildBinDir $(UTILITIES-EXECUTABLES)

buildBinDir:
	mkdir -p bin

# Build the P9-XIP image code
$(P9_XIP_BINDIR)/p9_xip_image.o:
	$(MAKE) -I $(P9_XIP_SRCDIR) -C $(P9_XIP_SRCDIR) -f Makefile

CXXFLAGS+=-DDEBUG_SBE_XIP_IMAGE=1
CXXFLAGS+=-DFAPI2_NO_FFDC

bin/%.o: %.c
	$(CXX) -std=c++11 $(INCLUDES) $(CXXFLAGS) -c -o $@ $<

bin/sbe_default_tool: $(P9_XIP_BINDIR)/p9_xip_image.o bin/sbe_default_tool.o
	$(CXX) $(CXXFLAGS) ${INCLUDES} -o $@ $^
	ln -sf bin/sbe_default_tool sbe_default_tool

clean:
	rm -f sbe_default_tool
	rm -rf bin
	mkdir -p bin
