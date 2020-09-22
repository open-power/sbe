# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPutGetHWReg.py $
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
import testHwRegUtil

print("********************FIFO:I PROC SCOM******************************\n")
i_fifoType = 0
# getHWReg success
testHwRegUtil.getHWReg([0, 0, 0, 0], 0x02040064,i_fifoType)
# putHWReg success
testHwRegUtil.putHWReg([0, 0, 0, 0], 0x000F001A, testHwRegUtil.getHWReg([0, 0, 0, 0], 0x000F001A,i_fifoType), i_fifoType)

print("\nFIFO:I Test completed Successfully\n")

print("\n********************FIFO:II PROC SCOM******************************\n")
i_fifoType = 1
# getHWReg success
testHwRegUtil.getHWReg([0, 0, 0, 0], 0x02040064, i_fifoType)
# putHWReg success
testHwRegUtil.putHWReg([0, 0, 0, 0], 0x000F001A, testHwRegUtil.getHWReg([0, 0, 0, 0], 0x000F001A, i_fifoType), i_fifoType)

print("\nFIFO:II Test completed Successfully\n")

print("********************FIFO:I OCMB SCOM******************************\n")

print("Enable OCMBs\n")

simics.SIM_run_command("set-class-attr i2c_ocmb_slave cmds_EXP_FW_STATUS 0x00030000")

i_fifoType = 0
# getHWReg success
testHwRegUtil.getHWReg([0, 0x4, 0, 1], 0x00002134,i_fifoType)
# putHWReg success
testHwRegUtil.putHWReg([0, 0x4, 0, 1], 0x00002134, testHwRegUtil.getHWReg([0, 0x4, 0, 1], 0x00002134,i_fifoType), i_fifoType)

print("\nFIFO:I Test completed Successfully\n")

print("\n********************FIFO:II OCMB SCOM******************************\n")
i_fifoType = 1
# getHWReg success
testHwRegUtil.getHWReg([0, 0x4, 0, 1], 0x00002134, i_fifoType)
# putHWReg success
testHwRegUtil.putHWReg([0, 0x4, 0, 1], 0x00002134, testHwRegUtil.getHWReg([0, 0x4, 0, 1], 0x00002134, i_fifoType), i_fifoType)

print("\nFIFO:II Test completed Successfully\n")

print("********************FIFO:I INVALID TARGET******************************\n")
i_fifoType = 0
# getHWReg success
testHwRegUtil.getHWReg([0, 0xF, 0, 1], 0x02040064,i_fifoType, [0, 0x02, 0, 0x04])
# putHWReg success
testHwRegUtil.putHWReg([0, 0xF, 0, 1], 0x000F001A,i_fifoType, 0x0, [0, 0x02, 0, 0x04])

print("********************FIFO:I INVALID SCOM******************************\n")
i_fifoType = 0
# getHWReg success
testHwRegUtil.getHWReg([0, 0x0, 0, 1], 0x82040064,i_fifoType, [0, 0x03, 0, 0x03])
# putHWReg success
testHwRegUtil.putHWReg([0, 0x0, 0, 1], 0x800F001A,i_fifoType, 0x0, [0, 0x03, 0, 0x03])
