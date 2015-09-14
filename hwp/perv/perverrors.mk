#  @file perverrors.mk
#
#  @brief mk for including library common error files
#
#  @page ChangeLogs Change Logs
#  @section perverrors.mk
#
##########################################################################
# Error Files
##########################################################################

PERV_CURR_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# This variable name must not change
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_chiplet_init_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_arrayinit_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_gear_switcher_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_setup_evid_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_common_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_perv_sbe_cmn_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_nest_startclocks_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_tp_chiplet_init3_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_npll_setup_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_chiplet_pll_setup_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_tp_arrayinit_errors.xml
