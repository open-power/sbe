# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/sbefw/sbefwseepromfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2017
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
SBEFWSEEPROM-CPP-SOURCES = sbecmdgeneric.C
SBEFWSEEPROM-CPP-SOURCES += sbecmdmpipl.C
SBEFWSEEPROM-CPP-SOURCES += sbecmdmemaccess.C
SBEFWSEEPROM-CPP-SOURCES += sbeSecureMemRegionManager.C
SBEFWSEEPROM-CPP-SOURCES += sbecmdiplcontrol.C

SBEFWSEEPROM-C-SOURCES =
SBEFWSEEPROM-S-SOURCES =

SBEFWSEEPROM_OBJECTS = $(SBEFWSEEPROM-C-SOURCES:.c=.o) $(SBEFWSEEPROM-CPP-SOURCES:.C=.o) $(SBEFWSEEPROM-S-SOURCES:.S=.o)
