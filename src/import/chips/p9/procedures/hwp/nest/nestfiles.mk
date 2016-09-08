# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p9/procedures/hwp/nest/nestfiles.mk $
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
#  @file nestfiles.mk
#
#  @brief mk for including nest object files
#
##########################################################################
# Object Files
##########################################################################

NEST-CPP-SOURCES = p9_sbe_mcs_setup.C
NEST-CPP-SOURCES +=p9_sbe_scominit.C
NEST-CPP-SOURCES +=p9_sbe_fabricinit.C
NEST-CPP-SOURCES +=p9_fbc_utils.C
NEST-CPP-SOURCES +=p9_sbe_load_bootloader.C
NEST-CPP-SOURCES +=p9_pba_access.C
NEST-CPP-SOURCES +=p9_pba_coherent_utils.C
NEST-CPP-SOURCES +=p9_pba_setup.C
NEST-CPP-SOURCES +=p9_adu_access.C
NEST-CPP-SOURCES +=p9_adu_setup.C
NEST-CPP-SOURCES +=p9_adu_coherent_utils.C
# TODO via RTC 152424
# p9_ram_core.C will go to runtime makefile. Currently this procedure is in
# perv directory. As we are putting perv procedures currently in SEEPROM.
# So compiling p9_ram_core.C in nest makefile.
NEST-CPP-SOURCES +=p9_ram_core.C
# TODO via RTC 152424
# swicth gear proecdures are in perv directory. But these procedures needs to
# be executed from PIBMEM. So compiling these is nest makefile.
NEST-CPP-SOURCES +=p9_sbe_gear_switcher.C
NEST-CPP-SOURCES +=p9_sbe_tp_switch_gears.C
# TODO via RTC 152424
# thread control procedure is in core directory, but this procedure needs to be
# in PIBMEM for quiesce case, So compiling in nest makefile
NEST-CPP-SOURCES +=p9_thread_control.C
NEST-CPP-SOURCES +=p9_sbe_instruct_start.C
NEST-C-SOURCES =
NEST-S-SOURCES =

NEST_OBJECTS += $(NEST-CPP-SOURCES:.C=.o)
NEST_OBJECTS += $(NEST-C-SOURCES:.c=.o)
NEST_OBJECTS += $(NEST-S-SOURCES:.S=.o)
