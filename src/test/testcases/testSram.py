# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testSram.py $
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
#from __future__ import print_function
import sys
import os
sys.path.append("targets/p10_standalone/sbeTest" )
import testUtil
err = False
#from testWrite import *

def gethalfword(dataInInt):
    hex_string = '0'*(4-len(str(hex(dataInInt))[2:])) + str(hex(dataInInt))[2:]
    return list(struct.unpack('<BB',hex_string.decode('hex')))
def getsingleword(dataInInt):
    hex_string = '0'*(8-len(str(hex(dataInInt))[2:])) + str(hex(dataInInt))[2:]
    return list(struct.unpack('<BBBB',hex_string.decode('hex')))
def getdoubleword(dataInInt):
    hex_string = '0'*(16-len(str(hex(dataInInt))[:18][2:])) + str(hex(dataInInt))[:18][2:]
    return list(struct.unpack('<BBBBBBBB',hex_string.decode('hex')))

def putsram(addr, mode, data, primStatus, secStatus,fifoType):
    req = (getsingleword(6 + (len(data)/4))
           + [0, 0, 0xA4, 0x04]
           + getsingleword(mode)
           + getdoubleword(addr)
           + getsingleword(len(data))
           + data)
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( req,fifoType )
    testUtil.writeEot(fifoType)
    
    length = len(data)
    if((primStatus != 0) or (secStatus != 0)):
        length = 0
    
    expData = (getsingleword(length)
               + [0xc0, 0xde, 0xa4, 0x04]
               + gethalfword(primStatus)
               + gethalfword(secStatus)
               + getsingleword(0x03))
    testUtil.readDsFifo(expData,fifoType)
    testUtil.readEot(fifoType )

def getsram(addr, mode, length, primStatus, secStatus,fifoType):
    
    req = (getsingleword(0x06)
           + getsingleword(0xa403)
           + getsingleword(mode)
           + getdoubleword(addr)
           + getsingleword(length))

    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( req,fifoType )
    testUtil.writeEot(fifoType )
    testUtil.runCycles( 10000000 )
    data = []
    if((primStatus != 0) or (secStatus != 0)):
        length = 0
    for i in range(0, int(-(-float(length)//4))):
        data += list(testUtil.readDsEntryReturnVal(fifoType))
    readLen = testUtil.readDsEntryReturnVal(fifoType)
    if(getsingleword(length) != list(readLen)):
        print(getsingleword(length))
        print(list(readLen))
        raise Exception("Invalid Length")

    expResp = (getsingleword(0xc0dea403)
               + gethalfword(primStatus)
               + gethalfword(secStatus)
               + getsingleword(0x03))
    testUtil.readDsFifo(expResp,fifoType)
    testUtil.readEot(fifoType)

    return data[:length]


def validateSRAMAccessFailureCases( fifoType ):
   testcase = ""
   try:
       #Data used to write to SRAM
       data = os.urandom(128*2)
       data = [ord(c) for c in data]
       #---------------------------------------------
       # PutSram Failure Test
       #---------------------------------------------
       testUtil.runCycles( 10000000 )
       testcase = "Non-aligned address Put SRAM Testcase"
       #0x208000 : Chiplet 0x10 Multicast Bit:1
       putsram(0xffff2803, 0x108000, data, 2, 3,fifoType)
       print("Success: Non-aligned address Put SRAM Testcase:%d"%fifoType)

   except:
        print("Failed Test Case:"+str(testcase))
        raise Exception('Failure')


def validateSRAMAccess( fifoType ):
   testcase = ""
   try:
       #Data used to write to SRAM
       data = os.urandom(128*2)
       data = [ord(c) for c in data]

       #---------------------------------------------
       # Put and Get Occ Sram test - Circular Mode
       #---------------------------------------------
       testcase = "Circlular OCC SRAM Put and GET Testcase"
       #0xC0 : Chiplet_ID 0x0 (not applicable for OCC access)
       #       Circular Access Mode
       #       Channel bits 0(which makes HWP to pick channel 3 as default)
       putsram(0x00000000FFFBE000, 0xC0, data, 0, 0,fifoType)
       readData = getsram(0x00000000FFFBE000, 0xC0, (128*2), 0, 0,fifoType)
       if(data == readData):
          print("Success: Put-get OCC sram, Circular Mode:FIFOTYPE:%d"%fifoType)
       else:
          print("Read data does not match with data used to write")
          print(data)
          print(readData)
          raise Exception('data mistmach')

       #---------------------------------------------
       # Put and Get Occ Sram test - Linear Mode
       #---------------------------------------------
       testUtil.runCycles( 10000000 )
       testcase = "Linear OCC SRAM Put and GET Testcase"
       #0x50 : Chiplet_ID 0x0 (not applicable for OCC access)
       #       Liner Access Mode
       #       Channel 1
       putsram(0x00000000FFFBE000, 0x50, data, 0, 0,fifoType)
       readData = getsram(0x00000000FFFBE000, 0x50, (128*2), 0, 0,fifoType)
       if(data == readData):
           print("Success: Put-Get sram Linear Mode,FIFOTYPE:%d"%fifoType)
       else:
           print("Read data does not match with data used to write")
           print(data)
           print(readData)
           raise Exception('data mistmach')
  
       #---------------------------------------------
       # Put and Get QME Sram test
       #---------------------------------------------
       testUtil.runCycles( 10000000 )
       testcase = "QME SRAM Put and GET Testcase"
       #0x208000 : Chiplet 0x20 Multicast Bit:1
       putsram(0x00000000ffff0000, 0x208000, data, 0, 0,fifoType)
       testUtil.runCycles( 10000000 )
       readData = getsram(0x00000000ffff0000, 0x200000, (128*2), 0, 0,fifoType)
       if(data == readData):
          print("Success: QME SRAM Put and GET Testcase,FIFOTYPE:%d"%fifoType)
       else:
          print("Read data does not match with data used to write")
          print(data)
          print(readData)
          raise Exception('data mistmach')

       #---------------------------------------------
       # Put and Get IO_PPE Sram test
       #---------------------------------------------
       testUtil.runCycles( 10000000 )
       testcase = "IO_PPE SRAM Put and GET Testcase"
       #0x208000 : Chiplet 0x10 Multicast Bit:1
       putsram(0xffff2800, 0x108000, data, 0, 0,fifoType)
       testUtil.runCycles( 10000000 )
       readData = getsram(0xffff2800, 0x100000, (128*2), 0, 0,fifoType)
       if(data == readData):
          print("Success: IO_PPE SRAM Put and GET Testcase,FIFOTYPE:%d"%fifoType)
       else:
          print ("Read data does not match with data used to write")
          print data
          print readData
          raise Exception('data mistmach')

   except:
        print "FAILED Test Case:"+str(testcase)
        raise Exception('Failure')

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
      #Validate all SRAM access usecases using First FIFO
      fifoType=0
      validateSRAMAccessFailureCases(fifoType)
      validateSRAMAccess(fifoType)
      testUtil.runCycles( 10000000 )

      #Validate all SRAM access usecases using Second FIFO
      fifoType=1
      validateSRAMAccess(fifoType)
      testUtil.runCycles( 10000000 )

#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
try:
    main()
except:
    print ( "\nTest Suite completed with error(s)" )
    testUtil.collectFFDC()
    raise()

print ( "\nTest Suite completed with no errors" )

