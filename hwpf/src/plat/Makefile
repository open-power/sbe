# This Makefile is designed to be invoked with the -I argument
include img_defs.mk
include fapi2ppeplatfiles.mk

OBJS := $(addprefix $(OBJDIR)/, $(FAPI2PLATLIB_OBJECTS))

all: $(OBJS)


$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d) 
endif

