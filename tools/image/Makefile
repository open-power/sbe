############################################################################

# Makefile for image tools
# works on X86 Linux hosts.

# Make targets:

# all          :
#
# utilities    : Build utility programs and procedures
#
# clean        : Removes the bin/ directory and all symbolic links
#

############################################################################


ifeq ($(CTEPATH),)
$(warning CTEPATH not defined; defaulting to awd)
CTEPATH = /afs/awd/projects/cte
endif

#  Locations of required headers.
INCLUDES += -I. -I../../ -I../../utils
INCLUDES += -I ../../sbe/image/
INCLUDES += -I ../../sbe/sbefw/
INCLUDES += -I ../../sbe/hwpf/include/plat
INCLUDES += -I ../../sbe/hwpf/include
INCLUDES += -I ../../hwpf/include/plat
INCLUDES += -I ../../hwpf/include/
INCLUDES += -I ../../pk/ppe42/
INCLUDES += -I ../../pk/kernel/
INCLUDES += -I ../../pk/std/
INCLUDES += -I ../../pk/trace/
INCLUDES += -I ../../tools/ppetracepp/
INCLUDES += -I ../../import/hwpf/fapi2/include/

# Under Linux the scheme is to use a common compiler to create procedures.
# However, the common compiler can be VERY slow, so if the system compiler is
# also 4.1.2 we're using that one instead.  Also, the Linux FAPI libraries we
# link with are 32-bit only so we need to force 32-bit mode.

ifeq ($(wildcard /etc/ldap.conf), )
  GSACELL     = ausgsa
else
  GSACELL     = $(shell cat /etc/ldap.conf | grep "host " | \
        cut -d" " -f2 | cut -d. -f1)
endif

GCC-RELEASE = 4.8.2
GCC-VERSION = $(shell gcc -v 2>&1 | grep "$(GCC-RELEASE)")

ifeq ($(GCC-VERSION),)
$(error wrong compiler version. Use $(GCC-RELEASE) compiler. Try: "scl enable devtoolset-2 bash")
else
CC   = gcc
CXX  = g++
endif

#UTILITIES-SOURCES += ../../sbe/image/sbe_xip_image.c
UTILITIES-SOURCES = sbe_xip_tool.c sbe_default_tool.c

UTILITIES = sbe_xip_tool sbe_default_tool

#  Utility targets
UTILITIES-OBJc         = $(patsubst %.c,bin/%.o,$(UTILITIES-SOURCES))
UTILITIES-OBJECTS      += $(patsubst %.C,bin/%.o,$(UTILITIES-OBJc))
UTILITIES-DEPENDENCIES = $(patsubst %.o,%.d,$(UTILITIES-OBJECTS))
UTILITIES-EXECUTABLES  = $(patsubst %,bin/%,$(UTILITIES))


.PHONY : utilities buildBinDir
utilities: buildBinDir $(UTILITIES-EXECUTABLES)

buildBinDir:
	mkdir -p bin

bin/%.o: %.c
	$(CXX) -std=c++11 $(INCLUDES) $(CXXFLAGS) -DDEBUG_SBE_XIP_IMAGE=1 -DFAPI2_NO_FFDC -c -o $@ $<

bin/sbe_xip_image.o: ../../sbe/image/sbe_xip_image.c
	$(CXX) -std=c++11 $(INCLUDES) $(CXXFLAGS) -DDEBUG_SBE_XIP_IMAGE=1 -DFAPI2_NO_FFDC -c -o $@ $<

bin/sbe_xip_tool: bin/sbe_xip_image.o bin/p9_ring_identification.o bin/sbe_xip_tool.o
	$(CXX) $(CXXFLAGS) ${INCLUDES} -o $@ $^
	ln -sf bin/sbe_xip_tool sbe_xip_tool

bin/sbe_default_tool: bin/sbe_xip_image.o bin/sbe_default_tool.o
	$(CXX) $(CXXFLAGS) ${INCLUDES} -o $@ $^
	ln -sf bin/sbe_default_tool sbe_default_tool

clean:
	rm -f sbe_xip_tool sbe_default_tool
	rm -rf bin
	mkdir -p bin
