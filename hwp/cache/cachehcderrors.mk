#  @file cachehcderrors.mk
#
#  @brief mk for including cache error files
#
#  @page ChangeLogs Change Logs
#  @section cachehcderrors.mk
#
##########################################################################
# Error Files
##########################################################################

CACHE_CURR_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# This variable name must not change
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_arrayinit.C
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_chiplet_init.C
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_chiplet_reset.C
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_dpll_setup.C
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_gptr_time_initf.C
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_initf.C
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_occ_runtime_scom.C
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_poweron.C
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_ras_runtime_scom.C
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_repair_initf.C
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_runinit.C
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_scomcust.C
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_scominit.C
#ERROR_XML_FILES += $(CACHE_CURR_DIR)/p9_hcd_cache_startclocks.C
