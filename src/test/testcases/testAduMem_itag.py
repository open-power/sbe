# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testAduMem_itag.py $
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
from __future__ import print_function
import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
import testMemUtil as testMemProcUtil
err = False

GETMEMADU_TESTDATA_ITAG =  [0,0,0,0x6,
                            0,0,0xA4,0x01,
                            0,0,0x0,0xB5, #CoreChipletId/EccByte/Flags -> CacheInhibit/FastMode/Tag/NoEcc/AutoIncr/Adu/Proc
                            0,0,0,0,              # Addr Upper 32 bit
                            0x08,0x00,0x00,0x00,  # Addr Lower 32 bit
                            0x00,0x00,0x00,0x40]  # length of data

GETMEMADU_EXPDATA_ITAG =   [0x00,0x00,0x00,0x48,  # length of data
                            0xc0,0xde,0xa4,0x01,
                            0x0,0x0,0x0,0x0,
                            0x00,0x0,0x0,0x03];

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    #PutMemAdu with Itag
    data = os.urandom(80)
    data = [ord(c) for c in data]
    testMemProcUtil.putmem(0x08000000, data, 0xB5)
    data = testMemProcUtil.addItagEcc(data,True, False)

    # GetMemAdu test with ECC and Itag
    readData = testMemProcUtil.getmem(0x08000000, 80, 0xB5)
    if(data == readData):
        print ("Success - Write-Read ADU with Itag")
    else:
        print(data)
        print(readData)
        raise Exception('data mistmach')

    # Partial Write test
    readData = testMemProcUtil.getmem(0x08000000, 40, 0xB5)
    data = os.urandom(8)
    data = [ord(c) for c in data]
    testMemProcUtil.putmem(0x08000008, data, 0xB5, 0xEF)
    data = testMemProcUtil.addItagEcc(data,True, False)
    readBackData = testMemProcUtil.getmem(0x08000000, 40, 0xB5)
    sandwichData = readData[:9]+data+readData[len(data)+9:]
    if(sandwichData == readBackData):
        print ("Success - Write_Part-Read ADU with Itag")
    else:
        print(readData)
        print(data)
        print(readBackData)
        print(sandwichData)
        raise Exception('data mistmach')
#-------------------------------------------------
# Calling all test code
#-------------------------------------------------
main()

if err:
    print ("\nTest Suite completed with error(s)")
    #sys.exit(1)
else:
    print ("\nTest Suite completed with no errors")
    #sys.exit(0);

