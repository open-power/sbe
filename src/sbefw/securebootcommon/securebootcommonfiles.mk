# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/sbefw/securebootcommon/securebootcommonfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2020,2021
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
COMMON-CPP-SOURCES = sbeTPMaccess.C
COMMON-CPP-SOURCES += sbeRoleIdentifier.C
COMMON-CPP-SOURCES += sbeTPMCommand.C

COMMON-C-SOURCES =
COMMON-S-SOURCES =

SECUREBOOT_COMMON_OBJECTS = $(COMMON-C-SOURCES:.c=.o) $(COMMON-CPP-SOURCES:.C=.o) $(COMMON-S-SOURCES:.S=.o)
