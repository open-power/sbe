# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPutGetScom.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2015,2016
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
sys.path.append("targets/p9_nimbus/sbeTest" )
import testUtil
err = False
#from testWrite import *

# @TODO via RTC : 141905
#       Modify the test sequence in such a way that
#       the test does not leave the Register value altered.

PUTSCOM_TESTDATA = [0,0,0,6,
                    0,0,0xA2,0x02,
                    0,0,0x0,0x00,
                    0,0x05,0x00,0x3E, #scratch reg 7 (32-bit)
                    0xde,0xca,0xff,0xee,
                    0x00,0x00,0x00,0x00 ]

PUTSCOM_TESTDATA_INVALID = [0,0,0,6,
                            0,0,0xA2,0x02,
                            0,0,0x0,0x00,
                            # TODO via RTC 152952: This address is invalid for
                            # Nimbus but not for Cumulus
                            0x0a,0x00,0x00,0x00,
                            0xde,0xca,0xff,0xee,
                            0x00,0x00,0x00,0x00 ]

PUTSCOM_EXPDATA = [0xc0,0xde,0xa2,0x02,
                   0x0,0x0,0x0,0x0,
                   0x00,0x0,0x0,0x03];

PUTSCOM_EXPDATA_INVALID = [0xc0,0xde,0xa2,0x02,
                           0x0,0xfe,0x0,0x11,
                           0x00,0x0,0x0,0x04,
                           0x00,0x0,0x0,0x04];

GETSCOM_TESTDATA = [0,0,0,4,
                    0,0,0xA2,0x01,
                    0,0,0x0,0x00,
                    0,0x05,0x0,0x3E]

GETSCOM_TESTDATA_INVALID = [0,0,0,4,
                            0,0,0xA2,0x01,
                            0,0,0x0,0x00,
                            # TODO via RTC: 152952: This address is invalid for
                            # Nimbus but not for Cumulus
                            0x0a,0x0,0x0,0x0]

GETSCOM_EXPDATA = [0xde,0xca,0xff,0xee,
                   0x00,0x00,0x00,0x00,
                   0xc0,0xde,0xa2,0x01,
                   0x0,0x0,0x0,0x0,
                   0x00,0x0,0x0,0x03];

GETSCOM_EXPDATA_INVALID = [0xc0,0xde,0xa2,0x01,
                           0x0,0xfe,0x0,0x11,
                           0x00,0x0,0x0,0x04,
                           0x00,0x0,0x0,0x04];

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )
    print ("\nStarting putscom test")
    testUtil.writeUsFifo( PUTSCOM_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( PUTSCOM_EXPDATA )
    testUtil.readEot( )
    print ("\nStarting invalid putscom test")
    testUtil.writeUsFifo( PUTSCOM_TESTDATA_INVALID )
    testUtil.writeEot( )
    testUtil.readDsFifo( PUTSCOM_EXPDATA_INVALID )
    testUtil.readEot( )
    print ("\nStarting getscom test")
    testUtil.writeUsFifo( GETSCOM_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( GETSCOM_EXPDATA )
    testUtil.readEot( )
    print ("\nStarting invalid getscom test")
    testUtil.writeUsFifo( GETSCOM_TESTDATA_INVALID )
    testUtil.writeEot( )
    testUtil.readDsFifo( GETSCOM_EXPDATA_INVALID )
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

