# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/import_hwp_mk/power/verif_hwp/verifhwpfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2021
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
#  @file measure_spifiles.mk
#
#  @brief mk for including measurement spi object files
#
##########################################################################
# Object Files
##########################################################################
VERIFHWP-CPP-SOURCES = p10_sbe_spi_cmd.C
VERIFHWP-C-SOURCES =
VERIFHWP-S-SOURCES =

VERIFHWP_OBJECTS += $(VERIFHWP-CPP-SOURCES:.C=.o)
VERIFHWP_OBJECTS += $(VERIFHWP-C-SOURCES:.c=.o)
VERIFHWP_OBJECTS += $(VERIFHWP-S-SOURCES:.S=.o)
