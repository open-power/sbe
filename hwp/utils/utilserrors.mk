#  @file utilserrors.mk
#
#  @brief mk for including utils error files
#
#  @page ChangeLogs Change Logs
#  @section utilserrors.mk
#
##########################################################################
# Error Files
##########################################################################

UTILS_CURR_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# This variable name must not change
#ERROR_XML_FILES += $(UTILS_CURR_DIR)/p9_XXX_errors.xml
