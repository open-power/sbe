#  Make header for GPE PK builds
#
# The application may define the following variables to control the
# build process:
#
# IMG_INCLUDES       : Aplication-specific header search paths
#
# DEFS               : A string of -D<symbol>[=<value>] to control compilation
#
# PK                : Default ..; The path to the PK source code.
#                      The default is set for building the PK
#                      subdirectories. 
#
# PK_THREAD_SUPPORT : (0/1, default 1); Compile PK thread and
#                      semaphore suppprt 
#
# PK_TIMER_SUPPORT  : (0/1, default 1); Compile PK timer suppprt
#
# SIMICS_ENVIRONMENT : (0/1, current default 0); Compile for Simics
#
# SIMICS_MAGIC_PANIC : (0/1, current default 0); Use Simics Magic
#                      breakpoint for PK_PANIC() instead of PowerPC trap.
#                      Note that Simics does not model trap correctly in
#                      external debug mode.
#
# GCC-O-LEVEL        : The optimization level passed to GCC (default -Os). May
#                      also be defined empty (GCC-O-LEVEL=) to disable
#                      optimization. This variable can also be used to pass
#                      any other non-default setting to GCC, e.g.
#                      make GCC-O-LEVEL="-Os -fno-branch-count-reg"
#
# GCC-TOOL-PREFIX    : The full path (including executable file prefixes) to
#                      the GCC cross-development tools to use.  The default is 
#                      "ppcnf-mcp5-"
#
# CTEPATH            : This variable defaults to the afs/awd CTE tool
#                      installation - The PORE binutils are stored there. If
#                      you are not in Austin be sure to define CTEPATH in
#                      your .profile.
#
# OBJDIR             : target directory for all generated files

IMAGE_NAME := sample_ppe

PPE_TYPE := ppe

ifndef IMAGE_SRCDIR
export IMAGE_SRCDIR = $(abspath .)
endif

ifndef IMG_INCLUDES
export IMG_INCLUDES = -I$(IMAGE_SRCDIR)
endif

ifndef BASE_OBJDIR
export BASE_OBJDIR = $(abspath ../../../obj)
endif

export IMG_OBJDIR = $(BASE_OBJDIR)/$(IMAGE_NAME)

ifndef PK_SRCDIR
export PK_SRCDIR = $(abspath ../../pk)
endif

#ifndef COMMONLIB_SRCDIR
#export COMMONLIB_SRCDIR = $(abspath ../lib/common)
#endif

ifndef GCC-TOOL-PREFIX
#GCC-TOOL-PREFIX = $(CTEPATH)/tools/ppcgcc/prod/bin/powerpc-linux-
GCC-TOOL-PREFIX = /afs/bb/u/rembold/openpower/op-build/output/host/usr/bin/powerpc64-linux-
#GCC-TOOL-PREFIX = /afs/bb/u/rembold/openpower/op-build/buildroot/output/host/usr/bin/powerpc-linux-
#GCC-TOOL-PREFIX = /afs/bb/u/rembold/openpower/op-build/output/host/usr/powerpc64-buildroot-linux-gnu/bin/
#GCC-TOOL-PREFIX = /afs/bb/u/rembold/openpower/opcustom/op-build/buildroot/output/host/usr/bin/powerpc-linux-
endif

ifndef BINUTILS-TOOL-PREFIX
BINUTILS-TOOL-PREFIX = $(CTEPATH)/tools/ppetools/prod/powerpc-eabi/bin/
#BINUTILS-TOOL-PREFIX = /afs/bb/u/rembold/openpower/op-build/output/host/usr/bin/powerpc64-linux-
endif

ifndef P2P_SRCDIR
export P2P_SRCDIR = $(abspath ../../tools/PowerPCtoPPE)
endif

ifndef PPETRACEPP_DIR
export PPETRACEPP_DIR = $(abspath ../../tools/ppetracepp)
endif

OBJDIR = $(BASE_OBJDIR)$(SUB_OBJDIR)


CC_ASM  = $(GCC-TOOL-PREFIX)gcc
TCC      = $(PPETRACEPP_DIR)/ppetracepp $(GCC-TOOL-PREFIX)gcc
CC      = $(GCC-TOOL-PREFIX)gcc
AS      = $(BINUTILS-TOOL-PREFIX)as
AR      = $(BINUTILS-TOOL-PREFIX)ar
LD      = $(BINUTILS-TOOL-PREFIX)ld
OBJDUMP = $(BINUTILS-TOOL-PREFIX)objdump
OBJCOPY = $(BINUTILS-TOOL-PREFIX)objcopy
TCPP    = $(PPETRACEPP_DIR)/ppetracepp $(GCC-TOOL-PREFIX)gcc
THASH	= $(PPETRACEPP_DIR)/tracehash.pl
CPP     = $(GCC-TOOL-PREFIX)gcc

ifdef P2P_ENABLE
PCP     = $(P2P_SRCDIR)/ppc-ppe-pcp.py
endif


ifndef CTEPATH
$(warning The CTEPATH variable is not defined; Defaulting to /afs/awd)
export CTEPATH = /afs/awd/projects/cte
endif

ifeq "$(PK_TIMER_SUPPORT)" ""
PK_TIMER_SUPPORT  = 1
endif

ifeq "$(PK_THREAD_SUPPORT)" ""
PK_THREAD_SUPPORT = 1
endif

ifeq "$(PK_TRACE_SUPPORT)" ""
PK_TRACE_SUPPORT = 1
endif

# Generate a 16bit trace string hash prefix value based on the name of this image.  This will form
# the upper 16 bits of the 32 bit trace hash values.
ifndef PK_TRACE_HASH_PREFIX
PK_TRACE_HASH_PREFIX := $(shell echo $(IMAGE_NAME) | md5sum | cut -c1-4 | xargs -i printf "%d" 0x{})
endif


ifndef GCC-O-LEVEL
#GCC-O-LEVEL = -Os
GCC-O-LEVEL = -O -g
endif

GCC-DEFS += -DIMAGE_NAME=$(IMAGE_NAME)
GCC-DEFS += -DPK_TIMER_SUPPORT=$(PK_TIMER_SUPPORT) 
GCC-DEFS += -DPK_THREAD_SUPPORT=$(PK_THREAD_SUPPORT) 
GCC-DEFS += -DPK_TRACE_SUPPORT=$(PK_TRACE_SUPPORT)
GCC-DEFS += -DPK_TRACE_HASH_PREFIX=$(PK_TRACE_HASH_PREFIX)
GCC-DEFS += -D__PK__=1
DEFS += $(GCC-DEFS)

############################################################################

INCLUDES += $(IMG_INCLUDES) \
	-I$(PK_SRCDIR)/kernel -I$(PK_SRCDIR)/ppe42 -I$(PK_SRCDIR)/trace \
	-I$(PK_SRCDIR)/$(PPE_TYPE) -I$(PK_SRCDIR)/../include \
	-I$(PK_SRCDIR)/../tools/ppetracepp


#INCLUDES += $(IMG_INCLUDES) \
#	-I$(PK_SRCDIR)/kernel -I$(PK_SRCDIR)/ppe42 -I$(PK_SRCDIR)/trace \
#	-I$(PK_SRCDIR)/$(PPE_TYPE) -I$(PK_SRCDIR)/../include \
#	-I$(PK_SRCDIR)/../tools/ppetracepp \
#	-I$(COMMONLIB_SRCDIR)

PIPE-CFLAGS = -pipe -Wa,-m405

GCC-CFLAGS += -Wall -fsigned-char -msoft-float  \
	-mcpu=405 -m32 -mmulhw -mmultiple \
	-meabi -msdata=eabi \
	-ffreestanding -fno-common -Werror \
	-fno-inline-functions-called-once \
	-ffixed-r11 -ffixed-r12 \
    -ffixed-r14 -ffixed-r15 -ffixed-r16 -ffixed-r17 \
    -ffixed-r18 -ffixed-r19 -ffixed-r20 -ffixed-r21 \
    -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 \
    -ffixed-r26 -ffixed-r27 \
    -ffixed-cr1 -ffixed-cr2 -ffixed-cr3 -ffixed-cr4 \
    -ffixed-cr5 -ffixed-cr6 -ffixed-cr7 #-lstdc++


#GCC-CFLAGS += -Wall -fsigned-char -msoft-float  \
#	-mcpu=405 -mmulhw -mmultiple \
#	-meabi -msdata=eabi \
#	-ffreestanding -fno-common -Werror \
#	-fno-inline-functions-called-once \
#	-ffixed-r11 -ffixed-r12 \
#    -ffixed-r14 -ffixed-r15 -ffixed-r16 -ffixed-r17 \
#    -ffixed-r18 -ffixed-r19 -ffixed-r20 -ffixed-r21 \
#    -ffixed-r22 -ffixed-r23 -ffixed-r24 -ffixed-r25 \
#    -ffixed-r26 -ffixed-r27 \
#    -ffixed-cr1 -ffixed-cr2 -ffixed-cr3 -ffixed-cr4 \
#    -ffixed-cr5 -ffixed-cr6 -ffixed-cr7

CFLAGS      =  -c $(GCC-CFLAGS) $(PIPE-CFLAGS) $(GCC-O-LEVEL) $(INCLUDES) 

CPPFLAGS    = -E

#ASFLAGS		= -mppe42
ASFLAGS		=  -mppe42

ifdef P2P_ENABLE
#use this to disable optimizations (fused compare/branch etc.)
PCP-FLAG    =  

#use this to enable optimizations
#PCP-FLAG =
endif
############################################################################

#override the GNU Make implicit rule for going from a .C to a .o
%.o: %.C

$(OBJDIR)/%.s: %.C
	$(TCC) $(CFLAGS) $(DEFS) -S -o $@ $<


#override the GNU Make implicit rule for going from a .c to a .o
%.o: %.c

$(OBJDIR)/%.s: %.c
	$(TCC) $(CFLAGS) $(DEFS) -S -o $@ $<

#override the GNU Make implicit rule for going from a .S to a .o
%.o: %.S

$(OBJDIR)/%.s: %.S
	$(TCPP) $(CFLAGS) $(DEFS) $(CPPFLAGS) -o $@ $<
.PRECIOUS: $(OBJDIR)/%.s

ifndef P2P_ENABLE

$(OBJDIR)/%.o: $(OBJDIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

else

$(OBJDIR)/%.es: $(OBJDIR)/%.s
	$(PCP) $(PCP-FLAG) -f $<
.PRECIOUS: $(OBJDIR)/%.es

$(OBJDIR)/%.o: $(OBJDIR)/%.es
	$(AS) $(ASFLAGS) -o $@ $<

endif

# From the GNU 'Make' manual - these scripts uses the preprocessor to
# create dependency files (*.d), then mungs them slightly to make them
# work as Make targets. The *.d files are include-ed in the
# subdirectory Makefiles.

$(OBJDIR)/%.d: %.C
	@set -e; rm -f $@; \
	echo -n "$(OBJDIR)/" > $@.$$$$; \
	$(CC_ASM) -MM $(INCLUDES) $(CPPFLAGS) $(DEFS) $< >> $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJDIR)/%.d: %.c
	@set -e; rm -f $@; \
	echo -n "$(OBJDIR)/" > $@.$$$$; \
	$(CC_ASM) -MM $(INCLUDES) $(CPPFLAGS) $(DEFS) $< >> $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJDIR)/%.d: %.S
	@set -e; rm -f $@; \
	echo -n "$(OBJDIR)/" > $@.$$$$; \
	$(CC_ASM) -MM $(INCLUDES) $(CPPFLAGS) $(DEFS) $< >> $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

