# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/sbefw/app/power_dft/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2020
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
export SUB_OBJDIR = /sbefw/app/power_pibmem
OBJS = $(addprefix $(OBJDIR)/, $(APPPOWERPIBMEM_OBJECTS))
LIBNAME = libsbeapppowerpibmem
else
export SUB_OBJDIR = /sbefw/app/power_seeprom
GCC-CFLAGS += -mlongcall
GCC-DEFS += -D__SBEFW_SEEPROM__=1
OBJS = $(addprefix $(OBJDIR)/, $(APPPOWERSEEPROM_OBJECTS))
LIBNAME = libsbeapppowerseeprom
endif

include img_defs.mk
include apppowerfiles.mk

$(LIBNAME).a: $(OBJS)
	$(AR) crs $(OBJDIR)/$(LIBNAME).a $(OBJDIR)/*.o

.PHONY: clean sbeapppower
sbeapppower: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

#clean the kernel directory first, then the application level clean
clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif





