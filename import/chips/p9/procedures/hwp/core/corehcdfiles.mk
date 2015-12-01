#  @file corehcdfiles.mk
#
#  @brief mk for including core hcode object files
#
##########################################################################
# Object Files
##########################################################################

CORE-CPP-SOURCES += p9_hcd_core_arrayinit.C
CORE-CPP-SOURCES += p9_hcd_core_chiplet_init.C
CORE-CPP-SOURCES += p9_hcd_core_chiplet_reset.C
CORE-CPP-SOURCES += p9_hcd_core_gptr_time_initf.C
CORE-CPP-SOURCES += p9_hcd_core_initf.C
CORE-CPP-SOURCES += p9_hcd_core_occ_runtime_scom.C
CORE-CPP-SOURCES += p9_hcd_core_pcb_arb.C
CORE-CPP-SOURCES += p9_hcd_core_poweron.C
CORE-CPP-SOURCES += p9_hcd_core_ras_runtime_scom.C
CORE-CPP-SOURCES += p9_hcd_core_repair_initf.C
CORE-CPP-SOURCES += p9_hcd_core_runinit.C
CORE-CPP-SOURCES += p9_hcd_core_scomcust.C
CORE-CPP-SOURCES += p9_hcd_core_scominit.C
CORE-CPP-SOURCES += p9_hcd_core_startclocks.C
CORE-CPP-SOURCES += p9_thread_control.C
CORE-CPP-SOURCES += p9_sbe_instruct_start.C

CORE-C-SOURCES   +=
CORE-S-SOURCES   +=

CORE_OBJECTS     += $(CORE-CPP-SOURCES:.C=.o)
CORE_OBJECTS     += $(CORE-C-SOURCES:.c=.o)
CORE_OBJECTS     += $(CORE-S-SOURCES:.S=.o)

