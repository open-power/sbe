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
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_setup_evid_errors.xml


