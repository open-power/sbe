
#all generated files from this makefile will end up in obj/*
export SUB_OBJDIR = /$(IMPORT_OBJDIR)/istep14_16

include img_defs.mk
include istep14_16files.mk

export VPATH = $(PERV_SRCDIR):$(NEST_SRCDIR):$(CACHE_SRCDIR):$(CORE_SRCDIR):$(PM_SRCDIR):$(INITFILES_SRCDIR):$(LIB_SRCDIR)

OBJSISTEP14_16 := $(addprefix $(OBJDIR-ISTEP14_16)/, $(ISTEP14_16_OBJECTS))

.PHONY: clean istep14_16

libistep14_16.a: istep14_16
	$(AR) crs $(OBJDIR-ISTEP14_16)/libistep14_16.a $(OBJDIR-ISTEP14_16)/*.o

istep14_16: $(OBJSISTEP14_16)

$(OBJSISTEP14_16) $(OBJSISTEP14_16:.o=.d): | $(OBJDIR-ISTEP14_16)

$(OBJDIR-ISTEP14_16):
	mkdir -p $(OBJDIR-ISTEP14_16)

clean:
	rm -fr $(OBJDIR-ISTEP14_16)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJSISTEP14_16:.o=.d)
endif
