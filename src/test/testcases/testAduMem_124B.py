# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testAduMem_124B.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017
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
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
import testMemUtil as testMemProcUtil
err = False

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

# Test case 1: 1byte access
    #PutMemAdu Test
    data = os.urandom(1)
    data = [ord(c) for c in data]
    testMemProcUtil.putmem(0x08000000, data, 0xA5)

    # GetMemAdu test
    readData = testMemProcUtil.getmem(0x08000000, 1, 0xA5)
    if(data == readData):
        print ("Success - Write-Read ADU 1byte")
    else:
        print data
        print readData
        raise Exception('data mistmach')

# Test case 2: 2byte access
    #PutMemAdu Test
    data = os.urandom(2)
    data = [ord(c) for c in data]
    testMemProcUtil.putmem(0x08000000, data, 0xA5)

    # GetMemAdu test
    readData = testMemProcUtil.getmem(0x08000000, 2, 0xA5)
    if(data == readData):
        print ("Success - Write-Read ADU 2byte")
    else:
        print data
        print readData
        raise Exception('data mistmach')

# Test case 3: 4byte access
    #PutMemAdu Test
    data = os.urandom(4)
    data = [ord(c) for c in data]
    testMemProcUtil.putmem(0x08000000, data, 0xA5)

    # GetMemAdu test
    readData = testMemProcUtil.getmem(0x08000000, 4, 0xA5)
    if(data == readData):
        print ("Success - Write-Read ADU 4byte")
    else:
        print data
        print readData
        raise Exception('data mistmach')

# Test case 4: Invalid length - 3
    # GetMemAdu test
    testMemProcUtil.getmem_failure(0x08000000, 3, 0xA5, 0x0002000A)
    print ("Success - invalid length test")

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

