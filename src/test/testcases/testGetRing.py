# IBM_PROLOG_BEGIN_TAG
# This is an automatically generated prolog.
#
# $Source: src/test/testcases/testGetRing.py $
#
# OpenPOWER sbe Project
#
# Contributors Listed Below - COPYRIGHT 2016,2017
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

LOOP_COUNT = 1

#aligned Data
GETRING_TESTDATA =  [0,0,0,0x6,
                     0,0,0xA3,0x01,
                     0x20,0x03,0x70,0x01,      # address
                     0,0,0x54,0xA5,           # length of data in bits
                     0x00,0x00,0x00,0x01]

GETRING_EXPDATA =   [0,0,0x54,0xA5,  # length of data in bits
                     0xc0,0xde,0xa3,0x01,
                     0x0,0x0,0x0,0x0,
                     0x00,0x0,0x0,0x03];


# MAIN Test Run Starts Here...
#-------------------------------------------------
def main( ):
    testUtil.runCycles( 10000000 )

    # GetRing test - Aligned Data
    testUtil.writeUsFifo( GETRING_TESTDATA )
    testUtil.writeEot( )
    testUtil.readDsEntry ( 678 )  ## 6242 entries
    testUtil.readDsFifo( GETRING_EXPDATA )
    testUtil.runCycles( 10000000 )
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

