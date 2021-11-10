# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p9/procedures/xml/error_info/hwpErrors.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2021
# [+] International Business Machines Corp.
#
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied. See the License for the specific language governing
# permissions and limitations under the License.
#
# IBM_PROLOG_END_TAG
#  @file hwpErrors.mk
#
#  @brief mk for including library common error files
#
##########################################################################
# Error Files
##########################################################################

P9_PERV_CURR_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# This variable name must not change
ERROR_XML_FILES += $(wildcard $(P9_PERV_CURR_DIR)/p9_sbe_check_master_stop15_errors.xml)
ERROR_XML_FILES += $(wildcard $(P9_PERV_CURR_DIR)/p9_sbe_tracearray_errors.xml)
ERROR_XML_FILES += $(wildcard $(P9_PERV_CURR_DIR)/p9_block_wakeup_intr_errors.xml)
