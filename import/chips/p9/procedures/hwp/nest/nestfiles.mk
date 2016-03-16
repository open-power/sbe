#  @file nestfiles.mk
#
#  @brief mk for including nest object files
#
##########################################################################
# Object Files
##########################################################################

NEST-CPP-SOURCES = p9_sbe_mcs_setup.C
NEST-CPP-SOURCES +=p9_sbe_scominit.C
NEST-CPP-SOURCES +=p9_sbe_fabricinit.C
NEST-CPP-SOURCES +=p9_fbc_utils.C
NEST-CPP-SOURCES +=p9_sbe_load_bootloader.C
NEST-CPP-SOURCES +=p9_pba_access.C
NEST-CPP-SOURCES +=p9_pba_coherent_utils.C
NEST-CPP-SOURCES +=p9_pba_setup.C
NEST-CPP-SOURCES +=p9_adu_access.C
NEST-CPP-SOURCES +=p9_adu_setup.C
NEST-CPP-SOURCES +=p9_adu_coherent_utils.C
NEST-C-SOURCES =
NEST-S-SOURCES =

NEST_OBJECTS += $(NEST-CPP-SOURCES:.C=.o)
NEST_OBJECTS += $(NEST-C-SOURCES:.c=.o)
NEST_OBJECTS += $(NEST-S-SOURCES:.S=.o)
