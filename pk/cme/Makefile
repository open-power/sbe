# This Makefile compiles all of the PK code required for the CME port
# of PK.  See the "pk.mk" file in this directory.

#all generated files from this makefile will end up in obj/$(IMAGE_NAME)/pk
export SUB_OBJDIR = /pk

include img_defs.mk
include pkcmefiles.mk

ifeq "$(PK_TIMER_SUPPORT)" "1"
CME_OBJECTS += ${CME-TIMER-C-SOURCES:.c=.o} ${CME-TIMER-S-SOURCES:.S=.o}
endif

ifeq "$(PK_THREAD_SUPPORT)" "1"
CME_OBJECTS += ${CME-THREAD-C-SOURCES:.c=.o} ${CME-THREAD-S-SOURCES:.S=.o}
endif

ifeq "$(CME_ASYNC_SUPPORT)" "1"
CME_OBJECTS += ${CME-ASYNC-C-SOURCES:.c=.o} ${CME-ASYNC-S-SOURCES:.S=.o}
endif

OBJS := $(addprefix $(OBJDIR)/, $(CME_OBJECTS))

libpk.a: kernel ppe42 trace cme
	$(AR) crs $(OBJDIR)/libpk.a $(OBJDIR)/*.o

.PHONY: clean cme kernel ppe42 trace
cme: $(OBJS)

trace:
	$(MAKE) -I $(IMAGE_SRCDIR) -C ../trace

kernel:
	$(MAKE) -I $(IMAGE_SRCDIR) -C ../kernel

ppe42:
	$(MAKE) -I $(IMAGE_SRCDIR) -C ../ppe42


$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -fr $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d) 
endif
