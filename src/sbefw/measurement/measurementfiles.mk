# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/sbefw/measurement/measurementfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2019,2020
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

MEASUREMENT-CPP-SOURCES =  measurement_main.C
MEASUREMENT-CPP-SOURCES += p10_sbe_spi_cmd_test.C
MEASUREMENT-CPP-SOURCES += sbeutil.C 
MEASUREMENT-CPP-SOURCES += test_lib.C

MEASUREMENT-C-SOURCES =
MEASUREMENT-S-SOURCES =

MEASUREMENT_OBJECTS = $(MEASUREMENT-C-SOURCES:.c=.o) $(MEASUREMENT-CPP-SOURCES:.C=.o) $(MEASUREMENT-S-SOURCES:.S=.o)
