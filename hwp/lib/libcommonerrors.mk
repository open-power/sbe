#  @file libcommonerrors.mk
#
#  @brief Error XML mk for common library files
#
#  @page ChangeLogs Change Logs
#  @section libcommonerrors.mk
#  @verbatim
#
#
# @endverbatim
#
##########################################################################
# Error Files
##########################################################################

LIB_CURR_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# This variable name must not change
ERROR_XML_FILES += $(LIB_CURR_DIR)/p9_avsbus_lib_errors.xml
#ERROR_XML_FILES +=  $(LIB_CURR_DIR)/p9_common_pro_epi_log_errors.xml
