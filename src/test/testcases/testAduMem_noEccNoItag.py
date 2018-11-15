# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testAduMem_noEccNoItag.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2019
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
sys.path.append("targets/p9_axone/sbeTest" )
import testUtil
import testMemUtil as testMemProcUtil
err = False



# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    # PutMemAdu - Secure operations
    data = os.urandom(4)
    data = [ord(c) for c in data]
    testMemProcUtil.putmem(0x00000000, data, 0x00002001)
    testMemProcUtil.putmem(0x00000000, data, 0x00000801)
    testMemProcUtil.putmem(0x00000000, data, 0x00008001)
    testMemProcUtil.putmem(0x00000000, data, 0x00000401)
    testMemProcUtil.putmem(0x00000000, data, 0x00004001)
    print ("Success - ADU secure operations")

    #PutMemAdu Test
    data = os.urandom(80)
    data = [ord(c) for c in data]
    testMemProcUtil.putmem(0x08000000, data, 0xA5)

    # GetMemAdu test
    readData = testMemProcUtil.getmem(0x08000000, 80, 0xA5)
    if(data == readData):
        print ("Success - Write-Read ADU")
    else:
        print data
        print readData
        raise Exception('data mistmach')

    # Partial Write test
    readData = testMemProcUtil.getmem(0x08000000, 40, 0xA5)
    data = os.urandom(8)
    data = [ord(c) for c in data]
    testMemProcUtil.putmem(0x08000008, data, 0xA5)
    readBackData = testMemProcUtil.getmem(0x08000000, 40, 0xA5)
    sandwichData = readData[:8]+data+readData[len(data)+8:]
    if(sandwichData == readBackData):
        print ("Success - Write_Part-Read ADU")
    else:
        print readData
        print data
        print readBackData
        print sandwichData
        raise Exception('data mistmach')


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

