
# This Makefile compiles all of the core hardware procedure code. See the
# "nestfiles.mk" file in this directory.

#all generated files from this makefile will end up in obj/nest
export SUB_OBJDIR = /nest

include img_defs.mk
include nestfiles.mk


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
