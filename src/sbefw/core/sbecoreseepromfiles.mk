# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/sbefw/core/sbecoreseepromfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2017
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
SBECORESEEPROM-CPP-SOURCES = sbecmdgeneric.C
SBECORESEEPROM-CPP-SOURCES += sbecmdmpipl.C
SBECORESEEPROM-CPP-SOURCES += sbecmdmemaccess.C
SBECORESEEPROM-CPP-SOURCES += sbeSecureMemRegionManager.C
SBECORESEEPROM-CPP-SOURCES += sbecmdiplcontrol.C
SBECORESEEPROM-CPP-SOURCES += sbecmdfastarray.C

SBECORESEEPROM-C-SOURCES =
SBECORESEEPROM-S-SOURCES =

SBECORESEEPROM_OBJECTS = $(SBECORESEEPROM-C-SOURCES:.c=.o) $(SBECORESEEPROM-CPP-SOURCES:.C=.o) $(SBECORESEEPROM-S-SOURCES:.S=.o)
