# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testMemUtil.py $
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
from __future__ import print_function
import sys
import os
import struct
sys.path.append("targets/p10_standalone/sbeTest" )
import testPSUUtil
import testRegistry as reg
import testUtil
err = False

RUN_CYCLES = 1
i_fifoType = 0

def gethalfword(dataInInt):
    hex_string = '0'*(4-len(str(hex(dataInInt))[2:])) + str(hex(dataInInt))[2:]
    return list(struct.unpack('<BB',hex_string.decode('hex')))
def getsingleword(dataInInt):
    hex_string = '0'*(8-len(str(hex(dataInInt))[2:])) + str(hex(dataInInt))[2:]
    return list(struct.unpack('<BBBB',hex_string.decode('hex')))
def getdoubleword(dataInInt):
    hex_string = '0'*(16-len(str(hex(dataInInt))[:18][2:])) + str(hex(dataInInt))[:18][2:]
    return list(struct.unpack('<BBBBBBBB',hex_string.decode('hex')))

def addItagEcc(arr, itag, ecc, eccVal=0):
    arrs = []
    while len(arr) > 8:
        pice = arr[:8]
        arrs += pice
        if(itag):
            arrs += [1]
        if(ecc):
            arrs += [eccVal]
        arr   = arr[8:]
    arrs += arr
    if(itag):
        arrs += [1]
    if(ecc):
        arrs += [eccVal]
    return arrs

def putmem(addr, data, flags, ecc=0):
    lenInBytes = len(data)
    if(len(data) < 8):
        data = data+[0]*(4-len(data))
    totalLen = 5 + len(data)/4
    coreChipletId = 0x00
    if (flags & 0x0040):
        # LCO mode is set, so chiplet id - 0x20
        coreChipletId = 0x20
    req = (getsingleword(totalLen)
          +[ 0,0,0xA4,0x02]
          +[coreChipletId, ecc]
          +gethalfword(flags)
          #0,0,0x0,0xA5] #CoreChipletId/EccByte/Flags -> NoEccOverride/CacheInhibit/FastMode/NoTag/NoEcc/AutoIncr/Adu/Proc
          + getdoubleword(addr)
          + getsingleword(lenInBytes)  # length of data
          + data)
    testUtil.writeUsFifo(req, i_fifoType)
    testUtil.writeEot(i_fifoType )
    testUtil.runCycles( RUN_CYCLES )
    if(flags & 0x0008):
        lenInBytes += int(len(data)/8)
    if(flags & 0x0010):
        lenInBytes += int(len(data)/8)
    expData = (getsingleword(lenInBytes)
               +[0xc0,0xde,0xa4,0x02,
                 0x0,0x0,0x0,0x0,
                 0x00,0x0,0x0,0x03])
    testUtil.readDsFifo(expData, i_fifoType)
    testUtil.readEot(i_fifoType )

def putmem_failure(addr, data, flags, responseWord, ecc=0):
    lenInBytes = len(data)
    if(len(data) < 8):
        data = data+[0]*(4-len(data))
    totalLen = 5 + len(data)/4
    coreChipletId = 0x00
    if (flags & 0x0040):
        # LCO mode is set, so chiplet id - 0x20
        coreChipletId = 0x20
    req = (getsingleword(totalLen)
          +[ 0,0,0xA4,0x02]
          +[coreChipletId, ecc]
          +gethalfword(flags)
          #0,0,0x0,0xA5] #CoreChipletId/EccByte/Flags -> NoEccOverride/CacheInhibit/FastMode/NoTag/NoEcc/AutoIncr/Adu/Proc
          + getdoubleword(addr)
          + getsingleword(lenInBytes)  # length of data
          + data)
    testUtil.writeUsFifo(req, i_fifoType)
    testUtil.writeEot(i_fifoType )
    testUtil.runCycles( RUN_CYCLES )
    expResp =  ([0x0, 0x0, 0x0, 0x0]
    + [0xc0,0xde,0xa4,0x02]
    + getsingleword(responseWord)
    + [0x0,0x0,0x0,0x03])
    testUtil.readDsFifo(expResp, i_fifoType)
    testUtil.readEot(i_fifoType )

def getmem(addr, len, flags):
    testUtil.runCycles( RUN_CYCLES )
    req = (getsingleword(6)
         + [0, 0, 0xA4, 0x01]
         + getsingleword(flags)
         + getdoubleword(addr)
         + getsingleword(len))
    testUtil.writeUsFifo(req, i_fifoType)
    testUtil.writeEot(i_fifoType)

    # read data
    data = []
    lenExp = len
    if(flags & 0x0008):
        lenExp += int(len/8)
    if(flags & 0x0010):
        lenExp += int(len/8)
    for i in range(0, int(-(-float(lenExp)//4))):
        data += list(testUtil.readDsEntryReturnVal(i_fifoType))

    readLen = testUtil.readDsEntryReturnVal(i_fifoType)
    if(getsingleword(lenExp) != list(readLen)):
        print(getsingleword(lenExp))
        print(list(readLen))
        raise Exception("Invalid Length")

    expResp =  [0xc0,0xde,0xa4,0x01,
                0x0,0x0,0x0,0x0,
                0x00,0x0,0x0,0x03];
    testUtil.readDsFifo(expResp, i_fifoType)
    testUtil.readEot(i_fifoType)
    return data[:lenExp]

def getmem_failure(addr, len, flags, responseWord, withLen = True):
    testUtil.runCycles( RUN_CYCLES )
    req = (getsingleword(6)
         + [0, 0, 0xA4, 0x01]
         + getsingleword(flags)
         + getdoubleword(addr)
         + getsingleword(len))
    testUtil.writeUsFifo(req, i_fifoType)
    testUtil.writeEot(i_fifoType)
    lenWord = []
    if withLen:
        lenWord = [0x0, 0x0, 0x0, 0x0]
    expResp =  (lenWord
    + [0xc0,0xde,0xa4,0x01]
    + getsingleword(responseWord)
    + [0x0,0x0,0x0,0x03])
    testUtil.readDsFifo(expResp, i_fifoType)
    testUtil.readEot(i_fifoType)

def setUnsecureMemRegion(addr, size, controlFlag, responseWord):
    testUtil.runCycles( RUN_CYCLES )
    req = (["write", reg.REG_MBOX0,"0"*(8-len(hex(controlFlag).split('0x')[-1]))+hex(controlFlag).split('0x')[-1] +"00F0D601", 8, "None", "Writing to MBOX0 address"],
           ["write", reg.REG_MBOX1, "0"*(16-len(hex(size).split('0x')[-1]))+hex(size).split('0x')[-1], 8, "None", "Writing to MBOX1 address"],
           ["write", reg.REG_MBOX2, "0"*(16-len(hex(addr).split('0x')[-1]))+hex(addr).split('0x')[-1], 8, "None", "Writing to MBOX1 address"],
           ["write", reg.PSU_SBE_DOORBELL_REG_WO_OR, "8000000000000000", 8, "None", "Update SBE Doorbell register to interrupt SBE"])

    host_polling_data = (
                            ["read", reg.PSU_HOST_DOORBELL_REG_WO_OR, "0", 8, "8000000000000000", "Reading Host Doorbell for Interrupt"],
                        )

    # Host to SBE req
    regObj = testPSUUtil.registry()
    regObj.ExecuteTestOp(testPSUUtil.simSbeObj, req)
    print("\n  Poll on Host side for INTR  ...\n")
    #Poll on HOST DoorBell Register for interrupt
    regObj.pollingOn( testPSUUtil.simSbeObj, host_polling_data, 5)

    response = (
                    ["read", reg.REG_MBOX4, "0", 8, "0"*(8-len(hex(responseWord).split('0x')[-1]))+hex(responseWord).split('0x')[-1]+"00F0D601", "Reading Host MBOX4 data to Validate"],
                )
    #SBE->HOST data set execution
    regObj.ExecuteTestOp( testPSUUtil.simSbeObj, response)
