# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p10/utils/p10_plat_ring_traverse.mk $
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
INC=$(ROOTPATH)/chips/p10/utils/imageProcs/
INC+=$(ROOTPATH)/chips/p10/utils/
$(call ADD_MODULE_INCDIR,$(PROCEDURE),$(INC))
lib$(PROCEDURE)_DEPLIBS += p10_putring_sbe_utils
lib$(PROCEDURE)_DEPLIBS += p10_ringId
lib$(PROCEDURE)_DEPLIBS += p10_putRingUtils
PROCEDURE=p10_plat_ring_traverse
$(call BUILD_PROCEDURE)
