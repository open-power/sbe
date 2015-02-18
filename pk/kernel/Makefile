# This Makefile is designed to be invoked with the -I argument set to
# the location of the "pk.mk" for the build

include img_defs.mk
include pkkernelfiles.mk

ifeq "$(PK_TIMER_SUPPORT)" "1"
PK_OBJECTS += ${PK-TIMER-C-SOURCES:.c=.o}
endif

ifeq "$(PK_THREAD_SUPPORT)" "1"
PK_OBJECTS += ${PK-THREAD-C-SOURCES:.c=.o} 
endif

OBJS := $(addprefix $(OBJDIR)/, $(PK_OBJECTS))

all: $(OBJS)

$(OBJS) $(OBJS:.o=.d): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif
