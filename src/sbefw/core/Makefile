# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/sbefw/core/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2018
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
#export P2P_ENABLE = 1

ifneq ($(img_mem), seeprom)
export SUB_OBJDIR = /sbefw/core_pibmem
OBJS = $(addprefix $(OBJDIR)/, $(COREPIBMEM_OBJECTS))
LIBNAME = libsbecorepibmem
else
export SUB_OBJDIR = /sbefw/core_seeprom
GCC-CFLAGS += -mlongcall
GCC-DEFS += -D__SBEFW_SEEPROM__=1
OBJS = $(addprefix $(OBJDIR)/, $(CORESEEPROM_OBJECTS))
LIBNAME = libsbecoreseeprom
endif

include img_defs.mk
include corefiles.mk

$(LIBNAME).a: $(OBJS)
	$(AR) crs $(OBJDIR)/$(LIBNAME).a $(OBJDIR)/*.o

.PHONY: clean sbecore
sbecore: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)


#clean the kernel directory first, then the application level clean
clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif





