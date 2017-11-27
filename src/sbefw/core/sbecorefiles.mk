# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/sbefw/core/sbecorefiles.mk $
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
SBECORE-CPP-SOURCES = sbemain.C
SBECORE-CPP-SOURCES += sbeirq.C
SBECORE-CPP-SOURCES += sbecmdreceiver.C
SBECORE-CPP-SOURCES += sbecmdprocessor.C
SBECORE-CPP-SOURCES += sbecmdparser.C
SBECORE-CPP-SOURCES += sbecmdscomaccess.C
SBECORE-CPP-SOURCES += sbecmdiplcontrol.C
SBECORE-CPP-SOURCES += pool.C
SBECORE-CPP-SOURCES += sbecmdgeneric.C
SBECORE-CPP-SOURCES += sbeFifoMsgUtils.C
SBECORE-CPP-SOURCES += sbecmdmemaccess.C
SBECORE-CPP-SOURCES += sbeHostUtils.C
SBECORE-CPP-SOURCES += sbecmdcntrldmt.C
SBECORE-CPP-SOURCES += sbecmdsram.C
SBECORE-CPP-SOURCES += sberegaccess.C
SBECORE-CPP-SOURCES += sbecmdcntlinst.C
SBECORE-CPP-SOURCES += sbecmdregaccess.C
SBECORE-CPP-SOURCES += sbeFFDC.C
SBECORE-CPP-SOURCES += sbecmdringaccess.C
SBECORE-CPP-SOURCES += sbescom.C
SBECORE-CPP-SOURCES += sbecmdmpipl.C
SBECORE-CPP-SOURCES += sbefapiutil.C
SBECORE-CPP-SOURCES += sbeutil.C
SBECORE-CPP-SOURCES += sbecmdtracearray.C
SBECORE-CPP-SOURCES += sbeTimerSvc.C
SBECORE-CPP-SOURCES += sbecmdCntrlTimer.C
SBECORE-CPP-SOURCES += sbeHostMsg.C
SBECORE-CPP-SOURCES += sbeSpMsg.C
SBECORE-CPP-SOURCES += sbeglobals.C
SBECORE-CPP-SOURCES += sbeMemAccessInterface.C
SBECORE-CPP-SOURCES += sbeSecureMemRegionManager.C
SBECORE-CPP-SOURCES += sbeSecurity.C

SBECORE-C-SOURCES =
SBECORE-S-SOURCES =

SBECORE_OBJECTS = $(SBECORE-C-SOURCES:.c=.o) $(SBECORE-CPP-SOURCES:.C=.o) $(SBECORE-S-SOURCES:.S=.o)
