# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPmicHealthCheck.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2021
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
sys.path.append("targets/p10_standalone/sbeTest" )
import testUtil
import testMemUtil
err = False

TESTDATA_INV_TARGET = [0,0,0,3,
            0,0,0xAC,0x01,
            0, 0x5, 0, 0x8 ]

EXPDATA_INV_TARGET = [0xc0,0xde,0xac,0x01,
           0x00,0x2,0x0,0x4,
           0, 0, 0, 3];

TESTDATA_INV_INST = [0,0,0,3,
            0,0,0xAC,0x01,
            0, 0x4, 0, 0x16 ]

EXPDATA_INV_INST = [0xc0,0xde,0xac,0x01,
           0x00,0x02,0x0,0x41,
           0, 0, 0, 3];

TESTDATA = [0,0,0,3,
            0,0,0xAC,0x01,
            0, 0x4, 0, 0x8 ]

EXPDATA = [
           0x01, 0x04, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0xB4,           # number of bytes returned = 0
           0xc0,0xde,0xac,0x01,
           0x00,0x0,0x0,0x0,
           0, 0, 0, 3];


# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    print("\n********************Invalid Target******************************\n")
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( TESTDATA_INV_TARGET )
    testUtil.writeEot( )
    testUtil.readDsFifo( EXPDATA_INV_TARGET )
    testUtil.readEot( )

    print("\n********************Invalid Instance******************************\n")
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( TESTDATA_INV_INST )
    testUtil.writeEot( )
    testUtil.readDsFifo( EXPDATA_INV_INST )
    testUtil.readEot( )

    print("\n********************Valid Parameters******************************\n")
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( EXPDATA )
    testUtil.readEot( )

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

