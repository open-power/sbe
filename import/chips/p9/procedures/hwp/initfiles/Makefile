
# This Makefile compiles all of the initfiles hardware procedure code. See the
# "initfiles.mk" file in this directory.

#all generated files from this makefile will end up in obj/initfiles
export SUB_OBJDIR = /initfiles

include img_defs.mk
include initfiles.mk


OBJS := $(addprefix $(OBJDIR)/, $(INITFILES_OBJECTS))

libinitfiles.a: initfiles
	$(AR) crs $(OBJDIR)/libinitfiles.a $(OBJDIR)/*.o

.PHONY: clean initfiles
initfiles: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
