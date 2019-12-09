# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/import_hwp_mk/power/sbem_hwp/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2020
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
export SUB_OBJDIR = /$(IMPORT_OBJDIR)/sbem_hwp

include img_defs.mk
include sbemhwpfiles.mk

export VPATH = $(SPI_SRCDIR):$(INITFILES_SRCDIR):$(LIB_SRCDIR)

OBJSBEMHWPFILES := $(addprefix $(OBJDIR-SBEM-HWP)/, $(SBEMHWP_OBJECTS))

.PHONY: clean sbem_hwp

libsbem_hwp.a: sbem_hwp
	$(AR) crs $(OBJDIR-SBEM-HWP)/libsbem_hwp.a $(OBJDIR-SBEM-HWP)/*.o

sbem_hwp: $(OBJSBEMHWPFILES)

$(OBJSBEMHWPFILES) $(OBJSBEMHWPFILES:.o=.d): | $(OBJDIR-SBEM-HWP)

$(OBJDIR-SBEM-HWP):
	mkdir -p $(OBJDIR-SBEM-HWP)

clean:
	rm -fr $(OBJDIR-SBEM-HWP)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJSBEMHWPFILES:.o=.d)
endif
