
# This Makefile compiles all of the core hardware procedure code. See the
# "pervfiles.mk" file in this directory.

#all generated files from this makefile will end up in obj/perv
export SUB_OBJDIR = /perv

include img_defs.mk
include pervfiles.mk

GCC-CFLAGS += -mlongcall

OBJS := $(addprefix $(OBJDIR)/, $(PERV_OBJECTS))

libperv.a: perv
	$(AR) crs $(OBJDIR)/libperv.a $(OBJDIR)/*.o

.PHONY: clean perv
perv: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
