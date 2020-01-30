# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/hwpf/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2020
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
# This Makefile is designed to be invoked with the -I argument

ifeq ($(img_mem), measurement)
export SUB_OBJDIR = /fapi2_measurement
GCC-DEFS += -D__SBEMFW_MEASUREMENT__=1
OBJS = $(addprefix $(OBJDIR)/, $(SBEM_FAPI2PLATLIB_OBJECTS))
LIBNAME = libfapi2measurement
else ifeq ($(img_mem), seeprom)
export SUB_OBJDIR = /fapi2_seeprom
GCC-DEFS += -D__SBEFW_SEEPROM__=1
OBJS = $(addprefix $(OBJDIR)/, $(FAPI2PLATLIB_SEEPROM_OBJECTS))
LIBNAME = libfapi2seeprom
else
export SUB_OBJDIR = /fapi2
GCC-DEFS += -D__SBEFW_PIBMEM__=1
OBJS = $(addprefix $(OBJDIR)/, $(FAPI2PLATLIB_OBJECTS))
LIBNAME = libfapi2
endif

include img_defs.mk
include fapi2sbefiles.mk

$(LIBNAME).a: $(OBJS)
	$(AR) crs $(OBJDIR)/$(LIBNAME).a $(OBJDIR)/*.o

.PHONY: clean fapi2 hwpf
fapi2: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif

