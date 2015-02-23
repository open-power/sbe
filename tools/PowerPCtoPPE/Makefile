
export SUB_OBJDIR = /p2p

include img_defs.mk
include p2pfiles.mk

OBJS := $(addprefix $(OBJDIR)/, $(P2P_OBJECTS))

libp2p.a: $(OBJS)
	$(AR) crs $(OBJDIR)/libp2p.a $(OBJDIR)/*.o

.PHONY: clean p2p
p2p: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif

