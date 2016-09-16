# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p9/procedures/hwp/nest/Makefile $
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

# This Makefile compiles all of the nest hardware procedure code. See the
# "nestfiles.mk" file in this directory.

#all generated files from this makefile will end up in obj/nest
export SUB_OBJDIR = /nest

include img_defs.mk
include nestfiles.mk

# TODO via RTC 152424
# Adding VPATH as there are few procedures which are in
# perv folder but we are compiling them here. Refer to
# nestfiles.mk for details.
export VPATH = $(PERV_SRCDIR):$(NEST_SRCDIR)
OBJS := $(addprefix $(OBJDIR)/, $(NEST_OBJECTS))

libnest.a: nest
	$(AR) crs $(OBJDIR)/libnest.a $(OBJDIR)/*.o

.PHONY: clean nest
nest: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
