# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/power_axone_defs.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017,2019
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
# Levels of SBE logging
# 0 - No tracing
# 1 - Error
# 2 - Error, info
# 3 - Error, info, entry/exit
# 4 - Error, info, entry/exit, debug
SBE_TRACE_LEVEL_DEF = 2

FAPI_TRACE_LEVEL_DEF = 2

HOST_INTERFACE_AVAILABLE = 1
PERIODIC_IO_TOGGLE_SUPPORTED = 1

export SBE_S0_SUPPORT = 1
export PIBMEM_REPAIR_SCOM_AXONE = 1

ISTEP2_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/istep2
ISTEP3_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/istep3
ISTEP4_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/istep4
ISTEP5_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/istep5
ISTEPMPIPL_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/istepmpipl
ISTEPCOMMON_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/istepcommon
ARRAYACCESS_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/arrayaccess

OBJDIR-ISTEP2 = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/istep2
OBJDIR-ISTEP3 = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/istep3
OBJDIR-ISTEP4 = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/istep4
OBJDIR-ISTEP5 = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/istep5
OBJDIR-ISTEPMPIPL = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/istepmpipl
OBJDIR-ISTEPCOMMON = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/istepcommon
OBJDIR-ARRAYACCESS = $(BASE_OBJDIR)/arrayaccess

PROJ_SUBDIRS += $(ISTEP2_INFRA_DIR)
PROJ_LIB_DIRS += -L$(OBJDIR-ISTEP2)
PROJ_LLIBS += -listep2

PROJ_SUBDIRS += $(ISTEP3_INFRA_DIR)
PROJ_LIB_DIRS += -L$(OBJDIR-ISTEP3)
PROJ_LLIBS += -listep3

PROJ_SUBDIRS += $(ISTEP4_INFRA_DIR)
PROJ_LIB_DIRS += -L$(OBJDIR-ISTEP4)
PROJ_LLIBS += -listep4

PROJ_SUBDIRS += $(ISTEP5_INFRA_DIR)
PROJ_LIB_DIRS += -L$(OBJDIR-ISTEP5)
PROJ_LLIBS += -listep5

PROJ_SUBDIRS += $(ISTEPMPIPL_INFRA_DIR)
PROJ_LIB_DIRS += -L$(OBJDIR-ISTEPMPIPL)
PROJ_LLIBS += -listepmpipl

PROJ_SUBDIRS += $(ISTEPCOMMON_INFRA_DIR)
PROJ_LIB_DIRS += -L$(OBJDIR-ISTEPCOMMON)
PROJ_LLIBS += -listepcommon

PROJ_SUBDIRS += $(ARRAYACCESS_INFRA_DIR)
PROJ_LIB_DIRS += -L$(OBJDIR-ARRAYACCESS)
PROJ_LLIBS += -larrayaccess
#########################################################
# mandatory defines                                     #
#########################################################
IMAGE_SUFFIX := AXONE
GCC-DEFS += -DDD2
GCC-DEFS += -DSBE_AXONE_CONFIG

IMAGE_SEEPROM_NAME := sbe_seeprom_$(IMAGE_SUFFIX)
IMAGE_SBE_NAME := sbe_pibmem_$(IMAGE_SUFFIX)

IMAGE_LOADER_NAME := sbe_loader_$(IMAGE_SUFFIX)
IMAGE_OTPROM_NAME := sbe_otprom_$(IMAGE_SUFFIX)
IMAGE_BASE_PPE_HEADER := base_ppe_header

SBE_SYMBOLS_NAME := sbe_$(IMAGE_SUFFIX).syms
SBE_STRINGFILE_NAME := sbeStringFile_$(IMAGE_SUFFIX)

PROJECT_APP_DIR := power
PROJECT_APP_PIBMEM_LIB := sbeapppowerpibmem
PROJECT_APP_SEEPROM_LIB := sbeapppowerseeprom
