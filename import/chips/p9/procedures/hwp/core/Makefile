
# This Makefile compiles all of the core hardware procedure code. See the
# "corehcdfiles.mk" file in this directory.

#all generated files from this makefile will end up in obj/cache
export SUB_OBJDIR = /core

include img_defs.mk
include corehcdfiles.mk


OBJS := $(addprefix $(OBJDIR)/, $(CORE_OBJECTS))

libcore.a: core
	$(AR) crs $(OBJDIR)/libcore.a $(OBJDIR)/*.o

.PHONY: clean core
core: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
