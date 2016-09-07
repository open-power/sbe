# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p9/procedures/hwp/core/corehcdfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2016
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
#  @file corehcdfiles.mk
#
#  @brief mk for including core hcode object files
#
##########################################################################
# Object Files
##########################################################################

CORE-CPP-SOURCES += p9_hcd_core_arrayinit.C
CORE-CPP-SOURCES += p9_hcd_core_chiplet_init.C
CORE-CPP-SOURCES += p9_hcd_core_chiplet_reset.C
CORE-CPP-SOURCES += p9_hcd_core_gptr_time_initf.C
CORE-CPP-SOURCES += p9_hcd_core_initf.C
CORE-CPP-SOURCES += p9_hcd_core_occ_runtime_scom.C
CORE-CPP-SOURCES += p9_hcd_core_pcb_arb.C
CORE-CPP-SOURCES += p9_hcd_core_poweron.C
CORE-CPP-SOURCES += p9_hcd_core_ras_runtime_scom.C
CORE-CPP-SOURCES += p9_hcd_core_repair_initf.C
CORE-CPP-SOURCES += p9_hcd_core_runinit.C
CORE-CPP-SOURCES += p9_hcd_core_scomcust.C
CORE-CPP-SOURCES += p9_hcd_core_scominit.C
CORE-CPP-SOURCES += p9_hcd_core_startclocks.C
# TODO via RTC 152424
# thread control procedure is in core directory, but this procedure needs to be
# in PIBMEM for quiesce case, So compiling in nest makefile
#CORE-CPP-SOURCES += p9_thread_control.C
#CORE-CPP-SOURCES += p9_sbe_instruct_start.C
CORE-CPP-SOURCES += p9_hcd_core_stopclocks.C

CORE-C-SOURCES   +=
CORE-S-SOURCES   +=

CORE_OBJECTS     += $(CORE-CPP-SOURCES:.C=.o)
CORE_OBJECTS     += $(CORE-C-SOURCES:.c=.o)
CORE_OBJECTS     += $(CORE-S-SOURCES:.S=.o)

