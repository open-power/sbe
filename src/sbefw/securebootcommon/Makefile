# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/sbefw/securebootcommon/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2021
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
# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
export SUB_OBJDIR = /sbefw/securebootcommon
export VPATH = $(SBE_FW_DIR)/core

OBJS = $(addprefix $(OBJDIR)/, $(SECUREBOOT_COMMON_OBJECTS))
LIBNAME = libsecurebootcommon

include img_defs.mk
include securebootcommonfiles.mk

$(LIBNAME).a: $(OBJS)
	$(AR) crs $(OBJDIR)/$(LIBNAME).a $(OBJDIR)/*.o

.PHONY: clean securebootcommon
securebootcommon: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

#clean the kernel directory first, then the application level clean
clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
	include $(OBJS:.o=.d)
endif
