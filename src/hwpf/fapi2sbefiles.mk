# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/hwpf/fapi2sbefiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2020
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
#  @file fapi2ppefiles.mk
#
#  @brief mk for including fapi2 object files
#
#  @page ChangeLogs Change Logs
#  @section fapi2ppefiles.mk
#  @verbatim
#
#
# Change Log ******************************************************************
# Flag     Defect/Feature  User        Date         Description
# ------   --------------  ----------  ------------ -----------
#
# @endverbatim
#
##########################################################################
# Object Files
##########################################################################

FAPI2PLAT-CPP-SOURCES += plat_hw_access.C
FAPI2PLAT-CPP-SOURCES += p10_scom_addr.C
ifneq ($(project),$(filter $(project), power_dft))
FAPI2PLAT-CPP-SOURCES += plat_i2c_access.C
endif
FAPI2PLAT-CPP-SOURCES += plat_utils.C
FAPI2PLAT-CPP-SOURCES += target.C
FAPI2PLAT-CPP-SOURCES += ffdc.C
FAPI2PLAT-CPP-SOURCES += plat_ring_traverse.C
FAPI2PLAT-CPP-SOURCES += plat_hwp_data_stream.C
FAPI2PLAT-C-SOURCES =
FAPI2PLAT-S-SOURCES =

FAPI2PLATLIB_OBJECTS += $(FAPI2PLAT-CPP-SOURCES:.C=.o) $(FAPI2PLAT-C-SOURCES:.c=.o) $(FAPI2PLAT-S-SOURCES:.S=.o)

FAPI2PLAT-SEEPROM-CPP-SOURCES += target.C
FAPI2PLATLIB_SEEPROM_OBJECTS += $(FAPI2PLAT-SEEPROM-CPP-SOURCES:.C=.o)

FAPI2PLAT-SBEM-CPP-SOURCES += plat_hw_access.C
FAPI2PLAT-SBEM-CPP-SOURCES += plat_utils.C
FAPI2PLAT-SBEM-CPP-SOURCES += target.C
FAPI2PLAT-SBEM-CPP-SOURCES += ffdc.C
FAPI2PLAT-SBEM-C-SOURCES =
FAPI2PLAT-SBEM-S-SOURCES =

SBEM_FAPI2PLATLIB_OBJECTS += $(FAPI2PLAT-SBEM-CPP-SOURCES:.C=.o) $(FAPI2PLAT-SBEM-C-SOURCES:.c=.o) $(FAPI2PLAT-SBEM-S-SOURCES:.S=.o)

