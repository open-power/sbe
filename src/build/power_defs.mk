# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/power_defs.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017,2018
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

IMAGE_SUFFIX := DD2
GCC-DEFS += -DDD2
IMAGE_SEEPROM_NAME := sbe_seeprom_$(IMAGE_SUFFIX)
IMAGE_SBE_NAME := sbe_pibmem_$(IMAGE_SUFFIX)

IMAGE_LOADER_NAME := sbe_loader
IMAGE_OTPROM_NAME := sbe_otprom_$(IMAGE_SUFFIX)
IMAGE_BASE_PPE_HEADER := base_ppe_header

SBE_SYMBOLS_NAME := sbe_$(IMAGE_SUFFIX).syms
SBE_STRINGFILE_NAME := sbeStringFile_$(IMAGE_SUFFIX)
