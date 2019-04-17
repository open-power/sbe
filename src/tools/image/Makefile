# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/image/Makefile $
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
############################################################################

# Makefile for image tools
# works on X86 Linux hosts.

# Make targets:

# all          : utilities
#
# utilities    : Build utility programs and procedures
#
# clean        : Removes generated files
#

############################################################################
include img_defs.mk

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

CXXFLAGS        =   -nostdinc++ -fno-rtti -fno-threadsafe-statics -fno-strict-aliasing
CXX  = g++

.PHONY : all utilities clean

all:objdir utilities

objdir:
	mkdir -p $(BASE_OBJDIR)/tools/image

utilities: $(IMG_DIR)/sbe_default_tool

CXXFLAGS+=-DFAPI2_NO_FFDC

$(BASE_OBJDIR)/tools/image/%.o: $(IMAGEPROCS_P10_SRCDIR)/%.C
	$(CXX) $(INCLUDES) $(DEFS) $(CXXFLAGS) -c -o $@ $<

$(BASE_OBJDIR)/tools/image/%.o: $(IMAGEPROCS_P9_SRCDIR)/%.c
	$(CXX) $(INCLUDES) $(DEFS) $(CXXFLAGS) -c -o $@ $<

$(BASE_OBJDIR)/tools/image/%.o: %.c
	$(CXX) $(INCLUDES) $(DEFS) $(CXXFLAGS) -c -o $@ $<

$(IMG_DIR)/sbe_default_tool: $(BASE_OBJDIR)/tools/image/p10_ipl_image.o $(BASE_OBJDIR)/tools/image/sbe_default_tool.o $(BASE_OBJDIR)/tools/image/p10_ddco.o
	$(CXX) $(CXXFLAGS) ${INCLUDES} -o $@ $^

clean:
	rm -f $(BASE_OBJDIR)/tools/image/sbe_default_tool*
	rm -f $(IMG_DIR)/sbe_default_tool*
