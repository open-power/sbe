# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testPutGetRegFpr.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016
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
err = False

PUTREG_TESTDATA =  [0,0,0,9,
                    0,0,0xA5,0x02,
                    0x00,0x20,0x02,0x02, # two fpr registers
                    0,0,0x0,0x01,
                    0,0,0x0,0x0,
                    0,0,0x0,0x1,
                    0,0,0x0,0x02,
                    0,0,0x0,0x0,
                    0,0,0x0,0x2 ]

PUTREG_EXPDATA =  [0xc0,0xde,0xa5,0x02,
                   0x0,0x0,0x0,0x0,
                   0x00,0x0,0x0,0x03];

GETREG_TESTDATA = [0,0,0,5,
                    0,0,0xA5,0x01,
                    0x00,0x20,0x02,0x02,  #two fpr registers
                    0,0,0x0,0x01,
                    0,0,0x0,0x02 ]

GETREG_EXPDATA = [0x00,0x00,0x00,0x00,
                   0x00,0x00,0x00,0x01,
                   0x00,0x00,0x00,0x00,
                   0x00,0x00,0x00,0x02,
                   0xc0,0xde,0xa5,0x01,
                   0x0,0x0,0x0,0x0,
                   0x00,0x0,0x0,0x03];

# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )
    testUtil.writeUsFifo( PUTREG_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( PUTREG_EXPDATA )
    testUtil.readEot( )
    testUtil.writeUsFifo( GETREG_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsFifo( GETREG_EXPDATA )
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

