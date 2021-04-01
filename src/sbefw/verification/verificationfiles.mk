# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/sbefw/verification/verificationfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2019,2021
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

VERIFICATION-CPP-SOURCES =  verification_main.C
VERIFICATION-CPP-SOURCES += test_lib.C
VERIFICATION-CPP-SOURCES += ecverify.C
VERIFICATION-CPP-SOURCES += sbesecureboot.C
VERIFICATION-CPP-SOURCES += sbevutil.C
VERIFICATION-CPP-SOURCES += sbevthreadroutine.C
VERIFICATION-CPP-SOURCES += sbevPcrStates.C
VERIFICATION-CPP-SOURCES += sbevsecuritysetting.C
VERIFICATION-CPP-SOURCES += sbeutil.C

VERIFICATION-C-SOURCES =
VERIFICATION-S-SOURCES =

VERIFICATION_OBJECTS = $(VERIFICATION-C-SOURCES:.c=.o) $(VERIFICATION-CPP-SOURCES:.C=.o) $(VERIFICATION-S-SOURCES:.S=.o)

