#  @file nestfiles.mk
#
#  @brief mk for including nest object files
#
#  @page ChangeLogs Change Logs
#  @section nestfiles.mk
#  @verbatim
#
#  @endverbatim
#
##########################################################################
# Object Files
##########################################################################

NEST-CPP-SOURCES = p9_sbe_mcs_setup.C
NEST-CPP-SOURCES +=p9_sbe_scominit.C
NEST-CPP-SOURCES +=p9_sbe_fabricinit.C

NEST-C-SOURCES =
NEST-S-SOURCES =

NEST_OBJECTS += $(NEST-CPP-SOURCES:.C=.o)
NEST_OBJECTS += $(NEST-C-SOURCES:.c=.o)
NEST_OBJECTS += $(NEST-S-SOURCES:.S=.o)
