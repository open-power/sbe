# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/sbefw/app/z/appzfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2018
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

APPPOWERPIBMEM-CPP-SOURCES  = chipop_table.C
APPPOWERPIBMEM-CPP-SOURCES += ipl_table.C
APPPOWERPIBMEM-CPP-SOURCES += sbecmdiplcontrol.C
APPPOWERPIBMEM-CPP-SOURCES += sbecmdcntrldmt.C

APPPOWERPIBMEM-C-SOURCES =
APPPOWERPIBMEM-S-SOURCES =

APPPOWERPIBMEM_OBJECTS = $(APPPOWERPIBMEM-C-SOURCES:.c=.o) $(APPPOWERPIBMEM-CPP-SOURCES:.C=.o) $(APPPOWERPIBMEM-S-SOURCES:.S=.o)

APPPOWERSEEPROM-CPP-SOURCES =  ipl_table.C
APPPOWERSEEPROM-CPP-SOURCES += sbecmdiplcontrol.C

APPPOWERSEEPROM-C-SOURCES =
APPPOWERSEEPROM-S-SOURCES =

APPPOWERSEEPROM_OBJECTS = $(APPPOWERSEEPROM-C-SOURCES:.c=.o) $(APPPOWERSEEPROM-CPP-SOURCES:.C=.o) $(APPPOWERSEEPROM-S-SOURCES:.S=.o)
