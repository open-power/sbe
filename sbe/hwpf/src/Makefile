# This Makefile is designed to be invoked with the -I argument

export SUB_OBJDIR = /fapi2

include img_defs.mk
include fapi2sbefiles.mk


OBJS := $(addprefix $(OBJDIR)/, $(FAPI2LIB_OBJECTS))

libfapi2.a: fapi2 hwpf
	$(AR) crs $(OBJDIR)/libfapi2.a $(OBJDIR)/*.o

.PHONY: clean fapi2 hwpf
fapi2: $(OBJS)

#plat:
#	$(MAKE) -I $(IMAGE_SRCDIR) -C $(PLAT_FAPI2_DIR)

hwpf:
	@echo "Processing fapi2 hwpf makefile"
	$(MAKE) -I $(IMAGE_SRCDIR) -C $(PPE_FAPI2_DIR)/src

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d) 
endif

