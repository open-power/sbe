
# This Makefile compiles all of the pm hardware procedure code. See the
# "pmfiles.mk" file in this directory.

#all generated files from this makefile will end up in obj/pm
export SUB_OBJDIR = /pm

include img_defs.mk
include pmfiles.mk


OBJS := $(addprefix $(OBJDIR)/, $(PM_OBJECTS))

libpm.a: pm
	$(AR) crs $(OBJDIR)/libpm.a $(OBJDIR)/*.o

.PHONY: clean pm
pm: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
