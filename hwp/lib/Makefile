
# This Makefile compiles all of the common library hardware procedure code.
# See the "libcommonfiles.mk" file in this directory.

#all generated files from this makefile will end up in obj/lib
export SUB_OBJDIR = /lib

include img_defs.mk
include libcommonfiles.mk


OBJS := $(addprefix $(OBJDIR)/, $(LIB_OBJECTS))

libcommon.a: lib
	$(AR) crs $(OBJDIR)/libcommon.a $(OBJDIR)/*.o

.PHONY: clean lib
lib: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
