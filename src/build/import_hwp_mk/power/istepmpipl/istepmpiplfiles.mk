# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/import_hwp_mk/power/istepmpipl/istepmpiplfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2019
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
#  @file istepmpipl.mk
#
#  @brief mk for including istepmpipl object files
#
##########################################################################
# Object Files
##########################################################################

ISTEPMPIPL-CPP-SOURCES = p9_hcd_core_stopclocks.C
ISTEPMPIPL-CPP-SOURCES +=p9_hcd_cache_stopclocks.C
ISTEPMPIPL-CPP-SOURCES +=p9_stopclocks.C
ISTEPMPIPL-CPP-SOURCES +=p9_common_stopclocks.C
ISTEPMPIPL-CPP-SOURCES +=p9_cplt_stopclocks.C
ISTEPMPIPL-CPP-SOURCES +=p9_nest_stopclocks.C
ISTEPMPIPL-CPP-SOURCES +=p9_tp_stopclocks.C
ISTEPMPIPL-CPP-SOURCES +=p9_check_chiplet_states.C
ISTEPMPIPL-CPP-SOURCES +=p9_hcd_l2_stopclocks.C
ISTEPMPIPL-CPP-SOURCES +=p9_block_wakeup_intr.C
ISTEPMPIPL-CPP-SOURCES +=p9_l2_flush.C
ISTEPMPIPL-CPP-SOURCES +=p9_l3_flush.C
ISTEPMPIPL-CPP-SOURCES +=p9_sbe_check_quiesce.C
ISTEPMPIPL-CPP-SOURCES +=p9_inject_local_xstop.C
ISTEPMPIPL-CPP-SOURCES +=p9_sbe_sequence_drtm.C
ISTEPMPIPL-CPP-SOURCES +=p9_query_core_access_state.C
ISTEPMPIPL-CPP-SOURCES +=p9_quad_power_off.C
ISTEPMPIPL-CPP-SOURCES +=p9_pm_pfet_control.C
ISTEPMPIPL-CPP-SOURCES +=p9_suspend_io.C
ISTEPMPIPL-CPP-SOURCES +=p9_perv_sbe_cmn.C
ISTEPMPIPL-CPP-SOURCES +=p9_suspend_powman.C
ISTEPMPIPL-CPP-SOURCES +=p9_collect_suspend_ffdc.C
ISTEPMPIPL-CPP-SOURCES +=p9_eq_clear_atomic_lock.C
ISTEPMPIPL-CPP-SOURCES +=p9_query_cache_access_state.C
# Fast array procedures are moved here to locate it on seeprom
ISTEPMPIPL-CPP-SOURCES += p9_sbe_fastarray_abist_catchup.C
ISTEPMPIPL-CPP-SOURCES += p9_sbe_fastarray_cleanup.C
ISTEPMPIPL-CPP-SOURCES += p9_sbe_fastarray_setup.C
# io eol toggle procedure moved here to locate it on seeprom
ISTEPMPIPL-CPP-SOURCES += p9_sbe_io_eol_toggle.C
ISTEPMPIPL-CPP-SOURCES += nvdimm_workarounds.C
ISTEPMPIPL-C-SOURCES =
ISTEPMPIPL-S-SOURCES =

ISTEPMPIPL_OBJECTS += $(ISTEPMPIPL-CPP-SOURCES:.C=.o)
ISTEPMPIPL_OBJECTS += $(ISTEPMPIPL-C-SOURCES:.c=.o)
ISTEPMPIPL_OBJECTS += $(ISTEPMPIPL-S-SOURCES:.S=.o)
