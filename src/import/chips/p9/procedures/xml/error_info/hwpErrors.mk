# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p9/procedures/xml/error_info/hwpErrors.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2017
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
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_select_ex_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_check_master_stop15_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_pm_ocb_indir_access_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_pm_ocb_init_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_block_wakeup_intr_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_ram_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_tp_switch_gears_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_adu_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_npll_initf_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_chiplet_pll_initf_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_scominit_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_plat_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_hcd_core_stopclocks_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_hcd_cache_stopclocks_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_hcd_l2_stopclocks_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_l2_flush_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_l3_flush_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_check_quiesce_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_pm_pfet_control_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_hcd_cache_scominit_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_sbe_tracearray_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_hcd_cache_initf_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_quad_power_off_errors.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_fastarray.xml
ERROR_XML_FILES += $(PERV_CURR_DIR)/p9_suspend_io_errors.xml
