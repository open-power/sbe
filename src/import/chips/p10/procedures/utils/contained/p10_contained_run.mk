# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_run.mk $
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
PROCEDURE=p10_contained_run

$(call ADD_MODULE_INCDIR,$(PROCEDURE),$(ROOTPATH)/chips/p10/procedures/hwp/perv)

$(call ADD_MODULE_OBJ,$(PROCEDURE),p10_contained.o)
$(call ADD_MODULE_OBJ,$(PROCEDURE),p10_contained_runn.o)

lib$(PROCEDURE)_COMMONFLAGS += -DP10_CONTAINED_ENABLE_SEEDING

ifeq ($(P10_CONTAINED_SIM),1)
	OBJS += p10_contained_sim.o
	lib$(PROCEDURE)_COMMONFLAGS += -DP10_CONTAINED_SIM
endif

$(call BUILD_PROCEDURE)
