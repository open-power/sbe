# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_ringspin.gen.mk $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2020
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
ENGD_ROOTDIR=$(ROOTPATH)/chips/p10/engd/p10
CONTAINED_ROOTDIR=$(ROOTPATH)/chips/p10/procedures/utils/contained
RINGSPIN_YAML_CFG=$(CONTAINED_ROOTDIR)/p10_contained_ringspin.gen.yaml

GENERATED=p10_contained_ringspin.gen
$(GENERATED)_PATH=$(CONTAINED_ROOTDIR)
TARGETS+=$(GENERATED).H

# Only generate the ringspinner procedures if we have engd
ifneq ($(wildcard $(ENGD_ROOTDIR)/*),)

COMMAND=ringspinner
SOURCES+=$(wildcard $(CONTAINED_ROOTDIR)/tools/src/*.py)
SOURCES+=$(RINGSPIN_YAML_CFG)

$(GENERATED)_COMMAND_PATH=$(CONTAINED_ROOTDIR)/tools/bin/
$(GENERATED)_RUN=$(call RUN_RINGSPINNER,$(RINGSPIN_YAML_CFG),$($(GENERATED)_PATH)/$(GENERATED))

# The generated prolog has an incorrect path set which results in `git status`
# differences which break CI EKB builds. Fix this by using `sed` to edit the
# path in-place after generating the prolog - gross.
define RUN_RINGSPINNER
$(C1) $$< $(1) -o $(2) && addCopyright update $(2).H > /dev/null 2>&1 && sed -i 's%\$$Source: ../../%\$$Source: %' $(2).H
endef

else

COMMAND=git

$(GENERATED)_COMMAND_PATH=$(dir $(shell which git))
$(GENERATED)_RUN=$(call RUN_RINGSPINNER,$($(GENERATED)_PATH)/$(GENERATED).H)

define RUN_RINGSPINNER
$(C1) $$< checkout -- $(1)
endef

endif

$(call BUILD_GENERATED)
