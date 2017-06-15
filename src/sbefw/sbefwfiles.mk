# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/sbefw/sbefwfiles.mk $
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
SBEFW-CPP-SOURCES = sbemain.C
SBEFW-CPP-SOURCES += sbeirq.C
SBEFW-CPP-SOURCES += sbecmdreceiver.C
SBEFW-CPP-SOURCES += sbecmdprocessor.C
SBEFW-CPP-SOURCES += sbecmdparser.C
SBEFW-CPP-SOURCES += sbecmdscomaccess.C
SBEFW-CPP-SOURCES += sbecmdiplcontrol.C
SBEFW-CPP-SOURCES += pool.C
SBEFW-CPP-SOURCES += sbecmdgeneric.C
SBEFW-CPP-SOURCES += sbeFifoMsgUtils.C
SBEFW-CPP-SOURCES += sbecmdmemaccess.C
SBEFW-CPP-SOURCES += sbeHostUtils.C
SBEFW-CPP-SOURCES += sbecmdcntrldmt.C
SBEFW-CPP-SOURCES += sbecmdsram.C
SBEFW-CPP-SOURCES += sberegaccess.C
SBEFW-CPP-SOURCES += sbecmdcntlinst.C
SBEFW-CPP-SOURCES += sbecmdregaccess.C
SBEFW-CPP-SOURCES += sbeFFDC.C
SBEFW-CPP-SOURCES += sbecmdringaccess.C
SBEFW-CPP-SOURCES += sbescom.C
SBEFW-CPP-SOURCES += sbecmdmpipl.C
SBEFW-CPP-SOURCES += sbefapiutil.C
SBEFW-CPP-SOURCES += sbeutil.C
SBEFW-CPP-SOURCES += sbecmdtracearray.C
SBEFW-CPP-SOURCES += sbeTimerSvc.C
SBEFW-CPP-SOURCES += sbecmdCntrlTimer.C
SBEFW-CPP-SOURCES += sbecmdfastarray.C
SBEFW-CPP-SOURCES += sbeHostMsg.C
SBEFW-CPP-SOURCES += sbeSpMsg.C
SBEFW-CPP-SOURCES += sbeglobals.C
SBEFW-CPP-SOURCES += sbeMemAccessInterface.C
SBEFW-CPP-SOURCES += sbeSecureMemRegionManager.C
SBEFW-CPP-SOURCES += sbeSecurity.C

SBEFW-C-SOURCES =
SBEFW-S-SOURCES =

SBEFW_OBJECTS = $(SBEFW-C-SOURCES:.c=.o) $(SBEFW-CPP-SOURCES:.C=.o) $(SBEFW-S-SOURCES:.S=.o)
