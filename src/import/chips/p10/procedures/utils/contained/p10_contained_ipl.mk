# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_ipl.mk $
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
PROCEDURE=p10_contained_ipl

lib$(PROCEDURE)_EXTRALIBS+=p10_sbe_chiplet_reset
lib$(PROCEDURE)_EXTRALIBS+=p10_sbe_gptr_time_initf
lib$(PROCEDURE)_EXTRALIBS+=p10_sbe_repr_initf
lib$(PROCEDURE)_EXTRALIBS+=p10_sbe_arrayinit
lib$(PROCEDURE)_EXTRALIBS+=p10_sbe_initf
lib$(PROCEDURE)_EXTRALIBS+=p10_sbe_startclocks

lib$(PROCEDURE)_EXTRALIBS+=p10_hcd_cache_reset
lib$(PROCEDURE)_EXTRALIBS+=p10_hcd_cache_gptr_time_initf
lib$(PROCEDURE)_EXTRALIBS+=p10_hcd_cache_repair_initf
lib$(PROCEDURE)_EXTRALIBS+=p10_hcd_cache_arrayinit
lib$(PROCEDURE)_EXTRALIBS+=p10_hcd_cache_initf
lib$(PROCEDURE)_EXTRALIBS+=p10_hcd_cache_startclocks
lib$(PROCEDURE)_EXTRALIBS+=p10_hcd_core_reset
lib$(PROCEDURE)_EXTRALIBS+=p10_hcd_core_gptr_time_initf
lib$(PROCEDURE)_EXTRALIBS+=p10_hcd_core_repair_initf
lib$(PROCEDURE)_EXTRALIBS+=p10_hcd_core_arrayinit
lib$(PROCEDURE)_EXTRALIBS+=p10_hcd_core_initf
lib$(PROCEDURE)_EXTRALIBS+=p10_hcd_core_startclocks

$(call ADD_MODULE_INCDIR,$(PROCEDURE),$(ROOTPATH)/chips/p10/procedures/hwp/corecache)
$(call ADD_MODULE_SRCDIR,$(PROCEDURE),$(ROOTPATH)/chips/p10/procedures/hwp/perv)
$(call ADD_MODULE_INCDIR,$(PROCEDURE),$(ROOTPATH)/chips/p10/procedures/hwp/customize)
$(call ADD_MODULE_SRCDIR,$(PROCEDURE),$(ROOTPATH)/chips/p10/procedures/hwp/customize)
$(call ADD_MODULE_INCDIR,$(PROCEDURE),$(ROOTPATH)/chips/p10/utils/imageProcs)

# Place the P10 include dir first and *then* the P9 include dir
$(call ADD_MODULE_INCDIR,$(PROCEDURE),$(ROOTPATH)/chips/p10/common/include/)
$(call ADD_MODULE_INCDIR,$(PROCEDURE),$(ROOTPATH)/chips/p9/common/include/)

$(call ADD_MODULE_OBJ,$(PROCEDURE),p10_contained.o)
$(call ADD_MODULE_OBJ,$(PROCEDURE),p10_perv_sbe_cmn.o)
$(call ADD_MODULE_OBJ,$(PROCEDURE),p10_dyninit_bitvec_utils.o)

ifeq ($(P10_CONTAINED_SIM),1)
	OBJS += p10_contained_sim.o
	lib$(PROCEDURE)_COMMONFLAGS += -DP10_CONTAINED_SIM
endif

$(call BUILD_PROCEDURE)
