# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/build/import_hwp_mk/power/istepcontained/istepcontainedfiles.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2019,2020
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
##########################################################################
# Object Files
##########################################################################
ISTEPCONTAINED-CPP-SOURCES = p10_contained.H
ISTEPCONTAINED-CPP-SOURCES += p10_contained.C
ISTEPCONTAINED-CPP-SOURCES += p10_contained_load.H
ISTEPCONTAINED-CPP-SOURCES += p10_contained_load.C
ISTEPCONTAINED-CPP-SOURCES += p10_contained_ipl.H
ISTEPCONTAINED-CPP-SOURCES += p10_contained_ipl.C
ISTEPCONTAINED-CPP-SOURCES += p10_contained_runn.H
ISTEPCONTAINED-CPP-SOURCES += p10_contained_runn.C
ISTEPCONTAINED-CPP-SOURCES += p10_contained_run.H
ISTEPCONTAINED-CPP-SOURCES += p10_contained_run.C
ISTEPCONTAINED-CPP-SOURCES += p10_contained_dft.H
ISTEPCONTAINED-CPP-SOURCES += p10_contained_dft.C

#ISTEPCONTAINEDDFT-CPP-SOURCES += p10_contained_dft.H
#ISTEPCONTAINEDDFT-CPP-SOURCES += p10_contained_dft.C

#ISTEPCONTAINED-CPP-SOURCES += p10_contained_sim.C
ISTEPCONTAINED-C-SOURCES =
ISTEPCONTAINED-S-SOURCES =

ISTEPCONTAINED_OBJECTS += $(ISTEPCONTAINED-CPP-SOURCES:.C=.o)
ISTEPCONTAINED_OBJECTS += $(ISTEPCONTAINED-C-SOURCES:.c=.o)
ISTEPCONTAINED_OBJECTS += $(ISTEPCONTAINED-S-SOURCES:.S=.o)
#ISTEPCONTAINEDDFT_OBJECTS += $(ISTEPCONTAINEDDFT-CPP-SOURCES:.C=.o)
#ISTEPCONTAINEDDFT_OBJECTS += $(ISTEPCONTAINEDDFT-C-SOURCES:.c=.o)
#ISTEPCONTAINEDDFT_OBJECTS += $(ISTEPCONTAINEDDFT-S-SOURCES:.S=.o)
