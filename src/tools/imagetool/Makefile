# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/imagetool/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2019,2020
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
				  -I$(IMAGEPROCS_P10_SRCDIR) \
				  -I$(IMAGEPROCS_P9_SRCDIR) \
				  -I$(IMAGEPROCS_TOOL_DIR)
.PHONY : all utilities clean

OBJS = $(P10_IMAGE_TOOL_BINDIR)/p10_ipl_image.o  \
       $(P10_IMAGE_TOOL_BINDIR)/ipl_image_tool.o \
	   $(P10_IMAGE_TOOL_BINDIR)/p10_ddco.o \
	   $(P10_IMAGE_TOOL_BINDIR)/p10_dynamic.o

all:objdir utilities

objdir:
	mkdir -p $(P10_IMAGE_TOOL_BINDIR)

utilities: $(P10_IMAGE_TOOL_BINDIR)/ipl_image_tool

CXXFLAGS = -DFAPI2_NO_FFDC -D__PPE__

$(P10_IMAGE_TOOL_BINDIR)/%.o: $(IMAGEPROCS_TOOL_DIR)/%.C
	$(CXX) $(INCLUDES) $(DEFS) $(CXXFLAGS) -c -o $@ $<

$(P10_IMAGE_TOOL_BINDIR)/%.o: $(IMAGEPROCS_P10_SRCDIR)/%.C
	$(CXX) $(INCLUDES) $(DEFS) $(CXXFLAGS) -c -o $@ $<

$(P10_IMAGE_TOOL_BINDIR)/ipl_image_tool: $(OBJS)
	$(CXX) $(CXXFLAGS) ${INCLUDES} -o $@ $^

clean:
	rm -rf $(P10_IMAGE_TOOL_BINDIR)
	rm -f $(IMG_DIR)/ipl_image_tool
