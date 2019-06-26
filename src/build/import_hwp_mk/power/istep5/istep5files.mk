# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/import_hwp_mk/power/istep5/istep5files.mk $
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
#  @file istep5files.mk
#
#  @brief mk for including istep5 object files
#
##########################################################################
# Object Files
##########################################################################

ISTEP5-CPP-SOURCES = p10_sbe_instruct_start.C
ISTEP5-CPP-SOURCES +=p10_thread_control.C
ISTEP5-CPP-SOURCES +=p10_sbe_core_spr_setup.C
ISTEP5-CPP-SOURCES +=p10_sbe_load_bootloader.C
ISTEP5-C-SOURCES =
ISTEP5-S-SOURCES =

ISTEP5_OBJECTS += $(ISTEP5-CPP-SOURCES:.C=.o)
ISTEP5_OBJECTS += $(ISTEP5-C-SOURCES:.c=.o)
ISTEP5_OBJECTS += $(ISTEP5-S-SOURCES:.S=.o)
