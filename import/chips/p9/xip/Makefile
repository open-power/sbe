# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: import/chips/p9/xip/Makefile $
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
# Makefile for P9-XIP image code and tools; works on X86 Linux hosts.
# Make targets:
# all          : Builds P9-XIP code and tools in $(OBJ_DIR)/
# clean        : Removes the $(OBJ_DIR)/ directory
############################################################################

#  Locations of required headers.
INCLUDES += -I.
INCLUDES += -I../../../hwpf/fapi2/include/plat          # ekb: define path variable covering ekb and ppe
INCLUDES += -I../../../../hwpf/include/plat/            # ppe: define path variable covering ekb and ppe

XIP_FLAGS += -DDEBUG_P9_XIP_IMAGE=1
XIP_FLAGS += -DFAPI2_NO_FFDC

CXX_FLAGS += -std=c++11

OBJ_DIR = bin

# Under Linux the scheme is to use a common compiler to create procedures.
# However, the common compiler can be VERY slow, so if the system compiler is
# also 4.1.2 we're using that one instead.  Also, the Linux FAPI libraries we
# link with are 32-bit only so we need to force 32-bit mode.

CXX  = g++

CXX_RELEASE = 4.8.2
CXX_VERSION = $(shell $(CXX) -v 2>&1 | grep "$(CXX_RELEASE)")
ifeq ($(CXX_VERSION),)
$(error wrong compiler version. Use $(CXX_RELEASE) compiler. Try: "scl enable devtoolset-2 bash")
endif

XIP_UTILS = p9_xip_tool

#  Utility targets
XIP_EXECUTABLES  = $(patsubst %,$(OBJ_DIR)/%,$(XIP_UTILS))

.PHONY : all clean

all: $(OBJ_DIR) $(XIP_EXECUTABLES)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $(XIP_FLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.C
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $(XIP_FLAGS) -c -o $@ $<

$(OBJ_DIR)/p9_xip_tool: $(OBJ_DIR)/p9_xip_image.o  $(OBJ_DIR)/p9_xip_tool.o
	$(CXX) -o $@ $^

clean:
	rm -rf $(OBJ_DIR)
