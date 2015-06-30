export P2P_ENABLE = 1

export SUB_OBJDIR = /sbefw

include img_defs.mk
include sbefwfiles.mk

OBJS := $(addprefix $(OBJDIR)/, $(SBEFW_OBJECTS))

libsbefw.a: $(OBJS)
	$(AR) crs $(OBJDIR)/libsbefw.a $(OBJDIR)/*.o

.PHONY: clean sbefw
sbefw: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)


#clean the kernel directory first, then the application level clean
clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif





