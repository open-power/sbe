# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testHwRegUtil.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017,2020
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
import sys
import os
import struct
sys.path.append("targets/p10_standalone/sbeTest" )
import testUtil
err = False

def getsingleword(dataInInt):
    hex_string = '0'*(8-len(str(hex(dataInInt))[2:])) + str(hex(dataInInt))[2:]
    return list(struct.unpack('<BBBB',hex_string.decode('hex')))
def getdoubleword(dataInInt):
    hex_string = '0'*(16-len(str(hex(dataInInt))[:18][2:])) + str(hex(dataInInt))[:18][2:]
    return list(struct.unpack('<BBBBBBBB',hex_string.decode('hex')))

def getHWReg(targetInfo, addr, i_fifoType, expStatus = [0, 0, 0, 0], HWPffdc = False):
    req = ([0, 0, 0, 5,
            0,0,0xA5,0x03]
          + targetInfo 
          + getdoubleword(addr))

    print req

    testUtil.writeUsFifo(req, i_fifoType)
    testUtil.writeEot(i_fifoType)

    expData = ([0xc0,0xde,0xa5,0x03]
              + expStatus)
    success = False
    if(expStatus == [0, 0, 0, 0]):
        success = True

    data = [0]*8
    if(success):
        data = testUtil.readDsEntryReturnVal(i_fifoType)
        data += testUtil.readDsEntryReturnVal(i_fifoType)

    print data

    testUtil.readDsFifo(expData, i_fifoType)
    if(not success and HWPffdc):
        testUtil.extractHWPFFDC(i_fifoType)
    #flush out distance
    testUtil.readDsEntryReturnVal(i_fifoType)
    testUtil.readEot(i_fifoType)

    val = 0
    for i in range(0, 8):
        val |= data[i] << ((7-i)*8)
    return val

def putHWReg(targetInfo, addr, data, i_fifoType, expStatus = [0, 0, 0, 0]):
    req = ([0,0,0,7,
            0,0,0xA5,0x04]
          +  targetInfo
          + getdoubleword(addr)
          + getdoubleword(data))
    print req
    testUtil.writeUsFifo(req, i_fifoType)
    testUtil.writeEot(i_fifoType)

    expData = ([0xc0,0xde,0xa5,0x04]
               + expStatus)

    success = False
    if(expStatus == [0, 0, 0, 0]):
        success = True
    testUtil.readDsFifo(expData, i_fifoType)
    #flush out distance
    testUtil.readDsEntryReturnVal(i_fifoType)
    testUtil.readEot(i_fifoType)
