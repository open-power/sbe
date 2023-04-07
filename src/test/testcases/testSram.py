# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testSram.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2023
# [+] International Business Machines Corp.
# [+] Marty E. Plummer
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
sys.path.append("targets/p9_nimbus/sbeTest" )
sys.path.append("targets/p9_axone/sbeTest" )
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

def putsram(addr, mode, data, primStatus, secStatus):
    req = (getsingleword(5 + (len(data)/4))
           + [0, 0, 0xA4, 0x04]
           + getsingleword(mode)
           + getsingleword(addr)
           + getsingleword(len(data))
           + data)

    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( req )
    testUtil.writeEot( )

    if((primStatus != 0) or (secStatus != 0)):
        data = []
    expData = (getsingleword(len(data))
               + [0xc0, 0xde, 0xa4, 0x04]
               + gethalfword(primStatus)
               + gethalfword(secStatus)
               + getsingleword(0x03))
    testUtil.readDsFifo(expData)
    testUtil.readEot( )

def getsram(addr, mode, length, primStatus, secStatus):
    req = (getsingleword(0x05)
           + getsingleword(0xa403)
           + getsingleword(mode)
           + getsingleword(addr)
           + getsingleword(length))

    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( req )
    testUtil.writeEot( )

    data = []
    if((primStatus != 0) or (secStatus != 0)):
        length = 0
    for i in range(0, int(-(-float(length)//4))):
        data += list(testUtil.readDsEntryReturnVal())
    readLen = testUtil.readDsEntryReturnVal()
    if(getsingleword(length) != list(readLen)):
        print(getsingleword(length))
        print(list(readLen))
        raise Exception("Invalid Length")

    expResp = (getsingleword(0xc0dea403)
               + gethalfword(primStatus)
               + gethalfword(secStatus)
               + getsingleword(0x03))
    testUtil.readDsFifo(expResp)
    testUtil.readEot( )

    return data[:length]

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testcase = ""
    try:
        testUtil.runCycles( 10000000 )

        # Put Occ Sram test - Linear - Can be tested over Normal
        # Debug mode
        data = os.urandom(128*2)
        data = [ord(c) for c in data]
        putsram(0xFFFBE000, 0x01, data, 0, 0)

        readData = getsram(0xFFFBE000, 0x01, 128*2, 0, 0)

        if(data == readData):
            print("Success: put - get sram")
        else:
            print(data)
            print(readData)
            raise Exception('data mistmach')

        # secure mem - write to disallowed mem
        # start and end completely outside
        testcase = "sec put test 1"
        putsram(0xFFFBE000-256, 0x01, data, 0x0005, 0x0014)
        print("Success: "+testcase)
        # start outside and end inside the window
        testcase = "sec put test 2"
        putsram(0xFFFBE000-128, 0x01, data, 0x0005, 0x0014)
        print("Success: "+testcase)
        # start inside and end outside the window
        testcase = "sec put test 3"
        putsram(0xFFFBE000-128, 0x01, data, 0x0005, 0x0014)
        print("Success: "+testcase)

        # comment out -ve tests for security wisnidw as we allow read always
        # secure mem - read on disallowed mem
        # start and end completely outside
        #testcase = "sec get test 1"
        #getsram(0xFFFBE000-256, 0x01, 256, 0x0005, 0x0014)
        #print("Success: "+testcase)
        # start outside and end inside the window
        #testcase = "sec get test 2"
        #getsram(0xFFFBE000-128, 0x01, 256, 0x0005, 0x0014)
        #print("Success: "+testcase)
        # start inside and end outside the window
        #testcase = "sec get test 3"
        #getsram(0xFFFBE000-128, 0x01, 256, 0x0005, 0x0014)
        #print("Success: "+testcase)

        # Put Occ Sram test - Circular - Can be enabled once we get
        # valid address range to read the circular data
        #testUtil.writeUsFifo( PUTSRAM_OCC_TESTDATA_1 )
        #testUtil.writeEot( )
        #testUtil.readDsFifo( PUTSRAM_OCC_EXPDATA_1 )
        #testUtil.readEot( )
        #testUtil.writeUsFifo( GETSRAM_OCC_TESTDATA_1 )
        #testUtil.writeEot( )
        #testUtil.readDsFifo( GETSRAM_OCC_EXPDATA_1 )
        #testUtil.readEot( )
    except:
        print("FAILED Test Case:"+str(testcase))
        raise Exception('Failure')

#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
if testUtil.getMachineName() == "axone":
    try:
        main()
    except:
        print ( "\nTest Suite completed with error(s)" )
        testUtil.collectFFDC()
        raise()

    print ( "\nTest Suite completed with no errors" )
else:
    main()

    if err:
        print ("\nTest Suite completed with error(s)")
        #sys.exit(1)
    else:
        print ("\nTest Suite completed with no errors")
        #sys.exit(0);

