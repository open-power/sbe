# This Makefile is designed to be invoked with the -I argument set to
# the location of the "pk.mk" for the build

include img_defs.mk
include pkppe42files.mk

ifeq "$(PK_TIMER_SUPPORT)" "1"
PPE42_OBJECTS += ${PPE42-TIMER-C-SOURCES:.c=.o} ${PPE42-TIMER-S-SOURCES:.S=.o}
endif

ifeq "$(PK_THREAD_SUPPORT)" "1"
PPE42_OBJECTS += ${PPE42-THREAD-C-SOURCES:.c=.o} ${PPE42-THREAD-S-SOURCES:.S=.o}
endif

OBJS := $(addprefix $(OBJDIR)/, $(PPE42_OBJECTS))

all: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d) 
endif
