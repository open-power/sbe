# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testAduMem_ecc.py $
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
import sys
sys.path.append("targets/p10_standalone/sbeTest" )
import testUtil
import testMemUtil as testMemProcUtil
err = False

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    #PutMemAdu with ECC
    data = os.urandom(80)
    data = [ord(c) for c in data]
    testMemProcUtil.putmem(0xF0000000, data, 0xAD, 0xEF)
    data = testMemProcUtil.addItagEcc(data,False, True, 0xEF)

    # GetMemAdu test with ECC
    readData = testMemProcUtil.getmem(0xF0000000, 80, 0xAD)
    if(data == readData):
        print ("Success - Write-Read ADU with ECC")
    else:
        print data
        print readData
        raise Exception('data mistmach')

    # Partial Write test
    readData = testMemProcUtil.getmem(0xF0000000, 40, 0xAD)
    data = os.urandom(8)
    data = [ord(c) for c in data]
    testMemProcUtil.putmem(0xF0000008, data, 0xAD, 0xEF)
    data = testMemProcUtil.addItagEcc(data,False, True, 0xEF)
    readBackData = testMemProcUtil.getmem(0xF0000000, 40, 0xAD)
    sandwichData = readData[:9]+data+readData[len(data)+9:]
    if(sandwichData == readBackData):
        print ("Success - Write_Part-Read ADU with ECC")
    else:
        print readData
        print data
        print readBackData
        print sandwichData
        raise Exception('data mistmach')

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

