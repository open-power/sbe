# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/iplbuild/Makefile $
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

CXX  = g++

export VPATH = $(IMAGEPROCS_COMMON_SRCDIR):$(IMAGEPROCS_P10_SRCDIR):$(IMAGEPROCS_TOOL_DIR)
export INCLUDES = -I$(IMAGEPROCS_COMMON_SRCDIR) \
				  -I$(IMAGEPROCS_P9_SRCDIR) \
				  -I$(IMAGEPROCS_P10_SRCDIR) \
				  -I$(IMAGEPROCS_TOOL_DIR)
.PHONY : all utilities clean

OBJS = $(P10_IMAGE_TOOL_BINDIR)/p10_ipl_image.o  \
	   $(BASE_OBJDIR)/tools/iplbuild/p10_ddco.o \

all:objdir utilities

objdir:
	mkdir -p $(BASE_OBJDIR)/tools/iplbuild

utilities:

CXXFLAGS = -DFAPI2_NO_FFDC -D__PPE__

$(P10_IMAGE_TOOL_BINDIR)/%.o: $(IMAGEPROCS_TOOL_DIR)/%.C
	$(CXX) $(INCLUDES) $(DEFS) $(CXXFLAGS) -c -o $@ $<

$(P10_IMAGE_TOOL_BINDIR)/%.o: $(IMAGEPROCS_P10_SRCDIR)/%.C
	$(CXX) $(INCLUDES) $(DEFS) $(CXXFLAGS) -c -o $@ $<

$(BASE_OBJDIR)/tools/iplbuild/%.o: $(IMAGEPROCS_P9_SRCDIR)/%.c
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c -o $@ $<

$(BASE_OBJDIR)/tools/iplbuild/%.o: %.C
	$(CXX) $(INCLUDES) $(DEFS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(BASE_OBJDIR)/tools/iplbuild
