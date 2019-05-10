# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testMemPBA.py $
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
from __future__ import print_function
import sys
sys.path.append("targets/p9_nimbus/sbeTest" )
sys.path.append("targets/p9_axone/sbeTest" )
import testUtil
import testMemUtil
err = False

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
# First Case without Fast Mode without LCO
    testUtil.runCycles( 10000000 )
    # Put mem PBA
    data = os.urandom(128*2)
    data = [ord(c) for c in data]
    testMemUtil.putmem(0x08000000, data, 0x02)
    # Get mem PBA - WO FMODE, WO LCO
    readData = testMemUtil.getmem(0x08000000, 128*2, 0x02)
    if(data == readData):
        print ("Success - Write-Read PBA - WO FMODE, WO LCO")
    else:
        print(data)
        print(readData)
        raise Exception('data mistmach')
    # Partial Write test
    readData = testMemUtil.getmem(0x08000000, 128*3, 0x02)
    data = os.urandom(128)
    data = [ord(c) for c in data]
    testMemUtil.putmem(0x08000000+128, data, 0x02)
    readBackData = testMemUtil.getmem(0x08000000, 128*3, 0x02)
    sandwichData = readData[:128]+data+readData[len(data)+128:]
    if(sandwichData == readBackData):
        print ("Success - Write_Part-Read PBA - WO FMODE, WO LCO")
    else:
        print(readData)
        print(data)
        print(readBackData)
        print(sandwichData)
        raise Exception('data mistmach')

# Second Case with Fast Mode without LCO
    testUtil.runCycles( 10000000 )
    # Put mem PBA - W FMODE, WO LCO
    data = os.urandom(128*2)
    data = [ord(c) for c in data]
    testMemUtil.putmem(0x08000000, data, 0x22)
    # Get mem PBA
    readData = testMemUtil.getmem(0x08000000, 128*2, 0x22)
    if(data == readData):
        print ("Success - Write-Read PBA - W FMODE, WO LCO")
    else:
        print(data)
        print(readData)
        raise Exception('data mistmach')
    # Partial Write test
    readData = testMemUtil.getmem(0x08000000, 128*3, 0x22)
    data = os.urandom(128)
    data = [ord(c) for c in data]
    testMemUtil.putmem(0x08000000+128, data, 0x22)
    readBackData = testMemUtil.getmem(0x08000000, 128*3, 0x22)
    sandwichData = readData[:128]+data+readData[len(data)+128:]
    if(sandwichData == readBackData):
        print ("Success - Write_Part-Read PBA - W FMODE, WO LCO")
    else:
        print(readData)
        print(data)
        print(readBackData)
        print(sandwichData)
        raise Exception('data mistmach')

# Third Case with Fast Mode with LCO
    testUtil.runCycles( 10000000 )
    # Put mem PBA - W FMODE, W LCO
    data = os.urandom(128*2)
    data = [ord(c) for c in data]
    testMemUtil.putmem(0x08000000, data, 0x62)
    # Get mem PBA
    readData = testMemUtil.getmem(0x08000000, 128*2, 0x62)
    if(data == readData):
        print ("Success - Write-Read PBA - W FMODE, W LCO")
    else:
        print(data)
        print(readData)
        raise Exception('data mistmach')
    # Partial Write test
    readData = testMemUtil.getmem(0x08000000, 128*3, 0x62)
    data = os.urandom(128)
    data = [ord(c) for c in data]
    testMemUtil.putmem(0x08000000+128, data, 0x62)
    readBackData = testMemUtil.getmem(0x08000000, 128*3, 0x62)
    sandwichData = readData[:128]+data+readData[len(data)+128:]
    if(sandwichData == readBackData):
        print ("Success - Write_Part-Read PBA - W FMODE, W LCO")
    else:
        print(readData)
        print(data)
        print(readBackData)
        print(sandwichData)
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

