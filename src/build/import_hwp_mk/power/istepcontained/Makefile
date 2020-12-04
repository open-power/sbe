# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/import_hwp_mk/power/istepcontained/Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2020
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

#all generated files from this makefile will end up in obj/*
export SUB_OBJDIR = /$(IMPORT_OBJDIR)/istepcontained

include img_defs.mk
include istepcontainedfiles.mk

export VPATH = $(PERV_SRCDIR):$(NEST_SRCDIR):$(CACHE_SRCDIR):$(CORE_SRCDIR):$(PM_SRCDIR):$(INITFILES_SRCDIR):$(LIB_SRCDIR):$(CONTAINED_SRCDIR)

OBJSISTEPCONTAINED := $(addprefix $(OBJDIR-ISTEPCONTAINED)/, $(ISTEPCONTAINED_OBJECTS))

.PHONY: clean istepcontained

libistepcontained.a: istepcontained
	$(AR) crs $(OBJDIR-ISTEPCONTAINED)/libistepcontained.a $(OBJDIR-ISTEPCONTAINED)/*.o

istepcontained: $(OBJSISTEPCONTAINED)

$(OBJSISTEPCONTAINED) $(OBJSISTEPCONTAINED:.o=.d): | $(OBJDIR-ISTEPCONTAINED)

$(OBJDIR-ISTEPCONTAINED):
	mkdir -p $(OBJDIR-ISTEPCONTAINED)

clean:
	rm -fr $(OBJDIR-ISTEPCONTAINED)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJSISTEPCONTAINED:.o=.d)
endif
