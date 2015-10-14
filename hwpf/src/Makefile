# This Makefile is designed to be invoked with the -I argument

include img_defs.mk
include fapi2ppefiles.mk


OBJS := $(addprefix $(OBJDIR)/, $(FAPI2LIB_OBJECTS))

.PHONY: clean all plat
all: $(OBJS) plat

plat:
	@echo "Processing FAPI2 platform makefile"
	$(MAKE) -I $(IMAGE_SRCDIR) -C ./plat -f Makefile

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d) 
endif

