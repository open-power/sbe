# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/import_hwp_mk/power/arrayaccess/arrayaccessfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2021
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
#  @file arrayaccess.mk
#
#  @brief mk for including fastarray and tracearray object files
#
##########################################################################
# Object Files
##########################################################################

ARRAYACCESS-CPP-SOURCES = p10_sbe_tracearray.C
ARRAYACCESS-CPP-SOURCES += p10_sbe_fastarray.C
ARRAYACCESS-C-SOURCES =
ARRAYACCESS-S-SOURCES =

ARRAYACCESS_OBJECTS += $(ARRAYACCESS-CPP-SOURCES:.C=.o)
ARRAYACCESS_OBJECTS += $(ARRAYACCESS-C-SOURCES:.c=.o)
ARRAYACCESS_OBJECTS += $(ARRAYACCESS-S-SOURCES:.S=.o)
