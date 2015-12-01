#  @file cachehcdfiles.mk
#
#  @brief mk for including cache hcode object files
#
##########################################################################
# Object Files
##########################################################################

CACHE-CPP-SOURCES += p9_hcd_cache_arrayinit.C
CACHE-CPP-SOURCES += p9_hcd_cache_chiplet_init.C
CACHE-CPP-SOURCES += p9_hcd_cache_chiplet_reset.C
CACHE-CPP-SOURCES += p9_hcd_cache_dpll_setup.C
CACHE-CPP-SOURCES += p9_hcd_cache_gptr_time_initf.C
CACHE-CPP-SOURCES += p9_hcd_cache_initf.C
CACHE-CPP-SOURCES += p9_hcd_cache_occ_runtime_scom.C
CACHE-CPP-SOURCES += p9_hcd_cache_poweron.C
CACHE-CPP-SOURCES += p9_hcd_cache_ras_runtime_scom.C
CACHE-CPP-SOURCES += p9_hcd_cache_repair_initf.C
CACHE-CPP-SOURCES += p9_hcd_cache_runinit.C
CACHE-CPP-SOURCES += p9_hcd_cache_scomcust.C
CACHE-CPP-SOURCES += p9_hcd_cache_scominit.C
CACHE-CPP-SOURCES += p9_hcd_cache_startclocks.C

CACHE-C-SOURCES   +=
CACHE-S-SOURCES   +=

CACHE_OBJECTS     += $(CACHE-CPP-SOURCES:.C=.o)
CACHE_OBJECTS     += $(CACHE-C-SOURCES:.c=.o)
CACHE_OBJECTS     += $(CACHE-S-SOURCES:.S=.o)

