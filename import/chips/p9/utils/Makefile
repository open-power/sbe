# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: chips/p9/utils/Makefile $
#
# IBM CONFIDENTIAL
#
# EKB Project
#
# COPYRIGHT 2016
# [+] International Business Machines Corp.
#
#
# The source code for this program is not published or otherwise
# divested of its trade secrets, irrespective of what has been
# deposited with the U.S. Copyright Office.
#
# IBM_PROLOG_END_TAG

# @brief This Makefile compiles all of the PutRing Utils code.
# All the generated files from this makefile will end up in obj/utils
#
#
export SUB_OBJDIR = /utils

include img_defs.mk
include utils.mk

OBJS := $(addprefix $(OBJDIR)/, $(UTILS_OBJECTS))

libutils.a: utils
	$(AR) crs $(OBJDIR)/libutils.a $(OBJDIR)/*.o

.PHONY: clean utils
utils: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
