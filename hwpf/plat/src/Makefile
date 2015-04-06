# This Makefile is designed to be invoked with the -I argument set to
# the location of the "pk.mk" for the build

include img_defs.mk
include fapi2ppefiles.mk


OBJS := $(addprefix $(OBJDIR)/, $(FAPI2LIB_OBJECTS))

all: $(OBJS)


$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d) 
endif

