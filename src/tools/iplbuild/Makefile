# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/iplbuild/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2018
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

CXX  = g++

export VPATH = $(P9_XIP_SRCDIR):$(IMAGEPROCS_COMMON_SRCDIR):$(IMAGEPROCS_P9_SRCDIR):$(IMAGEPROCS_CEN_SRCDIR):$(IMAGEPROCS_TOOL_DIR)
export INCLUDES = -I$(P9_XIP_SRCDIR) \
				  -I$(IMAGEPROCS_COMMON_SRCDIR) \
				  -I$(IMAGEPROCS_P9_SRCDIR) \
				  -I$(IMAGEPROCS_CEN_SRCDIR) \
		      	  -I$(IMAGEPROCS_TOOL_DIR)
.PHONY : all utilities clean

OBJS = $(P9_XIP_BINDIR)/p9_xip_image.o \
	   $(BASE_OBJDIR)/tools/iplbuild/common_ringId.o \
	   $(BASE_OBJDIR)/tools/iplbuild/p9_ringId.o \
	   $(BASE_OBJDIR)/tools/iplbuild/cen_ringId.o \
	   $(BASE_OBJDIR)/tools/iplbuild/p9_tor.o \
	   $(BASE_OBJDIR)/tools/iplbuild/p9_ipl_build.o \
	   $(BASE_OBJDIR)/tools/iplbuild/p9_dd_container.o \

all:objdir utilities

objdir:
	mkdir -p $(BASE_OBJDIR)/tools/iplbuild

utilities: $(IMG_DIR)/p9_ipl_build

CXXFLAGS =-DFAPI2_NO_FFDC

$(P9_XIP_BINDIR)/%.o: %.c
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c -o $@ $<

$(BASE_OBJDIR)/tools/iplbuild/%.o: $(IMAGEPROCS_P9_SRCDIR)/%.c
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c -o $@ $<

$(BASE_OBJDIR)/tools/iplbuild/%.o: %.C
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c -o $@ $<

$(IMG_DIR)/p9_ipl_build: $(OBJS)
	$(CXX) $(CXXFLAGS) ${INCLUDES} -o $@ $^

clean:
	rm -rf $(BASE_OBJDIR)/tools/iplbuild
	rm -f $(IMG_DIR)/p9_ipl_build
