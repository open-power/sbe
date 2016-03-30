# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: chips/p9/xip/Makefile $
#
# IBM CONFIDENTIAL
#
# EKB Project
#
# COPYRIGHT 2015,2016
# [+] International Business Machines Corp.
#
#
# The source code for this program is not published or otherwise
# divested of its trade secrets, irrespective of what has been
# deposited with the U.S. Copyright Office.
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

XIP_SOURCES  = p9_xip_image.c
XIP_SOURCES += p9_xip_tool.c

XIP_UTILS = p9_xip_tool

#  Utility targets
XIP_OBJc         = $(patsubst %.c,$(OBJ_DIR)/%.o,$(XIP_SOURCES))
XIP_OBJECTS      = $(patsubst %.C,$(OBJ_DIR)/%.o,$(XIP_OBJc))
#XIP_DEPENDENCIES = $(patsubst %.o,%.d,$(XIP_OBJECTS))
XIP_EXECUTABLES  = $(patsubst %,$(OBJ_DIR)/%,$(XIP_UTILS))

.PHONY : all clean

all: $(OBJ_DIR) $(XIP_EXECUTABLES)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $(XIP_FLAGS) -c -o $@ $<

$(OBJ_DIR)/p9_xip_tool: $(OBJ_DIR)/p9_xip_image.o  $(OBJ_DIR)/p9_xip_tool.o
	$(CXX) -o $@ $^

clean:
	rm -rf $(OBJ_DIR)
