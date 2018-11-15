# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testMatchStashPair.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2017,2019
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
import testMemUtil
err = False

data =[0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
       0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
       0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,
       0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,
       0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,0xdd,
       0xee,0xee,0xee,0xee,0xee,0xee,0xee,0xee,
       0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
       0xab,0xab,0xab,0xab,0xab,0xab,0xab,0xab,
       0xcd,0xcd,0xcd,0xcd,0xcd,0xcd,0xcd,0xcd];

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
# First Case without Fast Mode without LCO
    testUtil.runCycles( 10000000 )
    # Get mem PBA - WO FMODE, WO LCO
    readData = testMemUtil.getmem(0x08200000, 128, 0x02)[44:][:-12]
    if(data == readData):
        print ("Success - Write-Read PBA - WO FMODE, WO LCO")
    else:
        print data
        print readData
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