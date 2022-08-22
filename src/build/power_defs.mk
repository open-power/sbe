# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/power_defs.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017,2022
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
# Levels of SBE logging
# 0 - No tracing
# 1 - Error
# 2 - Error, info
# 3 - Error, info, entry/exit
# 4 - Error, info, entry/exit, debug
SBE_TRACE_LEVEL_DEF = 2
SBEM_TRACE_LEVEL_DEF = 2
SBEV_TRACE_LEVEL_DEF = 2

FAPI_TRACE_LEVEL_DEF = 2

HOST_INTERFACE_AVAILABLE = 1
PERIODIC_IO_TOGGLE_SUPPORTED = 1

SBE_CONSOLE_SUPPORT = 1

#If MEASUREMENT_IMAGE_SUPPORTED is 1 then only sbe_measurement_seeprom image will creates
#This is a terminology used in this makefile that only means the compilation of measurement
#seeprom is blocked. But the over-all support of measurement seeprom is still there from the
#locked/golden measurement image already present in the PPE repository
export MEASUREMENT_IMAGE_SUPPORTED = 0

#If VERIFICATION_IMAGE_SUPPORTED is 1 then only sbe_verification image will create
#This is a terminology used in this makefile that only means the compilation of verification
#image is blocked. But the over-all support of verification image is still there from the
#locked/golden verification image already present in the PPE repository
export VERIFICATION_IMAGE_SUPPORTED = 1

AWAN_NMZ_MODEL = 1

export SBE_S0_SUPPORT = 1
export PIBMEM_REPAIR_SCOM_P9 = 1

ISTEP2_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/istep2
ISTEP3_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/istep3
ISTEP4_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/istep4
ISTEP5_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/istep5
ISTEP14_16_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/istep14_16
ISTEPMPIPL_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/istepmpipl
ISTEPCOMMON_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/istepcommon
ARRAYACCESS_INFRA_DIR = $(IMPORT_HWP_MK_DIR)/arrayaccess
SBEM_HWP_SRCDIR = $(IMPORT_HWP_MK_DIR)/sbem_hwp
SBEV_HWP_SRCDIR = $(IMPORT_HWP_MK_DIR)/sbev_hwp

OBJDIR-ISTEP2 = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/istep2
OBJDIR-ISTEP3 = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/istep3
OBJDIR-ISTEP4 = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/istep4
OBJDIR-ISTEP5 = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/istep5
OBJDIR-ISTEP14_16 = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/istep14_16
OBJDIR-ISTEPMPIPL = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/istepmpipl
OBJDIR-ISTEPCOMMON = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/istepcommon
OBJDIR-ARRAYACCESS = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/arrayaccess

OBJDIR-SBEM-HWP = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/sbem_hwp
OBJDIR-MEASUREMENT = $(BASE_OBJDIR)/sbefw/measurement
OBJDIR-SECUREBOOTCOMMON = $(BASE_OBJDIR)/sbefw/securebootcommon
OBJDIR-VERIFICATION = $(BASE_OBJDIR)/sbefw/verification
OBJDIR-SBEV-HWP = $(BASE_OBJDIR)/$(IMPORT_OBJDIR)/sbev_hwp

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

PROJ_SUBDIRS += $(ISTEP14_16_INFRA_DIR)
PROJ_LIB_DIRS += -L$(OBJDIR-ISTEP14_16)
PROJ_LLIBS += -listep14_16

PROJ_SUBDIRS += $(ISTEPMPIPL_INFRA_DIR)
PROJ_LIB_DIRS += -L$(OBJDIR-ISTEPMPIPL)
PROJ_LLIBS += -listepmpipl

PROJ_SUBDIRS += $(ISTEPCOMMON_INFRA_DIR)
PROJ_LIB_DIRS += -L$(OBJDIR-ISTEPCOMMON)
PROJ_LLIBS += -listepcommon

PROJ_SUBDIRS += $(ARRAYACCESS_INFRA_DIR)
PROJ_LIB_DIRS += -L$(OBJDIR-ARRAYACCESS)
PROJ_LLIBS += -larrayaccess

PROJ_SUBDIRS += $(SECUREBOOT_COMMON_SRCDIR)
PROJ_LIB_DIRS += -L$(OBJDIR-SECUREBOOTCOMMON)
PROJ_LLIBS += -lsecurebootcommon

MEASUREMENT_PROJ_SUBDIRS := $(SBEM_HWP_SRCDIR)
MEASUREMENT_PROJ_LIB_DIRS += -L$(OBJDIR-SBEM-HWP)
MEASUREMENT_PROJ_LLIBS += -lsbem_hwp

MEASUREMENT_PROJ_SUBDIRS += $(SECUREBOOT_COMMON_SRCDIR)
MEASUREMENT_PROJ_LIB_DIRS += -L$(OBJDIR-SECUREBOOTCOMMON)
MEASUREMENT_PROJ_LLIBS += -lsecurebootcommon

MEASUREMENT_PROJ_LIB_DIRS += -L$(OBJDIR)/fapi2_measurement
MEASUREMENT_PROJ_LLIBS += -lfapi2measurement

MEASUREMENT_PROJ_SUBDIRS += $(MEASUREMENT_SRCDIR)
MEASUREMENT_PROJ_LIB_DIRS += -L$(OBJDIR-MEASUREMENT)
MEASUREMENT_PROJ_LLIBS += -ltestMeasure

VERIFICATION_PROJ_SUBDIRS := $(SBEV_HWP_SRCDIR)
VERIFICATION_PROJ_LIB_DIRS += -L$(OBJDIR-SBEV-HWP)
VERIFICATION_PROJ_LLIBS += -lsbev_hwp

VERIFICATION_PROJ_SUBDIRS += $(SECUREBOOT_COMMON_SRCDIR)
VERIFICATION_PROJ_LIB_DIRS += -L$(OBJDIR-SECUREBOOTCOMMON)
VERIFICATION_PROJ_LLIBS += -lsecurebootcommon

VERIFICATION_PROJ_SUBDIRS += $(VERIFICATION_SRCDIR)
VERIFICATION_PROJ_LIB_DIRS += -L$(OBJDIR-VERIFICATION)
VERIFICATION_PROJ_LLIBS += -lVerification

#########################################################
# mandatory defines                                     #
#########################################################
IMAGE_SUFFIX := DD1
GCC-DEFS += -DDD1
IMAGE_SEEPROM_NAME := sbe_seeprom_$(IMAGE_SUFFIX)
IMAGE_SBE_NAME := sbe_pibmem_$(IMAGE_SUFFIX)

IMAGE_LOADER_NAME := sbe_loader_$(IMAGE_SUFFIX)
IMAGE_OTPROM_NAME := sbe_otprom_$(IMAGE_SUFFIX)
IMAGE_BASE_PPE_HEADER := base_ppe_header
IMAGE_BASE_MEASUREMENT_HEADER := base_measurement_header
IMAGE_BASE_VERIFICATION_HEADER := base_verification_header

IMAGE_MEASUREMENT_NAME := sbe_measurement_seeprom
SBE_MEASUREMENT_STRINGFILE_NAME := sbeMeasurementStringFile

IMAGE_VERIFICATION_NAME := sbe_verification
SBE_VERIFICATION_STRINGFILE_NAME := sbeVerificationStringFile

SBE_SYMBOLS_NAME := sbe_$(IMAGE_SUFFIX).syms
MEASUREMENT_SYMBOLS_NAME := sbe_measurement_seeprom.syms
VERIFICATION_SYMBOLS_NAME := sbe_verification.syms
SBE_STRINGFILE_NAME := sbeStringFile_$(IMAGE_SUFFIX)

PROJECT_APP_DIR := power
PROJECT_APP_PIBMEM_LIB := sbeapppowerpibmem
PROJECT_APP_SEEPROM_LIB := sbeapppowerseeprom

INCLUDES += -I$(BUILDDATA_SRCDIR)/power
