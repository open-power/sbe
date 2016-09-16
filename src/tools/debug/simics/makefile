# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/tools/debug/simics/makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016
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
#
# FSP Destination: obj/ppc/sbei/sbfw/simics/makefile
#
# NOTE: Do NOT modify this file in CMVC directly!  It comes from the SBE 
#       repository and will be overwritten.

.include <${RULES_MK}>

OTHERS = copy_scripts

SBE_SIMICS_PATH = ${MAKETOP}simu/scripts/sbfw/

SBE_SRC_PATHS = ${.PATH:M*src*sbei*sbfw*simics*} ${.PATH:M*obj*sbei*sbfw*simics*}
SBE_SRC_DIRS = ${SBE_SRC_PATHS:XD}
SBE_SRC_FILES = ${:!${SBE_SRC_DIRS:@path@ls ${path};@}!}

SBE_SCRIPTS_TO_COPY = ${SBE_SRC_FILES:Nmakefile}
SBE_SCRIPTS_PATHS = ${SBE_SCRIPTS_TO_COPY:p}

SBE_COPY_COMMAND = \
    ${SBE_SCRIPTS_PATHS:@file@cp -r ${file} ${SBE_SIMICS_PATH}; @}
SBE_REMOVE_COMMAND = \
    ${SBE_SCRIPTS_TO_COPY:@file@rm -rf ${SBE_SIMICS_PATH}${file}; @}

copy_scripts:
	mkdir -p ${SBE_SIMICS_PATH}
	${SBE_COPY_COMMAND}

clobber_copy_scripts:
	${SBE_REMOVE_COMMAND}
