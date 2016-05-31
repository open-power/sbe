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
# TODO via RTC 152424
# p9_ram_core.C will go to runtime makefile. Currently this procedure is in
# perv directory. As we are putting perv procedures currently in SEEPROM.
# So compiling p9_ram_core.C in nest makefile.
NEST-CPP-SOURCES +=p9_ram_core.C
# TODO via RTC 152424
# swicth gear proecdures are in perv directory. But these procedures needs to
# be executed from PIBMEM. So compiling these is nest makefile.
NEST-CPP-SOURCES +=p9_sbe_gear_switcher.C
NEST-CPP-SOURCES +=p9_sbe_tp_switch_gears.C
NEST-C-SOURCES =
NEST-S-SOURCES =

NEST_OBJECTS += $(NEST-CPP-SOURCES:.C=.o)
NEST_OBJECTS += $(NEST-C-SOURCES:.c=.o)
NEST_OBJECTS += $(NEST-S-SOURCES:.S=.o)
