#  @file nesterrors.mk
#
#  @brief mk for including error files
#
#  @page ChangeLogs Change Logs
#  @section nesterrors.mk
#
##########################################################################
# Error Files
##########################################################################

NEST_CURR_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# This variable name must not change
ERROR_XML_FILES += $(NEST_CURR_DIR)/p9_sbe_fabricinit_errors.xml


