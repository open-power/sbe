#  @file pervfiles.mk
#
#  @brief mk for including perv object files
#
#  @page ChangeLogs Change Logs
#  @section pervfiles.mk
#  @verbatim
#
#  @endverbatim
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
PERV-CPP-SOURCES +=p9_select_boot_master.C
PERV-CPP-SOURCES +=p9_select_clock_mux.C
PERV-CPP-SOURCES +=p9_set_fsi_gp_shadow.C
PERV-CPP-SOURCES +=p9_setup_clock_term.C
PERV-CPP-SOURCES +=p9_setup_sbe_config.C
PERV-CPP-SOURCES +=p9_start_cbs.C

PERV-C-SOURCES =
PERV-S-SOURCES =

PERV_OBJECTS += $(PERV-CPP-SOURCES:.C=.o)
PERV_OBJECTS += $(PERV-C-SOURCES:.c=.o)
PERV_OBJECTS += $(PERV-S-SOURCES:.S=.o)
