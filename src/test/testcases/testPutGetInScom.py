# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPutGetInScom.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2016
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
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False
#from testWrite import *

# @TODO via RTC : 141905
#       Modify the test sequence in such a way that
#       the test does not leave the Register value altered.

# Indirect scom form 0 test case
PUTSCOM_TESTDATA = [0,0,0,6,
                    0,0,0xA2,0x02,
                    0x80,0x0,0x0,0x83,
                    0x0D,0x01,0x0C,0x3F,
                    0xde,0xca,0xff,0xee,
                    0x00,0x00,0x12,0x34 ]

PUTSCOM_EXPDATA = [0xc0,0xde,0xa2,0x02,
                   0x0,0x0,0x0,0x0,
                   0x00,0x0,0x0,0x03];

GETSCOM_TESTDATA = [0,0,0,4,
                    0,0,0xA2,0x01,
                    0x80,0x0,0x0,0x83,
                    0x0D,0x01,0x0C,0x3F]

GETSCOM_EXPDATA = [0x00,0x00,0x00,0x00,
                   0x00,0x00,0x12,0x34, # Only last 16 bits will be returned 
                   0xc0,0xde,0xa2,0x01,
                   0x0,0x0,0x0,0x0,
                   0x00,0x0,0x0,0x03];

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( PUTSCOM_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( PUTSCOM_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( GETSCOM_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( GETSCOM_EXPDATA )
    testUtil.readEot( )

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

