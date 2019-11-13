# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/import_hwp_mk/power/istep14_16/istep14_16files.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2019
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
##########################################################################
# Object Files
##########################################################################
ISTEP14_16-CPP-SOURCES = p10_sbe_apply_xscom_inits.C
ISTEP14_16-CPP-SOURCES += p10_sbe_apply_fbc_rt_settings.C
ISTEP14_16-CPP-SOURCES += p10_sbe_exit_cache_contained.C
ISTEP14_16-CPP-SOURCES += p10_sbe_stop_hb.C
ISTEP14_16-CPP-SOURCES += p10_revert_sbe_mcs_setup.C
ISTEP14_16-CPP-SOURCES += p10_sbe_purge_hb.C
ISTEP14_16-CPP-SOURCES += p10_l2_flush.C
ISTEP14_16-CPP-SOURCES += p10_l3_flush.C
ISTEP14_16-CPP-SOURCES += p10_sbe_check_master_stop15.C
ISTEP14_16-CPP-SOURCES += p10_sbe_powerdown_backing_caches.C
ISTEP14_16-CPP-SOURCES += p10_block_wakeup_intr.C
ISTEP14_16-CPP-SOURCES += p10_query_core_stop_state.C
ISTEP14_16-C-SOURCES =
ISTEP14_16-S-SOURCES =

ISTEP14_16_OBJECTS += $(ISTEP14_16-CPP-SOURCES:.C=.o)
ISTEP14_16_OBJECTS += $(ISTEP14_16-C-SOURCES:.c=.o)
ISTEP14_16_OBJECTS += $(ISTEP14_16-S-SOURCES:.S=.o)
