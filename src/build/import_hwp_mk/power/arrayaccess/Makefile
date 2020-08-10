# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/import_hwp_mk/power/arrayaccess/Makefile $
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
# This Makefile compiles all of the hardware procedure code. See the
# "*.mk" file in this directory.

#all generated files from this makefile will end up in obj/*
export SUB_OBJDIR = /$(IMPORT_OBJDIR)/arrayaccess

include img_defs.mk
include arrayaccessfiles.mk

export VPATH = $(PERV_SRCDIR):$(NEST_SRCDIR):$(CACHE_SRCDIR):$(CORE_SRCDIR):$(PM_SRCDIR):$(INITFILES_SRCDIR):$(LIB_SRCDIR)

GCC-CFLAGS += -mlongcall

OBJSARRAYACCESS := $(addprefix $(OBJDIR-ARRAYACCESS)/, $(ARRAYACCESS_OBJECTS))

.PHONY: clean arrayaccess

libarrayaccess.a: arrayaccess
	$(AR) crs $(OBJDIR-ARRAYACCESS)/libarrayaccess.a $(OBJDIR-ARRAYACCESS)/*.o

arrayaccess: $(OBJSARRAYACCESS)

$(OBJSARRAYACCESS) $(OBJSARRAYACCESS:.o=.d): | $(OBJDIR-ARRAYACCESS)

$(OBJDIR-ARRAYACCESS):
	mkdir -p $(OBJDIR-ARRAYACCESS)

clean:
	rm -fr $(OBJDIR-ARRAYACCESS)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJSARRAYACCESS:.o=.d)
endif
