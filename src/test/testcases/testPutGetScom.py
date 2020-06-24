# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPutGetScom.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2020
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
from __future__ import print_function
import os
import sys
import struct
import testScomUtil

print("********************FIFO:I******************************\n")
i_fifoType = 0
# getscom success
testScomUtil.getscom(0x02040064,i_fifoType)
# putscom success
testScomUtil.putscom(0x000F001A, testScomUtil.getscom(0x000F001A,i_fifoType), i_fifoType)

print("\nFIFO:I Test completed Successfully\n")

print("\n********************FIFO:II******************************\n")
i_fifoType = 1
# getscom success
testScomUtil.getscom(0x02040064, i_fifoType)
# putscom success
testScomUtil.putscom(0x000F001A, testScomUtil.getscom(0x000F001A, i_fifoType), i_fifoType)

print("\nFIFO:II Test completed Successfully\n")

