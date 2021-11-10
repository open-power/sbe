# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/sbefw/app/common/appcommonfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2021
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
APPCOMMONPIBMEM-CPP-SOURCES  = sbecmdscomaccess.C
APPCOMMONPIBMEM-CPP-SOURCES += sbecmdringaccess.C
APPCOMMONPIBMEM-CPP-SOURCES += sbecmdCntrlTimer.C
APPCOMMONPIBMEM-CPP-SOURCES += sbecmdgeneric.C
APPCOMMONPIBMEM-CPP-SOURCES += sbecmddump.C
APPCOMMONPIBMEM-CPP-SOURCES += sbeCollectDump.C
APPCOMMONPIBMEM-CPP-SOURCES += sbeDumpConstants.C
APPCOMMONPIBMEM-CPP-SOURCES += sbecmdpmictelemetry.C
APPCOMMONPIBMEM-CPP-SOURCES += xmlErrorInfo.C

APPCOMMONPIBMEM-C-SOURCES =
APPCOMMONPIBMEM-S-SOURCES =

APPCOMMONPIBMEM_OBJECTS = $(APPCOMMONPIBMEM-C-SOURCES:.c=.o) $(APPCOMMONPIBMEM-CPP-SOURCES:.C=.o) $(APPCOMMONPIBMEM-S-SOURCES:.S=.o)

APPCOMMONSEEPROM-CPP-SOURCES = sbecmdgeneric.C

APPCOMMONSEEPROM-C-SOURCES =
APPCOMMONSEEPROM-S-SOURCES =

APPCOMMONSEEPROM_OBJECTS = $(APPCOMMONSEEPROM-C-SOURCES:.c=.o) $(APPCOMMONSEEPROM-CPP-SOURCES:.C=.o) $(APPCOMMONSEEPROM-S-SOURCES:.S=.o)
