#  @file pervfiles.mk
#
#  @brief mk for including perv object files
#
##########################################################################
# Object Files
##########################################################################

PERV-CPP-SOURCES =p9_sbe_arrayinit.C
PERV-CPP-SOURCES +=p9_sbe_attr_setup.C
PERV-CPP-SOURCES +=p9_sbe_check_master.C
PERV-CPP-SOURCES +=p9_sbe_chiplet_init.C
PERV-CPP-SOURCES +=p9_sbe_chiplet_pll_initf.C
PERV-CPP-SOURCES +=p9_sbe_chiplet_pll_setup.C
PERV-CPP-SOURCES +=p9_sbe_chiplet_reset.C
PERV-CPP-SOURCES +=p9_sbe_enable_seeprom.C
PERV-CPP-SOURCES +=p9_sbe_gptr_time_repr_initf.C
PERV-CPP-SOURCES +=p9_sbe_lpc_init.C
PERV-CPP-SOURCES +=p9_sbe_nest_enable_ridi.C
PERV-CPP-SOURCES +=p9_sbe_nest_initf.C
PERV-CPP-SOURCES +=p9_sbe_nest_startclocks.C
PERV-CPP-SOURCES +=p9_sbe_npll_initf.C
PERV-CPP-SOURCES +=p9_sbe_npll_setup.C
PERV-CPP-SOURCES +=p9_sbe_select_ex.C
PERV-CPP-SOURCES +=p9_sbe_startclock_chiplets.C
PERV-CPP-SOURCES +=p9_sbe_tp_arrayinit.C
PERV-CPP-SOURCES +=p9_sbe_tp_chiplet_init1.C
PERV-CPP-SOURCES +=p9_sbe_tp_chiplet_init2.C
PERV-CPP-SOURCES +=p9_sbe_tp_chiplet_init3.C
PERV-CPP-SOURCES +=p9_sbe_tp_chiplet_reset.C
PERV-CPP-SOURCES +=p9_sbe_tp_enable_ridi.C
PERV-CPP-SOURCES +=p9_sbe_tp_gptr_time_repr_initf.C
PERV-CPP-SOURCES +=p9_sbe_tp_initf.C
PERV-CPP-SOURCES +=p9_sbe_tp_ld_image.C
PERV-CPP-SOURCES +=p9_sbe_tp_switch_gears.C
PERV-CPP-SOURCES +=p9_sbe_setup_evid.C
PERV-CPP-SOURCES +=p9_perv_sbe_cmn.C
PERV-CPP-SOURCES +=p9_sbe_common.C
PERV-CPP-SOURCES +=p9_sbe_gear_switcher.C
PERV-CPP-SOURCES +=p9_sbe_check_master_stop15.C
PERV-CPP-SOURCES +=p9_ram_core.C

PERV-C-SOURCES =
PERV-S-SOURCES =

PERV_OBJECTS += $(PERV-CPP-SOURCES:.C=.o)
PERV_OBJECTS += $(PERV-C-SOURCES:.c=.o)
PERV_OBJECTS += $(PERV-S-SOURCES:.S=.o)
