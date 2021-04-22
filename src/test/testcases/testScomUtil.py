# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testScomUtil.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017,2021
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
import codecs
import struct
sys.path.append("targets/p10_standalone/sbeTest" )
import testUtil
err = False

def getsingleword(dataInInt):
    hex_string = '0'*(8-len(str(hex(dataInInt))[2:])) + str(hex(dataInInt))[2:]
    return list(struct.unpack('<BBBB',codecs.decode(hex_string,'hex')))
def getdoubleword(dataInInt):
    hex_string = '0'*(16-len(str(hex(dataInInt))[:18][2:])) + str(hex(dataInInt))[:18][2:]
    return list(struct.unpack('<BBBBBBBB',codecs.decode(hex_string,'hex')))

def getscom(addr, i_fifoType, expStatus = [0, 0, 0, 0], HWPffdc = False):
    req = ([0, 0, 0, 4]
          + [0,0,0xA2,0x01]
          + getdoubleword(addr))

    testUtil.writeUsFifo(req, i_fifoType)
    testUtil.writeEot(i_fifoType)

    expData = ([0xc0,0xde,0xa2,0x01]
              + expStatus)
    success = False
    if(expStatus == [0, 0, 0, 0]):
        success = True

    data = [0]*8
    if(success):
        data = testUtil.readDsEntryReturnVal(i_fifoType)
        data += testUtil.readDsEntryReturnVal(i_fifoType)
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

def putscom(addr, data, i_fifoType, expStatus = [0, 0, 0, 0]):
    req = ([0,0,0,6,
            0,0,0xA2,0x02]
          + getdoubleword(addr)
          + getdoubleword(data))
    testUtil.writeUsFifo(req, i_fifoType)
    testUtil.writeEot(i_fifoType)

    expData = ([0xc0,0xde,0xa2,0x02]
               + expStatus)

    success = False
    if(expStatus == [0, 0, 0, 0]):
        success = True
    testUtil.readDsFifo(expData, i_fifoType)
    #flush out distance
    testUtil.readDsEntryReturnVal(i_fifoType)
    testUtil.readEot(i_fifoType)

def putScomUnderMask(addr, data, mask, i_fifoType, expStatus = [0, 0, 0, 0]):
    req = ([0,0,0,8,
            0,0,0xA2,0x04]
          + getdoubleword(addr)
          + getdoubleword(data)
          + getdoubleword(mask))
    testUtil.writeUsFifo(req, i_fifoType)
    testUtil.writeEot(i_fifoType)

    expData = ([0xc0,0xde,0xa2,0x04]
               + expStatus)

    success = False
    if(expStatus == [0, 0, 0, 0]):
        success = True
    testUtil.readDsFifo(expData, i_fifoType)
    #flush out distance
    testUtil.readDsEntryReturnVal(i_fifoType)
    testUtil.readEot(i_fifoType)

def modifyScom(operation, addr, data, i_fifoType, expStatus = [0, 0, 0, 0]):
    req = ([0,0,0,7,
            0,0,0xA2,0x03]
          + getsingleword(operation)
          + getdoubleword(addr)
          + getdoubleword(data))
    testUtil.writeUsFifo(req, i_fifoType)
    testUtil.writeEot(i_fifoType)

    expData = ([0xc0,0xde,0xa2,0x03]
               + expStatus)

    success = False
    if(expStatus == [0, 0, 0, 0]):
        success = True
    testUtil.readDsFifo(expData, i_fifoType)
    #flush out distance
    testUtil.readDsEntryReturnVal(i_fifoType)
    testUtil.readEot(i_fifoType)
