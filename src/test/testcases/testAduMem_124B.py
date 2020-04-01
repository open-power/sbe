# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testAduMem_124B.py $
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
import testMemUtil as testMemProcUtil
err = False

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

# Test case 1: Valid lengths
    bytes = [1, 2, 4]
    offsets = {1:(0, 1, 2, 3, 4, 5, 6, 7), 2:(0, 2, 4, 6), 4:(0, 4)}
    for byte in bytes:
        for offset in offsets[byte]:
            #PutMemAdu Test
            data = os.urandom(byte)
            data = [ord(c) for c in data]
            testMemProcUtil.putmem(0xF0000000 + offset, data, 0xA5)

            # GetMemAdu test
            readData = testMemProcUtil.getmem(0xF0000000 + offset, byte, 0xA5)
            if(data == readData):
                print ("Success - Write-Read ADU byte["+str(byte)+"] offset[" + str(offset)+"]")
            else:
                print [hex(a) for a in data]
                print [hex(a) for a in readData]
                raise Exception('data mistmach')

# Test case 2: Invalid length - 3
    # GetMemAdu test
    testMemProcUtil.getmem_failure(0xF0000000, 3, 0xA5, 0x0002000A)
    print ("Success - invalid length test")

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
