# This Makefile is designed to be invoked with the -I argument set to
# the location of the "pk.mk" for the build

include img_defs.mk
include pktracefiles.mk

ifeq "$(PK_TIMER_SUPPORT)" "1"
PKTRACE_OBJECTS += ${PKTRACE-TIMER-C-SOURCES:.c=.o} ${PKTRACE-TIMER-S-SOURCES:.S=.o}
endif

ifeq "$(PK_THREAD_SUPPORT)" "1"
PKTRACE_OBJECTS += ${PKTRACE-THREAD-C-SOURCES:.c=.o} ${PKTRACE-THREAD-S-SOURCES:.S=.o}
endif

OBJS := $(addprefix $(OBJDIR)/, $(PKTRACE_OBJECTS))

all: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d) 
endif
