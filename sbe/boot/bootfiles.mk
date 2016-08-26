# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: sbe/boot/bootfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016
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
BOOTTOP-S-SOURCES = loader_l1.S

BOOT_OBJECTS += $(BOOTTOP-S-SOURCES:.S=.o)

BASE-LOADER-C-SOURCES = loader_l2.c
BASE-LOADER-S-SOURCES = loader_l2_setup.S

BASE_LOADER_OBJECTS = $(BASE-LOADER-C-SOURCES:.c=.o)  $(BASE-LOADER-S-SOURCES:.S=.o)

OTPROM-LOADER-S-SOURCES = otprom_init.S
OTPROM_LOADER_OBJECTS = $(OTPROM-LOADER-S-SOURCES:.S=.o)
