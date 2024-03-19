# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/import_hwp_mk/power/istepcommon/istepcommonfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2024
# [+] International Business Machines Corp.
# [+] Justin.Ginn@ibm.com
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
#  @file istepCommonfiles.mk
#
#  @brief mk for including istepcommon object files
#
##########################################################################
# Object Files
##########################################################################
ISTEPCOMMON-CPP-SOURCES +=p10_pba_access.C
ISTEPCOMMON-CPP-SOURCES +=p10_pba_coherent_utils.C
ISTEPCOMMON-CPP-SOURCES +=p10_pba_setup.C
ISTEPCOMMON-CPP-SOURCES +=p10_getsram.C
ISTEPCOMMON-CPP-SOURCES +=p10_getputsram_utils.C
ISTEPCOMMON-CPP-SOURCES +=p10_getsram_io_ppe.C
ISTEPCOMMON-CPP-SOURCES +=p10_io_ppe_utils.C
ISTEPCOMMON-CPP-SOURCES +=p10_iop_xram_utils.C
ISTEPCOMMON-CPP-SOURCES +=p10_putsram.C
ISTEPCOMMON-CPP-SOURCES +=p10_putsram_io_ppe.C
ISTEPCOMMON-CPP-SOURCES +=p10_read_xram.C
ISTEPCOMMON-CPP-SOURCES +=p10_write_xram.C
ISTEPCOMMON-CPP-SOURCES +=p10_qme_sram_access.C
ISTEPCOMMON-CPP-SOURCES +=p10_pm_sram_access_utils.C
ISTEPCOMMON-CPP-SOURCES +=p10_pm_ocb_indir_access.C
ISTEPCOMMON-CPP-SOURCES +=p10_pm_ocb_indir_setup_circular.C
ISTEPCOMMON-CPP-SOURCES +=p10_pm_ocb_indir_setup_linear.C
ISTEPCOMMON-CPP-SOURCES +=p10_pm_ocb_init.C
ISTEPCOMMON-CPP-SOURCES +=p10_ram_core.C
ISTEPCOMMON-CPP-SOURCES +=p10_getmempba.C
ISTEPCOMMON-CPP-SOURCES +=p10_getputmempba_utils.C
ISTEPCOMMON-CPP-SOURCES +=p10_getmemproc.C
ISTEPCOMMON-CPP-SOURCES +=p10_putmemproc.C
ISTEPCOMMON-CPP-SOURCES +=p10_getputmemproc_utils.C
ISTEPCOMMON-CPP-SOURCES +=p10_plat_ring_traverse.C
ISTEPCOMMON-CPP-SOURCES +=p10_putRingUtils.C
ISTEPCOMMON-CPP-SOURCES +=p10_suspend_io.C
ISTEPCOMMON-CPP-SOURCES +=exp_i2c.C
ISTEPCOMMON-CPP-SOURCES +=p10_query_host_meminfo.C
ISTEPCOMMON-CPP-SOURCES +=p10_sbe_spi_cmd.C
#ifdef DFT
ISTEPCOMMON-CPP-SOURCES +=p10_putmempba.C
#endif
#istep5 Procedure but this is required to run from PIBMEM
ISTEPCOMMON-C-SOURCES =
ISTEPCOMMON-S-SOURCES =

ISTEPCOMMON_OBJECTS += $(ISTEPCOMMON-CPP-SOURCES:.C=.o)
ISTEPCOMMON_OBJECTS += $(ISTEPCOMMON-C-SOURCES:.c=.o)
ISTEPCOMMON_OBJECTS += $(ISTEPCOMMON-S-SOURCES:.S=.o)
