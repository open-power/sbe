# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/sbefw/core/corefiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2022
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
COREPIBMEM-CPP-SOURCES = sbemain.C
COREPIBMEM-CPP-SOURCES += sbeirq.C
COREPIBMEM-CPP-SOURCES += sbecmdreceiver.C
COREPIBMEM-CPP-SOURCES += sbecmdprocessor.C
COREPIBMEM-CPP-SOURCES += sbecmdparser.C
COREPIBMEM-CPP-SOURCES += pool.C
COREPIBMEM-CPP-SOURCES += sbeFifoMsgUtils.C
COREPIBMEM-CPP-SOURCES += sbeHostUtils.C
COREPIBMEM-CPP-SOURCES += sberegaccess.C
COREPIBMEM-CPP-SOURCES += sbeFFDC.C
COREPIBMEM-CPP-SOURCES += sbescom.C
COREPIBMEM-CPP-SOURCES += sbefapiutil.C
COREPIBMEM-CPP-SOURCES += sbeutil.C
COREPIBMEM-CPP-SOURCES += sbeXipUtils.C
COREPIBMEM-CPP-SOURCES += sbeTimerSvc.C
COREPIBMEM-CPP-SOURCES += sbeHostMsg.C
COREPIBMEM-CPP-SOURCES += sbeSpMsg.C
COREPIBMEM-CPP-SOURCES += sbeglobals.C
COREPIBMEM-CPP-SOURCES += sbeMemAccessInterface.C
COREPIBMEM-CPP-SOURCES += sbeSecureMemRegionManager.C
COREPIBMEM-CPP-SOURCES += sbeSecurity.C
COREPIBMEM-CPP-SOURCES += securityAlgo.C
COREPIBMEM-CPP-SOURCES += chipop_handler.C
COREPIBMEM-CPP-SOURCES += ipl.C
COREPIBMEM-CPP-SOURCES += sbes1handler.C
ifeq ($(SBE_S0_SUPPORT), 1)
COREPIBMEM-CPP-SOURCES += sbes0handler.C
endif

COREPIBMEM-C-SOURCES =
COREPIBMEM-S-SOURCES =

COREPIBMEM_OBJECTS = $(COREPIBMEM-C-SOURCES:.c=.o) $(COREPIBMEM-CPP-SOURCES:.C=.o) $(COREPIBMEM-S-SOURCES:.S=.o)

# seeprom objects
CORESEEPROM-CPP-SOURCES = sbeSecureMemRegionManager.C
ifeq ($(SBE_S0_SUPPORT), 1)
CORESEEPROM-CPP-SOURCES += sbes0handler.C
endif
ifeq ($(SBE_CONSOLE_SUPPORT), 1)
CORESEEPROM-CPP-SOURCES += sbeConsole.C
endif

CORESEEPROM-C-SOURCES =
CORESEEPROM-S-SOURCES =

CORESEEPROM_OBJECTS = $(CORESEEPROM-C-SOURCES:.c=.o) $(CORESEEPROM-CPP-SOURCES:.C=.o) $(CORESEEPROM-S-SOURCES:.S=.o)
