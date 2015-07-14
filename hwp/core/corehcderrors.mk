#  @file corehcderrors.mk
#
#  @brief mk for including core error files
#
#  @page ChangeLogs Change Logs
#  @section corehcderrors.mk
#
##########################################################################
# Error Files
##########################################################################

CORE_CURR_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# This variable name must not change
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_arrayinit_errors.xml
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_chiplet_init_errors.xml
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_chiplet_reset_errors.xml
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_gptr_time_initf_errors.xml
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_initf_errors.xml
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_occ_runtime_scom_errors.xml
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_pcb_arb_errors.xml
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_poweron_errors.xml
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_ras_runtime_scom_errors.xml
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_repair_initf_errors.xml
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_runinit_errors.xml
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_scomcust_errors.xml
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_scominit_errors.xml
#ERROR_XML_FILES += $(CORE_CURR_DIR)/p9_hcd_core_startclocks_errors.xml


