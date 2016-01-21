#  @file hwpErrors.mk
#
#  @brief mk for including library common error files
#
##########################################################################
# Error Files
##########################################################################

PERV_CURR_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# This variable name must not change
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_perv_sbe_cmn_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_arrayinit_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_chiplet_init_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_chiplet_pll_setup_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_common_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_fabricinit_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_gear_switcher_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_nest_startclocks_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_npll_setup_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_tp_arrayinit_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_tp_chiplet_init3_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_mcs_setup_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_hcd_cache_dpll_setup_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_common_poweronoff_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_hcd_cache_startclocks_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_hcd_core_startclocks_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_pba_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_load_bootloader_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_thread_control_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_startclock_chiplets_errors.xml
