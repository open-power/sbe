# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: Makefile $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2017
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
BUILD_DIR = src/build
.PHONY: install all clean tar

del_objects:
	@rm -rf obj/sbefw
	@rm -rf obj/import_hwp_mk
	@rm -rf obj/boot
	@rm -rf obj/build/utils
	@rm -rf obj/fapi2

install: all tar

tar:
	$(MAKE) -C $(BUILD_DIR) tar

DD1_build:
	$(MAKE) -C $(BUILD_DIR) all ddlevel=DD1

DD2_build:
	$(MAKE) -C $(BUILD_DIR) all ddlevel=DD2

all: DD1_build del_objects DD2_build

clean:
	$(MAKE) -C $(BUILD_DIR) clean
